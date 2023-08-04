#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#define N 4

// calculate the linear index in C (starting at 0) using the row and column indexing used in the book
// (both starting at 1)
#define IDX(i, j, n) ((i - 1) * n + (j - 1)) 

// quick sanity check: a 2x2 array should map the following way:
// i | j | linear        i | j | linear
// 1   1      0          1   1     0
// 1   2      1          1   2     1
// 2   1      2          1   3     2
// 2   2      3          1   4     3
//                       2   1     4
bool test_IDX() {
    int linear_expected = 0;
    for(int i = 1; i <= N; i++) {
        for(int j = 1; j <= N; j++) {
            if(IDX(i, j, N) != linear_expected++) return false;
        }
    }
    return true;
}

void square_matrix_multiply(int C[], int A[], int B[], int n) {
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            int sum = 0;
            for(int k = 1; k <= n; k++) {
                sum += A[IDX(i, k, n)] * B[IDX(k, j, n)];
            }
            C[IDX(i, j, n)] = sum;
        }
    } 
}

//  "We identify a submatrix by a range of row indices and a range of column indices
//  of the original matrix. We end up representing a submatrix a little differently 
//  from how we represent the original matrix, which is the subtlety we are glossing over."
typedef struct {
    // the rows are captured by i_low and i_high. the matrix will always be at least 1x1
    int i_low, i_high, j_low, j_high;
    int* data; // pointer to base of initial matrix 
    int n; // dimension size of the initial matrix
    int* new_to_old_map;
} submatrix;

void partitionMatrix(submatrix A, submatrix A_p[]) {
    int A_i_mid = (A.i_low + A.i_high) / 2;
    int A_j_mid = (A.j_low + A.j_high) / 2;
    submatrix* A11 = &A_p[IDX(1, 1, 2)];
    submatrix* A12 = &A_p[IDX(1, 2, 2)];
    submatrix* A21 = &A_p[IDX(2, 1, 2)];
    submatrix* A22 = &A_p[IDX(2, 2, 2)];

    // set base data for everyone
    A11->data = A12->data = A21->data = A22->data = A.data;
    // set size for everyone
    A11->n = A12->n = A21->n = A22->n = A.n;
    
    // top left
    A11->i_low = A.i_low;
    A11->i_high = A_i_mid;
    A11->j_low = A.j_low;
    A11->j_high = A_j_mid;

    // top right
    A12->i_low = A.i_low;;
    A12->i_high = A_i_mid;
    A12->j_low = A_j_mid + 1;
    A12->j_high = A.j_high;

    // bottom left
    A21->i_low = A_i_mid + 1;
    A21->i_high = A.i_high;
    A21->j_low = A.j_low;
    A21->j_high = A_j_mid;

    // bottom right
    A22->i_low = A_i_mid + 1;
    A22->i_high = A.i_high;
    A22->j_low = A_j_mid + 1;
    A22->j_high = A.j_high;
}

// A and B are passed in with temp (stack) buffers, so their
// indexing should go off of locally calculated size. C was allocated
// before the beginning driver call should its indexing should use the
// original size
void submatrix_sum(submatrix C, submatrix A, submatrix B) {
    int A_i, B_i, C_i, A_j, B_j, C_j;
    A_i = A.i_low; 
    B_i = B.i_low;
    C_i = C.i_low;

    int size = A.i_high - A.i_low + 1; // C is 4 times as big, but we're only using a quarter of it. The number of reps we want in each direction is given by the size calculation of A or B
    for(int i = 0; i < size; i++) {
        A_j = A.j_low; 
        B_j = B.j_low;
        C_j = C.j_low;
        for(int j = 0; j < size; j++) {
            int C_idx = IDX(C_i, C_j, C.n);
            int A_idx = IDX(A_i, A_j, A.n);
            int B_idx = IDX(B_i, B_j, B.n);
            C.data[C_idx] = A.data[A_idx] + B.data[B_idx];
            A_j++; B_j++; C_j++;
        }
        A_i++; B_i++; C_i++; 
    }

}

