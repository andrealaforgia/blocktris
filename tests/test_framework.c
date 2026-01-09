/**
 * @file test_framework.c
 * @brief Simple test framework implementation
 */

#include "test_framework.h"

// Global test statistics
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

void test_init(void) {
    tests_run = 0;
    tests_passed = 0;
    tests_failed = 0;
    printf("=== Tetris Game Test Suite ===\n\n");
}

void test_summary(void) {
    printf("=== Test Summary ===\n");
    printf("Total tests: %d\n", tests_run);
    printf(TEST_COLOR_GREEN "Passed: %d" TEST_COLOR_RESET "\n", tests_passed);
    if (tests_failed > 0) {
        printf(TEST_COLOR_RED "Failed: %d" TEST_COLOR_RESET "\n", tests_failed);
    } else {
        printf("Failed: %d\n", tests_failed);
    }
    
    double success_rate = tests_run > 0 ? (double)tests_passed / tests_run * 100.0 : 0.0;
    printf("Success rate: %.1f%%\n", success_rate);
    
    if (tests_failed == 0) {
        printf(TEST_COLOR_GREEN "\nAll tests passed! 🎉" TEST_COLOR_RESET "\n");
    } else {
        printf(TEST_COLOR_RED "\nSome tests failed! 💥" TEST_COLOR_RESET "\n");
    }
}

bool test_all_passed(void) {
    return tests_failed == 0 && tests_run > 0;
}
