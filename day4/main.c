#include <array.h>
#include <stdio.h>
#include <stdlib.h>

#include "xmas_finder/xmas_finder.h"

static const char *INPUT_PATH = "input.txt";

int part1() {
  FILE *fp = fopen(INPUT_PATH, "r");
  if (!fp) {
    perror("Error open the file");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  size_t bytes_read;

  while ((bytes_read = fread(&buffer, 1, sizeof(buffer), fp)) > 0) {
    for (int i = 0; i < bytes_read; i++) {
      printf("%c", buffer[i]);
    }

    printf("\n");
  }

  if (ferror(fp)) {
    perror("Error reading the file");
  } else if (feof(fp)) {
    printf("End of file reached.\n");
  }

  fclose(fp);
  return 0;
}

int main() {
  // int res1 = part1();
  // printf("result p1: %d\n", res1);

  // return EXIT_SUCCESS;

  int area[7][7];

  printf("%d\n", present(0, 1, area));
}
