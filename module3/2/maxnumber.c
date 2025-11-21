#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Введите больше цифр\n");
        return 1;
    }

    double maxNumber = atof(argv[1]);
    for (int i = 2; i < argc; i++) {
        double current = atof(argv[i]);
        if (current > maxNumber) {
            maxNumber = current;
        }
    }
    printf("Максимально число: %0.2f \n", maxNumber);
    return 0;
}