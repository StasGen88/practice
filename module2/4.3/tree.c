#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

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