#include "xmas_finder.h"
#include <stdbool.h>
#include <stdlib.h>

int xmas_center_count(int area[7][7]) {
  int cnt = 0;

  int center = area[3][3];
  if (center != 'X') {
    return cnt;
  }

  return cnt;
}

Position *position_new(int i, int j) {
  Position *pos;

  pos = malloc(sizeof(Position));
  if (pos == NULL) {
    return NULL;
  }

  pos->i = i;
  pos->j = j;

  return pos;
}

Position *check_neighbours(int sym, int i, int j, int area[7][7]) {
  if (i < 0 || i >= 7 || j < 0 || j >= 7) {
    return NULL;
  }

  Position *pos;

  // right
  if (i + 1 < 7 && j < 7) {
    if (area[i + 1][j] == sym) {
      pos = position_new(i + 1, j);
      return pos;
    }
  }

  // left
  if (i - 1 > 0 && j < 7) {
    if (area[i - 1][j] == sym) {
      pos = position_new(i - 1, j);
      return pos;
    }
  }

  // down
  if (i < 7 && j + 1 < 7) {
    if (area[i][j + 1] == sym) {
      pos = position_new(i, j + 1);
      return pos;
    }
  }

  // up
  if (i < 7 && j - 1 > 0) {
    if (area[i][j - 1] == sym) {
      pos = position_new(i, j - 1);
      return pos;
    }
  }

  // right down
  if (i + 1 < 7 && j + 1 < 7) {
    if (area[i + 1][j + 1] == sym) {
      pos = position_new(i + 1, j + 1);
      return pos;
    }
  }

  // right up
  if (i + 1 < 7 && j - 1 > 0) {
    if (area[i + 1][j - 1] == sym) {
      pos = position_new(i + 1, j - 1);
      return pos;
    }
  }

  // left down
  if (i - 1 > 0 && j + 1 < 7) {
    if (area[i - 1][j + 1] == sym) {
      pos = position_new(i - 1, j + 1);
      return pos;
    }
  }

  // left up
  if (i - 1 > 0 && j - 1 > 0) {
    if (area[i - 1][j - 1] == sym) {
      pos = position_new(i - 1, j - 1);
      return pos;
    }
  }

  return NULL;
}
