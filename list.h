/*
 * Defines methods and functions.
 */
#ifndef LIST_H
#define LIST_H
#include <stddef.h>

//defines an init for list_head
#define LIST_HEAD_INIT(name) {&(name), &(name)}

//defines list_head
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

//defines list_entry
#define list_entry(ptr, type, member) ({ \
   void *__mptr = (void *)(ptr);        \
   ((type *)(__mptr - offsetof(type, member)));  \
     })

/*
 * Defines the struct list head.
 * next and prev are pointers to the next and previous list_head.
 */
struct list_head {
    struct list_head *next, *prev;
};

/*
 * @next the new list that you are adding to the existing one.
 * @head the existing head that will be added on to
 * Adds a new list to the front of the existing list.
 */
void list_add(struct list_head *next, struct list_head *head);

/*
 * @next the new list that you are adding to the existing one.
 * @head the existing head that will be added on to
 * Adds a new list to the end of the existing list.
 */
void list_add_tail(struct list_head *next, struct list_head *head);

/*
 * @entry the place where an entry in a list will be deleted from the list
 * Deletes an entry from a list
 */
void list_del(struct list_head *entry);

/*
 * @head the existing head that will be emptied
 * Empties out a list
 */
int list_empty(struct list_head *head);

/*
 * @list the list that will be added to the front of the other list
 * @head the existing head that will be added on to
 * Combines two lists so list is added to the front of head.
 */
void list_splice(struct list_head *list, struct list_head *head);

#endif