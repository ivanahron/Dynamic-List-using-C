#include <stdlib.h>
#include "list.h"

typedef struct node {
    struct node *next;
    struct node *prev;
    int64_t val;
} node;
typedef struct list {
    node *front;
    node *rear;
    int size;
    bool reversed; // true if list is reversed
} list;
list *make(int n, int64_t *seq){
    list *l = malloc(sizeof(list));
    *l = (list) {NULL, NULL, 0, false};
    for (int i = 0; i < n; i++) {push_right(l, seq[i]);}
    return l;
}
void push_left(list *l, int64_t v) {
    node *x = malloc(sizeof(node));
    *x = (node) {NULL, NULL, v};
    if (empty(l)) {
        l->front = x;
        l->rear = x;
    }
    else if (!l->reversed) {
        x->next = l->front;
        l->front->prev = x;
        l->front = x;
    }
    else {
        x->prev = l->rear;
        l->rear->next = x;
        l->rear = x;
    }
    l->size++;
}
void push_right(list *l, int64_t v) {
    reverse(l);
    push_left(l, v);
    reverse(l);
}
bool pop_left(list *l) {
    if (empty(l)) {return false;}
    if (l->size == 1) {
        free(l->front);
        l->front = NULL;
        l->rear = NULL;
    }
    else if (!l->reversed) {
        l->front = l->front->next;
        free(l->front->prev);
        l->front->prev = NULL;
    }
    else {
        l->rear = l->rear->prev;
        free(l->rear->next);
        l->rear->next = NULL;
    }
    l->size--;
    return true;
}
bool pop_right(list *l) {
    reverse(l);
    bool res = pop_left(l);
    reverse(l);
    return res;
}
int64_t peek_left(list *l) {return get(l, 0);}
int64_t peek_right(list *l) {return get(l, size(l)-1);}
int size(list *l) {return l->size;}
bool empty(list *l) {return !size(l);}
int64_t get(list *l, int i) {
    if (l->reversed) {i = l->size-1-i;}
    node *curr = l->front;
    while (i--) {curr = curr->next;}
    return curr->val;
}
void set(list *l, int i, int64_t v){
    if (l->reversed) {i = l->size-1-i;}
    node *curr = l->front;
    while (i--) {curr = curr->next;}
    curr->val = v;
}
void reverse(list *l) {l->reversed = !l->reversed;}
