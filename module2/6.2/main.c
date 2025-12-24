#include "contact.h"

int main() {
    ContactList *list = createContactList();
    menu(list);
    destroyContactList(list);
    return 0;
}