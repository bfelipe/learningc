#include <stdio.h>
#include "munit.h"
#include "vm.h"
#include "object.h"

/* --- Setup and Teardown --- */

// This runs before every single test to ensure a clean slate
static void* test_setup(const MunitParameter params[], void* user_data) {
    return new_vm();
}

// This runs after every test to clean up the VM and check for memory leaks
static void test_tear_down(void* fixture) {
    vm_t* vm = (vm_t*)fixture;
    free_vm(vm);
}

/* --- Test Cases --- */

// Test 1: Verify that objects are correctly created and tracked by the VM
static MunitResult test_allocation(const MunitParameter params[], void* fixture) {
    vm_t* vm = (vm_t*)fixture;

    object_t* obj = new_int(vm, 42);
    
    munit_assert_not_null(obj);
    munit_assert_int(obj->kind, ==, INT);
    munit_assert_int(obj->data.v_int, ==, 42);
    
    // The VM should have exactly 1 object in its global list
    munit_assert_int(vm->objects->count, ==, 1);

    return MUNIT_OK;
}

// Test 2: Verify that unreferenced objects are collected and referenced ones survive
static MunitResult test_gc_basic_cleanup(const MunitParameter params[], void* fixture) {
    vm_t* vm = (vm_t*)fixture;
    
    // Create a frame to act as our root set
    frame_t* frame = new_frame(vm);
    push_frame(vm, frame);

    // 1. Create a "Live" object and register it in the frame
    object_t* live = new_float(vm, 3.14);
    frame_ref(frame, live);

    // 2. Create a "Dead" object (not in a frame)
    new_string(vm, "I am garbage");

    munit_assert_int(vm->objects->count, ==, 2);
    
    collect_garbage(vm);

    // Only the float should remain
    munit_assert_int(vm->objects->count, ==, 1);
    munit_assert_int(((object_t*)vm->objects->data[0])->kind, ==, FLOAT);

    return MUNIT_OK;
}

// Test 3: Verify that children of a Root object survive even if not directly in a frame
static MunitResult test_gc_tracing_reachability(const MunitParameter params[], void* fixture) {
    vm_t* vm = (vm_t*)fixture;
    frame_t* frame = new_frame(vm);
    push_frame(vm, frame);

    // Create children
    object_t* x = new_int(vm, 10);
    object_t* y = new_int(vm, 20);
    object_t* z = new_int(vm, 30);
    
    // Create parent Vector3
    object_t* vec = new_vector3(vm, x, y, z);
    
    // Only reference the parent Vector in the frame
    frame_ref(frame, vec);

    // Before GC: vec + x + y + z = 4 objects
    munit_assert_int(vm->objects->count, ==, 4);
    
    collect_garbage(vm);

    // After GC: All 4 must survive because vec is a root and x,y,z are children
    munit_assert_int(vm->objects->count, ==, 4);

    return MUNIT_OK;
}

// Test 4: Verify that the GC handles circular references without infinite loops
static MunitResult test_gc_circular_references(const MunitParameter params[], void* fixture) {
    vm_t* vm = (vm_t*)fixture;
    frame_t* frame = new_frame(vm);
    push_frame(vm, frame);

    object_t* arr = new_array(vm, 1);
    
    // Circularity: The array contains itself
    arr_set(arr, arr, 0);
    
    frame_ref(frame, arr);

    // Running GC should not crash or loop infinitely
    collect_garbage(vm);

    munit_assert_int(vm->objects->count, ==, 1);
    munit_assert_ptr_equal(vm->objects->data[0], arr);

    return MUNIT_OK;
}

/* --- Test Suite Configuration --- */

static MunitTest tests[] = {
    { "/allocation", test_allocation, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { "/gc/basic_cleanup", test_gc_basic_cleanup, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { "/gc/tracing", test_gc_tracing_reachability, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { "/gc/circular", test_gc_circular_references, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    "/vm_tests",     /* name */
    tests,           /* tests */
    NULL,            /* suites */
    1,               /* iterations */
    MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}