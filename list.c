/*
 * Implements the methods defined in list.h
 */
#include "list.h"

/*
 * @next the new list that you are adding to the existing one.
 * @head the existing head that will be added on to
 * Adds a new list to the front of the existing list.
 */
void list_add(struct list_head *newNode, struct list_head *head){
    struct list_head *next = head->next;
    head->next = newNode;
    newNode->prev = head;
    newNode->next = next;
    next->prev = newNode;
}

/*
 * @next the new list that you are adding to the existing one.
 * @head the existing head that will be added on to
 * Adds a new list to the end of the existing list.
 */
void list_add_tail(struct list_head *newNode, struct list_head *head){
    struct list_head *oldTail = head->prev;
    oldTail->next = newNode;
    newNode->prev = oldTail;
    newNode->next = head;
    head->prev = newNode;
}

/*
 * @entry the place where an entry in a list will be deleted from the list
 * Deletes an entry from a list
 */
void list_del(struct list_head *entry) {
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;
}

/*
 * @head the existing head that will be emptied
 * Empties out a list
 */
int list_empty(struct list_head *head){
    if(head->next == head){
        return 1;
    }
    return 0;
}

/*
 * @list the list that will be added to the front of the other list
 * @head the existing head that will be added on to
 * Combines two lists so list is added to the front of head.
 */
void list_splice(struct list_head *list, struct list_head *head){
    struct list_head *oldTail = head->prev;
    struct list_head *listTail = list->prev;
    list->prev = oldTail;
    oldTail->next = list;
    listTail->next = head;
    head->prev = listTail;
}
