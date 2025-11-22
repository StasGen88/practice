#ifndef CONTACT_H
#define CONTACT_H

#include "phone.h"

#define MAX_NAME_LANE 20
#define MAX_CONTACTS 20

extern int contactCount;

typedef struct {
    char firstName[MAX_NAME_LANE];
    char lastName[MAX_NAME_LANE];
    int id;
    PhoneNumber phone;
} Contact;

extern Contact contacts[MAX_CONTACTS];

void inputString(char* buffer, int maxLen);
void addContact();
void ShowContact(int id);
void ListContacts();
void DeleteContact(int id);
void EditContact(int id);
void cleanup();
void showMenu();

#endif