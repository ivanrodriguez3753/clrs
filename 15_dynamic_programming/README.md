Divide and conquer is DISJOINT subproblems, solve subproblems recursively, then combine solutions. In contrast, dynamic programming applies when the subproblems overlap, when subproblems share subproblems. In this case, D&C does extra work, repeatedly solving common subproblems. In DP, each subproblem is solved just once then saved in a table.

Typically applied to *optimization problems*. Such problems can have many solutions. Each solution has a value, and we want the optimal (min/max) value. We call such a solution *AN* optimal solution to the problem, not *THE* solution. Solutions usually not unique.

DP Steps:
1. Characterize structure of optimal solution
2. Recursively define value of an optimal solution
3. Compute value of an optimal solution, typically bottom up
4. Construct an optimal solution from computed information (if needed)

15.1 Rod Cutting
Problem statement: Given a rod of length n inches and table of prices p_i for i = 1, ..., n, determine the max revenue r_n obtainable by cutting up the rod and selling the pieces.
Note that if the price pn for a rod of length n is large enough, an optimal solution may require no cutting at all.
Consider the case n = 4. We can cut a rod of length n in 2^(n-1) different ways, since there are n-1 possible cuts and each one can either be cut or not cut.

Figure 15.1
length i  | 1 | 2 | 3 | 4 | 5  | 6  | 7  | 8  | 9  | 10
price p_i | 1 | 5 | 8 | 9 | 10 | 17 | 17 | 20 | 24 | 30

Look at all the possible cuts in Figure 15.2 and realize it's just counting in binary if you look at it correctly.

Denote a decomposition into pieces using ordinary additive notation, so 
7 = 2 + 2 + 3
indicates that a rod of length 7 is cut into three pieces: 2, 2, and 3.
If an optimal solution cuts the rod into k pieces, for some 1 <= k <= n, then an optimal decomposition
n = i_1 + ... + i_k
of the rod into pieces of lengths i_1, ..., i_k provides max corresponding revenue
r_n = p_i1 + ... + p_ik.

For our sample n = 4 problem, we can determine the optimal revenue figure r_i for i = 1,2,...,10 by inspection, with the corresponding optimal decompositions
r1 has 1 possibility, 1, so choose 1 which has r1 = 1
r2 has 2 possibilities, 2, 1 + 1, so choose 2 which has r2 = 5
r3 has 4 possibilities, 
    |0|0|, |0|1|, |1|0|, |1|1|, 
    3, 2 + 1, 1 + 2, 1 + 1 + 1
    8, 6, 6, 1
                                  so choose 3 which has r3=8
r4 has 8 possibilities,
      |0|0|0|, |0|0|1|, |0|1|0|, |0|1|1|, |1|0|0|, |1|0|1|, |1|1|0|, |1|1|1|
lens  4,       3 + 1,   2 + 2    2+1+1    1 + 3    1+2+1    1+1+2    1+1+1+1
price 9        8+1      5+5      5+1+1    1+8      1+5+1    1+1+5    1+1+1+1
price 9        9        10       7        9        7        7        4
                                  so choose 2 + 2 which has r4=10
Can stop here instead of doing n = 5 because that gives 16 possibilities. In general, we can frame the values r_n for n >= 1 in terms of optimal revenues from shorter rods:
r_n = max(p_n, r_1 + r_n-1, r2 + r_n-2, ..., r_n-1 + r_1)       (15.1)
The first argument p_n corresponds to making no cuts at all and selling the rod of length n as is. The other n - 1 arguments correspond to the max revenue obtained by making an initial cut of the rod into two pieces of size i and n - i, for each i = 1, 2, ..., n - 1, and then optimally cutting up those pieces further, obtaining revenues r_i and r_n-i from those two pieces. Since we don't know ahead of time which value of i optimizes revenue, we have to consider all possible values for i and pick the one that maximizes the revenue. We also have the option of picking no i at all if we decide not to cut.
Note: to solve original problem of size n, we solve smaller problems of the same type but smaller sizes. Once we make the first cut, we may consider two pieces as independent instances of the rod-cutting problem. The overall optimal solution incorporates optimal solutions to the two related subproblems, max'ing revenue from each of those two pieces. We say that the rod-cutting problem exhibits *optimal substructure*: optimal solutions to a problem incorporate optimal solutions to related subproblems, which we may solve independently. 

