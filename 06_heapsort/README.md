The heapsort implementation was pretty straightforward, although I didn't use recursion. Seeing the book explain it with recursion was helpful, though, because it gives insight about the data structure itself. 

For the max priority queue implementation, I want to update key instead of just increase key, so that one will be a little different.

Since I don't want to deal with dynamic data structures, I'll just use a large stack allocation and pass the size as argument. We can assume the size will never be too small in our implementations.

A few other notes: the book calls trickle down "MAX-HEAPIFY". On the other hand, it doesn't have a name for trickle up. 

Exercise 6.4-3
What is the running time of HEAPSORT on an array A of length n that is already sorted in increasing order? What about decreasing order?
Initially, I tried analyzing this by analyzing a function by itself, and leaving components of a runtime by the runtime of the subroutine. For example, we could characterize the runtime of HEAPSORT by:
    T_heapsort(n) = T_build_max_heap(n) + sum(A.length, 2, i)(T_max_heapify(n))
In words, the runtime of heapsort is the sum of (the runtime of build_max_heap) plus (the sum of all the calls to T_max_heapify). This was okay for a while but I didn't realize that the runtime characterizations depend on not just the size, but the state of the input. Which is now obvious since the question gives you two specific inputs (sorted increasing and sorted decreasing). So, we have to consider the state of the array when we call a function to correctly analyze that function. Here's an example of a past mistake: with an input of decreasing order, the indirect calls to max_heapify within build_max_heap become O(1). But, the direct calls to max_heapify from heapsort always take lg(n) time, since we just put the minimum element at the top and all minimum elements must trickle down to the last floor. So, even within the same root call from HEAPSORT and initial input, the runtime of max_heapify changes. So, build_max_heap improves from O(n * lgn) to O(n), n / 2 to be exact, since we call max_heapify (constant runtime at this point in the algorithm, for all the calls in build_max_heap) n / 2 times. As for loop part, we always take worst case (O(lgn)), and the size changes by one every time, so we need the value of (sum(A.length, 2, i)(lgi)) == lg(A.length) + lg(A.length - 1) + ... + lg2. The sum of the first n logs is O(n * lgn), and we only deleted the first one, so we should be asymptotically the same. So, in total, HEAPSORT on a decreasing array runs in O(n) + O(n * lgn). Now, even if we were to optimize by removing build_max_heap altogether(since it just serves as a check in the case of a decreasing array), the runtime of HEAPSORT would still be O(n * lgn) since the O(n) becomes O(1) + O(n * lgn) = O(n * lgn). 

Exercises 6.5-7 talks about implementing a FIFO queue with a priority queue, as well as implementing a stack with a priority queue. 
For both, it would just be a matter of keeping track of and modifying an ongoing 'priority' variable that gets passed to max_heap_insert. 
For example, building a FIFO queue with a priority queue could be implemented by having a variable current_priority (accessible to or passed to max_heap_insert) that keeps track of current priority. Since the first thing we insert is the first thing we want out, and we're assuming a max priority queue, we can initialize current_priority to INF. Every call to max_heap_insert could pass current_priority-- as its priority. The next call will have 1 less and will decrease 1 more, so this will work until we insert 2^num_bits times, which I don't think we have to worry about. 
For a stack, we would initialize current_priority to -INF and pass current_priority++ to max_heap_insert as its priority.

Exercise 6.5-8 talks about implementing HEAP-DELETE(A, i) for deleting an item with index i from the heap. It says it should run in O(lgn). 
I don't want to implement it, but I'll talk about how to do it. Note that on max_heap_insert, we don't insert into the middle of a tree. We insert at the end and then fixup. For delete, we have to do something similar. We can swap the element we want to delete with the last element, and then decrease size by 1 (deleting the element, basically). Then, fixup the tree by either trickling up or down. However, this is equivalent doing a modify-key on the element in question, using the value of the last element, then deleting the last element. So, pseudocode for HEAP-DELETE(A, I) would look something like the following(using 1-based indexing like the book):
HEAP-DELETE(A, i, key)
    HEAP-MODIFY-KEY(A, i, A[A.heap-size]) // this doesn't exist in the book but I implemented it, and it covers the decrease/equal case
    A.heap-size = A.heap-size - 1

Since HEAP-MODIFY-KEY runs in O(lgn) (we just travese a binary tree path either up or down), and changing A.heap-size runs in O(1), HEAP-DELETE also runs in O(lgn) as required by the exercise.

Exercise 6.5-9
Give an O(n * lgk)-time algorithm to merge k sorted lists into one sorted list, where n is the total number of elements in all the input lists. (Hint: use a min-heap for k-way merging).
Essentially, all you need to do is keep a heap of size k elements (max) at a time. Each list should have 1 element in the heap at a time (max, since it can be 0 if that list has been exhausted). To find the next element in the merged list, just pop from the min-heap and insert from the list it came from (if nonempty). Do this until all the lists are empty. Since the heap is of size k, popping takes O(lgk) time. Since we do this n times total (since n is the total number of elements from all the lists, it will take n iterations for all the lists to be empty and therefore terminate), this algorithm is O(n * lgk) as required by the exercise. As a mini sanity check, when k = 2 as in the usual 2-way merge, this runs in O(n * lg2) = O(n * 1) = O(n).

The exercises for 6.1-6.3 were uninteresting and mostly easy. I skimmed some solutions at https://walkccc.me/CLRS/ and none of them were surprising. 
The exercises at the end of the chapter looked interesting and I might go back and do them. In particular, implementing a d-ary heap seems like it would be fun and challenging.
