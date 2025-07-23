#include "list.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct node node; // prototype
typedef struct tree tree;

node *make_tree(node *l, int64_t *seq, int left, int right);
int powerof2(int exp);
tree *merge(tree *t1, tree *t2);
void merge_same_trees(list *l);
void remove_leftskewed_tree(list *l);
void remove_rightskewed_tree(list *l);

struct list {
    tree *head; // head of the sequence of trees
    tree *tail; // tail of the sequence of trees
    int64_t first; // leftmost element
    int64_t last; // rightmost element
    int elem_count; // total elem count
    bool reversed;
};

struct tree { // linked list of trees
    node *root; // root node of the tree
    tree *prev; // pointer to previous tree
    tree *next; // pointer to next tree
    int height; // order of the tree
    int node_count; // number of nodes in the tree
};

struct node {
    int64_t val; // integer data of the node
    int index; // index of the node in the original array
    int inc; // index shift
    node *left; // pointer to left node
    node *right; // pointer to right node
};

// constraint: No subsequent trees that have the same height (to avoid order 0 trees => O(n) operations instead of O(log n))
// we use binary operators to get a valid combi of heights for each tree (considering the constraint above)
list *make(int n, int64_t *seq) {
    list *t_seq = (list *) malloc(sizeof(list));

    if (n <= 0) {
        *t_seq = (list) {NULL, NULL, 0, 0, 0, false};
        return t_seq;
    }

    int cn = n; // copy of n

    tree *head = malloc(sizeof(tree));
    *head = (tree) {NULL, NULL, NULL, 0, 0};
    tree *ptr = head;
    int start = 0, rear = 0; // tree index, start and rear will keep track of the range of elements from seq to put in the leaves of some tree

    if (cn & 1) {
        node *root = (node *) malloc(sizeof(node)); // root of the head
        *root = (node) {seq[0], 0, 0, NULL, NULL};
        *head = (tree) {root, NULL, NULL, 0, 1};
        start++; rear++;
    }
    cn >>= 1;

    for (int i = 1; cn > 0; i++) {
        if (cn & 1) {
            int shift = (1LL << i);

            rear += shift;
            tree *t1 = (tree *) malloc(sizeof(tree)); // current tree
            node *root = (node *) malloc(sizeof(node)); // root of the current tree
            root = make_tree(root, seq, start, rear - 1);
            root->inc = 0;
            start += shift;

            if (!ptr->root) {
                *ptr = (tree) {root, NULL, NULL, i, rear - start + shift}; // first tree
            } else {
                *t1 = (tree) {root, ptr, NULL, i, rear - start + shift};
                ptr->next = t1;
                ptr = t1;
            }
        }
        cn >>= 1; // shift bits by 1
    }

    *t_seq = (list) {head, ptr, seq[0], seq[n - 1], n, false};
    return t_seq;
}

void push_left(list *l, int64_t v) {
    tree *new_tree = malloc(sizeof(tree));
    node *new_node = malloc(sizeof(node));

    *new_node = (node){v, 0, 0, NULL, NULL};
    if (!l->reversed) {
        if (!l->head && !l->tail) { // Empty list
            *new_tree = (tree){new_node, NULL, NULL, 0, 1};
            *l = (list) {new_tree, new_tree, v, v, 1, l->reversed};
        } else {
                *new_tree = (tree){new_node, NULL, l->head, 0, 1}; // make a new tree
                l->head->prev = new_tree;
                l->head = new_tree;
                l->first = v;
                l->elem_count++;

            // update indices
            tree *ptr = l->head->next;
            while (ptr) {
                if (ptr->root) {
                    ptr->root->inc++;
                }
                ptr = ptr->next;
            }           
        }  
    } else { // this does the same thing as the above code, just reversed, so just like pushing on the right side of the list
        new_node->index = l->elem_count;
        if (!l->tail && !l->head) {
            *new_tree = (tree){new_node, NULL, NULL, 0, 1};
            *l = (list) {new_tree, new_tree, v, v, 1, l->reversed};
            
        } else {
                *new_tree = (tree){new_node, l->tail, NULL, 0, 1}; // we would only always create a new tree since it won't really messed up the method our indexing (new node to add in the right would always be current size + 1)
                l->tail->next = new_tree;
                l->tail = new_tree;
                l->first = v;
                l->elem_count++;
        }
    }

    merge_same_trees(l);
}

