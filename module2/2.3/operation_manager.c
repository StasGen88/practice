#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation_manager.h"
#include "operations.h"

static Operation* operations = NULL;
static int operation_count = 0;
static int capacity = 0;

void add_operation(const char* name, double (*func)(double, double)) {
    if (operation_count >= capacity) {
        capacity += 5;
        operations = realloc(operations, capacity * sizeof(Operation));
    }
    
    strncpy(operations[operation_count].name, name, 19);
    operations[operation_count].name[19] = '\0';
    operations[operation_count].func = func;
    operation_count++;
}

double (*select_operation(int choice))(double, double) {
    if (choice >= 0 && choice < operation_count) {
        return operations[choice].func;
    }
    return add;
}

void show_menu() {
    printf("Доступные операции:\n");
    for (int i = 0; i < operation_count; i++) {
        printf("%d. %s\n", i, operations[i].name);
    }
}

void cleanup_operations() {
    free(operations);
    operations = NULL;
    operation_count = 0;
    capacity = 0;
}

const char* get_operation_name(int index) {
    if (index >= 0 && index < operation_count) {
        return operations[index].name;
    }
    return "Неизвестная операция";
}