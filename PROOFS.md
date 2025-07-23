# Proofs
It shall be shown and proven that the time complexity of every operation in **Dynamic Array, Sequence of Trees, and Skip List** is sufficient for a dynamic list.

---

## Dynamic Array
Here, what we want to focus on is proving that the **push and pop operations are really $$O(1)$$ amortized**. The idea is that as the size of the array grows, resizing it becomes less frequent, let's prove it using [*aggregate method*](https://www.cs.cmu.edu/~15451-f23/lectures/lecture05-amortized.pdf).

Let $$n$$ be the current size of the list and $$i$$ be the number of operations we did where an operation would either be push or pop.

##### **`pop()`**
In our implementation, the pop just replaces the leftmost element or rightmost element with a `NULL` value, and updates the left and right indices of the dynamic list. This only costs $$O(1)$$ asymptotically, and since $$O(1) \leq O(1) \, amortized$$, then the time complexity for pop operation holds.

##### **`push()`**
If the array still has some empty cells, pushing only costs $$O(1)$$. If instead, the array is full, we will double the size of it and copy the old elements to that new resized array. More formally, if $$n$$ is the current size of the array, we will allocate a new array of size $$2n$$ and copy the previous $$n$$ elements from the old array to the new one. This, obviously costs $$O(n)$$.

Since we're doubling the size of the array everytime it overflows, its sizes would be $$n_0, 2n_0, 4n_0, 8n_0, ... , 2^k(n_0)$$. From that, we can infer that the cost for each push operation $$k$$ is either:
- $$k$$ if $$k-1$$ is a perfect power of 2
- $$1$$ otherwise

Notice that we will only have cost of $$k$$ if $$k-1$$ is a perfect power of 2, then that's only $$\theta(lg$$ $$n)$$ times! 

Thus,

Total cost for i pushes:

$$\qquad = \sum_{k=1}^{i}{1} + \sum_{j=0}^{lg(i - 1)}{2^j}$$
$$\qquad = i + 2^0 + 2^1 + 2^2 + ... + 2^{lg(i - 1)}$$
$$\qquad = i + \frac{1(2^{lg(i-1) + 1} - 1)}{2-1}$$
$$\qquad = i + 2^{lg(i-1) + lg2}$$
$$\qquad = i + 2^{2lg(i-1)}$$
$$\qquad = i + 2(i-1)$$
$$\qquad = 3i-2 = O(i)$$

Now, since we had $$i$$ pushes, the amortized cost for each single push is $$\frac{total cost}{number of operations} = \frac{i}{i} = O(1)$$

Thus, indeed, the cost of each push is $$O(1)$$.

##### **`make()`**
Here, we're only copying the elements of the given sequence into a new array of the same size. So it will only take $$O(\ell)$$ where $$\ell$$ is the number of elements of the given sequence.

##### **`empty()`**, and **`size()`**
Since these functions just return a property of our list struct, then it's just constant time $$O(1)$$.

##### **`peek()`**, **`get()`**, and **`set()`**
Since these functions just do array indexing which takes constant time, then its time complexity is $$O(1)$$.

##### **`reverse()`**
In reverse, we're just swapping two properties of struct -- which is the left index and right index, and toggling its reverse property. This takes constant time $$O(1)$$.

---

## Skip List
Note that the time complexity of make() is directly proportional on the time complexity of push() and the number of elements passed in the list argument. Also, the push and pop operations of the list have their worst case time complexity dependent on the height or the number of layers of the current list. Because this list property is random, it must therefore be shown that the space complexity is expected to be $$O(1)$$ for any 'column' or singly linked list of nodes with the same index. 

Given a probability of 50% for each extra subsequent height above one layer, the expected height must then be the sum of values of each height times its probability. Let *H* be the random variable denoting the total height or number of nodes an element has. Thus,

$$\qquad H = \sum_{i=1}{(H_i*Pr[H_i])} = \sum_{i=1}{\frac{i}{2^i}}$$
$$\qquad = \frac{1}{2} + \frac{2}{4} + \frac{3}{8} + \frac{4}{16} + \frac{5}{32} + ...$$

Remember the formula for a geometric series, for all  $$|x| < 1$$

$$\qquad \sum_{n=0}^{+\infty}{x^n} = \frac{1}{1-x}$$

Taking the derivative, and multiplying by $x$,

$$\qquad \sum_{n=0}^{+\infty}{nx^{n-1}} = \frac{1}{(1-x)^2}$$
$$\qquad \sum_{n=0}^{+\infty}{nx^n} = \frac{x}{(1-x)^2}$$

Thus, if $$x = \frac{1}{2}$$,

$$\qquad H = \sum_{n=1}^{+\infty}{\frac{n}{2^n}} = \frac{\frac{1}{2}}{(1-\frac{1}{2})^2} = 2 = O(1)$$

Therefore, the space complexity for height of each element is expected to be constant and thus the operations for `push()`, `pop()`, and `make()` is expected to be $$O(1)$$, $$O(1)$$, and $$O(\ell)$$, respectively, where $$\ell$$ is the number of elements passed into the list argument.