void push_right(list *l, int64_t v) {
    reverse(l);
    push_left(l, v);
    reverse(l);
}
/* this func pops the left element not by directly removing the leaf but rather just making it unaccessible on some valid index i (the left memory can be used once we push some element)*/
bool pop_left(list *l) {
    if (empty(l)) {
        return false;
    }
    l->elem_count--; 
    if (!l->reversed) {
        for (tree *temp = l->head; temp != NULL; temp = temp->next) { // log n trees
            temp->root->inc--; // adjust the index of the current and proceeding trees
        }

        if (l->head->node_count <= 1) { // if node_count is 0 then tree is empty, we free it
            tree *temp = l->head->next;
            free(l->head);
            l->head = temp;
            if (l->head) {
                l->first = get(l, 0); // if there are elements left, we update the leftmost value
                l->head->prev = NULL;
            }
        }

        else {
            remove_leftskewed_tree(l); // unmerge the trees into orders 0-(h-1) by removing the leftskewed tree of the binary tree
        }     
    }
    else {
        if (l->tail->node_count <= 1) {

            tree *temp = l->tail->prev;
            free(l->tail);

            l->tail = temp;
            if (l->tail) {
                l->first = get(l, 0);
                l->tail->next = NULL;
                }
        }
        else {
            remove_rightskewed_tree(l); // unmerge the trees into orders 0-(h-1) by removing the rightskewed tree of the binary tree
        }     
    
    }
    if (l->elem_count <= 0) {
        l->head = NULL; // if list is empty, we set the head and tail to NULL
        l->tail = NULL;
    }
    
    return true;
}

bool pop_right(list *l) {
    reverse(l);
    pop_left(l);
    reverse(l);
}

int64_t peek_left(list *l) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }
    return l->first;
}
int64_t peek_right(list *l) {
    if (empty(l)) {
        fprintf(stderr, "List is empty!\n");
        exit(EXIT_FAILURE);
    }
    return l->last;
}

int size(list *l) {
    return l->elem_count;
}

bool empty(list *l) {
    return !l->elem_count;
}

