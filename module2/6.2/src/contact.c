#include "contact.h"

ContactList* createContactList() {
    ContactList *list = malloc(sizeof(ContactList));
    list->head = NULL;
    list->tail = NULL;
    list->nextID = 0;
    return list;
}

void destroyContactList(ContactList *list) {
    Contact *cur = list->head;
    while (cur != NULL) {
        Contact *next = cur->next;
        freePhoneNumber(&cur->phone);
        free(cur);
        cur = next;
    }
    free(list);
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freePhoneNumber(PhoneNumber *phone) {
    for (int i = 0; i < phone->count; i++) {
        free(phone->numbers[i]);
        phone->numbers[i] = NULL;
    }
    phone->count = 0;
}

void inputString(char *buf, int len, const char *prompt) {
    printf("%s", prompt);
    fgets(buf, len, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

void inputPhoneNumber(PhoneNumber *phone) {
    freePhoneNumber(phone);

    char choice;
    printf("Добавить телефон? (y/n): ");
    scanf(" %c", &choice);
    clearBuffer();

    int i = 0;
    while (tolower(choice) == 'y' && i < MAX_PHONES) {
        char temp[MAX_NUMBER_LEN];
        printf("Введите телефон %d: ", i + 1);
        fgets(temp, MAX_NUMBER_LEN, stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strlen(temp) > 0) {
            phone->numbers[i] = malloc(strlen(temp) + 1);
            strcpy(phone->numbers[i], temp);
            phone->count++;
            i++;
        }

        if (i < MAX_PHONES) {
            printf("Добавить ещё? (y/n): ");
            scanf(" %c", &choice);
            clearBuffer();
        }
    }
}

int compareContacts(Contact *a, Contact *b) {
    int c = strcmp(a->lastName, b->lastName);
    if (c != 0) return c;
    return strcmp(a->firstName, b->firstName);
}

void insertSorted(ContactList *list, Contact *c) {
    if (list->head == NULL) {
        list->head = list->tail = c;
        return;
    }
    Contact *cur = list->head;
    while (cur != NULL && compareContacts(cur, c) < 0)
        cur = cur->next;

    if (cur == list->head) {
        c->next = list->head;
        list->head->prev = c;
        list->head = c;
        return;
    }

    if (cur == NULL) {
        list->tail->next = c;
        c->prev = list->tail;
        list->tail = c;
        return;
    }

    c->next = cur;
    c->prev = cur->prev;
    cur->prev->next = c;
    cur->prev = c;
}

void addContact(ContactList *list) {
    Contact *c = malloc(sizeof(Contact));
    memset(c, 0, sizeof(Contact));

    inputString(c->firstName, MAX_NAME_LEN, "Введите имя: ");
    inputString(c->lastName, MAX_NAME_LEN, "Введите фамилию: ");

    inputPhoneNumber(&c->phone);

    c->id = list->nextID++;

    insertSorted(list, c);

    printf("Контакт добавлен (ID=%d)\n", c->id);
}

Contact *findByID(ContactList *list, int id) {
    Contact *cur = list->head;
    while (cur != NULL) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

void showContact(Contact *c) {
    printf("ID: %d, %s %s\n", c->id, c->firstName, c->lastName);
    if (c->phone.count == 0) {
        printf("  Телефоны: нет\n");
    } else {
        printf("  Телефоны:\n");
        for (int i = 0; i < c->phone.count; i++)
            printf("    %d. %s\n", i + 1, c->phone.numbers[i]);
    }
}

void listContacts(ContactList *list) {
    if (list->head == NULL) {
        printf("Список пуст\n");
        return;
    }
    Contact *cur = list->head;
    while (cur != NULL) {
        showContact(cur);
        cur = cur->next;
    }
}

void deleteContact(ContactList *list) {
    int id;
    printf("Введите ID для удаления: ");
    scanf("%d", &id);
    clearBuffer();

    Contact *c = findByID(list, id);
    if (!c) {
        printf("Контакт не найден\n");
        return;
    }

    if (c->prev) c->prev->next = c->next;
    else list->head = c->next;

    if (c->next) c->next->prev = c->prev;
    else list->tail = c->prev;

    freePhoneNumber(&c->phone);
    free(c);

    printf("Контакт удалён.\n");
}

void editContact(ContactList *list) {
    int id;
    printf("Введите ID для изменения: ");
    scanf("%d", &id);
    clearBuffer();

    Contact *c = findByID(list, id);
    if (!c) {
        printf("Контакт не найден\n");
        return;
    }

    printf("Новое имя (пусто — не менять): ");
    char temp[MAX_NAME_LEN];
    fgets(temp, MAX_NAME_LEN, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->firstName, temp);

    printf("Новая фамилия (пусто — не менять): ");
    fgets(temp, MAX_NAME_LEN, stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) strcpy(c->lastName, temp);

    printf("Изменить телефоны? (y/n): ");
    char ch;
    scanf(" %c", &ch);
    clearBuffer();

    if (tolower(ch) == 'y')
        inputPhoneNumber(&c->phone);

    if (c->prev) c->prev->next = c->next;
    else list->head = c->next;

    if (c->next) c->next->prev = c->prev;
    else list->tail = c->prev;

    c->prev = c->next = NULL;
    insertSorted(list, c);

    printf("Контакт обновлён.\n");
}

void menu(ContactList *list) {
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
            case 1: addContact(list); break;
            case 2: listContacts(list); break;
            case 3: deleteContact(list); break;
            case 4: editContact(list); break;
            case 5: printf("Выход...\n"); break;
            default: printf("Неверный выбор!\n");
        }
    } while (choice != 5);
}