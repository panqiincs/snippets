#include <stdio.h>
#include <stdlib.h>

void Exch(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(int A[], int p, int r)
{
    int x = A[r];
    int i = p - 1;
    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            ++i;
            Exch(A, i, j);
        }
    }
    Exch(A, i+1, r);
    
    return (i + 1);
}

int randomized_partition(int A[], int p, int r)
{
    int i = rand() % (r - p + 1) + p;
    Exch(A, r, i);
    return partition(A, p, r);
}

void quicksort(int A[], int p, int r)
{
    if (p < r) {
        //int q = partition(A, p, r);
        int q = randomized_partition(A, p, r);
        quicksort(A, p, q-1);
        quicksort(A, q+1, r);
    }
}


int main()
{
    int A[] = {3, 9, 7, 8, 1, 1, 6, 4, 2, 5};
    quicksort(A, 0, 9);
    for (int i = 0; i < 10; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}
