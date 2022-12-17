#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

typedef struct __queue {
    int head;
    int tail;
    int capacity;
    int *data;
} Queue;

void QueueInit(Queue *q, int n)
{
    q->data = malloc(n * sizeof(int));
    if (q->data == NULL) {
        printf("Alloc failed!\n");
        exit(1);
    }
    q->capacity = n - 1;
    q->head = 0;
    q->tail = 0;
}

int QueueFull(Queue *q)
{
    if (q->head == q->tail + 1) {
        return TRUE;
    }
    return FALSE;
}

int QueueEmpty(Queue *q)
{
    if (q->head == q->tail) {
        return TRUE;
    }
    return FALSE;
}

void Enqueue(Queue *q, int x)
{
    if (QueueFull(q)) {
        printf("Overflow!\n");
        exit(1);
    }

    q->data[q->tail] = x;
    if (q->tail == q->capacity) {
        q->tail = 0;
    } else {
        ++q->tail;
    }
}

int Dequeue(Queue *q)
{
    if (QueueEmpty(q)) {
        printf("Underflow!\n");
        exit(1);
    }

    int x = q->data[q->head];
    if (q->head == q->capacity) {
        q->head = 0;
    } else {
        ++q->head;
    }

    return x;
}

int main()
{
    Queue q;
    QueueInit(&q, 5);
    printf("head = %d, tail = %d, cap = %d\n\n", q.head, q.tail, q.capacity);

    Enqueue(&q, 1);
    printf("head = %d, tail = %d\n", q.head, q.tail);
    Enqueue(&q, 2);
    printf("head = %d, tail = %d\n", q.head, q.tail);
    Enqueue(&q, 3);
    printf("head = %d, tail = %d\n", q.head, q.tail);
    Enqueue(&q, 4);
    printf("head = %d, tail = %d\n", q.head, q.tail);
    //Enqueue(&q, 5);
    //printf("head = %d, tail = %d\n", q.head, q.tail);

    printf("%d\n", Dequeue(&q));
    printf("%d\n", Dequeue(&q));
    printf("%d\n", Dequeue(&q));
    printf("%d\n", Dequeue(&q));
    printf("%d\n", Dequeue(&q));
}