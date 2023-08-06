#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
  this is essentially trickle_down 
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

int extract_max(int* A, int* size) {
    int returnee = A[0];
    A[0] = A[*size - 1];
    (*size)--;

    max_heapify(A, 0, *size);

    return returnee;
}


bool trickle_up(int* A, int i) {
    // check the first iteration manually
    if(A[i] < A[parent(i)]) {
        return false;
    }
    swap(A, i, parent(i));
    i = parent(i);
    
    while((i > 0) && A[parent(i)] < A[i]) {
        swap(A, i, parent(i));
        i = parent(i);
    }
    
    return true;
}

void max_heap_insert(int* A, int* size, int key) {
    A[*size] = key;
    (*size)++;

    trickle_up(A, *size - 1);
}

/* returns whether or not the key modification changed the heap structure */
bool heap_modify_key(int* A, int i, int* size, int key) {
    // do nothing and return false
    if(key == A[i]) {
        return false;
    }

    int old_key = A[i];
    A[i] = key;
    // if the new key is bigger, we need to possibly trickle up
    if(key > old_key) {
        return trickle_up(A, i);
    }
    // if the old key is smaller, we ned to possibly trickle down
    return max_heapify(A, i, *size);
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

    // exercise 6.5-1
    // Illustrate the operation of HEAP-EXTRACT-MAX(A) on the heap
    // A = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1}
    int array5[12] = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1};
    int array5_size = 12;
    int array5_expected[11] = {13, 12, 9, 5, 6, 8, 7, 4, 0, 1, 2};
    assert(extract_max(array5, &array5_size) == 15);
    assert(array5_size == 11);
    assert(!memcmp(array5, array5_expected, sizeof(int) * 11));

    // exercise 6.5-2
    // Illustrate the operation of MAX-HEAP-INSERT(A,10) on the heap
    // A = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1}
    int array6[13] = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1};
    int array6_size = 12;
    int array6_expected[13] = {15, 13, 10, 5, 12, 9, 7, 4, 0, 6, 2, 1, 8};
    max_heap_insert(array6, &array6_size, 10);
    assert(array6_size == 13);
    assert(!memcmp(array6, array6_expected, sizeof(int) * 13));

    // modify key by increasing without structure change, and increasing with structure change
    // as an example, change the 5 to something less than its parent, 13. Then change it again
    // with a structure change.
    int array7[12] = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1};
    int array7_size = 12;
    int modify_key1 = 10;
    int array7_expected[12] = {15, 13, 9, modify_key1, 12, 8, 7, 4, 0, 6, 2, 1};
    assert(!heap_modify_key(array7, 3, &array7_size, modify_key1)); // we don't need the size to trickle up
    assert(!memcmp(array7, array7_expected, sizeof(int) * 12));
    // this will induce a structure change
    int modify_key2 = 16;
    array7_expected[0] = modify_key2;
    array7_expected[1] = 15;
    array7_expected[3] = 13;
    assert(heap_modify_key(array7, 3, &array7_size, modify_key2));
    assert(!memcmp(array7, array7_expected, sizeof(int) * 12));

    // modify key by decreasing without structure change, and decreasing with structure change
    // as an example, change the 15 to something more than its chidren, 13 and 9. Then change it again
    // with a structure change
    int array8[12] = {15, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1};
    int array8_size = 12;
    modify_key1 = 14;
    int array8_expected[12] = {modify_key1, 13, 9, 5, 12, 8, 7, 4, 0, 6, 2, 1};
    assert(!heap_modify_key(array8, 0, &array8_size, modify_key1));
    assert(!memcmp(array8, array8_expected, sizeof(int) * 12));
    // this will induce a structure change
    modify_key2 = 11;
    array8_expected[0] = 13; //{13, 12, same9, same5, 11, same til end 8, 7, 4, 0, 6, 2, 1}
    array8_expected[1] = 12;
    array8_expected[4] = modify_key2;
    assert(heap_modify_key(array8, 0, &array8_size, modify_key2));
    assert(!memcmp(array8, array8_expected, sizeof(int) * 12));
}