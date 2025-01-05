#include "xmas_finder.h"
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

int area[7][7] = {
  {0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
  {0,0,0,'U',0,0,0},
  {0,0,0,'X','M','A','S'},
  {0,0,'M',0,0,0,0},
  {0,'A',0,0,0,0,0},
  {'S',0,0,0,0,0,0},
};

void check_positions(Position* got, Position* expected) {
  if ((got == NULL && expected != NULL) || (expected == NULL && got != NULL)) {
    fprintf(stderr, "\ngot or expected is NULL\n");

    CU_FAIL();
    return;
  }
  
  if (memcmp(got, expected, sizeof(Position)) == 0 )  {
    return;
  }; 

  // fail

  fprintf(stderr, "\nERR: got.i = %d, got.j = %d; expected.i = %d, excpected.j = %d\n", got->i, got->j, expected->i, expected->j);

  CU_FAIL();
  return;
}


void test_check_neighbours_up(void) {
  Position* expected = position_new(2,3);
  Position* got = check_neighbours('U', 3, 3, area);

  check_positions(got, expected);
}

void test_check_neighbours_down(void) {
  Position* expected = position_new(3,3);
  Position* got = check_neighbours('X', 2, 3, area);

  check_positions(got, expected);
}

void test_check_neighbours_left(void) {
  Position* expected = position_new(3,3);
  Position* got = check_neighbours('X', 3, 4, area);

  check_positions(got, expected);
}

void test_check_neighbours_right(void) {
  Position* expected = position_new(3,6);
  Position* got = check_neighbours('S', 3, 5, area);

  check_positions(got, expected);
}

void test_check_neighbours_up_right(void) {
  Position* expected = position_new(3,6);
  Position* got = check_neighbours('S', 4, 6, area);

  check_positions(got, expected);
}

int init_suite(void) { return 0; }

int clean_suite(void) { return 0; }

int main() {
  if (CU_initialize_registry() != CUE_SUCCESS) {
    return CU_get_error();
  }

  CU_pSuite check_neighbours_suite =
      CU_add_suite("Test check_neighbours", init_suite, clean_suite);
  if (check_neighbours_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
    CU_add_test(check_neighbours_suite, "up", test_check_neighbours_up) == NULL ||
    CU_add_test(check_neighbours_suite, "down", test_check_neighbours_down) == NULL ||
    CU_add_test(check_neighbours_suite, "left", test_check_neighbours_left) == NULL ||
    CU_add_test(check_neighbours_suite, "right", test_check_neighbours_right) == NULL ||
    CU_add_test(check_neighbours_suite, "up right", test_check_neighbours_up_right) == NULL
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  CU_cleanup_registry();
  return CU_get_error();
}
