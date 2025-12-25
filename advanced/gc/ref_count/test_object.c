#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "munit.h"
#include "object.h" // Include your object header and function prototypes

/* --- Function Prototypes for External Functions (to avoid redefinition) --- */
extern object_t *new_int(int value);
extern object_t *new_float(float value);
extern object_t *new_string(char *value);
extern object_t *new_vector3(object_t *x, object_t *y, object_t *z);
extern object_t *new_array(int size);
extern bool arr_set(object_t *arr, object_t *obj, int idx);
extern object_t *arr_get(object_t *arr, int idx);
extern int len(object_t *obj);
extern void free_obj(object_t *obj);

/* --- Test Setup/Teardown (Context) --- */

// The setup/teardown functions are not strictly necessary for most of these
// simple tests, but are included as best practice.
static void *test_setup(const MunitParameter params[], void *user_data) {
    // No specific setup needed for most tests.
    (void)params;
    (void)user_data;
    return NULL;
}

static void test_teardown(void *fixture) {
    // No specific teardown needed as memory is freed in the tests.
    (void)fixture;
}

/* ---------------------------------------------------------------------- */
/* I. CONSTRUCTOR TESTS                        */
/* ---------------------------------------------------------------------- */

static MunitResult test_new_int(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *obj = new_int(42);
    munit_assert_not_null(obj);
    munit_assert_int(obj->kind, == , INT);
    munit_assert_int(obj->data.v_int, == , 42);

    free_obj(obj);
    return MUNIT_OK;
}

static MunitResult test_new_float(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *obj = new_float(3.14f);
    munit_assert_not_null(obj);
    munit_assert_int(obj->kind, == , FLOAT);
    munit_assert_float(obj->data.v_float, == , 3.14f);

    free_obj(obj);
    return MUNIT_OK;
}

static MunitResult test_new_string(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    char *test_str = "GC Test String";
    object_t *obj = new_string(test_str);
    munit_assert_not_null(obj);
    munit_assert_int(obj->kind, == , STRING);
    munit_assert_not_null(obj->data.v_string);
    munit_assert_string_equal(obj->data.v_string, test_str);
    // Ensure memory is separate (not just pointing to test_str)
    munit_assert_ptr_not_equal(obj->data.v_string, test_str);

    free_obj(obj);
    return MUNIT_OK;
}

static MunitResult test_new_vector3(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *x = new_int(1);
    object_t *y = new_int(2);
    object_t *z = new_int(3);
    object_t *vec = new_vector3(x, y, z);

    munit_assert_not_null(vec);
    munit_assert_int(vec->kind, == , VECTOR3);
    munit_assert_not_null(vec->data.v_vector3);
    
    // Check component integrity
    munit_assert_ptr_equal(vec->data.v_vector3->x, x);
    munit_assert_int(vec->data.v_vector3->y->data.v_int, == , 2);

    // Freeing vec should recursively free x, y, and z
    free_obj(vec);
    return MUNIT_OK;
}

static MunitResult test_new_array(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    int size = 10;
    object_t *arr = new_array(size);

    munit_assert_not_null(arr);
    munit_assert_int(arr->kind, == , ARRAY);
    munit_assert_not_null(arr->data.v_array);
    munit_assert_int(arr->data.v_array->size, == , size);
    munit_assert_not_null(arr->data.v_array->elements);
    
    // Check calloc behavior (all pointers should be NULL initially)
    for (int i = 0; i < size; i++) {
        munit_assert_null(arr->data.v_array->elements[i]);
    }

    free_obj(arr);
    return MUNIT_OK;
}

/* ---------------------------------------------------------------------- */
/* II. ARRAY ACCESS TESTS                       */
/* ---------------------------------------------------------------------- */

static MunitResult test_arr_set_get_valid(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *arr = new_array(5);
    object_t *val = new_int(99);
    int idx = 3;

    // Set test
    munit_assert_true(arr_set(arr, val, idx));
    
    // Get test
    object_t *retrieved = arr_get(arr, idx);
    munit_assert_not_null(retrieved);
    munit_assert_ptr_equal(retrieved, val);
    munit_assert_int(retrieved->data.v_int, == , 99);

    free_obj(arr); // Frees arr, which recursively frees val
    // munit_assert_ptr_equal(arr_get(arr, idx), val); // Should not assert after free
    return MUNIT_OK;
}

static MunitResult test_arr_set_boundary_checks(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *arr = new_array(5);
    object_t *val = new_int(1);

    // Test out of bounds (negative)
    munit_assert_false(arr_set(arr, val, -1));
    // Test out of bounds (too large)
    munit_assert_false(arr_set(arr, val, 5)); 
    
    // Test NULL inputs
    munit_assert_false(arr_set(NULL, val, 0));
    munit_assert_false(arr_set(arr, NULL, 0));
    
    free_obj(arr);
    free_obj(val); // Must be freed separately since it wasn't inserted
    return MUNIT_OK;
}

