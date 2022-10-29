#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "models.h"
#include "vector.h"

vector_t *new_vector(type_t type) {
    vector_t *vector = malloc(sizeof(vector_t));
    if (!vector) exit(1);

	vector->table = malloc(sizeof(void *));
	if (!vector->table) exit(1);

	vector->type = type;
	vector->count = 0;
	vector->table[0] = NULL;

    return vector;
}

int vector_count(vector_t *vector) {
	return vector->count;
}

void *vector_at(vector_t *vector, int index) {
	return vector->table[index];
}

void vector_push(vector_t *vector, void *element) {
	vector->table = realloc(vector->table, (vector->count + 2) * sizeof(void*));
	if (!vector->table) exit(1);

	vector->table[vector->count] = element;
	vector->count++;
	vector->table[vector->count] = NULL;
}

void vector_clear(vector_t *vector) {
	free(vector->table);
    free(vector);
}