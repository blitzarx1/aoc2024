// ---- array.h
#include <stdio.h>
#include <stdlib.h>

#define RESIZE_RATE 2

typedef struct {
  size_t size;
  size_t capacity;
  int *data;
} Array;

Array array_new(size_t capacity) {
  Array array;
  int *data = malloc(capacity * sizeof(int));

  if (data == NULL) {
    perror("Error allocating memory for array");
    exit(EXIT_FAILURE);
  }

  array.size = 0;
  array.capacity = capacity;
  array.data = data;

  return array;
}

Array *array_resize(Array *array, size_t new_capacity) {
  int *new_data = realloc(array->data, new_capacity * sizeof(int));
  if (new_data == NULL) {
    perror("Error allocating memory for array");
    free(array->data);
    exit(EXIT_FAILURE);
  }

  array->data = new_data;
  array->capacity = new_capacity;

  return array;
}

void array_add(Array *array, int val) {
  if (array->size == array->capacity) {
    array_resize(array, array->capacity * 2);
  }

  array->data[array->size] = val;
  array->size++;
}

// ---- array helpers
char *array_string(const Array *array) {
  // 12 chars per number, plus brackets and spaces
  size_t buffer_size = array->size * 12 + 2;
  char *result = malloc(buffer_size);
  if (result == NULL) {
    perror("Error allocating memory for array string");
    exit(EXIT_FAILURE);
  }

  char *current = result;
  *current++ = '[';
  for (size_t i = 0; i < array->size; i++) {
    int written = sprintf(current, "%d", array->data[i]);
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

char *array_string_debug(const Array *array) {
  char *array_contents = array_string(array);

  // Include extra space for size and capacity info
  size_t debug_buffer_size = strlen(array_contents) + 50;
  char *debug_string = malloc(debug_buffer_size);
  if (debug_string == NULL) {
    perror("Error allocating memory for debug string");
    free(array_contents);
    exit(EXIT_FAILURE);
  }

  snprintf(debug_string, debug_buffer_size,
           "Size: %zu, Capacity: %zu, Contents: %s", array->size,
           array->capacity, array_contents);

  free(array_contents);
  return debug_string;
}

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void array_sort(Array *array) {
  if (array == NULL || array->data == NULL || array->size == 0) {
    return;
  }

  qsort(array->data, array->size, sizeof(int), compare);
}

// ---- end
// ---- end

#include <stdio.h>
#include <stdlib.h>

static const char *INPUT_PATH = "input.txt";

void readinput(Array *a, Array *b) {
  FILE *fp;
  size_t len = 0;
  ssize_t read;

  fp = fopen(INPUT_PATH, "r");
  if (fp == NULL)
    exit(1);

  int num1, num2;
  while (fscanf(fp, "%d %d", &num1, &num2) == 2) {
    array_add(a, num1);
    array_add(b, num2);
  }

  fclose(fp);
}

void part1() {
  Array arr1 = array_new(2);
  Array arr2 = array_new(2);

  readinput(&arr1, &arr2);

  array_sort(&arr1);
  array_sort(&arr2);

  int res = 0;
  for (int i = 0; i < arr1.size; i++) {
    res += abs(arr1.data[i] - arr2.data[i]);
  }

  printf("%d\n", res);

  free(arr1.data);
  free(arr2.data);
}

int main() {
  part1();
  return 0;
}