int64_t get(list *l, int i) {
    if (!(0 <= i && i < size(l))) {
        fprintf(stderr, "Index out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    if (l->reversed) {
        i = size(l) - i - 1;
    }
    int curr_index = 0;
    tree *ptr = l->head;
    if (!ptr) {
        return 0;
    }
    while (ptr) { // look for the right tree
        curr_index += ptr->node_count;
        
        if (i < curr_index) {
            break;
        }
        ptr = ptr->next;
    }
    
    node *low = ptr->root->left, *high = ptr->root->right;
    i -= ptr->root->inc; // shift the index based on the inc of the root
    
    if (!ptr->root->left && !ptr->root->right) {
        return ptr->root->val;
    }

    while (low && high && low->index + low->inc <= high->index + high->inc) { // binary search since we applied bst property on the non leaf nodes of the tree
        //printf("index: %lld\n", i);
        if (!low->left && !low->right && i == low->index + low->inc) { // while its not yet a leaf, we keep traversing as some nonleaf node may have the same index as the leaf that we're looking for
            return low->val;
        }
        if (!high->left && !high->right && i == high->index + high->inc) {
            return high->val;
        }
        if (i < high->index + high->inc) {
            i -= low->inc; // adjust the index based on how many pushlefts we did (this essentially keeps the indices per tree as local ones)
            high = low->right;
            low = low->left;
        }
        else if (i >= high->index + high->inc) {
            i -= high->inc; // adjust the index based on how many pushlefts we did (this essentially keeps the indices per tree as local ones)
            low = high->left;
            high = high->right;
        }
    }
}

/* This function works like the get func but instead of getting, we're setting the value of index i to be v*/
void set(list *l, int i, int64_t v) {
    if (!(0 <= i && i < size(l))) {
        fprintf(stderr, "Index out of bounds!\n");
        exit(EXIT_FAILURE);
    }

    if (l->reversed) {
        i = size(l) - i - 1;
    }
    int curr_index = 0;

    tree *ptr = l->head;
    while (ptr) {
        curr_index += ptr->node_count;
        
        if (i < curr_index) {
            break;
        }
        ptr = ptr->next;
    }
    
    node *low = ptr->root->left, *high = ptr->root->right;
    i -= ptr->root->inc;
    
    if (!ptr->root->left && !ptr->root->right) {
        ptr->root->val = v;
        return;
    }

    while (low && high && low->index + low->inc <= high->index + high->inc) {
        
        if ((!low->left && !low->right && i == low->index + low->inc)) {
            low->val = v;
            return;
        }
        if (!high->left && !high->right && i == high->index + high->inc) {
            high->val = v;
            return;
        }
        if (i < high->index + high->inc) {
            i -= low->inc;
            high = low->right;
            low = low->left;
        }
        else if (i >= high->index + high->inc) {
            i -= high->inc;
            low = high->left;
            high = high->right;
        }
    }
}

void reverse(list *l) {
    l->reversed = !l->reversed;
    int64_t temp = l->first;
    l->first = l->last;
    l->last = temp;
}

/* func that converts a sequence into a complete binary tree in O(n) time */
node *make_tree(node *l, int64_t *seq, int left, int right) { // func that converts a seq into a complete binary tree in O(n) time

    if (abs(right - left) <= 1) { // if only two elements remain, we assign them as the leaves of the tree
        node *leftn = malloc(sizeof(node)); // left node
        node *rightn = malloc(sizeof(node)); // right node

        *leftn = (node) {seq[left], left, 0, NULL, NULL};
        *rightn = (node) {seq[right], right, 0, NULL, NULL};

        l->left = leftn;
        l->right = rightn;

        l->index = left;
        l->val = seq[left];
        return l;
    }

    int mid = (right + left) / 2; // split into two
    l->index = left;
    l->val = seq[left]; // assign leftmost element as the parent (kinda like bst property to make O(log n) search possible)

    node *leftn = malloc(sizeof(node));
    node *rightn = malloc(sizeof(node));

    l->left = make_tree(leftn, seq, left, mid); // left subtree
    l->right = make_tree(rightn, seq, mid + 1, right); // right subtree

    return l;
}

/* this func essentially works like the union method of binomial heaps but this ONLY merges trees that has same order and subsequent */
tree *merge(tree *t1, tree *t2) {
    node *new_root = malloc(sizeof(node));
    *new_root = (node) {t1->root->val, t1->root->index + t1->root->inc, 0, t1->root, t2->root};

    tree *combined_tree = malloc(sizeof(tree));
    *combined_tree = (tree) {new_root, NULL, NULL, t1->height + 1, t1->node_count + t2->node_count};

    return combined_tree;
}

void merge_same_trees(list *l) {
    tree *ptr2 = l->head;
    while (ptr2 && ptr2->next) { // union subsequent trees of the same order
        tree *curr_tree = ptr2, *next_tree = ptr2->next;
        while (next_tree && curr_tree->height == next_tree->height) { // while current tree is of the same order to the next one
            tree *new_tree = merge(curr_tree, next_tree); // combine the two trees of the same order into a new tree of order += 1
            new_tree->prev = curr_tree->prev; // insert the new_tree to the sequence of trees
            new_tree->next = next_tree->next;
            if (curr_tree->prev) {curr_tree->prev->next = new_tree;}
            else {l->head = new_tree; // make it the new head if there are no more tree before it
            }
            if (next_tree->next) {next_tree->next->prev = new_tree;}
            else {l->tail = new_tree; // make it the new tail if there are no more tree after it
            }
            free(curr_tree);
            free(next_tree);
            curr_tree = new_tree; // replace the current one with the unioned tree to check if the next tree has the same order with it (combine ulit if yes)
            next_tree = new_tree->next; // move the next tree
        }
        ptr2 = curr_tree->next; // move to the next tree if the current two dont have the same order
        
    }
}
/* This func splits the tree by removing the leftskewed subtree of the leftmost tree */
void remove_leftskewed_tree(list *l) {
    tree *to_free = l->head;
    node *temp = l->head->root;
    int height = l->head->height, count = l->head->node_count;
    l->head = (l->head->next) ? l->head->next: NULL;

    for (temp; temp->left != NULL; temp = temp->left, height--, count /= 2) {
        tree *new_tree = malloc(sizeof(tree));

        temp->right->inc += temp->inc;
        temp->left->inc += temp->inc;

        *new_tree = (tree) {temp->right, NULL, l->head, height - 1, count / 2};

        if (l->head) {
            l->head->prev = new_tree;
        }

        l->head = new_tree;
        node *next = temp->left;

        free(temp);
    }
    
    free(temp);
    free(to_free);
    
    tree *tail = l->head;
    while (tail->next) {
        tail = tail->next;
    }
    l->tail = tail; // reassign a new tail (if the head is actual just the tail aswell before we split the tree)

    l->first = get(l, 0);
}

/* This func splits the tree by removing the rightskewed subtree of the rightmost tree */
void remove_rightskewed_tree(list *l) {
    tree *to_free = l->tail;
    node *temp = l->tail->root;
    int height = l->tail->height, count = l->tail->node_count;
    l->tail = (l->tail->prev) ? l->tail->prev: NULL;

    for (temp; temp->right != NULL; temp = temp->right, height--, count /= 2) {
        tree *new_tree = malloc(sizeof(tree));

        temp->left->inc += temp->inc; 
        temp->right->inc += temp->inc;

        *new_tree = (tree) {temp->left, l->tail, NULL, height - 1, count / 2};

        if (l->tail) {
            l->tail->next = new_tree;
        }
        
        l->tail = new_tree;
        node *next = temp->right;

        free(temp);
    }
    
    free(temp);
    free(to_free);
    
    tree *head = l->tail;
    while (head->prev) {
        head = head->prev;
    }
    l->head = head;
    l->first = get(l, 0);
}