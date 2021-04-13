#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

// Gaussian distribution generator
double normal(int x, int mu, int sigma) {
    return pow(2 * M_PI * pow(sigma, 2), -0.5) * exp(-0.5 * pow(x - mu, 2) / pow(sigma, 2));
}

int* query_builder(int n) {
    
    int* q = (int*) malloc(sizeof(int) * n);
    
    // This loop fills each index's query counts to model
    // the frequency distribution of the skiplist
    
    for (int i = 0; i < n; i++) {
        q[i] = (int) (500000*normal(i, n/2, n/8));
        // q[i] = (int) (1 + i/1000);
    }
    
    return q;
    
}

// Both of these tests are horrible design, but I'm not writing functions to 
// randomly sample a generalized distribution. That's a job for the wizards at
// NumPy and SciPy

double normal_test(sl_entry* list, int* q, int n, int iter) {
    clock_t t = clock();
    
    for (int k = 0; k < iter; k++) {
        for (int i = 0; i < n; i++) {
            int count = q[i];
            for (int j = 0; j < count; j++) {
                char* result = sl_get(list, i);
                //free(result);
                result = NULL;  
            }
        }
    }

    t = clock() - t;
    return ((double) t)/(CLOCKS_PER_SEC * iter);
}

double augmented_test(struct guard_tree* list, int* q, int n, int iter) {
    clock_t t = clock();
    
    for (int k = 0; k < iter; k++){
        for (int i = 0; i < n; i++) {
            int count = q[i];
            for (int j = 0; j < count; j++) {
                char* result = sl_fast_get(list, i);
                //free(result);
                result = NULL;
            }
        }
    }
    
    t = clock() - t;
    return ((double) t)/(CLOCKS_PER_SEC * iter);
}

int main() {
    
    int n = 100000;
    int m = 50;
    // int m = (int) sqrt((double) n);

    printf("%d \n", m);
    
    sl_entry* list = sl_init();
    
    for (int i = 0; i < n; i++) {
        sl_set(list, i, "a");
    }
    
    int* q = query_builder(n);
    int iter = 1;

    // BEGIN BENCHMARKS

    printf("Normal time taken: %fs \n", normal_test(list, q, n, iter));

    struct guard_tree* tree = sl_augment(list, q, n, m);

    printf("Augmented time taken: %fs \n", augmented_test(tree, q, n, iter));
    
    return 0;
}
