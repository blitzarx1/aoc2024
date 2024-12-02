#include <array.h>
#include <stdio.h>
#include <stdlib.h>

static const char *INPUT_PATH = "input.txt";

void readinput(Array *a, Array *b) {
  FILE *fp;
  size_t len = 0;

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

int part1() {
  Array arr1 = array_new(2);
  Array arr2 = array_new(2);

  readinput(&arr1, &arr2);

  array_sort(&arr1);
  array_sort(&arr2);

  int distance = 0;
  for (int i = 0; i < arr1.size; i++) {
    distance += abs(arr1.data[i] - arr2.data[i]);
  }

  free(arr1.data);
  free(arr2.data);

  return distance;
}

long part2() {
  Array arr1 = array_new(2);
  Array arr2 = array_new(2);

  readinput(&arr1, &arr2);

  array_sort(&arr1);

  int prev;
  long score = 0;
  for (int i = 0; i < arr1.size; i++) {
    int curr = arr1.data[i];

    // printf("i: %d, curr: %d, prev: %d\n", i, curr, prev);

    int cnt = 0;
    for (int j = 0; j < arr2.size; j++) {
      if (arr2.data[j] == curr) {
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
