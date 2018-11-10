#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define ORDER 4


typedef struct node {
	uint32_t n_keys;
	uint32_t n_children;
	int keys[ORDER - 1];
	struct node *children[ORDER];
	struct node *parent;
	int i_parent;
} node_t;

typedef struct btree {
	uint32_t total_keys;
	node_t *root;
} btree_t;

btree_t *create();
