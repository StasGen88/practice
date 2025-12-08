#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 20
#define MAX_NUMBER 11
#define MAX_PHONES 5

typedef struct {
    char* numbers[MAX_PHONES];
    int count;
} PhoneNumber;

typedef struct {
    int id;
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    PhoneNumber phone;
} Contact;

typedef struct TreeNode {
    Contact data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* root = NULL;
int globalID = 0;
int insertCount = 0;

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void freePhone(PhoneNumber* p) {
    for (int i = 0; i < p->count; i++) {
        free(p->numbers[i]);
    }
    p->count = 0;
}

void inputPhone(PhoneNumber* phone) {
    freePhone(phone);
    phone->count = 0;

    char choice;
    printf("Добавить телефон? (y/n): ");
    scanf(" %c", &choice); clearBuffer();
    
    while ((choice=='y'||choice=='Y') && phone->count < MAX_PHONES) {
        char temp[MAX_NUMBER];
        printf("Введите номер %d: ", phone->count + 1);
        fgets(temp, MAX_NUMBER, stdin);
        temp[strcspn(temp, "\n")] = '\0';

        phone->numbers[phone->count] = malloc(strlen(temp)+1);
        strcpy(phone->numbers[phone->count], temp);
        phone->count++;

        if (phone->count < MAX_PHONES) {
            printf("Добавить еще? (y/n): ");
            scanf(" %c", &choice); clearBuffer();
        }
    }
}

int compareContacts(const Contact* a, const Contact* b) {
    int last = strcmp(a->lastName, b->lastName);
    if (last != 0) return last;
    return strcmp(a->firstName, b->firstName);
}

TreeNode* createNode(Contact c) {
    TreeNode* n = malloc(sizeof(TreeNode));
    n->data = c;
    n->left = n->right = NULL;
    return n;
}

TreeNode* insertNode(TreeNode* node, Contact c) {
    if (!node) return createNode(c);

    if (compareContacts(&c, &node->data) < 0)
        node->left = insertNode(node->left, c);
    else
        node->right = insertNode(node->right, c);

    return node;
}

TreeNode* searchByID(TreeNode* node, int id) {
    if (!node) return NULL;
    if (node->data.id == id) return node;

    TreeNode* left = searchByID(node->left, id);
    if (left) return left;

    return searchByID(node->right, id);
}

void inorderPrint(TreeNode* node) {
    if (!node) return;
    inorderPrint(node->left);
    printf("ID: %d | %s %s\n", node->data.id, node->data.firstName, node->data.lastName);
    for (int i=0;i<node->data.phone.count;i++)
        printf("   Тел %d: %s\n", i+1, node->data.phone.numbers[i]);
    inorderPrint(node->right);
}

void treeToArray(TreeNode* node, Contact arr[], int* idx) {
    if (!node) return;
    treeToArray(node->left, arr, idx);
    arr[(*idx)++] = node->data;
    treeToArray(node->right, arr, idx);
}

TreeNode* buildBalanced(Contact arr[], int L, int R) {
    if (L > R) return NULL;
    int mid = (L + R)/2;
    TreeNode* node = createNode(arr[mid]);
    node->left = buildBalanced(arr, L, mid-1);
    node->right = buildBalanced(arr, mid+1, R);
    return node;
}

void freeTree(TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    freePhone(&node->data.phone);
    free(node);
}

void balanceTree() {
    int n = 0;
    Contact arr[100];

    treeToArray(root, arr, &n);
    freeTree(root);
    root = buildBalanced(arr, 0, n-1);

    printf("Дерево сбалансировано! (всего %d элементов)\n", n);
}

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
