#include <stdio.h>
#include <stdlib.h>
#include "operation_manager.h"

int main() {
    load_operations_from_dir("./libs");

    int choice;
    double a, b;

    while (1) {
        show_menu();
        printf("Для выхода: -1\n");
        scanf("%d", &choice);

        if (choice == -1) break;

        printf("Введите два числа: ");
        scanf("%lf %lf", &a, &b);

        double (*op)(double, double) = select_operation(choice);
        if (!op) continue;

        double result = op(a, b);
        printf("Результат %s: %.2f\n\n", get_operation_name(choice), result);
    }

    cleanup_operations();
    return 0;
}
