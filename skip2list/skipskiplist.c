// (C) 2013 by Troy Deck; see LICENSE.txt for details

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "skipskiplist.h"

int seeded = 0;

void sl_free_entry(sl_entry * entry);

// Returns a random number in the range [1, max] following the geometric 
// distribution.
int grand (int max) {
    int result = 1;

    while (result < max && (rand() > RAND_MAX / 2)) {
        ++ result;
    }

    return result;
}

// The core part of the algorithm. It takes the frequency distribution
// of the queries and outputs an array containing the guard entries.

int* guard_optimizer(int* q, int n, int m) {
    
    int a = 0;
    int C = 1;
    
    // Stores the guard entry indices
    int* S = (int*) malloc(sizeof(int)*(m+2));
    
    for (int i = 0; i < m + 2; i++) {
        S[i] = (n*i)/(m+1);
    }
    
    // Temporary dynamic programming array to store relative frequencies
    int* T = (int*) malloc(sizeof(int)*(m+1));
    
    for (int i = 1; i < m + 2; i++) {
        int freq = 0;
        for(int j = S[i-1]; j < S[i]+a; j++) {
            freq += q[j];
        }
        T[i-1] = freq;
    }
    
    for (int k = 0; k < n; k++) {
        
        for (int i = 1; i < m + 1; i++) {
            
            if ((S[i] - S[i-1]) <= C || (S[i+1]-S[i]) <= C) {
                continue;
            }
                
            int l_freq = T[i-1];
            int r_freq = T[i];
            
            // I could use abs here, but is my dignity worth it?
            
            if (l_freq > r_freq) {
                T[i-1] -= q[S[i]-1];
                T[i] += q[S[i]-1];
                S[i] -= C;
            }
            else if (l_freq < r_freq) {
                T[i-1] += q[S[i]];
                T[i] -= q[S[i]];
                S[i] += C;
            }
            
        }
        
    }
    
    // i'm a memory management god
    free(T);
    T = NULL;

    return S;
    
}

// Returns a sentinel node representing the head node of a new skip list.
// Also seeds the random number generator the first time it is called.
sl_entry * sl_init() {
    // Seed the random number generator if we haven't yet
    if (!seeded) {
        srand((unsigned int) time(NULL));
        seeded = 1;
    }

    // Construct and return the head sentinel
    sl_entry * head = calloc(1, sizeof(sl_entry)); // Calloc will zero out next
    if (!head) return NULL; // Out-of-memory check
    head->height = MAX_SKIPLIST_HEIGHT;

    return head;
}

// Frees all nodes in the skiplist
void sl_destroy(sl_entry * head) {
    sl_entry * current_entry = head;
    sl_entry * next_entry = NULL;
    while (current_entry) {
        next_entry = current_entry->next[0];
        sl_free_entry(current_entry);
        current_entry = next_entry;
    }
}

// Calculates the optimal guard entry positions with guard_optimizer.
// Then it returns a guard_tree struct which contains
//  (1) A sorted array containing the pointers to each of the m + 2 guard entries
//  (2) Another sorted array containing the keys of each guard entry

struct guard_tree* sl_augment(sl_entry* head, int* q, int n, int m) {

    // int* S = guard_optimizer(q, n, m);

    int* S = (int*) malloc(sizeof(int)*(m+2));
    
    for (int i = 0; i < m + 2; i++) {
        S[i] = (n*i)/(m+1);
    }

    sl_entry** guards = (sl_entry**) malloc((m+2) * sizeof(sl_entry*));
    int* indices = (int*) malloc((m+2) * sizeof(int));

    sl_entry* curr = head;
    int level = 0;
    int i = 0;
    int j = 0;
    
    while (j < m + 2) {
        if (i == S[j]) {
            guards[j] = curr;
            indices[j] = curr->key;
            j++;
        }
        i++;
        curr = curr->next[level];
    }

    free(S);
    S = NULL;

    struct guard_tree* tree = (guard_tree*) malloc(sizeof(guard_tree));

    tree->indices = indices;
    tree->entries = guards;
    tree->length = m + 2;

    return tree;
        
}

// Uses binary search on the guard entries to find the closest guard entry.
// It then uses a regular sl_get skiplist search to find the queried key.

