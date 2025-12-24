#include "contact.h"

void menu() {
    ContactList contactList;
    initContactList(&contactList);
    
    int choice;
    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать контакты\n");
        printf("3. Удалить контакт\n");
        printf("4. Изменить контакт\n");
        printf("5. Выход\n");
        printf("Выберите: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
            case 1: addContact(&contactList); break;
            case 2: listContacts(&contactList); break;
            case 3: deleteContact(&contactList); break;
            case 4: editContact(&contactList); break;
            case 5: printf("Выход...\n"); break;
            default: printf("Неверный выбор!\n");
        }
    } while (choice != 5);
    
    freeContactList(&contactList);
}

int main() {
    menu();
    return 0;
}