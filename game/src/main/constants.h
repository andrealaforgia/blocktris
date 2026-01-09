/**
 * @file constants.h
 * @brief Global BlockTris game constants and configuration values
 *
 * Defines all the core constants used throughout the BlockTris game including
 * screen dimensions, game board dimensions, piece colors, and timing.
 */

#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H

// Screen height scale factor (window will be 90% of actual screen height)
#define SCREEN_HEIGHT_SCALE 0.9

// BlockTris board dimensions (reduced width to fit window better)
#define BOARD_WIDTH 12
#define BOARD_HEIGHT 30

// Piece dimensions
#define PIECE_SIZE 5 // 5x5 grid for each piece

// Dynamic sizing - calculated at runtime based on screen dimensions
extern int screen_width;
extern int screen_height;
extern int window_width;
extern int window_height;
extern int cell_size;
extern int field_width;
extern int field_height;
extern int ui_panel_width;
extern int ui_margin;
extern int board_offset_x;
extern int board_offset_y;

// Macros to use the dynamic values
#define LOGICAL_WIDTH window_width
#define LOGICAL_HEIGHT window_height
#define CELL_SIZE cell_size
#define FIELD_WIDTH field_width
#define FIELD_HEIGHT field_height
#define UI_PANEL_WIDTH ui_panel_width
#define UI_MARGIN ui_margin
#define BOARD_OFFSET_X board_offset_x
#define BOARD_OFFSET_Y board_offset_y

// Function to calculate dynamic sizing based on actual screen dimensions
void calculate_window_dimensions(int screen_width, int screen_height);

// Game timing
#define FPS 60
#define FRAME_DELAY (1000 / FPS)

// Border size around the game board
#define BORDER_SIZE 2

// UI positioning
#define NEXT_PIECE_X (BOARD_OFFSET_X + FIELD_WIDTH + UI_MARGIN)
#define NEXT_PIECE_Y (BOARD_OFFSET_Y + UI_MARGIN)
#define NEXT_PIECE_SIZE (CELL_SIZE * 3)
#define SCORE_X (NEXT_PIECE_X)
#define SCORE_Y (NEXT_PIECE_Y + NEXT_PIECE_SIZE + UI_MARGIN)
#define MENU_TITLE_Y (LOGICAL_HEIGHT / 3)
#define MENU_START_Y (LOGICAL_HEIGHT * 2 / 3)

// Game speeds (fall intervals in milliseconds)
#define INITIAL_FALL_SPEED 800
#define FAST_FALL_SPEED 50
#define SPEED_INCREASE_PER_LEVEL 50

// Line clear delays
#define LINE_CLEAR_DELAY 300

// Piece movement timing
#define MOVE_REPEAT_DELAY 250    // Delay between repeated inputs when key held (higher = less sensitive)
#define ROTATE_REPEAT_DELAY 300

// Scoring
#define POINTS_SINGLE_LINE 100
#define POINTS_DOUBLE_LINE 300
#define POINTS_TRIPLE_LINE 500
#define POINTS_TETRIS 800
#define POINTS_SOFT_DROP 1
#define POINTS_HARD_DROP 2

// Maximum number of pieces in object pool
#define MAX_PIECES 4

// Pentomino piece types (18 total - 6 symmetric + 12 asymmetric in pairs)
typedef enum {
    PIECE_I = 0,        // Symmetric
    PIECE_X = 1,        // Symmetric 
    PIECE_T = 2,        // Symmetric
    PIECE_U = 3,        // Symmetric
    PIECE_V = 4,        // Symmetric
    PIECE_W = 5,        // Symmetric
    PIECE_L = 6,        // Asymmetric
    PIECE_L_MIRROR = 7, // L'
    PIECE_N = 8,        // Asymmetric
    PIECE_N_MIRROR = 9, // N'
    PIECE_Y = 10,       // Asymmetric
    PIECE_Y_MIRROR = 11,// Y'
    PIECE_P = 12,       // Asymmetric
    PIECE_P_MIRROR = 13,// P'
    PIECE_F = 14,       // Asymmetric
    PIECE_F_MIRROR = 15,// F'
    PIECE_Z = 16,       // Asymmetric
    PIECE_Z_MIRROR = 17,// Z'
    PIECE_EMPTY = 18,
    NUM_PIECE_TYPES = 18
} piece_type_t;

#endif // TETRIS_CONSTANTS_H
