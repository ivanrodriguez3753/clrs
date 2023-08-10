#include <stdio.h>
#include <assert.h>
#include <string.h>


void swap(int* A, int i, int j) {
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    return;
}

int partition(int* A, int p, int r) {
    int pivot = A[r];
    int i = p - 1;
    for(int j = p; j < r; j++) {
        if(A[j] <= pivot) {
            swap(A, ++i, j);
        }
    }
    swap(A, ++i, r);
    return i;
}

void quicksort(int* A, int p, int r) {
    if(p < r) {
        int q = partition(A, p, r);
        // recursively quicksort the two partitions, excluding the pivot
        quicksort(A, p, q - 1);
        quicksort(A, q + 1, r);
    }
    return;
}

void partition_test() {
    int A[8] = {2, 8, 7, 1, 3, 5, 6, 4};
    int partition_expected[8] = {2, 1, 3, 4, 7, 5, 6, 8};
    assert(partition(A, 0, 7) == 3); // the pivot index should be 3
    assert(!memcmp(A, partition_expected, sizeof(int) * 8));
}

void quicksort_test() {
    int A[8] = {2, 8, 7, 1, 3, 5, 6, 4};
    int expected[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    quicksort(A, 0, 7);
    assert(!memcmp(A, expected, sizeof(int) * 8));
}

void exercise_7_1_1() {
    int A[12] = {13, 19, 9, 5, 12, 8, 7, 4, 21, 2, 6, 11};
    int expected[12] = {9, 5, 8, 7, 4, 2, 6, 11, 21, 13, 19, 12};
    partition(A, 0, 11);
    assert(!memcmp(A, expected, sizeof(int) * 12));
}

int main() {
    partition_test();
    quicksort_test();
    exercise_7_1_1();

    return 0;    
}