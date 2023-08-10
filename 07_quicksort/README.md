Essentially, it's all in the partition method which partitions in place based on the pivot element. So, you have pointers to left and right, which for some reason the book uses p and r for. Then you iterate over the (sub)array (using j), and increment i every time your current element elongs on the left of the pivot element. This is because i keeps track of the END of the left partition, which is also why it starts outside of the first element in the picture on page 172 of the book. Since it is in place, the beginning to i (inclusive), unless i less than the beginning (in which case all ended up on the right partition).

2, 8, 7, 1, 3, 5, 6, 4
We call partition(A, 0, 7)
r is 7, so we choose the pivot x = A[r](4), and initialize i to p - 1 (-1). j is initialized to p (0)
In the loop, 
j is 0
    belong to left partition, so increment i and swap the one we just scanned (j) with i so that the first of j is at the end of the right partition
        in this special case, i and j are the same so its a no-op
    i is 0
    2, 8, 7, 1, 3, 5, 6, 4
    
j is 1
    belongs in the right partition, so we don't do anything. but this means when j advances, there is a separation between i and j (with i != p, otherwise left partition is empty), meaning we have a left and a right partition (we previously only had a left so far, with right empty). Cool.
    2, 8, 7, 1, 3, 5, 6, 4
j is 2
    belong in the right partition, don't do anything.   
    2, 8, 7, 1, 3, 5, 6, 4
j is 3
    belong in the left, so we need to swap it with the leftmost element of right partition
    increment i, and swap i and j
    i is 1
    2, 1, 7, 8, 3, 5, 6, 4
j is 4
    belong in the left
    increment i, swap i and j
    i is 2
    2, 1, 3, 8, 7, 5, 6, 4
j is 5
    belong in the right
    do nothing
    2, 1, 3, 8, 7, 5, 6, 4
j is 6
    belong in the right
    do nothing
    2, 1, 3, 8, 7, 5, 6, 4
the loop is done. at this point, A[p..i] has the left partition (<= elements), and A[i+1..r-1] has the right partition (> elements). We need to put the pivot element in the pivot position while still maintaining the grouping, so we swap i+1 with r
    2, 1, 3, 4, 7, 5, 6, 8
We return the pivot index 3

Moving on with this example, we would have done this partitioning in the driving call and the first recursive call would be quicksort(A, 0, 2)
So let's quicksort 2, 1, 3
We pick 3 as the pivot element, initialize i to -1
j is 0
    belongs on left, but this is first special case so swap is a noop.
    i is 0
    2, 1, 3
j is 1
    belongs on the left, so we swap. this is also a noop because right is empty
    i is 1
    2, 1, 3
The loop is done, and we need to put r where it belongs. So increment i and swap
    i is 2, again a noop since right partition is empty
    2, 1, 3
    now we return pivot index 2
the second recursive call would be to quicksort everything from 0 to 1
so we quicksort 
    2, 1
We pick 1 as pivot element, initialize i to -1
j is 0
    belongs on the right, so we do nothing
the loop is over, increment i and swap.
    1, 2
we return the pivot index 0
we make a third recursive call to quicksort(A, 0, 0) and this exits because !(p < r)
I think this is enough to understand what's going on so far

Exercise 7.1-1
This is tested in the code using the solution from https://walkccc.me/CLRS/Chap07/7.1/

Exercise 7.1-2
It will return r, because every iteration of the for loop will enter the if and increment i, and then one more increment of i when we swap it, so it will end up being r. This exercise is suggesting that this algorithm is probably better if we load balance the recursive calls a little better.
We can count the number of iterations in which A[j] == pivot, and subtract half of that from the pivot index. This way, the next recurisve call will exclude not only the pivot but everything equal to the pivot. I think we would have to modify the windowing, though. Like, keep track of a new subwindow within (left) <= that keeps track of the = elements, so that when one belongs on the left, it goes into the subpartition so that on the final swap we get the ordering we want (there's no guarantee that the elements equal to the pivot would all end up on the right side of left partition).

Most of the rest of this chapter is analysis on the different inputs to quicksort. This is useful, and in particular it is useful to know that quicksort performs the worst whenever the input array is already sorted (if we're choosing A[r] as the pivot). I'm not interested in the probabilistic analysis for now.

