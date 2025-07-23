**NOTE: Originally created as a school project for CS 32 of Computer Science in UP Diliman, adapted and maintained as part of my personal portfolio.**
# CS32-241-MP1

---

## User Manual
This repository includes the four implementations of a dynamic list with each following a single header file containing all the required functions. It also includes a C program for unit testing, txt files for test_cases and results, and two markdown files for the details and proofs of each implementation.

## Details and Implementation
Each implementation of a dynamic list uses different data structures which offers significant advantages in speed and memory. 

### Doubly Linked List
A dynamic list was implemented by having each element in the list as the value of a node in a [Doubly Linked List](https://en.wikipedia.org/wiki/Doubly_linked_list) data structure. Each node can be accessed by traversing consecutive nodes one after another through the list.

### Dynamic Array
A [Dynamic array](https://en.wikipedia.org/wiki/Dynamic_array) data structure can also be used to implement a dynamic list by resizing or 'growing' memory allocation hence the word, dynamic. This mutating process only happens however when the number of elements currently in the list is sufficiently large or small enough relative to its current size.

### Skip List
This implementations work like a doubly linked list with multiple layers such that there are fewer nodes on the higher layers. The layers at the top of the Skip list serves as highways for index traversal thus skipping over nodes in the layers below. More details of the implementation can be found in this [link](https://en.wikipedia.org/wiki/Skip_list).

### Sequence of Trees
In this data structure, dynamic list elements are stored as the leaves of [*perfect*](https://www.geeksforgeeks.org/perfect-binary-tree/) binary trees such that the largest binary tree would be at the middle of the sequence with decreasing sizes to the left and right. Each tree in the sequence would then be connected via its root through a doubly linked list. Accessing any element on the sequence of trees would then depend on the number of trees and the maximum  height of any tree in the sequence. 

### Working Functions
| Operation | Description |
| ------ | ------ |
| `make(int n, int64_t *seq)` | Initializes the sequence of elements into a dynamic list|
| `push_left(list *l, int64_t v)` | Appends an element in the left side of the list |
| `push_right(list *l, int64_t v)` | Appends an element in the right side of the list |
| `pop_left(list *l)` | Removes the leftmost element of the list |
| `pop_right(list *l)` | Removes the rightmost element of the list |
| `peek_left(list *l)` | Access and checks the leftmost element of the list |
| `peek_right(list *l)` | Access and checks the leftmost element of the list |
| `size(list *l)` | Returns the number of the elements in the list |
| `empty(list *l)` | Returns whether the list is empty or not |
| `get(list *l, int i)` | Gets the number in the $$i$$ th element of the list |
| `set(list *l, int i, int64_t v)` | Replaces the number in the $$i$$ th element of the list with v |
| `reverse(list *l)` | Reverses the order of the list |

## Testing
Testing can be done by running the unit_tests.c file which includes smoke test cases for each function necessary towards the implementation of a dynamic list. It includes a 'random' tester where a random sequence is given of size n based on a seed which dictates the sequence of actions done on an empty dynamic list. The resulting list is then outputted. Since rand() is deterministic, the output must always stay the same. More tests can be added by increasing the sequence size and/or changing the seed. 

Comparison of outputs between the implementations was automatically done via the file results.txt. Although the results will be in results.txt, you can also easily print it out in your terminal by just uncommenting the unpack(l) in the last line of function test_dl.

Also, the generated random sequence of aoperations based on some seed will be stored in test_case.txt. This will help with double checking if the resulting sequence is really correct by simulating those sequence of operations in other **correct** dynamic list implementations outside this repository. To run the unit_tests.c file, you have to run these codes below in your terminal/prompt. Make sure that you've downloaded all the necessary files in a single folder. (This includes everything in this repository except the .md files)


Before every unique test case (e.g. you change the seed), you should uncomment the delete_file_contents(); in the main() and run the code below as we don't wanna mix up the results of different test cases and inevitably make an assertion error. Doing this will delete all the contents of results.txt and test_cases.txt.

```sh
gcc unit_tests.c doubly_linked_list.c -o ./unit_tests.out
```

#### Compiling
To Compile, you can write this command in your favorite bash.

```sh
gcc unit_tests.c {datastructure.c} -o ./{name of test file}.out
```

#### Running
To run the test case, you can write this command in the same bash.

```sh
./{name of test file}.out
```

Now, we're ready! Comment the delete_file_contents() first and uncomment the test_dl. You can assign your own seed and number of operations and the arrangement by which data structure would you wanna use first. In this specific example, we're going to run doubly_linked_list.c then dynamic_array.c then skip_list.c and lastly, sequence_of_trees.c.

### Sample Testing

```sh
gcc unit_tests.c doubly_linked_list.c -o unit_tests.out
./unit_tests.out
```

```sh
gcc unit_tests.c dynamic_array.c -o unit_tests.out
./unit_tests.out
```

```sh
gcc unit_tests.c skip_list.c -o unit_tests.out
./unit_tests.out
```

```sh
gcc unit_tests.c sequence_of_trees.c -o unit_tests.out
./unit_tests.out
```

If only "Success!" is printed out in your terminal, that's normal and means all the results in different data structures are all the same! Just look at the results.txt to see the resulting list and test_case.txt to see the sequence of operations used. 

### Sample Output

```sh
Success!
```

If instead, we use this as our main() func and the unpack() is uncommented in our test_dl().

```c
int main() {
    // Smoke Tests
    //smoke_test(); // Comment this if not needed as it prints a lot
    //Randomize Test
    test_dl(10000, 1);

    //delete_file_contents(); // only uncomment this if you will change the test case as it will delete all the existing data of results.txt and test_case.txt

    return 0;
}
```

The output would be:

```sh
Success!
The Final List is: 46 1 3 4 477 135 441 108 261 61 497 25 569 56 219 98 83 502 229 54 81 110 424 435 12 132 191 559 446 457 6 71 68 59 37 35 13
```

***Yoohoo!***