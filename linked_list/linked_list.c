#include "linked_list.h"

linked_list_t *create() {
	linked_list_t *q = (linked_list_t *)malloc(sizeof(linked_list_t));
	if (q == NULL)
		return NULL;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}

bool insert(linked_list_t *q, int index, int value) {
	if (q == NULL || index < 0 || index > q->size)
		return false;

 	// insert to head
	if (!index)
		return unshift(q, value);
	// insert to tail
	if (index == q->size)
		return push(q, value);

	// insert to the specific index (1 ~ size-1)
	list_el_t *cur = NULL;
	list_el_t *n = NULL;
	traverse_to_pre_node_safe(q->head, cur, n, index);
	list_el_t *node = (list_el_t *)malloc(sizeof(list_el_t));
	if (node == NULL)
		return false;
	cur->next = node;
	node->next = n;
	return true;
}

bool del(linked_list_t *q, int index, int *val) {
	if (q == NULL || !q->size || index < 0 || index > q->size -1) {
		val = NULL;
		return false;
	}
	if (!index)
		return shift(q, val);
	if (index == q->size-1)
		return pop(q, val);

	list_el_t *cur = NULL;
	list_el_t *n = NULL;
	traverse_to_pre_node_safe(q->head, cur, n, index);
	cur->next = n->next;
	*val = cur->value;
	return true;
}

size_t len(linked_list_t *q) {
	return q == NULL? 0: q->size;
}

list_el_t *find(linked_list_t *q, int value) {
	if (q == NULL)
		return NULL;
	list_el_t *cur = NULL;
	traverse_until(q->head, cur, cur->value == value);
	return cur;
}

bool get(linked_list_t *q, int index, int *val) {
	if (q == NULL || index < 0 || index > q->size -1)
		return false;

	list_el_t *cur = NULL;
	list_el_t *n = NULL;
	traverse_to_pre_node_safe(q->head, cur, n, index);
	*val = n->value;
	return true;
}

void clean(linked_list_t *q) {
	if (q == NULL)
		return;
	if (!q->size) {
		free(q);
		return;
	}
	list_el_t *cur = NULL;
	list_el_t *n = NULL;
	traverse_to_pre_node_safe(q->head, cur, n, q->size) {
		free(cur);
	}
	free(n);
	free(q);
}

bool unshift(linked_list *q, int value) {
	// insertstd
	list_el_t *node = (list_el_t *)malloc(sizeof(list_el_t));
	if (node == NULL)
		return false;
	q->size++;
	list_el_t *prev_head = q->head;
	q->head = node;
	if (q->size == 1)
		q->tail = node;
	node->next = prev_head;
	node->value = value;
	return true;
}

bool push(linked_list *q, int value) {
	// insert tail
	list_el_t *node = (list_el_t *)malloc(sizeof(list_el_t));
	if (node == NULL)
		return false;
	list_el_t *prev_tail = q->tail;
	q->tail = node;
	if (!q->size)
		q->head = node;
	prev_tail->next = node;
	node->next = NULL;
	node->value = value;
	q->size++;
	return true;
}

bool shift(linked_list_t *q, int *value) {
	// delete head
	if (q == NULL || !q->size)
		return false;
	list_el_t *prev_head = q->head;
	*value = q->head->value;
	q->size--;
	if (q->size == 0) {
		q->tail = NULL;
		q->head = NULL;
		free(prev_head);
		return true;
	}
	q->head = q->head->next;
	free(prev_head);
	return true;
}

bool pop(linked_list_t *q, int *value) {
	if (q == NULL || !q->size)
		return false;
	if (q->size == 1)
		return shift(q, value);
	list_el_t *cur = NULL;
	list_el_t *n = NULL;
	traverse_to_pre_node_safe(q->head, cur, n, q->size-1);
	q->tail = cur;
	*value = n->value;
	free(n);
	cur->next = NULL;
	q->size--;
	return true;
}
