#include <array.h>
#include <stdio.h>
#include <stdlib.h>

static const char *INPUT_PATH = "input.txt";

typedef struct {
} State;

void readinput(Array *a, Array *b) {
  FILE *fp;
  size_t len = 0;

  fp = fopen(INPUT_PATH, "r");
  if (fp == NULL)
    exit(1);

  int num1, num2;
  while (fscanf(fp, "%d %d", &num1, &num2) == 2) {
    int *n1 = malloc(sizeof(int));
    *n1 = num1;
    int *n2 = malloc(sizeof(int));
    *n2 = num2;

    array_add(a, n1);
    array_add(b, n2);
  }

  fclose(fp);
}

char *intstr(void *el) {
  char *res = malloc(sizeof(char *));
  sprintf(res, "%d", *(int *)el);
  return res;
}

int sort(const void *a, const void *b) { return *(int *)b - *(int *)a; }

int part1() {
  Array arr1 = array_new(2);
  Array arr2 = array_new(2);

  readinput(&arr1, &arr2);

  char *strabef = array_string(&arr1, intstr);
  printf("a before sort: %s\n", strabef);
  free(strabef);
  char *strbbef = array_string(&arr2, intstr);
  printf("b before sort: %s\n", strbbef);
  free(strbbef);

  array_sort(&arr1, sort);
  array_sort(&arr2, sort);

  char *straaf = array_string(&arr1, intstr);
  printf("a after sort: %s\n", straaf);
  free(straaf);

  char *strbaf = array_string(&arr2, intstr);
  printf("b after sort: %s\n", strbaf);
  free(strbaf);

  int distance = 0;
  for (int i = 0; i < arr1.size; i++) {
    distance += abs(*(int *)arr1.data[i] - *(int *)arr2.data[i]);
  }

  free(arr1.data);
  free(arr2.data);

  return distance;
}

long part2() {
  Array arr1 = array_new(2);
  Array arr2 = array_new(2);

  readinput(&arr1, &arr2);

  array_sort(&arr1, sort);

  int prev;
  long score = 0;
  for (int i = 0; i < arr1.size; i++) {
    int curr = *(int *)arr1.data[i];

    // printf("i: %d, curr: %d, prev: %d\n", i, curr, prev);

    int cnt = 0;
    for (int j = 0; j < arr2.size; j++) {
      if (*(int *)arr2.data[j] == curr) {
        cnt++;
      }
    }

    // printf("  found %d times\n", cnt);

    score += curr * cnt;
    prev = curr;
    cnt = 0;
  }

  return score;
}

int main() {
  int distance = part1();
  printf("distance: %d\n", distance);

  long similarity_score = part2();
  printf("similarity score: %ld\n", similarity_score);

  return EXIT_SUCCESS;
}