char* sl_fast_get(guard_tree* tree, int key) {
    
    int l = 0;
    int m = 0;
    int r = tree->length;
    
    while (l <= r) {
        m = l + ((r - l) / 2);
        
        if (tree->indices[m] == key){
            return tree->entries[m]->value;
        }
  
        if (tree->indices[m] < key)
            l = m + 1;
        else
            r = m - 1;
    }
    
    if (tree->entries[m]->key > key)
        return sl_get(tree->entries[m-1], key);
    else
        return sl_get(tree->entries[m], key);

}
    

// Searches for an entry by key in the skip list, and returns a copy of
// the associated value, or NULL if the key was not found.
char * sl_get(sl_entry * head, int key) {
    
    sl_entry * curr = head;
    
    int level = head->height - 1;
    
    // Find the position where the key is expected
    while (curr != NULL && level >= 0) {
        if (curr->next[level] == NULL) {
            -- level;
            
        } 
        else {
            
            int cmp = curr->next[level]->key - key;
            
            if (cmp == 0) { // Found a match
                return _strdup(curr->next[level]->value); 
            } else if (cmp > 0) { // Drop down a level 
                -- level;
            } else { // Keep going at this level
                curr = curr->next[level];
            }
        }
    }
    // Didn't find it
    return NULL;
}

// Inserts copies of a key, value pair into the skip list,
// replacing the value associated with the key if it is already
// in the list.
void sl_set(sl_entry * head, int key, char * value) {
    
    sl_entry * prev[MAX_SKIPLIST_HEIGHT];
    sl_entry * curr = head;
    int level = head->height - 1;

    // Find the position where the key is expected
    while (curr != NULL && level >= 0) {
        prev[level] = curr;
        
        if (curr->next[level] == NULL) {
            -- level;
        } 
        
        else {
            
            int cmp = curr->next[level]->key - key;
            
            if (cmp == 0) { // Found a match, replace the old value
                free(curr->next[level]->value);
                curr->next[level]->value = _strdup(value);
                return;
            } else if (cmp > 0) { // Drop down a level 
                -- level;
            } else { // Keep going at this level
                curr = curr->next[level];
            }
            
        }
    }

    // Didn't find it, we need to insert a new entry
    sl_entry * new_entry = malloc(sizeof(sl_entry));
    new_entry->height = grand(head->height);
    new_entry->key = key;
    new_entry->value = _strdup(value);
    
    int i;
    
    // Null out pointers above height
    for (i = MAX_SKIPLIST_HEIGHT - 1; i > new_entry->height; -- i) { 
        new_entry->next[i] = NULL;
    }
    
    // Tie in other pointers
    
    for (i = new_entry->height - 1; i >= 0; -- i) {
        new_entry->next[i] = prev[i]->next[i];
        prev[i]->next[i] = new_entry;
    }
    
}

// Frees the memory allocated for a skiplist entry.
void sl_free_entry(sl_entry * entry) {
    //free(entry->key);
    //entry->key = NULL;
    free(entry->value);
    entry->value = NULL;

    free(entry);
    entry = NULL;
}

// Removes a key, value association from the skip list.
void sl_unset(sl_entry * head, int key) {
    sl_entry * prev[MAX_SKIPLIST_HEIGHT];
    sl_entry * curr = head;
    int level = head->height - 1;

    // Find the list node just before the condemned node at every
    // level of the chain
    int cmp = 1;
    while (curr != NULL && level >= 0) {
        prev[level] = curr;
        if (curr->next[level] == NULL) {
            -- level;
        } else {
            cmp = curr->next[level]->key - key;
            if (cmp >= 0) { // Drop down a level 
                -- level;
            } else { // Keep going at this level
                curr = curr->next[level];
            }
        }
    }

    // We found the match we want, and it's in the next pointer
    if (curr && !cmp) { 
        sl_entry * condemned = curr->next[0];
        // Remove the condemned node from the chain
        
        int i;
        
        for (i = condemned->height - 1; i >= 0; -- i) {
          prev[i]->next[i] = condemned->next[i];
        }
        
        // Free it
        sl_free_entry(condemned);
        condemned = NULL;
    }
}