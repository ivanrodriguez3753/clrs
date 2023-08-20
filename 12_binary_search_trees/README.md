It's probably worthwhile to write down the binary-search-tree property:
Let x be a node in a binary search tree. 
    If y is a node in the left subtree of x, then y.key <= x.key
    If y is a node in the right subtree of x, then y.key >= x.key

An inorder traversal of a binary-search-tree will always produce a sorted sequence 

Exercise 12.1-2
The min-heap property only requires you to be smaller than your immediate children, while the binary-search-tree property requires you to be between your immediate children.
You can't use the min-heap property because it doesn't give you relative ordering between the children, just that the parent is greater than both of them. 

Exercise 12.1-3
I thought about it, was lazy, then looked at the solution at https://sites.math.rutgers.edu/~ajl213/CLRS/Ch10.pdf and it was wrong when I implented it. I finally found a solution at https://stackoverflow.com/questions/25403346/tree-traversal-without-recursion-and-without-stack-and-without-changing-the-tree and it worked but it was preorder (though it's really easy to make it inorder or postorder, as expected). Essentially, you just track where you came from out of FROM_PARENT, FROM_LEFT, and FROM_RIGHT, and act accordingly. It's a little tricker than you'd first think, particularly for going back to a parent node.

Exercise 12.2-5
I didn't implement predecessor but its symmetric to successor. So I'll just do the first part of the exercise.
Suppose a BST T has two children. We want to show its successor has no left child. 
Since T has two children, its successor must be in the right subtree. In this case, the successor is the minimum of the tree.
So, suppose the successor has a left child. Since the minimum of a tree is always found be traversing left until there is no left subtree. This is a contradiction, since the successor is a minimum and simultaneously has a left child. 

Exercise 12.2-6
Consider a BST T whose keys are distinct. Show that if the right subtree of a node x in T is empty and x has a successor y, then y is the lowest ancestor of x whose left child is also an ancestor of x (recall that every node is its own ancestor).

Suppose the right subtree of a node x in T is empty and that x's successor y exists. Then y must be an ancestor of x, since x's right subtree is empty and it cannot be in the left subtree since everything in the left subtree is < x.key. Now we must show that y is the LOWEST ancestor of x. Since we don't know what y is, only that it exists, we will show that y is the lowest ancestor by finding y. 
Since y must be an ancestor of x, we can only traverse up the tree. We can go up from either the right or the left.
Case right: since the current node is a right child, the parent is less than parent.key < x.key. While parent is an ancestor, its left child is not the current node (since current is a right child in this case), the left child of the current parent is not an ancestor of x. We go up a level for current and parent.
Case left: since the current node is a left child, the current parent satisfies x.key < answer.key. In this case, the left child of the current parent it an ancestor of x (it is x itself). This is our y and we can terminate the algorithm.

Everything we had checked before y was an ancestor but it did not satisfy the is_ancestor_of(x, parent.left). Since we were only going upwards, and everything before y was invalid, and y was the first such valid one, it is the lowest such ancestor of x that satisfies is_ancestor_of(x, parent.left).

After doing this, I looked at the exercise at https://sites.math.rutgers.edu/~ajl213/CLRS/Ch12.pdf and liked it better. It avoided constructing y and just used the existence of y and proof by contradiction to show what y must be every step of the way.

Exercise 12.2-7
An alternative method of performing an inorder tree walk of an n-node BST finds the minimum element by calling TREE-MINIMUM and then making n - 1 calls to TREE-SUCCESSOR. Prove that this algorithm runs in theta(n) time.
I'll only worry about the upper bound.
Well, tree minimum and and tree successor both run in O(h), so T(n) = 1 * O(h) + (n - 1) * O(h). But h is bounded by n, so we can say that 
T(n) <= 1 * c1 * n + (n - 1) * c2 * n = O(n^2). So this bound isn't tight enough and we have to look at what happens in these calls. The first call to TREE-MINIMUM will return either the root (if no left subtree) or the leftmost grandchild of the root. The first call to TREE-SUCCESSOR, in the case of having a left subtree, will return its immediate ancestor since it is a left child by being a minimum that isn't the root. All subsequent calls to tree-successor depend on what the last result was. If the last result was a left child, it will return immediately. If it is a right child, it will find its successor, which will call fall into two cases: having a right subtree, in which case it will call tree-minimum, and not having a right child
I couldn't figure this out. The solution asserts and shows that each edge is traversed exactly twice, and the number of edges in a tree is nodes - 1, so it is T(n) = 2 * (n - 1) = O(n)

The exercises for 12.3 were intuitive but I didn't do any of them all the way through, just thought about them and checked online answers. That being said, 12.3-6 was interesting because it shows that we can use the predecessor instead of successor to implement TREE-DELETE.

