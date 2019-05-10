#include <stdio.h>

typedef struct __heap {
    int  length;
    int  heap_size;
    int *data;
} Heap;

#define  PARENT(i)  (i/2)
#define  LEFT(i)    (2*i)
#define  RIGHT(i)   (2*i+1)

void Exch(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void MaxHeapify(Heap *hp, int root)
{
    int left = LEFT(root);
    int right = RIGHT(root); 
    int largest;
    
    if (left <= hp->heap_size && hp->data[left] > hp->data[root]) {
        largest = left;
    } else {
        largest = root;
    }
    if (right <= hp->heap_size && hp->data[right] > hp->data[largest]) {
        largest = right;
    }

    if (largest != root) {
        Exch(hp->data, root, largest);
        MaxHeapify(hp, largest);
    }
}

/* Iterative version

void MaxHeapify(Heap *hp, int root)
{
    while (root <= hp->heap_size) {
        int left = LEFT(root);
        int right = RIGHT(root);
        int largest;
        if (left <= hp->heap_size && hp->data[left] > hp->data[root]) {
            largest = left;
        } else {
            largest = root;
        }
        if (right <= hp->heap_size && hp->data[right] > hp->data[largest]) {
            largest = right;
        }
        if (largest == root) {
            break;
        }

        Exch(hp->data, root, largest);
        root = largest;
    }
}
 */

void BuildMaxHeap(Heap *hp, int A[], int n)
{
    hp->length = n;
    hp->heap_size = n;
    hp->data = A;
    
    for (int i = hp->heap_size/2; i >= 1; i--) {
        MaxHeapify(hp, i);
    }
}


void HeapSort(int A[], int n)
{
    Heap heap;
    BuildMaxHeap(&heap, A, n);

    for (int i = n; i >= 2; i--) {
        Exch(heap.data, 1, i);
        heap.heap_size--;
        MaxHeapify(&heap, 1);
    }
}

void PrintArray(int arr[], int start, int end)
{
    for (int i = start; i <= end; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


void TestHeapSort()
{
    int A[11] = {-1, 4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    printf("Before sorted:\n");
    PrintArray(A, 1, 10);
    HeapSort(A, 10);
    printf("After sorted:\n");
    PrintArray(A, 1, 10);
}

int main()
{
    TestHeapSort();
}
