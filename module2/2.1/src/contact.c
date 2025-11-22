#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "contact.h"
#include "phone.h"

int contactCount = 0;
Contact contacts[MAX_CONTACTS];

void inputString(char* buffer, int maxLen) {
    printf("Введите текст: ");
    fgets(buffer, maxLen, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void addContact() {
    Contact newContact = {0};
    inputString(newContact.firstName, MAX_NAME_LANE);
    inputString(newContact.lastName, MAX_NAME_LANE);

    for (int i = 0; i < MAX_PHONES; i++) {
        newContact.phone.numbers[i] = NULL;
    }
    newContact.phone.count = 0;
    inputPhoneNumber(&newContact.phone);

    newContact.id = contactCount;
    contacts[contactCount] = newContact;
    contactCount++;

}

void ShowContact(int id) {
    if (id < 0 || id >= contactCount || contacts[id].firstName[0] == '\0') {
        printf("Контакт с ID %d не найден\n", id);
        return;
    }
    
    printf("ID: %d, Имя: %s, Фамилия: %s\n", 
           contacts[id].id, contacts[id].firstName, contacts[id].lastName);
    
    if (contacts[id].phone.count > 0) {
        printf("Телефонные номера (%d):\n", contacts[id].phone.count);
        for (int i = 0; i < contacts[id].phone.count; i++) {
            if (contacts[id].phone.numbers[i] != NULL) {
                printf("  %d. %s\n", i + 1, contacts[id].phone.numbers[i]);
            }
        }
    } else {
        printf("Телефонные номера: не указаны\n");
    }
}

void ListContacts() {
    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].firstName[0] != '\0') {
            ShowContact(i);
        }
    }
}
void DeleteContact(int id) {
    if (id < 0 || id >= contactCount) {
        printf("Неверный ID контакта\n");
        return;
    }
    
    contacts[id].firstName[0] = '\0';
    contacts[id].lastName[0] = '\0';
    freePhoneNumber(&contacts[id].phone);
    contacts[id].id = -1;
    printf("Контакт с ID %d удален\n", id);
}

void EditContact(int id) {
    
    clearBuffer();

    char newFirstName[MAX_NAME_LANE];
    if (fgets(newFirstName, MAX_NAME_LANE, stdin) != NULL) {
        newFirstName[strcspn(newFirstName, "\n")] = '\0';
        if (strlen(newFirstName) > 0) {
            strcpy(contacts[id].firstName, newFirstName);
        }
    }

    char newLastName[MAX_NAME_LANE];
    if (fgets(newLastName, MAX_NAME_LANE, stdin) != NULL) {
        newLastName[strcspn(newLastName, "\n")] = '\0';
        if (strlen(newLastName) > 0) {
            strcpy(contacts[id].lastName, newLastName);
        }
    }

    if (contacts[id].phone.count > 0) {
        for (int i = 0; i < contacts[id].phone.count; i++) {
            if (contacts[id].phone.numbers[i] != NULL) {
                printf("  %d. %s\n", i + 1, contacts[id].phone.numbers[i]);
            }
        }
    } else {
        printf("  Нет номеров\n");
    }
    
    printf("Хотите изменить телефонные номера? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    clearBuffer();
    
    if (choice == 'y' || choice == 'Y') {
        inputPhoneNumber(&contacts[id].phone);
    }

    printf("Контакт с ID %d успешно изменен\n", id);
}

void cleanup() {
    for (int i = 0; i < contactCount; i++) {
        freePhoneNumber(&contacts[i].phone);
    }
}

void showMenu() {
    for (int i = 0; i < MAX_CONTACTS; i++) {
        for (int j = 0; j < MAX_PHONES; j++) {
            contacts[i].phone.numbers[j] = NULL;
        }
        contacts[i].phone.count = 0;
    }

    int choice;
    do {
        printf("\n=== МЕНЮ КОНТАКТОВ ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать все контакты\n");
        printf("3. Удалить контакт\n");
        printf("4. Изменить контакт\n");
        printf("5. Выйти\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        clearBuffer();
        
        switch(choice) {
            case 1:
                if (contactCount >= MAX_CONTACTS) {
                    printf("Достигнут максимум контактов!\n");
                    break;
                }
                addContact();
                printf("Контакт добавлен с ID %d\n", contactCount - 1);
                break;
            case 2:
                ListContacts();
                break;
            case 3:
                printf("Введите ID для удаления: ");
                int idDelete;
                scanf("%d", &idDelete);
                DeleteContact(idDelete);
                break;
            case 4:
                printf("Введите ID для изменения: ");
                int idEdit;
                scanf("%d", &idEdit);
                if (idEdit < 0 || idEdit >= contactCount || contacts[idEdit].firstName[0] == '\0') {
                    printf("Контакт с ID %d не найден\n", idEdit);
                    break;
                }
                EditContact(idEdit);
                break;
            case 5:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор!\n");
        }
    } while(choice != 5);
    cleanup();
}