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
  array_string[k++] = string; // save the first substring pointer
  // split 'string' into substrings with \0 character
  while (k < size) {
    if (string[i++] == sep) {
      string[i - 1] = '\0';             // set end of substring
      array_string[k++] = (string + i); // save the next substring pointer
    }
  }

  *r_array_string = array_string;
  return size;
}

short is_safe(Array *nums, int *idx) {
  if (nums->size == 0) {
    return 0;
  }

  int prev = *(int *)(nums->data[0]);
  int g_diff = *(int *)nums->data[nums->size - 1] - prev;
  int g_sgn = (g_diff) / abs(g_diff);
  for (int i = 1; i < nums->size; i++) {
    int curr = *(int *)nums->data[i];
    int diff = curr - prev;

    int sgn = diff / abs(diff);
    if (sgn != g_sgn) {
      if (idx != NULL) {
        *idx = i;
      }

      return 0;
    }

    if (abs(diff) < 1 || abs(diff) > 3) {
      if (idx != NULL) {
        *idx = i;
      }

      return 0;
    }

    prev = curr;
  }

  return 1;
}

char *str(void *el) {
  char *buff = malloc(sizeof(int));
  sprintf(buff, "%d", *(int *)el);
  return buff;
}

short is_safe_with_mod(Array *nums, int *mod) {
  if (mod == NULL) {
    printf("\n");
  }

  printf("checking: ");

  if (mod == NULL) {
    char *nums_str = array_string(nums, &str);
    printf("%s", nums_str);
    free(nums_str);

    int idx = 0;
    if (is_safe(nums, &idx) == 0) {
      printf(" bad;\n skipping %d -> ", idx);

      if (is_safe_with_mod(nums, &idx) == 0) {
        int prev = idx - 1;
        printf(" skipping %d -> ", prev);
        return is_safe_with_mod(nums, &prev);
      } else {
        return 1;
      }
    }
  } else {
    Array nums_copy = array_copy(nums);
    array_pop(&nums_copy, *mod);

    char *nums_copy_str = array_string(&nums_copy, str);
    printf("%s", nums_copy_str);
    free(nums_copy_str);

    int idx = 0;
    if (is_safe(&nums_copy, &idx) == 0) {
      printf(" bad;\n");
      return 0;
    }
  }

  printf(" good;\n");
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
      int *num = malloc(sizeof(int));
      *num = atoi(splitted[i]);
      array_add(&nums, num);
    }

    if (is_safe(&nums, NULL) == 1) {
      safe_cnt++;
    }

    free(nums.data);
  }

  fclose(fp);
  free(line);

  return safe_cnt;
}

int part2() {
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
      int *num = malloc(sizeof(int));
      *num = atoi(splitted[i]);
      array_add(&nums, num);
    }

    if (is_safe_with_mod(&nums, NULL) == 1) {
      safe_cnt++;
    }

    free(nums.data);
  }

  fclose(fp);
  free(line);

  return safe_cnt;
}

int main() {
  int safe_reports_p1 = part1();
  printf("safe reports: %d\n", safe_reports_p1);

  int safe_reports_p2 = part2();
  printf("safe reports with modifications: %d\n", safe_reports_p2);

  return EXIT_SUCCESS;
}
