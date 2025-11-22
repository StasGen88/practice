#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef struct {
    uint8_t octet[4];
} ip_addr_t;

int parse_ip(const char *ip_str, ip_addr_t *ip) {
    return sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", 
                  &ip->octet[0], &ip->octet[1], 
                  &ip->octet[2], &ip->octet[3]) == 4;
}

uint32_t ip_to_uint32(const ip_addr_t *ip) {
    return ((uint32_t)ip->octet[0] << 24) |
           ((uint32_t)ip->octet[1] << 16) |
           ((uint32_t)ip->octet[2] << 8) |
           (uint32_t)ip->octet[3];
}

void uint32_to_ip(uint32_t ip_val, ip_addr_t *ip) {
    ip->octet[0] = (ip_val >> 24) & 0xFF;
    ip->octet[1] = (ip_val >> 16) & 0xFF;
    ip->octet[2] = (ip_val >> 8) & 0xFF;
    ip->octet[3] = ip_val & 0xFF;
}

void print_ip(const ip_addr_t *ip) {
    printf("%u.%u.%u.%u", 
           ip->octet[0], ip->octet[1], 
           ip->octet[2], ip->octet[3]);
}

int is_same_subnet(const ip_addr_t *gateway, const ip_addr_t *mask, const ip_addr_t *dest) {
    uint32_t gateway_val = ip_to_uint32(gateway);
    uint32_t mask_val = ip_to_uint32(mask);
    uint32_t dest_val = ip_to_uint32(dest);
    
    return (gateway_val & mask_val) == (dest_val & mask_val);
}

void generate_random_ip(ip_addr_t *ip) {
    for (int i = 0; i < 4; i++) {
        ip->octet[i] = rand() % 256;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Использование: %s <IP_шлюза> <маска_подсети> <количество_пакетов>\n", argv[0]);
        printf("Пример: %s 192.168.1.1 255.255.255.0 10\n", argv[0]);
        return 1;
    }
    
    ip_addr_t gateway, mask;
    int packet_count;
    
    if (!parse_ip(argv[1], &gateway)) {
        printf("Ошибкаq: неверный формат IP адреса шлюза\n");
        return 1;
    }
    
    if (!parse_ip(argv[2], &mask)) {
        printf("Ошибка: неверный формат маски подсети\n");
        return 1;
    }
    
    packet_count = atoi(argv[3]);
    if (packet_count <= 0) {
        printf("Ошибка: количество пакетов должно быть положительным числом\n");
        return 1;
    }
    
    srand((unsigned int)time(NULL));
    
    printf("=== Имитация отправки пакетов ===\n");
    printf("IP шлюза: ");
    print_ip(&gateway);
    printf("\nМаска подсети: ");
    print_ip(&mask);
    printf("\nКоличество пакетов: %d\n\n", packet_count);
    
    int same_subnet_count = 0;
    int other_subnet_count = 0;
    ip_addr_t dest_ip;
    
    printf("Обработка пакетов:\n");
    for (int i = 0; i < packet_count; i++) {
        generate_random_ip(&dest_ip);
        
        printf("Пакет %d: IP назначения ", i + 1);
        print_ip(&dest_ip);
        
        if (is_same_subnet(&gateway, &mask, &dest_ip)) {
            printf(" - СВОЯ подсеть\n");
            same_subnet_count++;
        } else {
            printf(" - ДРУГАЯ подсеть\n");
            other_subnet_count++;
        }
    }
    
    printf("\n=== Статистика ===\n");
    printf("Пакеты для своей подсети: %d шт. (%.2f%%)\n", 
           same_subnet_count, 
           (double)same_subnet_count / packet_count * 100);
    printf("Пакеты для других сетей: %d шт. (%.2f%%)\n", 
           other_subnet_count, 
           (double)other_subnet_count / packet_count * 100);
    
    return 0;
}