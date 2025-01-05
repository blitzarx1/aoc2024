#ifndef XMAS_FINDER
#define XMAS_FINDER

#include <stdbool.h>

typedef struct {
  int i;
  int j;
} Position;

/*
recursively scans for X-M-A-S entries, starting from the center position (3,3)
*/
int xmas_center_count(int area[7][7]);
Position *position_new(int i, int j);
Position *check_neighbours(int sym, int i, int j, int area[7][7]);

#endif // XMAS_FINDER
