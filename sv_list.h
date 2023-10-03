#ifndef SV_LIST_H
#define SV_LIST_H

#include <stdlib.h>


/**
 * Linked list
 */
typedef struct ListNode {
    void* data;
    struct ListNode* tail;
} List;


/**
 * Adds data to the front of the list l
 * @param l: the list to add the data to 
 * @param data: void pointer to the data to be added to the list
 */
void list_add(List** l, void* data)
{
    List* head = malloc(sizeof(List));
    head->data = data;
    head->tail = *l;
    *l = head;
}

#endif
