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

typedef struct {
    Contact *head;
    Contact *tail;
    int nextID;
} ContactList;

void initContactList(ContactList *list);
void freeContactList(ContactList *list);
void clearBuffer();
void freePhoneNumber(PhoneNumber *phone);
void inputString(char *buf, int len, const char *prompt);
void inputPhoneNumber(PhoneNumber *phone);
void insertSorted(ContactList *list, Contact *c);
Contact *findByID(const ContactList *list, int id);
void addContact(ContactList *list);
void listContacts(const ContactList *list);
void deleteContact(ContactList *list);
void editContact(ContactList *list);
void showContact(const Contact *c);
int compareContacts(const Contact *a, const Contact *b);

#endif