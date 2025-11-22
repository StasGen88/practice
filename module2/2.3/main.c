#include <stdio.h>
#include <stdlib.h>
#include "operations.h"
#include "operation_manager.h"

int main() {
    add_operation("Сложение", add);
    add_operation("Вычитание", subtract);
    add_operation("Умножение", multiply);
    add_operation("Деление", divide);
    
    int choice;
    double a, b;
    
    while (1) {
        show_menu();
        printf("Для выхода: -1\n");
        scanf("%d", &choice);
        
        if (choice == -1) break;
        
        printf("Введите два числа: ");
        scanf("%lf %lf", &a, &b);
        
        double (*operation)(double, double) = select_operation(choice);
        double result = operation(a, b);
        
        printf("Результат %s: %.2f\n\n", get_operation_name(choice), result);
    }
    
    cleanup_operations();
    return 0;
}