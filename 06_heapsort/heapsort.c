#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * (i + 1) - 1;
}

int right(int i) {
    return left(i) + 1; // hahaha 
}

bool has_left(int i, int n) {
    return left(i) < n;
}

bool has_right(int i, int n) {
    return right(i) < n;
}


bool isHeap(int* A, int i, int n) {
    bool isHeap = true;
    for(int j = i; j < n; j++ ){
        if(has_left(j, n) && (A[j] < A[left(j)])) {
            isHeap = false;
            // printf("violated heap property at %d because %d < %d\n", i, A[i], A[left(i)]);
        }
        if(has_right(j, n) && (A[j] < A[right(j)])) {
            isHeap = false;
            // printf("violated heap property at %d because %d < %d\n", i, A[i], A[right(i)]);
        }
    }
    return isHeap;
}

void swap(int* A, int i, int j) {
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    return;
}

/*fixes up the heap rooted i. it is important to note
  that it must already be known that the left and right
  subtrees are heaps.
  returns whether or not a fixup was needed on i */
bool max_heapify(int* A, int i, int n) {
    bool changed = false;
    
    int j = i; 
    while(j < n) {
        int largest = j;
        if(has_left(j, n) && A[largest] < A[left(j)]) {
            largest = left(j);
        }
        if(has_right(j, n) && A[largest] < A[right(j)]) {
            largest = right(j);
        }

        if(largest == j) {
            break;
        }
        
        changed = true;
        swap(A, j, largest);
        j = largest;
    }
    return changed;
}

bool build_max_heap(int* A, int n) {
    bool changed = false;

    // we only have to check nodes that have children,
    // since singletons are heaps
    for(int i = n / 2; i >= 0; i--) {
        if(max_heapify(A, i, n) && !changed) {
            changed = true;
        }
    }
    return changed;
}

/*                  16
              14         10*/

void _heapsort(int* A, int n) {
    build_max_heap(A, n);
    for(int i = n - 1; i >= 1; i--) {
        swap(A, 0, i);
        // call max_heapify on a heap that considers 1 less element (size down by 1).
        // we know the left and right subtrees are valid because they already were 
        // before the swap, and after the swap, we're no longer considering that last element.
        // Note that the index i is also the new size
        max_heapify(A, 0, i);
    }
}

bool is_sorted(int* A, int n) {
    for(int i = 0; i < n - 1; i++) {
        if(A[i + 1] < A[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int array1[10] = {16, 14, 10, 8, 7, 9, 3, 2, 4, 1}; // page 152
    assert(isHeap(array1, 0, 10));
    int array2[10] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7}; // page 158
    assert(!isHeap(array2, 0, 10));
    int array3[10] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1}; // page 155
    // array3[1] violates the heap property. however, since left(1) and right(1)
    // are valid heaps, we can fixup array3[1]. the remaining right subtree at array3[2]
    // was already a valid heap, so this fixup should result in array3 being a heap
    assert(!isHeap(array3, 0, 10));
    // max_heapify preconditions state that left and right subtrees of what we're
    // trying to fix must be heaps
    assert(isHeap(array3, left(1), 10));
    assert(isHeap(array3, right(1), 10));
    // we know array3[1] needed a fixup so assert max_heapify
    assert(max_heapify(array3, 1, 10));
    assert(isHeap(array3, 0, 10));
    
    // array 2 is the full example of build_max_heap on page 158
    assert(build_max_heap(array2, 10));
    // calling build_max_heap on a max_heap should return false
    assert(!build_max_heap(array2, 10));

    // the entire heapsort should work on a randomized set
    int array4[100];
    srand(0);
    for(int i = 0; i < 100; i++) {
        array4[i] = rand();
    }

    _heapsort(array4, 100);
    assert(is_sorted(array4, 100));

}