Furthermore, the expected time complexity of the `get()` and `set()` operations is determined to be $$O(\lg{n})$$ according to this [article](https://en.wikipedia.org/wiki/Skip_list#Implementation_details). The main idea is that the randomness of the element heights allow for logarithmic distribution among the layers where there are about twice as many nodes in one layer than the layer directly above it. This means that the number of nodes skipped in a traversal decreases in about half when going to the node below it.

The time complexities for the other operations are $$O(1)$$ since they only access and assign a property of the list or a node without any randomness involved.

---

## Sequence of Trees
Here, we'll focus on proving that the time complexity of **make() func is $$O(\ell)$$**, and **push, pop, get, and set are all $$O(lg$$ $$n)$$**

##### **`make()`**
The first step is to determine the orders of perfect binary trees to construct. We want the total number of leaves to be $\ell$, and we want it to satisfy the invariant about the sequence of orders. We do this as follows:

1. Calculate an upper bound to the number of perfect binary trees needed to represent the sequence. Our upper bound for our construction below is $\lfloor \lg \ell \rfloor + 1$.
2. We then calculate the specific orders of the perfect binary trees needed by taking the binary representation of $\ell$, say $b_{k-1}\ldots b_1b_0$, and for each $b_i = 1$, we construct a tree of order $i$.
3. We then construct the tree by splitting the elements in order.

The resulting sequence of trees will have decreasing orders, satisfying the invariant. Furthermore, the total number of leaves is $\ell$, because we constructed the trees directly out of $\ell$'s binary representation:

$$\ell = 2^{k-1} b_{k-1} + \ldots + \ldots 2^0 b_0.$$

Note that if $b_i = 1$, then we construct a tree with $2^i$ leaves; otherwise, we construct nothing.

In particular, there are $\le \lg \ell + 1$ trees&mdash;$2^{k-1} \le \ell$, so $k-1 \le \lg \ell$ and $k \le \lg \ell + 1$&mdash;proving that our upper bound above is correct.

A tree of order $k$ is constructed in $O(2^k)$ time, so the trees are constructed in time

$$O(2^{k-1} + 2^{k-2} + \ldots + 2^0) = O(2^k) = O(2\ell) = O(\ell).$$

Lastly, the roots of the trees are connected in a doubly linked list. Since there are $O(\lg \ell)$ trees, this takes $O(\lg \ell)$ time. Thus, the overall time complexity is $$O(\ell + \lg \ell) = O(\ell)$.

##### **`push()`**
As stated in [DETAILS.md](https://github.com/UPD-DCS/machine-problem-1-coding-eyyy/blob/main/DETAILS.md), this operation took inspiration on the push with merge operation of binomial heaps which takes $$O(lgn)$$ time. We can just argue that it should have the same time complexity as that of binomial heaps, but let's still prove it by showing how the operation really works.

To keep the push to a $$O(lg$$ $$n)$$ time complexity, our invariant is that there should only be at most $$lg$$ $$n$$ trees in our sequence. This is also linked to the constraint that we can't have a sequence of all $$0$$ ordered trees.

Now, what this func does is that everytime its called, it will create a new tree of order $$0$$ and assign the pushed value as the root val. Now, this is where the binomial heap comes in, after we connect the new tree, it will look for subsequent trees that has the same height. If there's one it merges them, this takes constant time as it is only connecting them in a new root. However, we know that we have at most $$lg n$$ trees and that's what we're checking! We are checking all the merge-able trees in a sequence of $$lg$$ $$n$$ trees.

Notice also that pushing this way doesn't violate our invariant as this is essentially just the same as incrementing the binary representation of the sequence by $$1$$ that will cause a carry (the merge). Then, since the binary representation of $$n$$ has at most $$lg$$ $$n$$ bits, then the merging is limited to $$lg$$ $$n$$ times! Thus, having an overall time complexity of $$O(lg$$ $$n)$$.

##### **`get()`** and **`set()`**
Let $$n$$ be the number of elements in the list. We know that we have at most $$lg n$$ trees in our sequence, and each of those tree is ordered $$i$$ with $$2^i$$ leaves.

Suppose we have $$lgn$$ trees and each tree holds elements of count $$i$$. In order to get to the right tree that we're looking for, we have to traverse the sequence of trees and get their element count. Getting their element count takes constant time, then this takes $$lg n$$ constant time. Thus, a work of $$O(lg$$ $$n)$$ in getting the right tree.

Now, suppose that we're on the right tree and this tree holds some elements of count $$k$$. As we've used the property of binary trees using the index of the nodes as the "key", we can go to any leaves in $$lg$$ $$k$$ steps and either get or set the value of it to some other element! However, since this tree is just a part of the sequence of trees, then $$k \leq n$$ must always hold, making $$O(lg$$ $$k) \leq O(lg$$ $$n)$$ hold aswell. Thus the time complexity of get and set is indeed $$O(lg$$ $$n)$$.

##### **`pop()`**
Here, we're popping the leftmost or rightmost node of a tree and then the left skewed binary tree or the right skewed binary tree will be removed and freed. This will essentially unmerge the tree, removing the excess memory while keeping the efficiency. This part of popping would only take at most $$log$$ $$n$$ time as we're essentially just reconnecting the split trees as some new individual trees in our list, this only requires constant work. Thus, $$O(log n)$$ time complexity for this part. Moreover, we will adjust the indices of all the other trees too by decrementing 1 to the roots of all the trees (more of this in [DETAILS.md](https://github.com/UPD-DCS/machine-problem-1-coding-eyyy/blob/main/DETAILS.md)). However, since we wanna keep the leftmost element and rightmost element to be always accessible in constant time and popping would inevitably alter those values, we access the get() func after decrementing here. Since we've proven earlier that get() takes $$O(lg$$ $$n)$$ time, then the time complexity of pop() is $$O(lg$$ $$n + $$ $$lg$$ $$n)$$.

##### **`peek()`**, **`empty()`**, and **`size()`**
Since these functions just return a property of our list struct, then its just constant time $$O(1)$$.

##### **`reverse()`**
In reverse, we're just swapping two properties of struct -- which is the leftmost val and rightmost val, and toggling its reverse property. This takes constant time $$O(1)$$.

---
