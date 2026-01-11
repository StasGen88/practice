#include <math.h>
double operation(double a, double b) {
    return b == 0 ? NAN : a / b;
}
const char* operation_name() {
    return "Деление";
}
