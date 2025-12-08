#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_NAME_LEN 20
#define MAX_NUMBER_LEN 20
#define MAX_PHONES 5

typedef struct {
    char *numbers[MAX_PHONES];
    int count;
} PhoneNumber;

typedef struct Contact {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int id;

    PhoneNumber phone;

    struct Contact *prev;
    struct Contact *next;
} Contact;

extern Contact *head;
extern Contact *tail;
extern int nextID;

void clearBuffer();
void freePhoneNumber(PhoneNumber *phone);
void inputString(char *buf, int len, const char *prompt);
void inputPhoneNumber(PhoneNumber *phone);
int compareContacts(Contact *a, Contact *b);
void insertSorted(Contact *c);
void addContact();
Contact *findByID(int id);
void showContact(Contact *c);
void listContacts();
void deleteContact();
void editContact();
void menu();

#endif