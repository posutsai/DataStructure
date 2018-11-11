#include "btree.h"
// one node can have m children and m - 1 keys
// the number of children should be in [ceil(m / 2), m]
btree_t *create_btree() {
	btree_t *btree_ptr = (btree_t *)malloc(sizeof(btree_t));
	btree_ptr->total_keys = 0;
	btree_ptr->root = (node_t *)malloc(sizeof(node_t));
}

void show_node(node_t *node) {
	printf("[ ");
	for (int i = 0; i < node->n_keys; i++)
		printf("%d%s ",node->keys[i], i == node->n_keys - 1? "": ",");
	printf("]");
}

void show_tree(btree_t *btree) {
	node_t **layer = (node_t **)malloc(sizeof(node_t *));
	layer[0] = btree->root;
	int layer_len = 1;
	int next_len;
	do {
		next_len = 0;
		node_t **next_layer = (node_t **)malloc(sizeof(node_t *) * layer_len * ORDER);
		for(int i = 0; i < layer_len; i++) {
			show_node(layer[i]);
			for(int j = 0; j < layer[i]->n_children; j++) {
				next_layer[next_len] = layer[i]->children[j];
				++next_len;
			}
		}
		printf("\n");
		free(layer);
		layer = next_layer;
		layer_len = next_len;
	} while (next_len > 0);
}

node_t *which_node(node_t *cur, int val) {
	// current node is leaf
	if (!cur->n_children)
		return cur;
	int i;
	for (i = 0; i < cur->n_children -1 && val > cur->keys[i]; ++i);
	return which_node(cur->children[i], val);
}

void adjust(btree_t *btree, node_t *cur, int val, int splitted_index, node_t *left, node_t *right) {
	if (cur == NULL) {
		node_t *root = (node_t *)malloc(sizeof(node_t));
		memset(root->children, 0, sizeof(node_t *) * ORDER);
		btree->root = root;
		root->n_keys = 1;
		root->n_children = 2;
		root->keys[0] = val;
		root->children[0] = left;
		root->children[1] = right;
		root->parent = NULL;
		root->i_parent = 0;
		left->parent = root;
		left->i_parent = 0;
		right->parent = root;
		right->i_parent = 1;
		return;
	}

	if (cur->n_keys < ORDER - 1) {
		int i;
		for (i = splitted_index; i < cur->n_children; i++)
			cur->children[i]->i_parent++;
		memmove(cur->children + splitted_index + 2, cur->children + splitted_index +1, sizeof(node_t *) * (cur->n_children - 1));
		cur->children[splitted_index] = left;
		left->i_parent = splitted_index;
		for (i = 0; i < cur->n_keys && val > cur->keys[i]; i++);
		memmove(cur->keys + i + 1, cur->keys + i, (cur->n_keys - i) * sizeof(int));
		cur->keys[i] = val;
		cur->n_keys++;
		cur->n_children++;
		cur->children[splitted_index + 1] = right;
		right->i_parent = splitted_index + 1;
		left->parent = cur;
		right->parent = cur;
		return;
	}

	// choose which key should be passed up
	int temp[ORDER];
	int i;
	memcpy(temp, cur->keys, sizeof(int) * (ORDER - 1));
	for (i = 0; i < ORDER - 1 && val > temp[i]; i++);
	memmove(temp + i + 1, temp + i, sizeof(int) * (ORDER - 1 - i));
	temp[i] = val;
	int val_passed_up = temp[(ORDER - 1) / 2];
	node_t *parent = cur->parent;


	// create left and right children
	node_t *split_left = (node_t *)malloc(sizeof(node_t));
	node_t *split_right = (node_t *)malloc(sizeof(node_t));
	int n_left = (ORDER - 1) / 2;
	int n_right = ORDER - n_left - 1;
	memcpy(split_left->keys, temp, sizeof(int) * n_left);
	memcpy(split_right->keys, temp + n_left + 1, sizeof(int) * n_right);
	split_left->n_keys = n_left;
	split_right->n_keys = n_right;

	// merge children
	int index = cur->i_parent;
	node_t *rearrange[ORDER + 1] = {NULL};
	memcpy(rearrange, cur->children, sizeof(node_t *) * splitted_index); // copy left segment
	memcpy(rearrange + splitted_index + 2, cur->children + splitted_index + 1, sizeof(node_t *) * (ORDER - 1 - splitted_index));
	rearrange[splitted_index] = left;
	rearrange[splitted_index + 1] = right;

	if (left && right) { // escape from leaf spliting
		split_left->n_children = split_left->n_keys + 1;
		memcpy(split_left->children, rearrange, split_left->n_children * sizeof(node_t *));
		split_right->n_children = split_right->n_keys + 1;
		memcpy(split_right->children, rearrange + split_left->n_children, split_right->n_children * sizeof(node_t *));

		// udate i_parent in left and right
		for (i = 0; i < split_left->n_children; i++) {
			split_left->children[i]->i_parent = i;
			split_left->children[i]->parent = split_left;
		}
		for (i = 0; i < split_right->n_children; i++) {
			split_right->children[i]->i_parent = i;
			split_right->children[i]->parent = split_right;
		}
	}
	free(cur);
	return adjust(btree, parent, val_passed_up, index, split_left, split_right);
}

