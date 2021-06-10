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
        //q[i] = (int) (250000*normal(i, n/2, 20000));
        q[i] = (int) (2);
    }
    
    return q;
    
}

// Both of these tests are horrible design, but I'm not writing functions to 
// randomly sample a generalized distribution. That's a job for the wizards at
// NumPy and SciPy

double normal_test(sl_entry* list, int* q, int n, int iter) {
    clock_t t = clock();

    for (int i = 0; i < n; i++) {
        int count = q[i];
        for (int j = 0; j < count; j++) {
            char* result = sl_get(list, i);
            //free(result);
            result = NULL;
        }
    }

    return ((double) (clock() - t))/(CLOCKS_PER_SEC);
}

double augmented_test(struct guard_tree* list, int* q, int n, int iter) {
    clock_t t = clock();

    for (int i = 0; i < n; i++) {
        int count = q[i];
        for (int j = 0; j < count; j++) {
            char* result = sl_fast_get(list, i);
            //free(result);
            result = NULL;
        }
    }

    return ((double) (clock() - t))/(CLOCKS_PER_SEC);

}

int main() {
    
    int n = 100000;
    // int m = 50;
    int m = (int) sqrt((double) n);

    int* q = query_builder(n);
    int total_queries = 0;

    for (int i = 0; i < n; i++) {
        total_queries += q[i];
    }

    printf("%d %d \n \n", m, total_queries);

    int iter = 100;

    double normal_time_sum = 0;
    double augmented_time_sum = 0;

    for (int k = 0; k < iter; k++){

        sl_entry* list = sl_init();

        for (int i = 0; i < n; i++) {
            sl_set(list, i, "a");
        }

        struct guard_tree* tree = sl_augment(list, q, n, m);

        double time1 = augmented_test(tree, q, n, iter);

        augmented_time_sum += time1;

        // double time2 = normal_test(list, q, n, iter);
        double time2 = 1;

        normal_time_sum += time2;

        printf("%d: %fs vs %fs \n", k, time1, time2);

        sl_destroy(list);
        
        free(tree->entries);
        free(tree->indices);
        free(tree);

    }

    printf("\nRegular time taken on average for %d iterations: %fs \n", iter, normal_time_sum/iter);
    printf("New time taken on average for %d iterations: %fs \n \n", iter, augmented_time_sum/iter);

    printf("Average normal query throughput: %f q/s \n", total_queries/(normal_time_sum/iter));
    printf("Average new query throughput: %f q/s \n \n", total_queries/(augmented_time_sum/iter));

    printf("Speed difference: %f \n", normal_time_sum/augmented_time_sum);

    return 0;
}
