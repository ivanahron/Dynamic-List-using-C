#include "list.h"
#include <stdlib.h>
#include <stdio.h>
struct list {
    int64_t *array; // array containing the elements of the list
    int left; // index of the leftmost element
    int right; // index of the rightmost element
    int size; // current size of the list
    bool reversed; // is reversed?
};

list *make(int n, int64_t *seq) {
    list *d_list = (list *) malloc(sizeof(list));
    *d_list = (list) {malloc(n*sizeof(int64_t)), 0, n-1, n, false};
    for (int i = 0; i < n; i++) {d_list->array[i] = seq[i];}
    return d_list;
}

void push_left(list *l, int64_t v) {
    if (empty(l)) {
        *l = (list) {malloc(sizeof(int64_t)), 0, 0, 1, l->reversed};
        l->array[0] = v;
        return;
    }
    else if ((l->left <= 0 && !l->reversed) || (l->left == l->size - 1 && l->reversed)) {
        int shift = l->size;
        l->size += shift; // double the size of the list
        l->left += shift; // shift the left index
        l->right += shift; // shift the right index

        int64_t *doubleSized_list = (int64_t *) malloc(l->size * sizeof(int64_t)); // resized list

        if (!l->reversed) {
            for (int i = l->left; i <= l->right; i++) { // copy the elements of old list to the resized one
                doubleSized_list[i] = l->array[i - shift]; // copy the non shifted index from the original list
            }
        }
        else {
            l->left -= shift; // undo the shifting of indices if its reversed (since left basically becomes the right)
            l->right -= shift;
            for (int i = l->left; i >= l->right; i--) {
                doubleSized_list[i] = l->array[i]; // copy the elements of old list to the resized one
            }
        }
        free(l->array);
        l->array = doubleSized_list; // replace the old list with the resized one
    }
    
    (l->reversed) ? l->left++: l->left--; // reverse the assigment if its reversed
    l->array[l->left] = v; // insert the value
}

void push_right(list *l, int64_t v) {
    reverse(l);
    push_left(l, v);
    reverse(l);
}

bool pop_left(list *l) {
    if (empty(l)) { 
        return false; // no more elements to pop
    }
    l->array[l->left] = -1; // default val (represents NULL)
    (l->reversed) ? l->left--: l->left++; // reverse the traversal if its reversed
    return true;
}

bool pop_right(list *l) {
    if (empty(l)) {
        return false; // no more elements to pop
    }
    l->array[l->right] = -1; // default val (represents NULL)
    (l->reversed) ? l->right++: l->right--; // reverse the traversal if its reversed
    return true;
}

int64_t peek_left(list *l) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }
    return l->array[l->left];
}

int64_t peek_right(list *l) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }
    return l->array[l->right];
}

int size(list *l) {
    return (l->reversed) ? l->left - l->right + 1: l->right - l->left + 1;
}

bool empty(list *l) {
    if (l->reversed) {
        return l->left < l->right;
    }
    return l->right < l->left;
}

int64_t get(list *l, int i) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }

    if (i < 0 || i >= size(l)) {
        fprintf(stderr, "Index out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    return (l->reversed) ? l->array[l->left - i]: l->array[l->left + i]; // there are memory with empty values on the left of the array, so we treat l->left as the 0th element

}

void set(list *l, int i, int64_t v) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }

    if (i < 0 || i >= size(l)) {
        fprintf(stderr, "Index out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    if (l->reversed) {
        l->array[l->left - i] = v;
    }
    else {
        l->array[l->left + i] = v;
    }
    
}

void reverse(list *l) {
    int temp = l->right;
    l->right = l->left;
    l->left = temp;
    l->reversed = !l->reversed;
}