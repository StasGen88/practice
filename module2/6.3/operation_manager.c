#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "operation_manager.h"

static Operation* operations = NULL;
static int count = 0;

void load_operations_from_dir(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* entry;

    while ((entry = readdir(dir))) {
        if (!strstr(entry->d_name, ".so")) continue;

        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        void* handle = dlopen(fullpath, RTLD_LAZY);
        if (!handle) continue;

        const char* (*name_func)() = dlsym(handle, "operation_name");
        double (*op_func)(double, double) = dlsym(handle, "operation");

        if (!name_func || !op_func) {
            dlclose(handle);
            continue;
        }

        operations = realloc(operations, (count + 1) * sizeof(Operation));
        strncpy(operations[count].name, name_func(), 31);
        operations[count].func = op_func;
        operations[count].handle = handle;
        count++;
    }
    closedir(dir);
}

double (*select_operation(int choice))(double, double) {
    if (choice >= 0 && choice < count)
        return operations[choice].func;
    return NULL;
}

void show_menu() {
    for (int i = 0; i < count; i++)
        printf("%d. %s\n", i, operations[i].name);
}

void cleanup_operations() {
    for (int i = 0; i < count; i++)
        dlclose(operations[i].handle);
    free(operations);
}

const char* get_operation_name(int index) {
    if (index >= 0 && index < count)
        return operations[index].name;
    return "";
}
