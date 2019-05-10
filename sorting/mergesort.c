#include <stdio.h>
#include <stdlib.h>

void merge(int nums[], int aux[], int left, int mid, int right)
{
    for (int k = left; k <= right; k++) {
        aux[k] = nums[k];
    }

    int i = left;
    int j = mid + 1;
    for (int k = left; k <= right; k++) {
        if (i > mid) {
            nums[k] = aux[j++];
        } else if (j > right) {
            nums[k] = aux[i++];
        } else if (aux[j] < aux[i]) {
            nums[k] = aux[j++];
        } else {
            nums[k] = aux[i++];
        }
    }
}

void merge_sort(int nums[], int aux[], int left, int right)
{
    if (left < right) {
        int mid = left + (right - left)/2;
        merge_sort(nums, aux, left, mid);
        merge_sort(nums, aux, mid + 1, right);
        merge(nums, aux, left, mid, right);
    }
}

void sort(int nums[], int n)
{
    int *aux = malloc(n * sizeof(int));
    if (aux != NULL) {
        merge_sort(nums, aux, 0, n - 1);
        free(aux);
    } else {
        printf("Error: malloc() failed!\n");
    }
}

int main()
{
    int A[] = {3, 9, 7, 8, 1, 1, 6, 4, 2, 5};
    sort(A, 10);
    for (int i = 0; i < 10; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}
