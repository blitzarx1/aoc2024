#include "array.h"
#include <stdio.h>
#include <stdlib.h>

#define RESIZE_RATE 2

Array array_new(size_t capacity) {
  Array array;
  void *data = malloc(capacity * sizeof(void *));

  if (data == NULL) {
    perror("ERROR allocating memory for array");
    exit(EXIT_FAILURE);
  }

  array.size = 0;
  array.capacity = capacity;
  array.data = data;

  return array;
}

Array *array_resize(Array *array, size_t new_capacity) {
  void *new_data = realloc(array->data, new_capacity * sizeof(void *));
  if (new_data == NULL) {
    perror("ERROR allocating memory for array");
    free(array->data);
    exit(EXIT_FAILURE);
  }

  array->data = new_data;
  array->capacity = new_capacity;

  return array;
}

void array_add(Array *array, void *val) {
  if (array->size == array->capacity) {
    array_resize(array, array->capacity * 2);
  }

  array->data[array->size] = val;
  array->size++;
}

char *array_string(const Array *array, char *(*str)(void *el)) {
  // 12 chars per number, plus brackets and spaces
  size_t buffer_size = array->size * 12 + 2;
  char *result = malloc(buffer_size);
  if (result == NULL) {
    perror("ERROR allocating memory for array string");
    exit(EXIT_FAILURE);
  }

  char *current = result;
  *current++ = '[';
  for (size_t i = 0; i < array->size; i++) {
    char *elstr = str(array->data[i]);
    int written = sprintf(current, "%s", elstr);
    free(elstr);

    current += written;

    if (i < array->size - 1) {
      *current++ = ',';
      *current++ = ' ';
    }
  }
  *current++ = ']';
  *current = '\0';

  return result;
}

#include <string.h>

char *array_string_debug(const Array *array, char *(*str)(void *el)) {
  char *array_contents = array_string(array, str);

  // include extra space for size and capacity info
  size_t debug_buffer_size = strlen(array_contents) + 50;
  char *debug_string = malloc(debug_buffer_size);
  if (debug_string == NULL) {
    perror("ERROR allocating memory for array debug string");
    free(array_contents);
    exit(EXIT_FAILURE);
  }

  snprintf(debug_string, debug_buffer_size,
           "size: %zu, capacity: %zu, contents: %s", array->size,
           array->capacity, array_contents);

  free(array_contents);
  return debug_string;
}

int diff(int (*fn)(const void *a, const void *b), void *a, void *b) {
  return fn(a, b);
}

void array_sort(Array *array, int (*compare)(const void *a, const void *b)) {
  if (array == NULL || array->data == NULL || array->size == 0) {
    return;
  }

  qsort(array->data, array->size, sizeof(void *), compare);
}

void *array_pop(Array *array, int idx) {
  if (idx >= array->size) {
    return NULL;
  }

  void *res = &array->data[idx];

  for (int i = idx + 1; i < array->size; i++) {
    array->data[i - 1] = array->data[i];
  }

  array->size--;
  return res;
}

Array array_copy(Array *array) {
  Array new_array = array_new(array->capacity);

  for (int i = 0; i < array->size; i++) {
    array_add(&new_array, array->data[i]);
  }

  return new_array;
}
