#include "contact.h"

void menu() {
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
            case 1: addContact(); break;
            case 2: listContacts(); break;
            case 3: deleteContact(); break;
            case 4: editContact(); break;
            case 5: printf("Выход...\n"); break;
            default: printf("Неверный выбор!\n");
        }
    } while (choice != 5);
}

int main() {
    menu();
    Contact *cur = head;
    while (cur != NULL) {
        Contact *next = cur->next;
        freePhoneNumber(&cur->phone);
        free(cur);
        cur = next;
    }
    
    return 0;
}