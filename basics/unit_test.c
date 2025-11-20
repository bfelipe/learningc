#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "munit.h"
#include "functions.h"   // Your header declaring sum, say_hello, get_name

/* -------- Test: sum() -------- */
static MunitResult test_sum(const MunitParameter params[], void *user_data) {
    munit_assert_int(sum(1, 2), ==, 3);
    munit_assert_int(sum(-5, 5), ==, 0);
    munit_assert_int(sum(100, 200), ==, 300);
    return MUNIT_OK;
}

/* -------- Test: say_hello() -------- */
/* We redirect stdout to a buffer and check the printed message */
static MunitResult
test_say_hello(const MunitParameter params[], void *user_data) {
    char buffer[256];

    // Redirect stdout to buffer
    FILE *temp = freopen("test_output.txt", "w+", stdout);
    munit_assert_not_null(temp);

    char * name = get_name();
    say_hello(name);

    fflush(stdout);
    fseek(temp, 0, SEEK_SET);
    fgets(buffer, sizeof(buffer), temp);

    munit_assert_string_equal(buffer, "hello Alan Turing\n");

    // Restore stdout
    freopen("/dev/tty", "w", stdout);

    return MUNIT_OK;
}


/* -------- Test: get_name() -------- */
static MunitResult test_get_name(const MunitParameter params[], void *user_data) {
    const char *name = get_name();
    munit_assert_string_equal(name, "Alan Turing");
    return MUNIT_OK;
}

/* -------- Test Suite Definition -------- */
static MunitTest tests[] = {
    { "/test_sum", test_sum, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/test_say_hello", test_say_hello, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/test_get_name", test_get_name, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
    "/functions_tests",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

/* -------- Main -------- */
int main(int argc, char* argv[]) {
    return munit_suite_main(&test_suite, NULL, argc, argv);
}
