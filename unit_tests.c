#include "list.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

/* prints list elements in order */
void unpack(list *l) {
    printf("The Final List is: ");
    for (int i = 0; i < size(l); i++) {
        printf("%lld ", get(l, i));
    }
    printf("\n");
}

/* returns a random int sequence with elements from 0-5 based on seed */
int *randseq(int n) {
    int *seq = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        seq[i] = rand() % 6;
    }
    return seq;
}

/* applies a sequence of n actions on a dynamic list with elements based on the remaining number of actions left */
void runseq(list *l, int n, int *seq) {
    if (!seq || n <= 0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        switch (seq[i]) {
            case 0:
                push_left(l, n - i);
                break;
            case 1:
                push_right(l, n - i);
                break;
            case 2:
                if (!empty(l)) {
                    pop_left(l);
                }
                break;
            case 3:
                if (!empty(l)) {
                    pop_right(l);
                }
                break;
            case 4:
                if (!empty(l)) {
                    set(l, (n - i) % size(l), n - i);
                }
                break;
            default:
                reverse(l);
                break;
        }
    }
}
void delete_file_contents() {
    fclose(fopen("test_case.txt", "w"));
    fclose(fopen("results.txt", "w"));
}
/* tests dynamic list implementation given sequence size and random generator seed */
void test_dl(int n, int seed) {
    FILE *test_case_seq = fopen("test_case.txt", "w");
    srand(seed);
    int *seq = randseq(n);

    for (int i = 0; i < n; i++) { // write the sequence on the very top of the file
        fprintf(test_case_seq, "%d ", seq[i]);
    }
    fprintf(test_case_seq, "\n");
    fclose(test_case_seq);

    FILE *test_case_res = fopen("results.txt", "r+"); // read and write

    list *l = make(0, NULL);
    runseq(l, n, seq);
    int res_size = size(l);

    fseek(test_case_res, 0, SEEK_END);
    if (ftell(test_case_res) == 0) { // if results file is empty, we append the first/current result
        for (int i = 0; i < res_size; i++) {
            fprintf(test_case_res, "%lld ", get(l, i)); // append the result in results.txt
        }
        fprintf(test_case_res, "\n");
    }
    else { // if theres a data in result file, then we must've append before this run so we just compare that result to our current one
        rewind(test_case_res);

        int64_t *val_ref = malloc(res_size * sizeof(int64_t));
        for (int i = 0; i < res_size; i++) {
            fscanf(test_case_res, "%lld", &val_ref[i]); // store the current_result in an array
        }

        for (int i = 0; i < res_size; i++) {
            assert(get(l, i) == val_ref[i]); // compare the first result to the current one. If any are not equal, then atleast one must be wrong.
        }

        free(val_ref);
    }
    fclose(test_case_res);
    printf("Success!\n");
    unpack(l);
}

