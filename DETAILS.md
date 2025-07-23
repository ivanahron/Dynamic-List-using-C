# Details and Implementation
Each operation of a dynamic list was implemented with their corresponding time complexities by using these four data structures:
- **Doubly Linked List**
- **Dynamic Array**
- **Skip List**
- **Sequence of Trees**

> Note: In the implementation details below, $$\ell$$ is the length of the list passed to the initializer, and $$n$$ represents the current number of elements in the list.

## Doubly Linked List
This is the simplest dynamic list implementation in this repository. 

| Operation | Time Complexity | Strategy |
| ------ | ------ | ------ |
| `make()` | $$O(\ell)$$ | The elements of the list argument are each pushed to the right of the list. |
| `push_*()` | $$O(1)$$ | A node is allocated to either the front or rear pointers of the list with the argument value. If the list is empty, then it is allocated to both. If the list is reversed, then the node is pushed to the other end. Pushing to the right is the same as reversing the list and pushing to the left. |
| `pop_*()` | $$O(1)$$ | The front or rear node is deallocated and its pointer is updated to the next node. If there is only one node initially in the list, the pointer becomes `NULL`. If the list is reversed, then the node is popped at the other end. Popping the right is effectively the same as reversing the list and popping the left. |
| `peek_*()` | $$O(1)$$ | The value of the front or rear node is returned. |
| `size()` | $$O(1)$$ | The value of the list size is returned. |
| `empty()` | $$O(1)$$ | It returns true if the size is nonzero. |
| `get()` | $$O(n)$$ | The nodes of the list are iterated until the target index is reached. If the list is reversed, then iteration is done in the opposite direction from the other end. The value of the node obtained is returned. | 
| `set()` | $$O(n)$$ | Similar to get, but instead of returning, the value is changed. |
| `reverse()` | $$O(1)$$ | The 'reversed' property of the list is toggled. |

The space needed for a doubly linked list is $$O(n)$$ where *n* is the number of nodes in the list since each node has constant memory complexity.

> [!NOTE]
> Much of the implementations in this repository will use the trick of reversing the list and operating on one end as an equivalent way of operating on the other end. This was done to save both time, effort, tears, and friends. This was only efficiently feasible because reversing a list takes constant time.

## Dynamic Array
Implementation of dynamic list using dynamic array is very straightforward and doesn't go that far from the typical dynamically growing array.

| Operation | Time Complexity | Strategy |
| ------ | ------ | ------ |
| `make()` | $$O(\ell)$$ | The elements of list passed here is copied to some allocated array inside the dynamic list. Then it returns the dynamic list. |
| `push_*()` | $$O(1)$$ amortized | If the list is full, it allocates a memory with double the size of the original array to a new one, copy the old elements, inserts the element to left or right and assign it as the new array of the dynamic list. |
| `pop_*()` | $$O(1)$$ amortized | It sets the value of leftmost or rightmost element into a `NULL` and adjust the index of the left or right index, making that `NULL` value inaccessible for valid indices while keeping the memory still intact, for future pushes. |
| `peek_*()` | $$O(1)$$ | It returns the first or last element for peek_left, peek_right, respectively. |
| `size()` | $$O(1)$$ | Returns the difference of the rightmost element index and leftmost element index and add 1 to it. |
| `empty()` | $$O(1)$$ | It returns true if $$\mathit{size}() \leq 0$$, and false otherwise. |
| `get()` | $$O(1)$$ | Instead of just directly returning the $$i$$-th element, it accounts for the memory with empty values in the left, so it treats the index of the leftmost element of the list as the $$0$$-th element. Thus, returning $$(\text{leftmost index} + i)$$-th element. | 
| `set()` | $$O(1)$$ | Similar to get, but instead of returning, it sets the value into a new one. |
| `reverse()` | $$O(1)$$ | It toggles the reverse property of the dynamic list struct and swap the index of leftmost and rightmost element. |

The memory complexity of dynamic array (SSO(n)$$) will depend on the allocated size of the array during the last time that it was doubled.

## Skip List
Elements in the list are stored in values of a multilayered doubly linked list where nodes with same index store the same value. Nodes of the same index are linked in a singly linked list where the tail node is always at the lowest layer. Doubly linked lists in the higher layers have fewer nodes such that traversal would skip the nodes in the lower layers that were not included. 

The 'height' or the number of layers each element of the list would have would be randomly determined so that elements would have a height of at least 1 and a 50% chance of a higher height during its insertion. Subsequent higher heights are fixed with the same probability. 

The front and rear head and tail pointers of the list are always allocated to the top and bottom sentinel nodes at the two ends of the list, respectively. These nodes are always included in all layers and therefore grow with the list's height. An empty list would only include two sentinel nodes in one layer. The key idea is to keep the element indices consecutive but not necessarily fixed to start or end with a certain integer.

