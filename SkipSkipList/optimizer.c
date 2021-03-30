#include <stdio.h>
#include <stdlib.h>

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
        q[i] = i*i;
    }
    
    return q;
    
}

void query_test(sl_entry* list, int* q, int n) {
    
    // This is horrible design, but I'm not writing a function to 
    // randomly sample a generalized distribution
    
    for (int i = 0; i < n; i++) {
                
        int count = q[i];
        
        for (int j = 0; j < count; j++) {
            
            char* result = sl_get(list, i);
            free(result);
            result = NULL;
            
        }
        
        printf("%d \n", i);
        
    }
    
}

int main() {
    
    int n = 10000;
    int m = 400;
    
    sl_entry* list = sl_init();
    
    for (int i = 0; i < n; i++) {
        sl_set(list, i, "a");
    }
    
    int* q = query_builder(n);
    
    sl_entry** G = sl_augment(list, q, n, m);
    
    for (int i = 0; i < m + 2; i++) {
        
        printf("%p %d \n", (void*) G[i], G[i]->key);
        
    }
    
    
/*    
    int* q = query_builder(n);
    int* S = guard_optimizer(q, n, m);
    
    print_array(S, m+2);
    
    free(q);
    q = NULL;
    
    free(S);
    S = NULL;
*/
    return 0;
}
