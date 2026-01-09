/**
 * @file test_framework.h
 * @brief Simple test framework for Tetris game testing
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Test statistics
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

// Colors for output (using TEST_ prefix to avoid conflicts)
#define TEST_COLOR_GREEN "\033[32m"
#define TEST_COLOR_RED "\033[31m"
#define TEST_COLOR_YELLOW "\033[33m"
#define TEST_COLOR_RESET "\033[0m"

// Test macros
#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf(TEST_COLOR_GREEN "✓" TEST_COLOR_RESET " %s\n", message); \
        } else { \
            tests_failed++; \
            printf(TEST_COLOR_RED "✗" TEST_COLOR_RESET " %s\n", message); \
            printf("  Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            printf(TEST_COLOR_GREEN "✓" TEST_COLOR_RESET " %s\n", message); \
        } else { \
            tests_failed++; \
            printf(TEST_COLOR_RED "✗" TEST_COLOR_RESET " %s (expected: %d, got: %d)\n", message, (int)(expected), (int)(actual)); \
            printf("  Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_STRING_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            printf(TEST_COLOR_GREEN "✓" TEST_COLOR_RESET " %s\n", message); \
        } else { \
            tests_failed++; \
            printf(TEST_COLOR_RED "✗" TEST_COLOR_RESET " %s (expected: '%s', got: '%s')\n", message, expected, actual); \
            printf("  Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        printf(TEST_COLOR_YELLOW "Running " #test_func "..." TEST_COLOR_RESET "\n"); \
        test_func(); \
        printf("\n"); \
    } while(0)

// Test suite functions
void test_init(void);
void test_summary(void);
bool test_all_passed(void);

#endif // TEST_FRAMEWORK_H
