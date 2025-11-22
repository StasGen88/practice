#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define PERMISSION_SIZE 10

mode_t current_mode = 0000;

void print_permissions(mode_t mode) {
    char letters[10];
    char octal[4];
    char binary[10];
  
    letters[0] = (mode & S_IRUSR) ? 'r' : '-';
    letters[1] = (mode & S_IWUSR) ? 'w' : '-';
    letters[2] = (mode & S_IXUSR) ? 'x' : '-';
    letters[3] = (mode & S_IRGRP) ? 'r' : '-';
    letters[4] = (mode & S_IWGRP) ? 'w' : '-';
    letters[5] = (mode & S_IXGRP) ? 'x' : '-';
    letters[6] = (mode & S_IROTH) ? 'r' : '-';
    letters[7] = (mode & S_IWOTH) ? 'w' : '-';
    letters[8] = (mode & S_IXOTH) ? 'x' : '-';
    letters[9] = '\0';
    
    int owner = ((mode & S_IRUSR) ? 4 : 0) + ((mode & S_IWUSR) ? 2 : 0) + ((mode & S_IXUSR) ? 1 : 0);
    int group = ((mode & S_IRGRP) ? 4 : 0) + ((mode & S_IWGRP) ? 2 : 0) + ((mode & S_IXGRP) ? 1 : 0);
    int other = ((mode & S_IROTH) ? 4 : 0) + ((mode & S_IWOTH) ? 2 : 0) + ((mode & S_IXOTH) ? 1 : 0);
    sprintf(octal, "%d%d%d", owner, group, other);
    
    for (int i = 0; i < 9; i++) {
       binary[8-i] = (mode & (1 << i)) ? '1' : '0';
    }
    binary[9] = '\0';
    
    printf("Буквенное: %s\n", letters);
    printf("Цифровое:  %s\n", octal);
    printf("Битовое:   %s\n", binary);
}

void digitalToBits(int digital) {
    printf("Битовое представление: ");
    for (int i = 8; i >= 0; i--) {
        if (digital & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
        if (i % 3 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

void letterTobits(const char* permission) {
    if (strlen(permission) != 9) {
        printf("Ошибка: строка должна содержать 9 символов\n");
        return;
    }
    
    int result = 0;
    int bit_position = 8;
    
    for (int i = 0; i < 9; i++) {
        if (permission[i] != '-') {
            result |= (1 << bit_position);
        }
        bit_position--;
    }
    
    printf("Вычисленное значение: %d\n", result);
    printf("Восьмеричное представление: %o\n", result);
    digitalToBits(result);
}

void permissionTransfer() {
    char permission[PERMISSION_SIZE];
    printf("Введите права доступа (число или rwxrwxrwx): ");
    scanf("%9s", permission);
    
    if (isdigit(permission[0])) {
        int digital = 0;
        sscanf(permission, "%o", &digital);
        printf("Цифровой ввод: %s\n", permission);
        digitalToBits(digital);
    } else {
        printf("Буквенный ввод: %s\n", permission);
        letterTobits(permission);
    }
    printf("%s\n", permission);
}

void apply_chmod_command(const char* command) {
    mode_t new_mode = current_mode;
    
    if (isdigit(command[0])) {
        int mode_val;
        if (sscanf(command, "%o", &mode_val) == 1) {
            new_mode = mode_val;
            printf("Применена цифровая команда: %s\n", command);
        } else {
            printf("Ошибка: неверный цифровой формат\n");
            return;
        }
    }

    else {
        char *cmd = strdup(command);
        char *saveptr;
        char *token = strtok_r(cmd, ",", &saveptr);
        
        while (token != NULL) {
            mode_t who = 0;
            char *ptr = token;
            
            while (*ptr && strchr("ugoa", *ptr)) {
                switch (*ptr) {
                    case 'u': who |= S_IRWXU; break;
                    case 'g': who |= S_IRWXG; break;
                    case 'o': who |= S_IRWXO; break;
                    case 'a': who |= S_IRWXU | S_IRWXG | S_IRWXO; break;
                }
                ptr++;
            }
            
            if (who == 0) who = S_IRWXU | S_IRWXG | S_IRWXO;
            
            char op = *ptr++;
            mode_t perm = 0;
            
            while (*ptr && strchr("rwx", *ptr)) {
                switch (*ptr) {
                    case 'r': perm |= S_IRUSR | S_IRGRP | S_IROTH; break;
                    case 'w': perm |= S_IWUSR | S_IWGRP | S_IWOTH; break;
                    case 'x': perm |= S_IXUSR | S_IXGRP | S_IXOTH; break;
                }
                ptr++;
            }
            
            switch (op) {
                case '+':
                    new_mode |= (perm & who);
                    break;
                case '-':
                    new_mode &= ~(perm & who);
                    break;
                case '=':
                    new_mode &= ~who;
                    new_mode |= (perm & who);
                    break;
                default:
                    printf("Ошибка: неизвестная операция '%c'\n", op);
                    free(cmd);
                    return;
            }
            
            token = strtok_r(NULL, ",", &saveptr);
        }
        free(cmd);
        printf("Применена символьная команда: %s\n", command);
    }
    
    current_mode = new_mode;
    printf("\nНовые права доступа:\n");
    print_permissions(current_mode);
}

void chmod_interface() {
    char command[100];
    
    printf("\n=== ИНТЕРФЕЙС ИЗМЕНЕНИЯ ПРАВ ДОСТУПА (chmod) ===\n");
    printf("Текущие права доступа:\n");
    print_permissions(current_mode);
    printf("\n");
    
    while (1) {
        printf("Введите команду chmod (например, '755', 'u+x', 'go-w', 'a=r')\n");
        printf("или 'q' для выхода: ");
        
        if (scanf("%99s", command) != 1) {
            break;
        }
        
        if (strcmp(command, "q") == 0) {
            break;
        }
        
        apply_chmod_command(command);
        printf("\n");
    }
}

void menu() {
    int choice;
    
    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Расчёт маски\n");
        printf("2. Анализ прав доступа файла\n");
        printf("3. Интерфейс изменения прав доступа (chmod)\n");
        printf("4. Выход\n");
        printf("Выберите опцию: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: 
                {
                    permissionTransfer();
                    break;
                }
            case 2:
                {
                    char filename[256];
                    struct stat file_stat;
                    
                    printf("Введите имя файла: ");
                    scanf("%255s", filename);
                    
                    if (stat(filename, &file_stat) == -1) {
                        perror("Ошибка");
                    } else {
                        printf("\nПрава доступа для '%s':\n", filename);
                        print_permissions(file_stat.st_mode);
                    }
                }
                break;
                
            case 3:
                chmod_interface();
                break;
                
            case 4:
                printf("Выход...\n");
                break;
                
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 4);
}

int main() {
    menu();
    return 0;
}