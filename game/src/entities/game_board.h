/**
 * @file game_board.h
 * @brief Tetris game board management
 *
 * Defines the game board structure and provides functions for
 * board management, line clearing, and piece placement.
 */

#ifndef GAME_BOARD_H_
#define GAME_BOARD_H_

#include "constants.h"
#include "color.h"
#include <stdbool.h>

/**
 * Game board cell structure
 */
typedef struct {
    bool filled;
    piece_type_t piece_type;
    color_t color;
} board_cell_t;

/**
 * Game board structure
 */
typedef struct {
    board_cell_t cells[BOARD_HEIGHT][BOARD_WIDTH];
} game_board_t;

/**
 * Initialize the game board
 *
 * @param board Pointer to the board to initialize
 */
void game_board_init(game_board_t *board);

/**
 * Reset the game board to empty state
 *
 * @param board Pointer to the board to reset
 */
void game_board_reset(game_board_t *board);

/**
 * Set a cell on the board
 *
 * @param board Pointer to the board
 * @param x X coordinate
 * @param y Y coordinate
 * @param piece_type Type of piece to place
 * @param color Color of the cell
 */
void game_board_set_cell(game_board_t *board, int x, int y, piece_type_t piece_type, color_t color);

/**
 * Clear a cell on the board
 *
 * @param board Pointer to the board
 * @param x X coordinate
 * @param y Y coordinate
 */
void game_board_clear_cell(game_board_t *board, int x, int y);

/**
 * Check if a cell is filled
 *
 * @param board Pointer to the board
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if the cell is filled, false otherwise
 */
bool game_board_is_cell_filled(const game_board_t *board, int x, int y);

/**
 * Check if a position is valid (within board bounds)
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if position is valid, false otherwise
 */
bool game_board_is_position_valid(int x, int y);

/**
 * Check if a line is complete (all cells filled)
 *
 * @param board Pointer to the board
 * @param y Line to check
 * @return true if line is complete, false otherwise
 */
bool game_board_is_line_complete(const game_board_t *board, int y);

/**
 * Clear a line and move lines above down
 *
 * @param board Pointer to the board
 * @param y Line to clear
 */
void game_board_clear_line(game_board_t *board, int y);

/**
 * Find all complete lines
 *
 * @param board Pointer to the board
 * @param lines Array to store line indices (size 4)
 * @return Number of complete lines found
 */
int game_board_find_complete_lines(const game_board_t *board, int lines[4]);

/**
 * Clear multiple lines at once
 *
 * @param board Pointer to the board
 * @param lines Array of line indices to clear
 * @param num_lines Number of lines to clear
 */
void game_board_clear_lines(game_board_t *board, const int lines[], int num_lines);

/**
 * Place a piece on the board
 *
 * @param board Pointer to the board
 * @param piece_type Type of piece to place
 * @param piece_rotation Rotation of the piece
 * @param piece_x X position of the piece
 * @param piece_y Y position of the piece
 */
void game_board_place_piece(game_board_t *board, piece_type_t piece_type, 
                           int piece_rotation, int piece_x, int piece_y);

/**
 * Get the color of a cell
 *
 * @param board Pointer to the board
 * @param x X coordinate
 * @param y Y coordinate
 * @return Color of the cell
 */
color_t game_board_get_cell_color(const game_board_t *board, int x, int y);

/**
 * Check if the board has reached the top (game over condition)
 *
 * @param board Pointer to the board
 * @return true if any cell in the top row is filled, false otherwise
 */
bool game_board_is_game_over(const game_board_t *board);

#endif // GAME_BOARD_H_
