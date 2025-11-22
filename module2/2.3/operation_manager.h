#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H

typedef struct {
    char name[20];
    double (*func)(double, double);
} Operation;

void add_operation(const char* name, double (*func)(double, double));
double (*select_operation(int choice))(double, double);
void show_menu(void);
void cleanup_operations(void);
const char* get_operation_name(int index);

#endif