#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phone.h"

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freePhoneNumber(PhoneNumber* phone) {
    for (int i = 0; i < phone->count; i++) {
        free(phone->numbers[i]);
        phone->numbers[i] = NULL;
    }
    phone->count = 0;
}

void inputPhoneNumber(PhoneNumber* phone) {
    freePhoneNumber(phone);
    
    char choice;
    printf("Хотите добавить телефон? (y/n): ");
    scanf(" %c", &choice);
    clearBuffer();

    int i = 0;
    while ((choice == 'y' || choice == 'Y') && i < MAX_PHONES) {
        printf("Введите номер телефона %d: ", i + 1);
        char temp[MAX_NUMBERLENGTH];
        fgets(temp, MAX_NUMBERLENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strlen(temp) > 0) {
            phone->numbers[i] = (char*)malloc((strlen(temp)+1));
            if (phone->numbers[i] == NULL) {
                printf("Ошибка выделения памяти \n");
                return;
            }
            strcpy(phone->numbers[i], temp);
            phone->count++;
            i++;
        }
        
        if (i < MAX_PHONES) {
            printf("Добавить еще один номер? (y/n): ");
            scanf(" %c", &choice);
            clearBuffer();
        } else {
            printf("Достигнуто максимальное количество номеров (%d)\n", MAX_PHONES);
        }
    }
    
    for (int j = i; j < MAX_PHONES; j++) {
        phone->numbers[j] = NULL;
    }
}