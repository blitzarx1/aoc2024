#include "xmas_finder.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* Example function to be tested */
int add(int a, int b) {
    return a + b;
}

/* Test function */
void test_addition(void) {
    CU_ASSERT(add(2, 3) == 5);
    CU_ASSERT(add(-1, 1) == 0);
}

/* Initialize test suite */
int init_suite(void) {
    return 0;
}

/* Clean up test suite */
int clean_suite(void) {
    return 0;
}

int main() {

    int area[7][7];
    present(1, 2, area);
    
    /* Initialize CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    /* Add a suite to the registry */
    CU_pSuite pSuite = CU_add_suite("Example_Suite", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the test to the suite */
    if (CU_add_test(pSuite, "Test Addition", test_addition) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run the tests */
    CU_basic_run_tests();

    /* Clean up the registry */
    CU_cleanup_registry();
    return CU_get_error();
}