Slightly simpler way to arrange a recursive structure for the rod-cutting problem, we view a decomposition as consisting of a first piece of length i cut off the left end, then a right hand remainder of length n - i. Only the remainder, not the first piece, may be further divided. The "no cut" solution can be stated as first piece has size i = n and revenue p_n, and remainder has size 0 with r_0 = 0. We obtain the simpler equation
r_n = max(1<=i<=n)(p_i + r_n-i)             (15.2)
In this formulation, an optimal solution embodies the solution to only *one* related subproblem, the remainder, rather than two.

Let's solve the problem for n = 4 using (15.2)
so
r_4 = max(1<=i<=4)(p_i + r_4-i)
    = max(p_1 + r_3,
          p_2 + r_2,
          p_3 + r_1,
          p_4 + r_0)
From this, we only know r_0 and all the p_i's. We need to solve the r_1 problem first
r_1 = max(1<=i<=1)(p_i + r_1-i)
    = max(p_1 + r_0)
    = max(1 + 0)
    = 1
Now we can find r_2
r_2 = max(1<=i<=2)(p_i + r_2-i)
    = max(p_1 + r_1,
          p_2 + r_0)
    = max(1 + 1,
          5 + 0)
    = 5
Now we can find r_3 
r_3 = max(1<=i<=3)(p_i + r_3-i)
    = max(p_1 + r_2,
          p_2 + r_1,
          p_3 + r_0)
    = max(1 + 5,
          5 + 1,
          8 + 0)
    = 8
No we can go back and find r_4
r4 = max(p_1 + r_3,
         p_2 + r_2,
         p_3 + r_1,
         p_4 + r_0)
   = max(1 + 8,
         5 + 5,
         8 + 1,
         9 + 0)
   = 10
RECURSIVE TOP-DOWN IMPLEMENTATION
CUT-ROD(p, n) implements (15.2) in a top-down, recursive manner
CUT-ROD(p, n)
if n == 0
    return 0 
q = -INF
for i = 1 to n
    q = max(q, p[i] + CUT-ROD(p, n - i))
return q

This algorithm doesn't do the same thing we did in the work above. It does it every time we need to get a sub-r. 
Look at figure 15.3 to see how inefficient it is.
CUT-ROD(4)
    loop 1 to 4
        iteration 1
            CUT-ROD(3)
                loop 1 to 3
                    iteration 1
                        CUT-ROD(2)
                            loop 1 to 2
                                iteration 1
                                    CUT-ROD(1)
                                        loop 1 to 1
                                            iteration 1
                                                CUT-ROD(0)
                                                    return 0
                                iteration 2
                                    CUT-ROD(0)
                                        return 0
                    iteration 2
                        CUT-ROD(1)
                            loop 1 to 1
                                iteration 1
                                    CUT-ROD(0)
                                        return 0
                    iteration 3
                        CUT-ROD(0)
                            return 0
        iteration 2
            CUT-ROD(2) 
                loop 1 to 2
                    iteration 1 
                        CUT-ROD(1)
                            loop 1 to 1
                                iteration 1
                                    CUT-ROD(0)
                                        return 0
                    iteration 2
                        CUT-ROD(0)
                            return 0
        iteration 3
            CUT-ROD(1)
                loop 1 to 1
                    iteration 1
                        CUT-ROD(0)
                            return 0
        iteration 4
            CUT-ROD(0)
                return 0
This quick sideways tree matches Figure 15.3, good on me.
(15.3) shows the recurrence for CUT-ROD, which is pretty straightforward from the tree we see
(15.3) T(n) = 1 + sum(j=0, n-1)(T(j))
Solving this recurrence gives 
(15.4) T(n) = 2^n

USING DYNAMIC PROGRAMMING FOR OPTIMAL ROD CUTTING
Pretty much arrange for each subproblem to be solved only *once*.
Usually 2 equivalent ways to implement a DP approach.
First is *top-down with memoization*. Essentially start by checking if you've already computed this. If you haven't proceed as you usually would, but save the result before returning.
Second is *bottom-up method*. Typically depends on some natural notion of the "size" of a subproblem, such that solving any particular subproblem depends only on solving "smaller" subproblems. Sort subproblems by size and solve them in size order, smallest first. 
These two approaches usually yield the same asymptotic runtime, but bottom-up usually better since it has less overhead for procedure calls. 
