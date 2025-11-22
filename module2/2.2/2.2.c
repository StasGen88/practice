#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double calculate(double a, double b, char operator) {
    switch (operator)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;

    case '/':
    if (b==0) {
        printf("ошибка  /0\n");
        return NAN;
    }
        return a/b;
    default:
    printf("ошибка\n");
        return NAN;
    }
}

int main() {
    double num1, num2, result;
    char operation;
    while (1)
    {
      printf("Введите операцию");
      scanf(" %c", &operation);
      printf("Введите 2 числа \n");
      scanf("%lf %lf", &num1, &num2);
      result = calculate(num1, num2, operation);
      printf("результат\n: %lf", result);
      break;
    }
    
    
    return 0;
}