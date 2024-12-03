#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef struct {
  size_t size;
  size_t capacity;
  int *data;
} Array;

Array array_new(size_t capacity);
Array *array_resize(Array *array, size_t new_capacity);
void array_add(Array *array, int val);
char *array_string(const Array *array);
char *array_string_debug(const Array *array);
int compare(const void *a, const void *b);
void array_sort(Array *array);
int *array_pop(Array *array, int idx);
Array array_copy(Array *array);

#endif // ARRAY_H
