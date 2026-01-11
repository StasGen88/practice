#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H

typedef struct {
    char name[32];
    double (*func)(double, double);
    void* handle;
} Operation;

void load_operations_from_dir(const char* path);
double (*select_operation(int choice))(double, double);
void show_menu(void);
void cleanup_operations(void);
const char* get_operation_name(int index);

#endif