bool insert(btree_t *btree, int val) {
	if (btree == NULL)
		return false;
	if (!btree->total_keys) {
		// insert in a empty tree
		btree->root = (node_t *)malloc(sizeof(node_t));
		btree->root->n_keys = 1;
		btree->root->n_children = 0;
		btree->root->keys[0] = val;
		btree->root->parent = NULL;
		btree->root->i_parent = 0;
		memset(btree->root->children, 0, ORDER);
		btree->total_keys++;
		return true;
	} else {
		// use DFS recursive find the node.
		node_t *cur = btree->root;
		cur = which_node(cur, val);
		if (cur->n_keys == ORDER - 1) {
			// the node is full
			adjust(btree, cur, val, cur->i_parent, NULL, NULL);
			return true;
		} else {
			int i;
			for (i = 0; i < cur->n_keys && val > cur->keys[i]; i++);
			memcpy(cur->keys + i + 1, cur->keys + i, cur->n_keys * sizeof(int));
			cur->keys[i] = val;
			cur->n_keys++;
			btree->total_keys++;
		}
		return true;
	}
}

bool has_rich_sibling(node_t *cur, int *index) {
	// search if there is any sibling is rich, maybe optimize in further version
	node_t *parent = cur->parent;
	if (parent == NULL)
		return false;
	int nearest = parent->n_children;
	for (int i = 0; i < parent->n_children; i++) {
		if (parent->children[i]->n_keys - 1 >= ceil(ORDER / 2) - 1) {
			nearest = abs(i - cur->i_parent) < nearest? i: nearest;
		}
	}
	*index = nearest;
	return nearest == parent->n_children? false: true;
}

bool del(btree_t *btree, int val) {
	node_t *cur = btree->root;
	int i = 0;
	do {
		for (i = 0; val > cur->keys[i] && i < cur->n_keys; i++);
		if (val == cur->keys[i])
			break;
		cur = cur->children[i];
	} while(cur->n_children > 0);

	printf("found node : \n");
	show_node(cur);
	printf("\n");

	int is_find = -1;
	for (i = 0; i < cur->n_keys; ++i) {
		is_find = cur->keys[i] == val ? i: is_find;
	}
	if (is_find == -1)
		return false;
	if (!cur->n_children) {
		if (cur->n_keys - 1 >= ceil(ORDER / 2) - 1) {
			// required key is in a richleaf
			memmove(cur->keys + is_find, cur->keys + is_find + 1, sizeof(int) * (ORDER - 2 - is_find));
			cur->n_keys--;
			return true;
		}

		int sib;
		if (has_rich_sibling(cur, &sib)) {
			// rich sibling exist
			node_t *parent = cur->parent;
			if (sib < cur->i_parent) {
				for (; sib < cur->i_parent; sib++) {
					node_t *start = parent->children[sib];
					node_t *end = parent->children[sib + 1];
					int passed_up = start->keys[start->n_keys - 1];
					start->n_keys--;
					int passed_down = parent->keys[sib];
					parent->keys[sib] = passed_up;
					memmove(end->keys + 1, end->keys, sizeof(int) * end->n_keys);
					end->keys[0] = passed_down;
					end->n_keys++;
				}
				is_find++;
				memmove(cur->keys + is_find, cur->keys + is_find + 1, sizeof(int) * (ORDER - 2 - is_find));
				cur->n_keys--;
			} else {
				for (; sib > cur->i_parent; sib--) {
					node_t *start = parent->children[sib];
					node_t *end = parent->children[sib - 1];
					int passed_up = start->keys[0];
					memmove(start->keys, start->keys + 1, (start->n_keys - 1) * sizeof(int));
					start->n_keys--;
					int passed_down = parent->keys[sib - 1];
					printf("pass %d down, sib is %d\n", passed_down, sib);
					parent->keys[sib - 1] = passed_up;
					end->keys[end->n_keys] = passed_down;
					end->n_keys++;
				}
				memmove(cur->keys + is_find, cur->keys + is_find + 1, sizeof(int) * (ORDER - 2 - is_find));
				cur->n_keys--;
			}
			printf("found sibling index is %d\n", sib);
			return true;
		}
		printf("no rich sibling\n");
		return true;

			// right brother node is rich
			memmove(cur->keys + is_find, cur->keys + is_find + 1, sizeof(int) * (ORDER - 2 - is_find));
			cur->keys[cur->n_keys - 1] = cur->parent->keys[0];

			// switch to its parent
			node_t *parent = cur->parent;
			memmove(parent->keys + cur->i_parent, parent->keys + cur->i_parent + 1, (parent->n_keys - cur->i_parent - 1) * sizeof(int));
			parent->keys[parent->n_keys - 1] = parent->children[cur->i_parent + 1]->keys[0];

			node_t *bro = parent->children[cur->i_parent + 1];
			memmove(bro->keys, bro->keys + 1, (bro->n_keys - 1) * sizeof(int));
			bro->n_keys--;

	}

}

int main(int argc, char *argv[]) {
	btree_t *btree = create_btree();
	for (int i = atoi(argv[1]); i > 0; i--)
		insert(btree, i);
	del(btree, 20);
	del(btree, 19);
	del(btree, 18);
	printf("=======================================\n");
	show_tree(btree);
	return 0;
}
