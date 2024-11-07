#include <assert.h>
#include <stdio.h>

// Optimal value for problem size up to n = 10
// r1  = 1
// r2  = 5
// r3  = 8
// r4  = 10
// r5  = 13
// r6  = 17
// r7  = 18
// r8  = 22
// r9  = 25
// r10 = 30

int max(int a, int b) {
    if(a < b) return b;
    return a;
}

int cut_rod(int* p, int n) {
    if(n == 0) return 0;
    int q = 0;
    for(int i = 0; i < n; i++) {
        q = max(q, p[i] + cut_rod(p, n - i - 1));
    }
    return q;
}


int memoized_cut_rod_aux(int* p, int n, int* memo) {
    if(memo[n] > 0) return memo[n];

    int q = 0;
    if(n == 0) return q;
    
    for(int i = 0; i < n; i++) {
        q = max(q, p[i] + memoized_cut_rod_aux(p, n - i - 1, memo));
    }

    return memo[n] = q;
}

int memoized_cut_rod(int* p, int n) {
    int memo[n+1];
    for(int i = 0; i < n+1; i++) {
        memo[i] = 0;
    }
    return memoized_cut_rod_aux(p, n, memo);
}

// int bottom_up_cut_rod(int* p, int n) {
//     int memo[n + 1];
//     memo[0] = 0;
//     for(int j = 0; j < n - 1; j++) {
//         int q = 0;
//         for(int i = 0; i < j; j++)
//     }
// }

void test_cut_rod(int* prices) {
    assert(cut_rod(prices, 1) == 1);
    assert(cut_rod(prices, 2) == 5);
    assert(cut_rod(prices, 3) == 8);
    assert(cut_rod(prices, 4) == 10);
    assert(cut_rod(prices, 10) == 30);
    printf("PASS test_cut_rod\n");
}

void test_memoized_cut_rod(int* prices) {
    assert(memoized_cut_rod(prices, 1) == 1);
    assert(memoized_cut_rod(prices, 2) == 5);
    assert(memoized_cut_rod(prices, 3) == 8);
    assert(memoized_cut_rod(prices, 4) == 10);
    assert(memoized_cut_rod(prices, 10) == 30);
    printf("PASS test_memoized_cut_rod\n");
}

void test_bottom_up_cut_rod(int* prices) {
    assert(bottom_up_cut_rod(prices, 1) == 1);
    printf("PASS test_bottom_up_cut_rod\n");
}

int main() {
    int prices[] = {1,5,8,9,10,17,17,20,24,30};
    test_cut_rod(prices);
    test_memoized_cut_rod(prices);
    test_bottom_up_cut_rod(prices);
}