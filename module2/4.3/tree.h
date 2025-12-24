#ifndef TREE_H
#define TREE_H

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

extern TreeNode* root;
extern int globalID;
extern int insertCount;

void clearBuffer();
void freePhone(PhoneNumber* p);
void inputPhone(PhoneNumber* phone);
int compareContacts(const Contact* a, const Contact* b);
TreeNode* createNode(Contact c);
TreeNode* insertNode(TreeNode* node, Contact c);
TreeNode* searchByID(TreeNode* node, int id);
void inorderPrint(TreeNode* node);
void treeToArray(TreeNode* node, Contact arr[], int* idx);
TreeNode* buildBalanced(Contact arr[], int L, int R);
void freeTree(TreeNode* node);
void balanceTree();

#endif