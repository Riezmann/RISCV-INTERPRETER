#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable {
    // TODO: define hashtable struct to use linkedlist as buckets
    int max_range;
    int size; //This shows the number of mapping
    linkedlist_t** array;
};
/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range) {
    // TODO: feel free to write your own hash function (NOT REQUIRED)$
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets) {
    // TODO: create a new hashtable
    hashtable_t* ht = malloc(sizeof(hashtable_t));

    ht->array = malloc(num_buckets * sizeof(linkedlist_t*));
    ht->max_range = num_buckets;
    ht->size = 0;

    for (unsigned int i = 0; i < num_buckets; i++) {
        ht->array[i] = ll_init();
    }

    return ht;
}

void ht_add(hashtable_t *table, int key, int value) {
    // TODO: create a new mapping from key -> value.
    // If the key already exists, replace the value.
    int pos = hash(key, table->max_range);
    linkedlist_t* temp = (linkedlist_t*) table->array[pos];    
    linkedlist_node_t *temp_node = temp -> first;
    
    bool addOne = true;

    if (temp_node != NULL) {
        while (temp_node->next != NULL && temp_node != NULL) {
            if (temp_node->key == key) {
                addOne = false;
            }
            temp_node = temp_node->next;
        }

        if (temp_node != NULL && temp_node->key == key) {
            addOne = false;
        }
    }
	
	if (addOne) {
		table -> size ++;
	}

    ll_add((linkedlist_t*) table->array[pos], key, value);
    
}

int ht_get(hashtable_t *table, int key) {
    // TODO: retrieve the value mapped to the given key.
    // If it does not exist, return 0
    int pos = hash(key, table->max_range);

    if (*(table->array + pos) == NULL) {
        return 0;
    }
    else {
        return ll_get(*(table->array + pos), key);
    }

}

int ht_size(hashtable_t *table) {
    // TODO: return the number of mappings in this hashtable
    return table->size;
}
