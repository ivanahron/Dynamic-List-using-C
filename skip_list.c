#include <stdlib.h>
#include "list.h"

typedef struct node {
    struct node *next;
    struct node *prev;
    struct node *below;
    int64_t val;
    int idx; // relative index
} node;
typedef struct list {
    node *fronthead; // head of front sentinel singly linked list
    node *fronttail; // tail of front sentinel singly linked list
    node *rearhead; // head of rear sentinel singly linked list
    node *reartail; // tail of rear sentinel singly linked list
    int size;
    bool reversed; // true if list is reversed
} list;
list *make(int n, int64_t *seq){
    node *x = malloc(sizeof(node));
    node *y = malloc(sizeof(node));
    *x = (node) {y, NULL, NULL, 0, 0}; // sentinel front
    *y = (node) {NULL, x, NULL, 0, 1}; // sentinel rear
    list *l = malloc(sizeof(list));
    *l = (list) {x, x, y, y, 0, false};
    for (int i = 0; i < n; i++) {push_right(l, seq[i]);}
    return l;
}
void push_left(list *l, int64_t v) {
    int height = 0, ss = 0;
    for (node *curr = l->fronthead; curr; curr = curr->below) {height++;}
    node **stack = malloc(height*sizeof(node*)); // for backtracking

    if (l->reversed) {
        for (node *curr = l->rearhead; curr; curr = curr->below) {stack[ss++] = curr;}
        node *last = NULL;
        do {
            node *x = malloc(sizeof(node));
            *x = (node) {stack[ss-1], stack[ss-1]->prev, last, v, stack[ss-1]->idx}; // add node at rear
            stack[ss-1]->prev->next = x;
            stack[ss-1]->prev = x;
            last = x; // update last added node
            ss--;
            if (ss == 0) {
                node *x = malloc(sizeof(node));
                node *y = malloc(sizeof(node));
                *x = (node) {y, NULL, l->fronthead, 0, l->fronthead->idx}; // add new sentinel front
                *y = (node) {NULL, x, l->rearhead, 0, l->rearhead->idx}; // add new sentinel rear
                l->fronthead = x;
                l->rearhead = y;
                stack[ss++] = l->rearhead; // contain element height within sentinels'
            }
        } while (rand() % 2);
        for (node *curr = l->rearhead; curr; curr = curr->below) {curr->idx++;} // update index of sentinel nodes at rear
    }
    else {
        for (node *curr = l->fronthead; curr; curr = curr->below) {stack[ss++] = curr;}
        node *last = NULL;
        do {
            node *x = malloc(sizeof(node));
            *x = (node) {stack[ss-1]->next, stack[ss-1], last, v, stack[ss-1]->idx}; // add node at front
            stack[ss-1]->next->prev = x;
            stack[ss-1]->next = x;
            last = x; // update last added node
            ss--;
            if (ss == 0) {
                node *x = malloc(sizeof(node));
                node *y = malloc(sizeof(node));
                *x = (node) {y, NULL, l->fronthead, 0, l->fronthead->idx}; // add new sentinel front
                *y = (node) {NULL, x, l->rearhead, 0, l->rearhead->idx}; // add new sentinel rear
                l->fronthead = x;
                l->rearhead = y;
                stack[ss++] = l->fronthead; // contain element height within sentinels'
            }
        } while (rand() % 2);
        for (node *curr = l->fronthead; curr; curr = curr->below) {curr->idx--;} // update index of sentinel nodes at front
    }
    free(stack);
    l->size++;
}
void push_right(list *l, int64_t v) {
    reverse(l);
    push_left(l, v);
    reverse(l);
}
bool pop_left(list *l) {
    if (empty(l)) {return false;}
    if (l->reversed) {
        for (node *curr = l->rearhead; curr; curr = curr->below) {
            if (curr->prev->idx + 1 == curr->idx) {
                curr->prev = curr->prev->prev;
                free(curr->prev->next);
                curr->prev->next = curr;
            }
            curr->idx--;
        }
    }
    else {
        for (node *curr = l->fronthead; curr; curr = curr->below) {
            if (curr->idx + 1 == curr->next->idx) {
                curr->next = curr->next->next;
                free(curr->next->prev);
                curr->next->prev = curr;
            }
            curr->idx++;
        }
    }
    node *a = l->fronthead, *b = l->rearhead;
    while (a->below && a->below->next == b->below) { // remove extra sentinels
        l->fronthead = l->fronthead->below;
        l->rearhead = l->rearhead->below;
        free(a);
        free(b);
        a = l->fronthead;
        b = l->rearhead;
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
int64_t peek_left(list *l) {return l->reversed? l->reartail->prev->val : l->fronttail->next->val;}
int64_t peek_right(list *l) {
    reverse(l);
    int64_t res = peek_left(l);
    reverse(l);
    return res;
}
int size(list *l) {return l->size;}
bool empty(list *l) {return !size(l);}
int64_t get(list *l, int i) {
    i = l->reversed? l->rearhead->idx-1-i : l->fronthead->idx+1+i; // get index relative to list
    node *curr = l->fronthead;
    while (curr) {
        if (curr->next) {
            if (curr->next->idx == i) {return curr->next->val;}
            else if (curr->next->idx < i) {curr = curr->next;}
            else {curr = curr->below;}
        }
        else {break;}
    }
}
void set(list *l, int i, int64_t v) {
    i = l->reversed? l->rearhead->idx-1-i : l->fronthead->idx+1+i;
    node *curr = l->fronthead;
    while (curr) {
        if (curr->next) {
            if (curr->next->idx == i) {
                for (node *a = curr->next; a; a = a->below) {a->val = v;}
                return;
            }
            else if (curr->next->idx < i) {curr = curr->next;}
            else {curr = curr->below;}
        }
        else {break;}
    }
}
void reverse(list *l) {l->reversed = !l->reversed;}