static MunitResult test_arr_get_boundary_checks(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *arr = new_array(5);
    
    // Test out of bounds
    munit_assert_null(arr_get(arr, -1));
    munit_assert_null(arr_get(arr, 5));
    
    // Test NULL inputs
    munit_assert_null(arr_get(NULL, 0));
    
    free_obj(arr);
    return MUNIT_OK;
}

/* ---------------------------------------------------------------------- */
/* III. LENGTH FUNCTION TEST                     */
/* ---------------------------------------------------------------------- */

static MunitResult test_len(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    object_t *arr = new_array(8);
    object_t *vec = new_vector3(new_int(0), new_int(0), new_int(0));
    object_t *str = new_string("Hi"); // Length should be 3 (H, i, \0)
    object_t *i = new_int(1);

    munit_assert_int(len(arr), == , 8);
    munit_assert_int(len(vec), == , 3);
    munit_assert_int(len(str), == , 3); // "Hi" + NULL terminator
    munit_assert_int(len(i), == , -1); // Default/Error case

    free_obj(arr);
    free_obj(vec);
    free_obj(str);
    free_obj(i);
    return MUNIT_OK;
}

/* ---------------------------------------------------------------------- */
/* IV. FREE OBJECT TEST                          */
/* ---------------------------------------------------------------------- */

static MunitResult test_free_obj_recursive_cleanup(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    // Create a complex, nested structure to test deep cleanup
    object_t *str1 = new_string("Nested String");
    object_t *i1 = new_int(10);
    object_t *v1 = new_vector3(i1, new_int(20), new_int(30)); // i1 is shared reference
    
    object_t *arr = new_array(2);
    arr_set(arr, str1, 0); // arr now owns str1
    arr_set(arr, v1, 1);   // arr now owns v1

    // Freeing 'arr' should free str1, v1, i1, and the other two ints in v1
    // If the program exits cleanly after free_obj(arr), the test passes.
    free_obj(arr); 

    // Note: We cannot assert that memory *was* freed, 
    // but the lack of a crash/segfault confirms the logic is sound.
    return MUNIT_OK;
}

static MunitResult test_cyclic_leak_detection(const MunitParameter params[], void *fixture) {
    (void)params;
    (void)fixture;

    // 1. Create two arrays
    object_t *A = new_array(1); // A count = 1
    object_t *B = new_array(1); // B count = 1

    // 2. Create the cycle
    arr_set(A, B, 0); // B count becomes 2 (Variable 'B' + Array 'A')
    arr_set(B, A, 0); // A count becomes 2 (Variable 'A' + Array 'B')

    munit_assert_int(A->ref_count, ==, 2);
    munit_assert_int(B->ref_count, ==, 2);

    // 3. The "User" drops their references (mimicking variables going out of scope)
    refcount_dec(A); // A count drops to 1 (Still held by B)
    refcount_dec(B); // B count drops to 1 (Still held by A)

    // --- THE MOMENT OF TRUTH ---
    
    // In a pure RC system, these assertions MUST be true:
    munit_assert_int(A->ref_count, ==, 1);
    munit_assert_int(B->ref_count, ==, 1);

    // If the test reaches here and the counts are 1, you have PROVED a leak.
    // These objects are now unreachable from your code, but they are still in memory.
    
    // IMPORTANT: To avoid a permanent leak in your test suite, 
    // you would technically need to manually "break" the cycle 
    // or call free_obj to clean up before finishing the test.
    
    return MUNIT_OK;
}

/* ---------------------------------------------------------------------- */
/* TEST SUITES                               */
/* ---------------------------------------------------------------------- */

static MunitTest all_tests[] = {
    {"/new_int", test_new_int, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/new_float", test_new_float, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/new_string", test_new_string, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/new_vector3", test_new_vector3, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/new_array", test_new_array, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/set_get_valid", test_arr_set_get_valid, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/set_boundary_checks", test_arr_set_boundary_checks, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/get_boundary_checks", test_arr_get_boundary_checks, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/len_function", test_len, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/free_obj_recursive", test_free_obj_recursive_cleanup, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {"/cyclic_leak_detection", test_cyclic_leak_detection, test_setup, test_teardown, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};


static const MunitSuite test_suite = {
    "/object_tests",
    all_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc)]) {
    // You must change the 'tests' array in the MunitSuite definition
    // to run the desired group (e.g., 'constructor_tests', 'array_tests', etc.).
    return munit_suite_main(&test_suite, NULL, argc, argv);
}