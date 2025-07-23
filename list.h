#ifndef LIST
#define LIST

#include <stdint.h>
#include <stdbool.h>

typedef struct list list;
/* Initializes the list with elements from an array sequence. */
list *make(int n, int64_t *seq);
/* Inserts a value as the leftmost of the list */
void push_left(list *l, int64_t v);
/* Inserts a value as the rightmost of the list */
void push_right(list *l, int64_t v);
/* Removes the leftmost element of the list. Returns True if operation was a success. */
bool pop_left(list *l);
/* Removes the rightmost element of the list. Returns True if operation was a success. */
bool pop_right(list *l);
/* Returns the value at the leftmost index. */
int64_t peek_left(list *l);
/* Returns the value at the rightmost index. */
int64_t peek_right(list *l);
/* Returns the number of elements currently in the list. */
int size(list *l);
/* Returns whether the list is currently empty or not. */
bool empty(list *l);
/* Returns the value of the element at the given index. */
int64_t get(list *l, int i);
/* Replaces the value of the element at the given index with a new one. */
void set(list *l, int i, int64_t v);
/* Reverses the entire list. Leftmost is now rightmost. */
void reverse(list *l);

#endif