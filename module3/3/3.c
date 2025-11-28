#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_NAME_LEN 20
#define MAX_CONTACTS 20
#define MAX_NUMBER_LEN 11
#define MAX_PHONES 5

typedef struct {
    char numbers[MAX_PHONES][MAX_NUMBER_LEN];
    int count;
} PhoneNumber;

typedef struct {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int id;
    PhoneNumber phone;
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inputString(char* buffer, int maxLen, const char* prompt) {
    printf("%s", prompt);
    fgets(buffer, maxLen, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void inputPhoneNumber(PhoneNumber* phone) {
    phone->count = 0;

    char choice;
    printf("Добавить телефон? (y/n): ");
    scanf(" %c", &choice);
    clearBuffer();

    while ((choice == 'y' || choice == 'Y') && phone->count < MAX_PHONES) {
        printf("Введите номер %d: ", phone->count + 1);
        fgets(phone->numbers[phone->count], MAX_NUMBER_LEN, stdin);
        phone->numbers[phone->count][strcspn(phone->numbers[phone->count], "\n")] = '\0';

        phone->count++;

        if (phone->count < MAX_PHONES) {
            printf("Добавить ещё номер? (y/n): ");
            scanf(" %c", &choice);
            clearBuffer();
        }
    }
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf("Достигнут предел количества контактов\n");
        return;
    }

    Contact newContact;

    inputString(newContact.firstName, MAX_NAME_LEN, "Введите имя: ");
    inputString(newContact.lastName, MAX_NAME_LEN, "Введите фамилию: ");

    inputPhoneNumber(&newContact.phone);

    newContact.id = contactCount;

    contacts[contactCount] = newContact;
    contactCount++;

    printf("Контакт добавлен!\n");
}

void showContact(int id) {
    if (id < 0 || id >= contactCount || contacts[id].firstName[0] == '\0') {
        printf("Контакт не найден!\n");
        return;
    }

    printf("\nID: %d\nИмя: %s\nФамилия: %s\n", contacts[id].id,
           contacts[id].firstName, contacts[id].lastName);

    if (contacts[id].phone.count == 0) {
        printf("Телефоны: нет\n");
    } else {
        printf("Телефоны (%d):\n", contacts[id].phone.count);
        for (int i = 0; i < contacts[id].phone.count; i++) {
            printf(" %d. %s\n", i + 1, contacts[id].phone.numbers[i]);
        }
    }
}

void listContacts() {
    if (contactCount == 0) {
        printf("Список контактов пуст\n");
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        if (contacts[i].firstName[0] != '\0')
            showContact(i);
    }
}

void deleteContact() {
    printf("Введите ID контакта: ");
    int id;
    scanf("%d", &id);
    clearBuffer();

    if (id < 0 || id >= contactCount) {
        printf("Неверный ID\n");
        return;
    }

    contacts[id].firstName[0] = '\0';
    contacts[id].lastName[0] = '\0';
    contacts[id].phone.count = 0;

    printf("Контакт удалён\n");
}

void editContact() {
    printf("Введите ID контакта: ");
    int id;
    scanf("%d", &id);
    clearBuffer();

    if (id < 0 || id >= contactCount || contacts[id].firstName[0] == '\0') {
        printf("Контакт не найден!\n");
        return;
    }

    inputString(contacts[id].firstName, MAX_NAME_LEN, "Введите новое имя: ");
    inputString(contacts[id].lastName, MAX_NAME_LEN, "Введите новую фамилию: ");

    printf("Изменить телефоны? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    clearBuffer();

    if (choice == 'y' || choice == 'Y')
        inputPhoneNumber(&contacts[id].phone);

    printf("Контакт обновлён\n");
}


void saveContactsToFile() {
    int fd = open("contacts.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return;
    }

    write(fd, &contactCount, sizeof(int));
    write(fd, contacts, sizeof(Contact) * contactCount);

    close(fd);
}



void loadContactsFromFile() {
    int fd = open("contacts.dat", O_RDONLY);
    if (fd == -1) {
        contactCount = 0;
        return;
    }

    read(fd, &contactCount, sizeof(int));
    read(fd, contacts, sizeof(Contact) * contactCount);

    close(fd);
}

void showMenu() {
    int choice;

    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать все контакты\n");
        printf("3. Удалить контакт\n");
        printf("4. Изменить контакт\n");
        printf("5. Выход\n");
        printf("Выберите действие: ");

        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1: addContact(); break;
        case 2: listContacts(); break;
        case 3: deleteContact(); break;
        case 4: editContact(); break;
        case 5: printf("Выход...\n"); break;
        default: printf("Неверный выбор\n");
        }

    } while (choice != 5);
}


int main() {
    loadContactsFromFile();
    showMenu();
    saveContactsToFile();   
    return 0;
}
