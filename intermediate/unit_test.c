#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "munit.h"

int * heap_allocation(int size) {
  int *int_array = (int *)malloc(size * sizeof(int)); // cast pointer to a int pointer
  if (int_array == NULL) {
    printf("fail to allocate memory\n");
    exit(1);
  }
  return int_array;
}

bool is_on_stack(void *ptr) {
  void *stack_top = __builtin_frame_address(0);
  uintptr_t stack_top_addr = (uintptr_t)stack_top;
  uintptr_t ptr_addr = (uintptr_t)ptr;

  // Check within a threshold in both directions (e.g., 1MB)
  uintptr_t threshold = 1024;

  return ptr_addr >= (stack_top_addr - threshold) && ptr_addr <= (stack_top_addr + threshold);
}

static MunitResult test_heap_allocation(const MunitParameter params[], void *user_data) {
    int *result = heap_allocation(20);
    munit_assert_ptr_not_null(result);
    result[0] = 1;
    munit_assert_true(1 == result[0]);
    munit_assert_false(is_on_stack(result));
    free(result);
    return MUNIT_OK;
}

static MunitTest tests[] = {
    { "/test_heap_allocation", test_heap_allocation, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    "/memory_allocation_tests",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&test_suite, NULL, argc, argv);
}
