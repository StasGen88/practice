#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void addContact() {
    Contact c = {0};
    c.id = globalID++;

    printf("Имя: ");
    fgets(c.firstName, MAX_NAME, stdin);
    c.firstName[strcspn(c.firstName, "\n")] = '\0';

    printf("Фамилия: ");
    fgets(c.lastName, MAX_NAME, stdin);
    c.lastName[strcspn(c.lastName, "\n")] = '\0';

    inputPhone(&c.phone);

    root = insertNode(root, c);
    insertCount++;

    if (insertCount % 5 == 0) {
        balanceTree();
    }
}

void editContact(int id) {
    TreeNode* node = searchByID(root, id);
    if (!node) {
        printf("Контакт не найден.\n");
        return;
    }

    printf("Новое имя (enter = без изменений): ");
    char buff[MAX_NAME];
    fgets(buff, MAX_NAME, stdin);
    buff[strcspn(buff, "\n")] = '\0';
    if (strlen(buff)>0) strcpy(node->data.firstName, buff);

    printf("Новая фамилия: ");
    fgets(buff, MAX_NAME, stdin);
    buff[strcspn(buff, "\n")] = '\0';
    if (strlen(buff)>0) strcpy(node->data.lastName, buff);

    printf("Изменить телефоны? (y/n): ");
    char ch; scanf(" %c", &ch); clearBuffer();
    if (ch=='y'||ch=='Y')
        inputPhone(&node->data.phone);

    printf("Готово!\n");
}

void deleteContact(int id) {
    printf("Удаление контактов в дереве полностью (проще всего)\n");
  
    Contact arr[100];
    int n = 0;
    treeToArray(root, arr, &n);

    int pos = -1;
    for (int i=0;i<n;i++)
        if (arr[i].id == id) pos = i;

    if (pos == -1) {
        printf("Контакт не найден.\n");
        return;
    }

    freePhone(&arr[pos].phone);

    for (int i=pos; i<n-1; i++)
        arr[i] = arr[i+1];
    n--;

    freeTree(root);
    root = buildBalanced(arr, 0, n-1);

    printf("Контакт удалён.\n");
}

void showMenu() {
    int ch;

    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Добавить контакт\n");
        printf("2. Показать все контакты\n");
        printf("3. Редактировать контакт\n");
        printf("4. Удалить контакт\n");
        printf("5. Балансировать дерево вручную\n");
        printf("6. Выход\n");
        printf("Выбор: ");
        scanf("%d", &ch); clearBuffer();

        if (ch==1) addContact();
        else if (ch==2) inorderPrint(root);
        else if (ch==3) {
            int id; printf("ID: "); scanf("%d",&id); clearBuffer();
            editContact(id);
        }
        else if (ch==4) {
            int id; printf("ID: "); scanf("%d",&id); clearBuffer();
            deleteContact(id);
        }
        else if (ch==5) balanceTree();

    } while (ch != 6);
}

int main() {
    showMenu();
    freeTree(root);
    return 0;
}