void smoke_test() {
    int64_t test[] = {1, 3, 2};
    int n = sizeof(test)/sizeof(int64_t);
    list *l = make(n, test);
    printf("%lld\n", peek_left(l)); // 1
    pop_left(l);
    printf("%lld\n", peek_right(l)); // 2
    pop_right(l);
    printf("%lld\n", peek_left(l)); // 3
    printf("size of list before pop is %d\n", size(l)); // 1
    int64_t x = get(l, 0);
    pop_left(l);
    printf("Element popped is %lld\n", x); // must be 3
    printf(empty(l)? "list is empty\n" : "list is not empty\n"); // must be empty
    push_right(l, 4);
    printf("%lld\n", peek_right(l)); // 4
    push_left(l, 5);
    printf("%lld\n", peek_left(l)); // 5
    unpack(l); // 5 4
    reverse(l);
    printf("Reversed: "); // 4 5
    unpack(l);
    int seqtest[] = {5,5,4,4,5,4,0,0,4,2,5,5,1,3,1,5,1,2,3,0,3,0,2,3,4,4,3,2,2,5,5,0,5,0,3,4,5,1,1,0,5,3,2,3,3,2,3,1,5,4,5,2,4,3,3,1,5,3,1,4,4,5,2,0,0,4,4,2,4,4,2,3,2,3,4,2,0,3,3,2,3,5,0,4,0,2,4,2,5,4,0,3,2,1,5,4,2,0,3,5,3,5,1,0,0,0,3,2,5,0,5,5,2,4,0,3,5,3,3,0,4,0,5,0,5,5,3,4,5,4,2,4,3,4,4,4,3,5,1,3,1,5,3,3,5,5,1,2,5,0,2,4,2,4,3,2,0,5,0,5,4,5,0,5,2,3,1,3,2,2,1,2,2,4,0,3,0,3,0,4,1,0,5,1,0,2,0,2,1,5,2,1,0,1,5,3,0,5,1,4,5,2,5,3,2,5,1,2,4,3,2,4,0,5,1,2,3,1,3,3,1,3,4,5,2,0,0,0,0,2,2,5,5,2,1,1,0,0,4,4,2,5,4,5,2,5,3,4,4,3,2,0,2,3,0,2,4,2,5,5,1,0,0,0,5,0,2,3,1,5,5,0,4,5,5,2,2,2,3,1,5,0,3,3,3,1,4,1,2,1,4,4,3,3,4,2,5,0,5,5,2,3,4,0,5,1,2,1,1,2,1,0,4,3,5,3,1,4,1,1,1,1,0,1,1,5,3,4,2,1,2,4,5,0,0,1,2,1,2,5,5,1,3,1,0,2,2,5,5,0,1,4,0,4,1,4,0,2,1,1,3,2,1,2,2,5,0,3,3,2,0,2,5,3,4,3,4,2,2,2,1,5,0,5,2,1,5,4,2,0,0,2,0,5,1,2,5,1,0,5,1,2,5,4,2,5,0,2,5,1,2,1,5,3,1,4,1,2,5,3,1,0,3,5,3,5,2,4,2,2,2,4,5,2,3,2,2,4,1,5,1,2,2,5,1,4,2,2,1,0,5,1,2,4,2,4,1,2,4,3,1,2,4,0,5,4,4,3,4,0,4,5,4,5,0,5,1,0,0,3,4,4,3,1,3,2,3,1,1,2,1,2,2,4,3,2,5,1,1,5,4,4,4,4,2,4,1,3,3,5,3,4,0,2,2,1,2,1,2,5,0,1,4,4,1,5,5,0,0,1,5,4,1,2,3,2,5,1,1,2,0,0,3,0,1,0,1,0,1,4,0,4,1,1,0,2,0,3,0,1,3,3,1,3,2,2,1,2,3,0,5,5,3,3,1,5,1,4,0,5,5,0,2,4,0,4,1,5,2,5,3,5,5,1,5,0,1,2,3,2,0,1,1,3,3,5,5,2,0,2,1,1,0,4,1,3,1,2,5,2,0,1,0,1,2,0,5,0,1,3,1,3,2,2,5,1,0,2,0,5,0,1,2,1,5,2,2,4,3,4,2,5,5,5,0,4,5,4,3,3,4,1,3,0,3,4,0,2,5,5,2,3,4,1,3,1,3,4,0,3,5,0,1,1,5,3,5,4,3,2,3,5,1,1,5,4,0,3,4,3,0,4,1,4,1,0,0,4,5,3,0,5,5,1,0,5,4,1,3,0,2,0,5,3,1,2,0,4,0,5,1,3,3,4,2,3,0,4,1,2,5,5,5,2,2,0,5,4,5,5,5,2,3,0,1,2,4,0,1,3,0,4,2,1,4,3,3,3,5,5,2,5,5,3,4,3,3,0,3,1,5,3,3,1,4,2,2,2,5,0,2,2,4,0,5,3,4,0,4,1,0,5,5,0,5,0,1,3,4,4,3,5,1,4,0,3,2,2,1,1,3,5,1,4,2,5,0,2,5,3,2,2,0,4,2,2,1,3,4,3,4,4,1,3,5,4,0,5,4,0,0,4,2,4,4,5,3,3,2,3,3,1,3,4,4,0,4,4,3,3,2,0,4,2,3,0,1,2,5,0,4,0,5,1,1,4,4,2,0,4,2,5,5,0,5,4,3,5,4,3,1,0,4,3,2,0,2,5,3,1,2,3,0,5,1,3,3,1,2,2,3,5,2,3,2,4,5,2,3,0,4,1,5,1,3,3,1,3,4,0,3,2,3,3,3,3,0,3,2,2,2,4,5,4,4,0,4,1,1,1,1,5,4,2,3,2,1,0,4,3,1,0,5,4,1,3,4,2,4,0,1,1,2,3,5,1,5,3,2,5,4,5,0,0,3,1,0,2,0,3};
    l = make(0, NULL);
    runseq(l, 1000, seqtest);
    printf("size: %d\n", size(l)); // 6
    unpack(l); // 2 7 8 38 10 22
    printf("lefttmost value: %lld\n", peek_left(l)); // 2
    printf("rightmost value: %lld\n", peek_right(l)); // 22
}

int main() {
    // Smoke Tests
    //smoke_test(); // Comment this if not needed as it prints a lot
    //Randomize Test
    test_dl(10000, 1);

    //delete_file_contents(); // only uncomment this if u will change the test case as it will delete all the existing data of results.txt and test_case.txt

    return 0;
}