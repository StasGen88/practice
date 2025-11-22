#include <stdio.h>
#include <math.h>
#include "operations.h"

double add(double x, double y) { 
    return x + y; 
}

double subtract(double x, double y) { 
    return x - y; 
}

double multiply(double x, double y) { 
    return x * y; 
} 

double divide(double x, double y) { 
    if (y == 0) {
        printf("Ошибка деления на 0 \n");
        return NAN;
    } else {
        return x / y; 
    }
}