#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

typedef struct __stack {
    int top;
    int capacity;
    int *data;
} Stack;

void StackInit(Stack *stk, int n)
{
    stk->data = malloc(n * sizeof(int));
    if (stk->data == NULL) {
        printf("Allocation failed!\n");
        exit(1);
    }
    stk->capacity = n;
    stk->top = 0;
}

int StackEmpty(Stack *stk)
{
    if (stk->top == 0) {
        return TRUE;
    }
    return FALSE;
}

int StackFull(Stack *stk)
{
    if (stk->top == stk->capacity) {
        return TRUE;
    }
    return FALSE;
}

void Push(Stack *stk, int x)
{
    if (StackFull(stk)) {
        printf("Overflow!\n");
        exit(1);
    }
    ++stk->top;
    stk->data[stk->top] = x;
}

int Pop(Stack *stk)
{
    if (StackEmpty(stk)) {
        printf("Underflow!\n");
        exit(1);
    } else {
        --stk->top;
        return stk->data[stk->top+1];
    }
}

int main()
{
    Stack stk;
    StackInit(&stk, 5);
    Push(&stk, 1);
    Push(&stk, 2);
    Push(&stk, 3);
    Push(&stk, 4);
    Push(&stk, 5);
    printf("%d\n", Pop(&stk));
    printf("%d\n", Pop(&stk));
    printf("%d\n", Pop(&stk));
    printf("%d\n", Pop(&stk));
    printf("%d\n", Pop(&stk));
    Push(&stk, 1);
    Push(&stk, 2);
    Push(&stk, 3);
    Push(&stk, 4);
    Push(&stk, 5);
    Push(&stk, 6);
    return 0;
}