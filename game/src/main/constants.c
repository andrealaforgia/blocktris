/**
 * @file constants.c
 * @brief Implementation of dynamic constants calculation
 */

#include "constants.h"

// Global variables for dynamic sizing
int screen_width = 0;
int screen_height = 0;
int window_width = 0;
int window_height = 0;
int cell_size = 0;
int field_width = 0;
int field_height = 0;
int ui_panel_width = 0;
int ui_margin = 0;
int board_offset_x = 0;
int board_offset_y = 0;

void calculate_window_dimensions(int actual_screen_width, int actual_screen_height) {
    // Store actual screen dimensions
    screen_width = actual_screen_width;
    screen_height = actual_screen_height;
    
    // Calculate window height as 90% of screen height
    window_height = (int)(actual_screen_height * SCREEN_HEIGHT_SCALE);
    
    // Calculate cell size so the playfield fits well in the window
    // Use most of the window height for the field, leaving space for margins
    int available_field_height = (int)(window_height * 0.9);
    cell_size = available_field_height / BOARD_HEIGHT;
    
    // Ensure minimum cell size
    if (cell_size < 8) {
        cell_size = 8;
    }
    
    // Calculate field dimensions based on cell size
    field_width = BOARD_WIDTH * cell_size;
    field_height = BOARD_HEIGHT * cell_size;
    
    // Calculate UI dimensions
    ui_panel_width = cell_size * 6;  // Space for next piece and UI (reduced)
    ui_margin = cell_size;           // Margins around the field
    
    // Calculate window width to fit field + UI + margins (minimal width)
    window_width = field_width + ui_panel_width + (ui_margin * 3);
    
    // Position the board within the window
    board_offset_x = ui_margin;
    board_offset_y = (window_height - field_height) / 2;
}
