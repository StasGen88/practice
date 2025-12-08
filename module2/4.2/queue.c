#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_PRIORITY 256

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *queues[MAX_PRIORITY];
} PriorityQueue;

void pq_init(PriorityQueue *pq) {
    for (int i = 0; i < MAX_PRIORITY; i++)
        pq->queues[i] = NULL;
}

void pq_push(PriorityQueue *pq, int value, uint8_t prio) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;

    if (pq->queues[prio] == NULL) {
        pq->queues[prio] = node;
    } else {
        Node *cur = pq->queues[prio];
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
}

int pq_pop_first(PriorityQueue *pq, int *ok) {
    for (int pr = MAX_PRIORITY - 1; pr >= 0; pr--) {
        if (pq->queues[pr]) {
            Node *node = pq->queues[pr];
            pq->queues[pr] = node->next;
            int val = node->value;
            free(node);
            *ok = 1;
            return val;
        }
    }
    *ok = 0;
    return 0;
}

int pq_pop_priority(PriorityQueue *pq, uint8_t pr, int *ok) {
    if (!pq->queues[pr]) {
        *ok = 0;
        return 0;
    }
    Node *node = pq->queues[pr];
    pq->queues[pr] = node->next;
    int val = node->value;
    free(node);
    *ok = 1;
    return val;
}

int pq_pop_min_priority(PriorityQueue *pq, uint8_t min_pr, int *ok) {
    for (int pr = MAX_PRIORITY - 1; pr >= min_pr; pr--) {
        if (pq->queues[pr]) {
            Node *node = pq->queues[pr];
            pq->queues[pr] = node->next;
            int val = node->value;
            free(node);
            *ok = 1;
            return val;
        }
    }
    *ok = 0;
    return 0;
}

void pq_print(PriorityQueue *pq) {
    printf("Содержимое очереди:\n");
    for (int pr = MAX_PRIORITY - 1; pr >= 0; pr--) {
        if (pq->queues[pr]) {
            printf("  Приоритет %3d: ", pr);
            Node *cur = pq->queues[pr];
            while (cur) {
                printf("%d ", cur->value);
                cur = cur->next;
            }
            printf("\n");
        }
    }
}

void pq_generate(PriorityQueue *pq) {
    int count, minVal, maxVal, minPr, maxPr;

    printf("Сколько элементов сгенерировать? ");
    scanf("%d", &count);

    printf("Диапазон значений (min max): ");
    scanf("%d %d", &minVal, &maxVal);

    printf("Диапазон приоритетов (0–255): ");
    scanf("%d %d", &minPr, &maxPr);

    if (minPr < 0) minPr = 0;
    if (maxPr > 255) maxPr = 255;
    if (minVal > maxVal || minPr > maxPr) {
        printf("Неверный диапазон!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        int val = minVal + rand() % (maxVal - minVal + 1);
        int pr = minPr + rand() % (maxPr - minPr + 1);
        pq_push(pq, val, (uint8_t)pr);
    }

    printf("Сгенерировано %d элементов.\n", count);
}

void print_menu() {
    printf("\n====== МЕНЮ ======\n");
    printf("1 — Добавить элемент\n");
    printf("2 — Извлечь элемент с максимальным приоритетом\n");
    printf("3 — Извлечь элемент по точному приоритету\n");
    printf("4 — Извлечь элемент с приоритетом >= заданного\n");
    printf("5 — Показать очередь\n");
    printf("6 — Сгенерировать случайные элементы\n");
    printf("0 — Выход\n");
    printf("Выберите пункт: ");
}

int main() {
    PriorityQueue pq;
    pq_init(&pq);
    srand(time(NULL));

    int running = 1;

    while (running) {
        print_menu();
        int cmd;
        scanf("%d", &cmd);

        if (cmd == 1) {
            int value, prio;
            printf("Введите значение: ");
            scanf("%d", &value);
            printf("Введите приоритет (0–255): ");
            scanf("%d", &prio);
            if (prio < 0 || prio > 255) {
                printf("Ошибка: приоритет вне диапазона!\n");
                continue;
            }
            pq_push(&pq, value, (uint8_t)prio);
            printf("Добавлено.\n");
        }

        else if (cmd == 2) {
            int ok;
            int v = pq_pop_first(&pq, &ok);
            if (ok) printf("Извлечено: %d\n", v);
            else printf("Очередь пустая.\n");
        }

        else if (cmd == 3) {
            int pr;
            printf("Введите приоритет: ");
            scanf("%d", &pr);
            int ok;
            int v = pq_pop_priority(&pq, (uint8_t)pr, &ok);
            if (ok) printf("Извлечено: %d\n", v);
            else printf("Нет элементов с таким приоритетом.\n");
        }

        else if (cmd == 4) {
            int pr;
            printf("Введите минимальный приоритет: ");
            scanf("%d", &pr);
            int ok;
            int v = pq_pop_min_priority(&pq, (uint8_t)pr, &ok);
            if (ok) printf("Извлечено: %d\n", v);
            else printf("Нет элементов с приоритетом >= %d.\n", pr);
        }

        else if (cmd == 5) {
            pq_print(&pq);
        }

        else if (cmd == 6) {
            pq_generate(&pq);
        }

        else if (cmd == 0) {
            running = 0;
        }

        else {
            printf("Неизвестная команда!\n");
        }
    }

    printf("Программа завершена.\n");
    return 0;
}