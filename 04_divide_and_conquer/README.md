4.1 the maximum-subarray problem
Essentially, given a list of future stock prices, maximize your profit given you can only buy once and sell once.
Note that the buy/sell dates don't necessarily use the dates that had global min/max values.

brute-force solution is just trying every combination of buy/sell dates, where buy precedes sell.
This is n choose 2, which is theta(n^2) (I think it's actually the same as the sum of the first n integers).

transform the input into the difference of prices (instead of prices) to make this easier to reason about.

we want to find a sequence of days over which the net change from the first day to last day is maximized.
Instead of looking at daily prices, look at daily change in prices (change[i] = price[i] - price[i - 1]).
That is, we want to "find the nonempty, contiguous subarray of A whose values have the largest sum". We call
this the *maximum subarray*. The book shouldn't have said 'the' and mentions this later, but that just means
solutions aren't necessarily unique. There could be more than 1 maximum subarray (with the same sum value).

Since we transformed to daily change in prices, this problem is only interesting if there are negative numbers
in the array. If they were all nonnegative, you would just buy first day and sell last day every single time. 

The main idea is pretty simple: think of the array as being split between left and right sides. Then the maximum subarray lies in exactly one of 3 region: entire on the left, entirely on the right, or crossing the midpoint. The first two cases
are smaller instances of the same problem, but the third case (crossing midpoint)
is not. Since we divide by half on the left and right recursive calls, that'll happen lg(n) times. On each call, for the third case, is linear in (right - left).
So, that's O(n * lg(n)). 
I think this explanation is correct, but the book just uses a recurrence relation:
    T(n) = 2 * T(n / 2) + theta(n)

The non-recursive part (finding max crossing subarray) deserves its own explanation. The way I understand it is that a crossing subarray has to start at least at [mid, mid + 1] (the call to this is never reached if left == right), then keep separate on going left sums from mid to left and mid + 1 to right and update the left and right for each if it becomes a bigger sum. It'll always find the best subarray that crosses because we start at the most pinched possibility and only expand if it's better to do so. 

Knowing a maximum subarray of A[1..j], extend the answer to find a maximum subarray ending at index j + 1 by using the following observation: a maximum subarray of A[1..j + 1] is either a maximum subarray of A[1..j] or a subarray A[i..j + 1], for some 1 <= i <= j + 1. Determine a maximum subarray of the form A[i..j + 1] in constant time based on knowing a maximum subarray ending at index j.

I'm not quite understanding the linear time algorithm. I looked at the code online but then stopped. What I should do is reverse-engineer my way out of it since I can: the description above says that you basically iterate over [left..right], where at each iteration j you have a maximum subarray of A[left..j]. So, if like in the example we have 16 elements (and index starting at 1), then we can have an array of 16 results and use those to check what our linear time algorithm SHOULD be. So run the whole algorithm n times to precalculate the result at each step for the linear time algorithm.

A[1..1] has result (1, 1, 13)
A[1..2] has result (1, 1, 13)
A[1..3] has result (1, 1, 13)
A[1..4] has result (4, 4, 20)
A[1..5] has result (4, 4, 20)
A[1..6] has result (4, 4, 20)
A[1..7] has result (4, 4, 20)
A[1..8] has result (4, 4, 20)
A[1..9] has result (8, 9, 38)
A[1..10] has result (8, 9, 38)
A[1..11] has result (8, 11, 43)
A[1..12] has result (8, 11, 43)
A[1..13] has result (8, 11, 43)
A[1..14] has result (8, 11, 43)
A[1..15] has result (8, 11, 43)
A[1..16] has result (8, 11, 43)

 I ended up looking at a solution, didn't understand, then understood it. Essentially, you either use the position you just added in your new max subarray, or you don't, and if you don't, you just keep the old max subarray. Reading the hint, I thought you had to find the i that successfully uses the j + 1 block, but that i is being kept track of, not calculated (essentially i is left and j + is right). The other key is to reset sum if it's not helping you. That is, if it's negative, then go ahead and assign the current data (I find it helpful to think of it as resetting to 0 and adding the current data). This is because it being negative means you're better off resetting to 0, whether that new first number is positive or negative. If it's negative, the same thing will happen again. But you do have to keep an ongoing sum (and an ongoing left pointer that gets the current r value when sum resets).