/**
 * As mentioned in the book, we need to specify the submatrices with 
 * index ranges. We'll use a simple struct to do this.
 * Note that n is the size of the original matrix, and never changes between calls.
 * The base case of size being 1 is found by doing (high - low + 1) in each dimension,
 * then multiplying and checking if it's 1. The matrix we use doesn't matter
 * result matrix C must be 0 initialized by the caller
*/
submatrix square_matrix_multiply_recursive(submatrix C, submatrix A, submatrix B) {
    int n = A.i_high - A.i_low + 1;
    int size = n * n;
    if(size == 1) {
        int a_idx = IDX(A.i_low, A.j_low, A.n);
        int b_idx = IDX(B.i_low, B.j_low, B.n);
        int c_idx = IDX(C.i_low, C.j_low, C.n); 
        // TODO: the bug is in these index calculations. You can't use the current i's and j's
        C.data[IDX(C.i_low, C.j_low, C.n)] = A.data[IDX(A.i_low, A.j_low, A.n)] * B.data[IDX(B.i_low, B.j_low, B.n)];
        return C;
    }

    // Partition A, B and C as in equations (4.9). In the book, they are partitioned
    // as 2x2 arrays, and I think that makes the recursion easier to visualize
    submatrix A_partition[2 * 2];
    submatrix B_partition[2 * 2];
    submatrix C_partition[2 * 2];
    partitionMatrix(A, A_partition);
    partitionMatrix(B, B_partition);
    partitionMatrix(C, C_partition);
    
    // I'm not sure if this can be done in place or if everything needs a temp
    // Anyway, just use temps for holding the results of the recursive calls.
    int temp1_data[n * n / 4];
    int temp2_data[n * n / 4];
    // memset(temp1_data, 0, n * n / 4);
    // memset(temp2_data, 0, n * n / 4);
    submatrix temp1 = {.i_low = 1, .i_high = 1 + n / 2 - 1, .j_low = 1, .j_high = 1 + n / 2 - 1, temp1_data, .n = n / 2};
    submatrix temp2 = {.i_low = 1, .i_high = 1 + n / 2 - 1, .j_low = 1, .j_high = 1 + n / 2 - 1, temp2_data, .n = n / 2};

    submatrix_sum(
      C_partition[IDX(1,1,2)],
      square_matrix_multiply_recursive(
        temp1, A_partition[IDX(1,1,2)], B_partition[IDX(1,1,2)]
      ),
      square_matrix_multiply_recursive(
        temp2, A_partition[IDX(1,2,2)], B_partition[IDX(2,1,2)]
      )
    );
    submatrix_sum(
      C_partition[IDX(1,2,2)],
      square_matrix_multiply_recursive(
        temp1, A_partition[IDX(1,1,2)], B_partition[IDX(1,2,2)]
      ),
      square_matrix_multiply_recursive(
        temp2, A_partition[IDX(1,2,2)], B_partition[IDX(2,2,2)]
      )
    );
    submatrix_sum(
      C_partition[IDX(2,1,2)],
      square_matrix_multiply_recursive(
        temp1, A_partition[IDX(2,1,2)], B_partition[IDX(1,1,2)]
      ),
      square_matrix_multiply_recursive(
        temp2, A_partition[IDX(2,2,2)], B_partition[IDX(2,1,2)]
      )
    );
    submatrix_sum(
      C_partition[IDX(2,2,2)],
      square_matrix_multiply_recursive(
        temp1, A_partition[IDX(2,1,2)], B_partition[IDX(1,2,2)]
      ),
      square_matrix_multiply_recursive(
        temp2, A_partition[IDX(2,2,2)], B_partition[IDX(2,2,2)]
      )
    );
    return C;    
}


int main() {
    assert(test_IDX());

    int A[N * N] = {0,  1,  2,  3,
                    4,  5,  6,  7, 
                    8,  9,  10, 11,
                    12, 13, 14, 15};
    int B[N * N] = {0,  2,  4,  6,
                    8,  10, 12, 14, 
                    16, 18, 20, 22,
                    24, 26, 28, 30};
    int expected[N * N] = {112, 124, 136, 148,
                           304, 348, 392, 436,
                           496, 572, 648, 724,
                           688, 796, 904, 1012};
    int brute_force_result[N * N];
    int divide_and_conquer_result[N * N];
    memset(brute_force_result, 0, N * N);
    memset(divide_and_conquer_result, 0, N * N);
    

    square_matrix_multiply(brute_force_result, A, B, N);
    for(int i = 0; i < N * N; i++) {
        assert(brute_force_result[i] == expected[i]);
    }
    printf("PASSED brute force\n");

    // we need a way to identify submatrices, so use this struct
    submatrix _C = {.i_low = 1, .i_high = N, .j_low = 1, .j_high = N, .data = divide_and_conquer_result, .n = N};
    submatrix _A = {.i_low = 1, .i_high = N, .j_low = 1, .j_high = N, .data = A, .n = N};
    submatrix _B = {.i_low = 1, .i_high = N, .j_low = 1, .j_high = N, .data = B, .n = N};

    // the algorithm is additive and i don't want a driver method, 
    // so the algorithm assumes each entry in the result was set to
    // the additive identity, 0. We initialized to this in the declaration
    square_matrix_multiply_recursive(_C, _A, _B);
    for(int i = 0; i < N * N; i++) {
        assert(divide_and_conquer_result[i] == expected[i]);
    }
    printf("PASSED divide and conquer\n");

    // try with randomized 8x8 then 16x16
    #define BIG_N 256 //this can be changed to any power of 2: 2, 4, 8, 16, 32, ...
    int bigA[BIG_N * BIG_N];
    int bigB[BIG_N * BIG_N];
    int bigExpected[BIG_N * BIG_N];
    int bigResult[BIG_N * BIG_N];
    for(int i = 0; i < BIG_N; i++) {
      for(int j = 0; j < BIG_N; j++) {
        bigA[IDX(i, j, BIG_N)] = (i + j);
        bigB[IDX(i, j, BIG_N)] = (2 * i + 2 * j);
      }
    }
    
    square_matrix_multiply(bigExpected, bigA, bigB, BIG_N);
    
    submatrix big_C = {.i_low = 1, .i_high = BIG_N, .j_low = 1, .j_high = BIG_N, .data = bigResult, .n = BIG_N};
    submatrix big_A = {.i_low = 1, .i_high = BIG_N, .j_low = 1, .j_high = BIG_N, .data = bigA, .n = BIG_N};
    submatrix big_B = {.i_low = 1, .i_high = BIG_N, .j_low = 1, .j_high = BIG_N, .data = bigB, .n = BIG_N}; 
    square_matrix_multiply_recursive(big_C, big_A, big_B);

    for(int i = 0; i < BIG_N * BIG_N; i++) {
      assert(bigResult[i] == bigExpected[i]);
    }
    printf("PASSED big divide and conquer\n");

    printf("Matrices were [%dx%d] with %d entries\n", BIG_N, BIG_N, BIG_N * BIG_N);


    return 0;
}