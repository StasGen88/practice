#ifndef PHONE_H
#define PHONE_H

#define MAX_NUMBERLENGTH 11
#define MAX_PHONES 5

typedef struct {
    char* numbers[MAX_PHONES];
    int count;
} PhoneNumber;

void clearBuffer();
void freePhoneNumber(PhoneNumber* phone);
void inputPhoneNumber(PhoneNumber* phone);

#endif