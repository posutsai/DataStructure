#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#define traverse_to_pre_node_safe(head, cur, n, index)		\
	int macro_i = 0;										\
	for (macro_i = 0, cur = (head), n = cur->next;			\
		 macro_i < index - 1;								\
		 macro_i++, cur = n, n = cur->next)					\

// be aware of variable name in condition. Don't use
// the macro function while deleting node, because
// safety is not guaranteed.
#define traverse_until(head, cur, condition) 				\
	for (cur = (head);										\
		 !#condition;										\
		 cur = cur->next)

typedef struct list_el {
	list_el *next;
	int value;
} list_el_t;

typedef struct linked_list {
	list_el_t *head;
	list_el_t *tail;
	unsigned int size;
}linked_list_t;

linked_list_t *create();
bool insert(linked_list_t *, int index, int value);
bool del(linked_list_t *, int index, int *value);
size_t len(linked_list_t *);
list_el_t *find(linked_list_t *, int value);
int get(linked_list_t *, int index);
void clean(linked_list_t *);

// insert head
bool unshift(linked_list_t *, int value);
// insert tail
bool push(linked_list_t *, int value);
// delete head
bool shift(linked_list_t *, int *value);
// delete tail
bool pop(linked_list_t *, int *value);

bool is_empty(linked_list_t *);

#endif
