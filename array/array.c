#include "array.h"
#define UNIT 5
struct array {
	unsigned int num_unit;
	int occupied;
	int *ptr;
};

bool is_empty(array_t *arr) {
	return arr->occupied? false: true;
}

void clean(array_t *arr) {
	free(arr);
}

int find(array_t *arr, int value) {
	int i;
	for (i = 0; *(arr->ptr + i) != value; i++) {
		if (i == arr->occupied)
			return -1;
	}
	return i;
}

int get(array_t *arr, int index) {
	if (index < 0 || index >= arr->occupied)
		return -1;
	return *(arr->ptr + index);
}

bool expand(array_t *arr) {
	if (arr == NULL)
		return false;

	int *new_space = (int *)malloc((arr->num_unit + 1) * UNIT * sizeof(int));
	if (new_space == NULL)
		return false;

	if (arr->ptr != NULL) {
		memmove(new_space, arr->ptr, arr->num_unit * UNIT * sizeof(int));
		free(arr->ptr);
	}
	arr->ptr = new_space;
	// initialization
	memset(arr->ptr + arr->num_unit * UNIT, -1, UNIT * sizeof(int));
	arr->num_unit++;
	printf("The array is expanded!!\n");
	return true;
}

bool shrink(array_t *arr) {
	if (arr == NULL)
		return false;
	int *new_space = (int *)malloc((arr->num_unit - 1) * UNIT * sizeof(int));
	if (arr->ptr!=NULL) {
		memmove(new_space, arr->ptr, (arr->num_unit - 1) * UNIT * sizeof(int));
		free(arr->ptr);
	}
	arr->ptr = new_space;
	arr->num_unit--;
	printf("The array is shrinked!!\n");
	return true;
}

array_t *create() {
	array_t *arr = (array_t *)malloc(sizeof(array_t));
	arr->num_unit = 0;
	arr->occupied = 0;
	arr->ptr = NULL;
}

bool unshift(array_t *arr, int value) {
	if (arr == NULL)
		return false;
	if (arr->occupied == arr->num_unit * UNIT)
		expand(arr);
	if (!arr->occupied)
		memmove(arr->ptr, arr->ptr+1, arr->occupied * sizeof(int));
	*(arr->ptr) = value;
	arr->occupied++;
	return true;
}

bool push(array_t *arr, int value) {
	if (arr == NULL)
		return false;
	if (arr->occupied == arr->num_unit * UNIT)
		expand(arr);
	*(arr->ptr + arr->occupied) = value;
	arr->occupied++;
	return true;
}

bool shift(array_t *arr, int *value) {
	if (arr == NULL || !arr->occupied)
		return false;
	memmove(arr->ptr, arr->ptr + 1, (arr->occupied - 1) * sizeof(int));
	*value = *(arr->ptr + arr->occupied -1);
	*(arr->ptr + arr->occupied -1) = -1;
	*(arr->ptr + arr->num_unit * UNIT - 1) = -1;
	arr->occupied--;
	if (!(arr->occupied % UNIT))
		shrink(arr);
	return true;
}

bool pop(array_t *arr, int *value) {
	if (arr == NULL)
		return false;
	*value = *(arr->ptr + arr->occupied -1);
	*(arr->ptr + arr->occupied -1) = -1;
	arr->occupied--;
	if (!(arr->occupied % UNIT))
		shrink(arr);
	return true;
}

bool insert(array_t *arr, int index, int value) {
	if (arr == NULL || index < 0 || index > arr->occupied)
		return false;
	if (!index) // index == 0
		return unshift(arr, value);
	if (index == arr->occupied) // insert at tail
		return push(arr, value);
	if (arr->occupied == arr->num_unit * UNIT)
		expand(arr);
	memmove(arr->ptr + index + 1, arr->ptr + index, (arr->occupied - index + 1) * sizeof(int));
	*(arr->ptr + index) = value;
	arr->occupied++;
	return true;
}

size_t len(array_t *arr) {
	return arr->occupied;
}

bool del(array_t *arr, int index, int *value) {
	if (arr == NULL || !arr->occupied || index < 0 || index > arr->occupied -1)
		return false;
	if (!index)
		return shift(arr, value);
	if (index == arr->occupied - 1)
		return pop(arr, value);
	memmove(arr->ptr + index, arr->ptr + index + 1, (arr->occupied - index +1) * sizeof(int));
	*(arr->ptr + arr->occupied -1) = -1;
	arr->occupied--;
	if (!(arr->occupied % UNIT))
		shrink(arr);
	return true;
}

