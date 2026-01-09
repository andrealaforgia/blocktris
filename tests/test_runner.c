/**
 * @file test_runner.c
 * @brief Main test runner for the Tetris game test suite
 */

#include "test_framework.h"
#include "unit/test_rotation.h"

int main(void) {
    test_init();
    
    // Run rotation tests
    run_rotation_tests();
    
    test_summary();
    
    // Return non-zero if any tests failed (for CI/build systems)
    return test_all_passed() ? 0 : 1;
}
