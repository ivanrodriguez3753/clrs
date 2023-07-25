#include <stdio.h>
#include <assert.h>
#include <string.h>

// first element of data starts at [1], do not access [0].
// try every possible combination of buy/sell dates.
// this is theta(n^2) because it amount to adding up the
// first n integers. 
// result should be used as (left, right, sum)
void bruteforce_find_max_subarray(int* result, int* data, int left, int right) {
    
    if(left == right) {
        result[0] = result[1] = left;
        result[2] = data[left];
        return;
    }

    assert(left < right);
    // initialize result to the first combination
    result[0] = left;
    result[1] = left + 1;
    result[2] = data[result[0]] + data[result[1]];

    // since we need a 2-combination, l should only go up to
    // the second to last element. r should start one after
    // l and go to the end.
    for(int l = left; l < right; l++) {
        int sum = data[l];
        // we're trying every possible buy/sell date, but that
        // involves summing all the entries in the range [buy_index, sell_index].
        // just use a single sum variable and take advantage of arranging 
        // calculations from left to right
        for(int r = l + 1; r <= right; r++) {
            sum += data[r];
            if(sum > result[2]) {
                result[0] = l;
                result[1] = r;
                result[2] = sum;
            }
        }
    }

    return;
}

// result should be used as (left, right, sum)
void find_max_crossing_subarray(int *result, int* data, int left, int mid, int right) {
    // always guaranteed at least one element for each of left and right,
    // and we're adding them, so just initialize to first element and 
    // start loop 1 later.
    // Note that the left_result doesn't use the 'right' since it's
    // implicitly mid, and the right_result doesn't use the 'left' 
    // since it's implicitly mid +1. We will combine these two results
    // into a single result by using left's left and right's right, 
    // and adding their sums as our new sum. The result is the maximal
    // subarray that crosses the mid point
    int left_result[3];
    int right_result[3];

    int sum = left_result[2] = data[mid];
    left_result[0] = mid;
    // left loop, start middle and go left
    for(int i = mid - 1; i >= left; i--) {
        sum += data[i];
        if(sum > left_result[2]) {
            left_result[2] = sum;
            left_result[0] = i;
        }
    }

    sum = right_result[2] = data[mid + 1];
    right_result[1] = mid + 1;
    // right loop, start middle and go right
    for(int i = mid + 2; i <= right; i++) {
        sum += data[i];
        if(sum > right_result[2]) {
            right_result[2] = sum;
            right_result[1] = i;
        }
    }

    result[0] = left_result[0];
    result[1] = right_result[1];
    result[2] = left_result[2] + right_result[2];
}

// Calculate the answer for the 3 cases and take the one with the max sum.
// result should be used as (left, right, sum)
void div_con_find_max_subarray(int* result, int* data, int left, int right) {
    //base case: 1 element
    if(left == right) {
        result[0] = result[1] = left;
        result[2] = data[left];
        return;
    }

    int left_result[3], right_result[3], crossing_result[3];
    int mid = (left + right) / 2;

    // first two cases, smaller instances of the same problem
    div_con_find_max_subarray(left_result, data, left, mid);
    div_con_find_max_subarray(right_result, data, mid + 1, right);

    // linear case of finding max crossing subarray
    find_max_crossing_subarray(crossing_result, data, left, mid, right);

    // take the max of the three results
    // we can initialize to one of the three then update if needed
    memcpy(result, left_result, sizeof(int) * 3);
    if(right_result[2] > result[2]) {
        memcpy(result, right_result, sizeof(int) * 3);
    }
    if(crossing_result[2] > result[2]) {
        memcpy(result, crossing_result, sizeof(int) * 3);
    }

    return;
}

// result should be used as (left, right, sum)
void linear_find_max_subarray(int* result, int* data, int left, int right) {
    // initialize ongoing result and final result to first iteration,
    // since we'll always have at least 1
    int sum = result[2] = data[left];
    int l, r;
    l = r = result[0] = result[1] = left;

    // start at left and end one less than right so we can calculate for
    // j + 1 like it says in the mathematical description. That is, 
    // knowing A[left..j], calculate a max subarray for A[left..j+1].
    // In this case, we know max subarray for A[left..left] so we have a foundation
    // for first j + 1.
    for(int j = left; j < right; j++) {
        r = j + 1;
        if(sum > 0) {
            sum += data[r];
        }
        else {
            sum = data[r]; //reset sum and add data[r]
            l = r;
        }

        if(sum > result[2]) {
            result[2] = sum;
            result[0] = l;
            result[1] = r;
        }
    }

    return;
}

int main() {
    int prices[17] = {100, 113, 110, 85, 105, 102, 86, 63, 81, 101, 94, 106, 101, 79, 94, 90, 97};
    int delta_prices[17]; //note that this wastes [0] to stay consistent with the indexing
    int partial_results[17][3]; // waste [0][:] 
    // that is, delta_prices[i] = prices[i] - prices[i-1] for i >= 1. this matches the table on figure4.1
    //delta_prices[0] should not be accessed ever.
    for(int i = 1; i < 17; i++) {
        delta_prices[i] = prices[i] - prices[i-1];
    }

    int bruteforce_result[3]; //(left, right, sum)
    int div_con_result[3];
    int linear_result[3];
    int expected_result[3] = {8, 11, 43};
    
    bruteforce_find_max_subarray(bruteforce_result, delta_prices, 1, 16);
    printf("bruteforce result was (left, right, sum) == (%d, %d, %d)\n", bruteforce_result[0], bruteforce_result[1], bruteforce_result[2]);
    for(int i = 0; i < 3; i++) {
        assert(bruteforce_result[i] == expected_result[i]);
    }

    div_con_find_max_subarray(div_con_result, delta_prices, 1, 16);
    printf("div_con result was (left, right, sum) == (%d, %d, %d)\n", div_con_result[0], div_con_result[1], div_con_result[2]);
    for(int i = 0; i < 3; i++) {
        assert(div_con_result[i] == expected_result[i]);
    }

    linear_find_max_subarray(linear_result, delta_prices, 1, 16);
    printf("linear result was (left, right, sum) == (%d, %d, %d)\n", linear_result[0], linear_result[1], linear_result[2]);
    for(int i = 0; i < 3; i++) {
        assert(linear_result[i] == expected_result[i]);
    }

    return 0;
}