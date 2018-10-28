#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct array array_t;

array_t *create();
bool insert(array_t *, int index, int value);
bool del(array_t *, int index, int *value);
size_t len(array_t *);
int find(array_t *, int value);
int get(array_t *, int index);
void clean(array_t *);
bool unshift(array_t *, int value);
bool push(array_t *, int value);
bool shift(array_t *, int *value);
bool pop(array_t *, int *value);
bool is_empty(array_t *);

#endif
