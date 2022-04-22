#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linkedlist.h"



linkedlist_t *ll_init()
{
    // TODO: create a new linked list
 
    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example, 
    // the linkedlist you are currently implementing usually have free() in the
    // ll_delete() function. Since we are not asking you to implement
    // the ll_delete() function, you will not be using free() in this case.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    // TODO: set metadata for your new list and return the new list

    list->first = NULL;
    list->length = 0;

    return list;
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.

	linkedlist_node_t* current_node = list->first;
    
    while (current_node != NULL && current_node -> next != NULL) {

        if (current_node->key == key) {
            current_node->value = value;
            return;
        }

        current_node = current_node->next;
    
    }

    linkedlist_node_t* new_node = malloc(sizeof(linkedlist_node_t));
    
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;

    if (list->first == NULL) {
        list->first = new_node;
        list->length++;
    } else if (list -> first -> key == key) {
    	list -> first -> value = value;
	} 

    else if (current_node -> next == NULL && current_node->key != key) {
        current_node->next = new_node;
        list->length++;
    }

}

int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.
    linkedlist_node_t* node = list->first;
    
    while (node != NULL) {

        if (node->key == key) {
            return node->value;
        }

        node = node->next;
    }

   
    return 0;
}

int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    return list->length;
}
