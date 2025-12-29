/*
 * kbleds.c - Управление светодиодами клавиатуры через sysfs
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/tty_port.h>

MODULE_DESCRIPTION("Keyboard LEDs control via sysfs with bit masks");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stas");

// Глобальные переменные
static struct timer_list my_timer;
static struct tty_struct *current_tty = NULL;
static struct kobject *kbleds_kobject;

// Параметры мигания
static unsigned char led_state = 0;        // Текущее состояние светодиодов
static unsigned char led_mask = 0;         // Маска светодиодов (из sysfs)
static unsigned char saved_led_state = 0;  // Сохраненное состояние для восстановления
static int blink_enabled = 0;              // Флаг включения мигания

// Константы
#define BLINK_DELAY   HZ/5                 // Частота мигания (5 раз в секунду)
#define ALL_LEDS      0x07                 // Все три светодиода (Scroll, Num, Caps)

// Прототипы функций sysfs
static ssize_t leds_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t leds_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t blink_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t blink_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

// Атрибуты sysfs
static struct kobj_attribute leds_attribute = __ATTR(led_mask, 0660, leds_show, leds_store);
static struct kobj_attribute blink_attribute = __ATTR(blink, 0660, blink_show, blink_store);

// Установка светодиодов через TTY
static void set_leds(unsigned char state)
{
    int ret;
    
    if (!current_tty || !current_tty->ops || !current_tty->ops->ioctl)
        return;
    
    ret = current_tty->ops->ioctl(current_tty, KDSETLED, (unsigned long)state);
    if (ret)
        printk(KERN_DEBUG "kbleds: ioctl returned %d\n", ret);
}

// Функция обновления светодиодов для мигания
static void update_leds(void)
{
    if (!blink_enabled)
        return;
    
    // Инвертируем состояние для мигания
    if (led_state == (saved_led_state & led_mask))
        led_state = saved_led_state & ~led_mask;  // Выключаем светодиоды по маске
    else
        led_state = saved_led_state & led_mask;   // Включаем светодиоды по маске
    
    set_leds(led_state);
}

// Функция таймера для мигания
static void my_timer_func(struct timer_list *t)
{
    update_leds();
    mod_timer(&my_timer, jiffies + BLINK_DELAY);
}

// Чтение маски светодиодов из sysfs
static ssize_t leds_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", led_mask);
}

// Запись маски светодиодов в sysfs
static ssize_t leds_store(struct kobject *kobj, struct kobj_attribute *attr,
                          const char *buf, size_t count)
{
    unsigned long value;
    int ret;
    
    ret = kstrtoul(buf, 0, &value);
    if (ret < 0)
        return ret;
    
    if (value > ALL_LEDS) {
        printk(KERN_WARNING "kbleds: invalid mask value %lu\n", value);
        return -EINVAL;
    }
    
    led_mask = (unsigned char)value;
    printk(KERN_INFO "kbleds: LED mask set to 0x%02x\n", led_mask);
    
    // Применяем новую маску
    if (blink_enabled) {
        saved_led_state = led_mask;  // Все светодиоды по маске включены
        led_state = saved_led_state & led_mask;
        set_leds(led_state);
    }
    
    return count;
}

// Чтение состояния мигания из sysfs
static ssize_t blink_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", blink_enabled);
}

// Запись состояния мигания в sysfs
static ssize_t blink_store(struct kobject *kobj, struct kobj_attribute *attr,
                           const char *buf, size_t count)
{
    unsigned long value;
    int ret;
    
    ret = kstrtoul(buf, 0, &value);
    if (ret < 0)
        return ret;
    
    value = !!value;  // Приводим к 0 или 1
    
    if (value == blink_enabled)
        return count;
    
    if (value) {
        // Включаем мигание
        blink_enabled = 1;
        
        // Сохраняем текущее состояние
        if (current_tty && current_tty->ops && current_tty->ops->ioctl) {
            // Получаем текущее состояние светодиодов
            // (это упрощенная версия, в реальности может потребоваться другой подход)
            saved_led_state = led_mask;  // Все светодиоды по маске включены
        } else {
            saved_led_state = led_mask;
        }
        
        led_state = saved_led_state & led_mask;
        set_leds(led_state);
        
        // Запускаем таймер
        mod_timer(&my_timer, jiffies + BLINK_DELAY);
        
        printk(KERN_INFO "kbleds: blinking enabled with mask 0x%02x\n", led_mask);
    } else {
        // Выключаем мигание
        blink_enabled = 0;
        
        // Восстанавливаем нормальное состояние
        if (led_mask == 0) {
            set_leds(0);  // Все выключены
        } else {
            // Включаем светодиоды согласно маске
            set_leds(led_mask);
        }
        
        // Останавливаем таймер
        del_timer_sync(&my_timer);
        
        printk(KERN_INFO "kbleds: blinking disabled\n");
    }
    
    return count;
}

// Инициализация модуля
static int __init kbleds_init(void)
{
    struct vc_data *vc;
    int console_num;
    int error = 0;
    
    printk(KERN_INFO "kbleds: loading for kernel 6.. \n");
    
    // Получаем текущую консоль
    console_num = fg_console;
    printk(KERN_INFO "kbleds: fg_console = %d\n", console_num);
    
    if (console_num < 0 || console_num >= MAX_NR_CONSOLES) {
        printk(KERN_ERR "kbleds: Invalid console number: %d\n", console_num);
        return -ENODEV;
    }
    
    // Получаем vc_data для текущей консоли
    vc = vc_cons[console_num].d;
    if (!vc) {
        printk(KERN_ERR "kbleds: No vc_data for console %d\n", console_num);
        return -ENODEV;
    }
    
    // Получаем tty_struct
    current_tty = tty_port_tty_get(&vc->port);
    
    if (!current_tty) {
        printk(KERN_ERR "kbleds: Failed to get tty for console %d\n", console_num);
        return -ENODEV;
    }
    
    printk(KERN_INFO "kbleds: Got tty successfully\n");
    
    // Проверяем доступность ioctl
    if (!current_tty->ops || !current_tty->ops->ioctl) {
        printk(KERN_ERR "kbleds: tty has no ioctl operation\n");
        tty_kref_put(current_tty);
        current_tty = NULL;
        return -ENOTTY;
    }
    
    // Инициализация таймера
    timer_setup(&my_timer, my_timer_func, 0);
    
    // Создаем kobject в sysfs
    kbleds_kobject = kobject_create_and_add("kbleds", kernel_kobj);
    if (!kbleds_kobject) {
        printk(KERN_ERR "kbleds: Failed to create kobject\n");
        tty_kref_put(current_tty);
        return -ENOMEM;
    }
    
    // Создаем файлы в sysfs
    error = sysfs_create_file(kbleds_kobject, &leds_attribute.attr);
    if (error) {
        printk(KERN_ERR "kbleds: failed to create leds file\n");
        goto error_out;
    }
    
    error = sysfs_create_file(kbleds_kobject, &blink_attribute.attr);
    if (error) {
        printk(KERN_ERR "kbleds: failed to create blink file\n");
        sysfs_remove_file(kbleds_kobject, &leds_attribute.attr);
        goto error_out;
    }
    
    // Инициализируем значения по умолчанию
    led_mask = 0;       // Все светодиоды выключены
    blink_enabled = 0;  // Мигание выключено
    
    printk(KERN_INFO "kbleds: module initialized successfully\n");
    printk(KERN_INFO "kbleds: sysfs interface at /sys/kernel/kbleds/\n");
    
    return 0;
    
error_out:
    if (current_tty) {
        tty_kref_put(current_tty);
        current_tty = NULL;
    }
    kobject_put(kbleds_kobject);
    return error;
}

// Выгрузка модуля
static void __exit kbleds_cleanup(void)
{
    printk(KERN_INFO "kbleds: unloading...\n");
    
    // Останавливаем мигание
    blink_enabled = 0;
    del_timer_sync(&my_timer);
    
    // Восстанавливаем нормальное состояние светодиодов
    if (current_tty && current_tty->ops && current_tty->ops->ioctl) {
        current_tty->ops->ioctl(current_tty, KDSETLED, (unsigned long)0);
    }
    
    // Удаляем файлы sysfs
    sysfs_remove_file(kbleds_kobject, &blink_attribute.attr);
    sysfs_remove_file(kbleds_kobject, &leds_attribute.attr);
    
    // Удаляем kobject
    kobject_put(kbleds_kobject);
    
    if (current_tty) {
        tty_kref_put(current_tty);
        current_tty = NULL;
    }
    
    printk(KERN_INFO "kbleds: module unloaded\n");
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);