#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "skipskiplist.h"

static void print_array(int *array, int size) {
    
    int *p = array;
    int *end = array + size;
    char *pad = "{ ";

    while (p < end)
    {
        printf("%s%d", pad, *p++);
        pad = ", ";
    }
    printf(" }\n");
}

int* query_builder(int n) {
    
    int* q = (int*) malloc(sizeof(int) * n);
    
    // This loop fills each index's query counts to model
    // the frequency distribution of the skiplist
    
    for (int i = 0; i < n; i++) {
        q[i] = 1 + (int) i/10000;
    }
    
    return q;
    
}

// Both of these tests are horrible design, but I'm not writing functions to 
// randomly sample a generalized distribution. That's a job for the wizards at
// Numpy and Scipy

double normal_test(sl_entry* list, int* q, int n) {
    clock_t t = clock();
    
    for (int i = 0; i < n; i++) {
        int count = q[i];
        for (int j = 0; j < count; j++) {
            char* result = sl_get(list, i);
            //free(result);
            result = NULL;  
        }
    }
    
    t = clock() - t;
    return ((double) t)/CLOCKS_PER_SEC;
}

double augmented_test(struct guard_tree* list, int* q, int n) {
    clock_t t = clock();
    
    for (int i = 0; i < n; i++) {
        int count = q[i];
        
        // printf("%d \n", i);
        
        for (int j = 0; j < count; j++) {
            char* result = sl_fast_get(list, i);
            //free(result);
            result = NULL;
        }
    }
    
    t = clock() - t;
    return ((double) t)/CLOCKS_PER_SEC;
}

int main() {
    
    int n = 100000;
    int m = 400;
    
    sl_entry* list = sl_init();
    
    for (int i = 0; i < n; i++) {
        sl_set(list, i, "a");
    }
    
    int* q = query_builder(n);
    
    // BEGIN BENCHMARKS

    printf("Normal time taken: %fs \n", normal_test(list, q, n));
        
    struct guard_tree* tree = sl_augment(list, q, n, m);

    printf("Augmented time taken: %fs \n", augmented_test(tree, q, n));
    
    return 0;
}
