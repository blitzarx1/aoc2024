#include <array.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *INPUT_PATH = "input.txt";

int string_split(char *string, char sep, char ***r_array_string) {
  int i, k, len, size;
  char **array_string;

  // determine size
  size = 1, len = strlen(string);
  for (i = 0; i < len; i++) {
    if (string[i] == sep) {
      size++;
    }
  }

  array_string = malloc(size * sizeof(char *));

  i = 0, k = 0;
  array_string[k++] = string; // Save the first substring pointer
  // Split 'string' into substrings with \0 character
  while (k < size) {
    if (string[i++] == sep) {
      string[i - 1] = '\0';             // Set end of substring
      array_string[k++] = (string + i); // Save the next substring pointer
    }
  }

  *r_array_string = array_string;
  return size;
}

short is_safe(Array *nums) {
  if (nums->size == 0) {
    return 0;
  }

  int prev = nums->data[0];
  int g_diff = nums->data[nums->size - 1] - prev;
  int g_sgn = (g_diff) / abs(g_diff);
  if (g_sgn == 0) {
    return 0;
  }
  for (int i = 1; i < nums->size; i++) {
    int curr = nums->data[i];
    int diff = curr - prev;
    if (diff == 0) {
      return 0;
    }

    int sgn = diff / abs(diff);
    if (sgn != g_sgn) {
      return 0;
    }

    if (abs(diff) < 1 || abs(diff) > 3) {
      return 0;
    }

    prev = curr;
  }

  char *safe_nums_string = array_string(nums);
  printf("safe nums: %s %d\n", safe_nums_string, g_sgn);
  free(safe_nums_string);

  return 1;
}

int part1() {
  int safe_cnt = 0;

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(INPUT_PATH, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    char **splitted;
    int splitted_size = string_split(line, ' ', &splitted);

    Array nums = array_new(splitted_size);
    for (int i = 0; i < splitted_size; i++) {
      int num = atoi(splitted[i]);
      array_add(&nums, num);
    }

    if (is_safe(&nums) == 1) {
      safe_cnt++;
    }

    free(nums.data);
  }

  fclose(fp);
  free(line);

  return safe_cnt;
}

int main() {
  int safe_reports_cnt = part1();
  printf("safe reports cnt: %d\n", safe_reports_cnt);

  return EXIT_SUCCESS;
}
