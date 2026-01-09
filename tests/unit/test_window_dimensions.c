#include "../test_framework.h"
#include "../../game/src/main/constants.h"
#include "test_window_dimensions.h"
#include <stdlib.h>

// Test that validates window width calculation
void test_window_width_calculation(void) {
    // Use a standard test screen size for calculation
    int test_screen_width = 1920;
    int test_screen_height = 1080;
    
    // Calculate window dimensions using the same logic as the game
    calculate_window_dimensions(test_screen_width, test_screen_height);
    
    // Calculate expected components
    int expected_field_width = BOARD_WIDTH * cell_size;
    int expected_ui_panel_width = cell_size * 7;  // As set in constants.c
    int expected_margin_space = cell_size * 3;    // 3 margins as per calculation
    
    // Calculate expected total with 5% tolerance
    int expected_base_width = expected_field_width + expected_ui_panel_width + expected_margin_space;
    int max_allowed_width = expected_base_width + (expected_base_width * 5 / 100); // +5%
    
    // Verify window width is within expected range
    char test_msg[256];
    snprintf(test_msg, sizeof(test_msg), 
             "Window width (%d) within expected range (max: %d)", 
             window_width, max_allowed_width);
    TEST_ASSERT(window_width <= max_allowed_width, test_msg);
    
    // Verify window width isn't unreasonably small
    int min_allowed_width = expected_base_width - (expected_base_width * 10 / 100);
    snprintf(test_msg, sizeof(test_msg),
             "Window width (%d) not too small (min: %d)",
             window_width, min_allowed_width);
    TEST_ASSERT(window_width >= min_allowed_width, test_msg);
}

// Test specific dimension component calculations
void test_window_component_calculation(void) {
    // Use standard test screen dimensions
    calculate_window_dimensions(1920, 1080);
    
    // Verify field width matches board dimensions
    int expected_field_width = BOARD_WIDTH * cell_size;
    TEST_ASSERT_EQUAL(expected_field_width, field_width,
                     "Field width calculation matches board dimensions");
    
    // Verify UI panel width is proportional to cell size
    int expected_ui_width = cell_size * 7;
    TEST_ASSERT_EQUAL(expected_ui_width, ui_panel_width,
                     "UI panel width calculation correct");
    
    // Verify total window width calculation
    int calculated_width = field_width + ui_panel_width + (ui_margin * 3);
    TEST_ASSERT_EQUAL(calculated_width, window_width,
                     "Window width calculation formula correct");
}

// Test that different screen sizes produce reasonable window widths
void test_window_scaling(void) {
    // Test with smaller screen
    calculate_window_dimensions(1366, 768);
    int small_screen_width = window_width;
    
    // Test with larger screen  
    calculate_window_dimensions(2560, 1440);
    int large_screen_width = window_width;
    
    // Verify scaling is reasonable (larger screen should have larger window)
    TEST_ASSERT(large_screen_width > small_screen_width,
                "Window width scales appropriately with screen size");
}

// Main test runner for window dimension tests
void run_window_dimension_tests(void) {
    printf("\n=== Window Dimension Validation Tests ===\n\n");
    
    RUN_TEST(test_window_width_calculation);
    RUN_TEST(test_window_component_calculation); 
    RUN_TEST(test_window_scaling);
}