| Operation | Time Complexity | Strategy |
| ------ | ------ | ------ |
| `make()` | $$O(\ell)$$ expected | The elements of the list argument are each pushed to the right of the list only after two sentinel nodes are initially allocated. |
| `push_*()` | $$O(1)$$ expected | A singly list of nodes with the same index is added to either the front or rear of the list with the argument value by adding a single initial node. The number of extra nodes added is then randomly determined with a probability of 50% for each subsequent node above the bottommost layer. The sentinels are traversed from the head to the tail and backtracking is done every after each node was possibly added. Update the index of the sentinels at the front or back. If the list is reversed, then the nodes are pushed to the other end. |
| `pop_*()` | $$O(1)$$ expected | The sentinels at the front or rear are traversed and for each sentinel node, if there is a node next to it with a consecutive index, then remove that node from that layer or doubly linked list. Update the index of all the sentinels traversed. Remove all extra sentinels starting from the top most layer. If the list is reversed, then the nodes are traversed and popped at the other end. |
| `peek_*()` | $$O(1)$$ | The value of the node next to the tails of the sentinels at the front or rear is returned.. |
| `size()` | $$O(1)$$ | The value of the list size is returned. |
| `empty()` | $$O(1)$$ | It returns true if the size is nonzero. |
| `get()` | $$O(log$$ $$n)$$ expected | The index is first updated relative to the front sentinels' since their index will vary. Then, the nodes at the topmost layer of the list are first traversed until a node with an index not less than the target is reached. If this node's index is the target, then its value is returned. Otherwise, the next layer below is traversed and the entire traversal process is repeated until the target index and its corresponding node is finally reached. If there are no more nodes to traverse into, then nothing is returned. | 
| `set()` | $$O(log$$ $$n)$$ expected | Similar to get, but instead of returning, the value is changed. |
| `reverse()` | $$O(1)$$ | The 'reversed' property of the list is toggled. |

The memory complexity for a skip list ($$O(n)$$ expected) would only depend on the number of layers and elements in the list which is also determined by the fixed probability of height increase of each element.

## Sequence of Trees
In this implementation, the elements of the list are in the leaves of "perfect" binary trees such that all the nodes have two children, and that there are $$2^i$$ leaves -- on the same depth, where $$i$$ is the order of the tree. Elements are stored in doubly linked sequence of these trees.

The sequence of trees is arranged in a way where at any point of time, there must exist a tree $$k$$ of index $$i,$$ $$0 \leq i \leq t$$ where $$t$$ is the number of trees in the sequence, such that:

$$k_1 < k_2 < k_3 < \dots < k_{i-1} < k_i \quad and,$$
$$k_i > k_{i+1} > k_{i+2} > ... > k_{t-1} > k_t$$

This ensures that there are $O(\log n)$ trees in the sequence at any given time. In particular, a degenerate sequence&mdash;like a sequence of $n$ type-$0$ trees, which is effectively just a linked list&mdash;is impossible. 

**Binary Search Tree**

The [property of binary search tree](https://en.wikipedia.org/wiki/Binary_search_tree) was used where the nodes in the left subtree has value that is less than the root, and the nodes in the right subtree has value that is greater than or equal to the root. This was implemented by using each element's index as the key value of the tree.

**Binomial Heaps**

To make sure that the invariant stated above is always satisfied, each time a node is pushed, consecutive trees of the same type $h$ are merged into a tree of type $h+1$.

To do this, inspiration was taken from the [merge function of binomial heaps](https://brilliant.org/wiki/binomial-heap/), but instead of checking which is smaller or bigger root like in binomial heaps, just assign the two trees into a single parent node without changing their order before assigning that parent node into some new tree, and then replace the two previous trees with that new tree.

| Operation | Time Complexity | Strategy |
| ------ | ------ | ------ |
| `make()` | $$O(\ell)$$ | It splits the sequence into trees where each order corresponds to the binary representation of the length of that sequence. This effectively help in determining the exact orders of trees that will have just enough leaves to store all the elements.|
| `push_*()` | $$O(log$$ $$n)$$ | In pushing left, this essentially just push some tree of order $$0$$ where the sole node contains the integer to push. Then it merges all the possible to merge trees. The index of node is $$0$$ if its push left, and $$n$$ if it is push right. The index shift of all the proceeding nodes is also adjusted only if it is pushing from the left, it will account for the multiple $$0$$ indices of nodes that we're pushed from left.|
| `pop_*()` | $$O(log$$ $$n)$$ | It removes and free all the nodes in the left skewed subtree/right skewed subtree and then split them into multiple trees of orders $$0-(h-1)$$, essentially just unmerging them back. This keeps the memory proportional to the actual number of elements in the list. Also, it manipulates the indices of all the trees to keep them proper, only when the push is from left. To do this, it decreases the shift index of the root of head as well as the proceeding trees in the sequence and reduce the head's node count by 1, see the `get()` strategy for more details. Also, to always keep the leftmost element and rightmost element accessible in $$O(1)$$, it takes the leftmost leaf of the head tree-- for pop_left, and store its value in the struct or get the rightmost leaf of the tail tree-- for pop_right, and store its value in the struct.|
| `peek_*()` | $$O(1)$$ | Return the leftmost value or rightmost value propertyu of the list |
| `size()` | $$O(1)$$ | It returns the size property of the list. |
| `empty()` | $$O(1)$$ | It returns true if $$\mathit{size}() \leq 0$$, and false otherwise. |
| `get()` | $$O(log$$ $$n)$$ | It finds the tree that contains the element of index $$i$$, then it will subtract the shift index of the root of that tree to the $$i$$. Then, it will do binary search using the index of each node in that tree, and everytime it goes down deeper, it will always subtract the shift index of the node that it goes (it will actually peek for these shift index of the next nodes before actually going down). That way, it will only be accessing indices that are not popped (i.e. $0$th index is popped so the root will have shift index of -1, the next time it gets the $0$th element, it will become $$0-(-1) = 1$$ as it stops in the root). If this is reverse, it instead does $$i = size() - i - 1$$ first. |  
| `set()` | $$O(log$$ $$n)$$ | Similar to get, but instead of returning, it sets the value into a new one. |
| `reverse()` | $$O(1)$$ | Here, it toggles the reverse property of the dynamic list struct and swap the values of the leftmost element and the rightmost element |

The memory complexity here ($$O(n)$$) will depend on the number of elements in the list and the number of trees that it was partitioned into.