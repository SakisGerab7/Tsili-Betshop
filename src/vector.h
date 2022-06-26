#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "types.h"

typedef struct {
	type_t type;
	void **table;
	int count;
} vector_t;

vector_t *new_vector(type_t type);
int vector_count(vector_t *vector);
void *vector_at(vector_t *vector, int index);
void vector_push(vector_t *vector, void *element);
void vector_clear(vector_t *vector);

#endif