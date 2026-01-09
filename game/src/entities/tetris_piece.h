/**
 * @file tetris_piece.h
 * @brief Tetris piece entity definition and operations
 *
 * Defines the tetris piece structure and provides functions for
 * piece creation, rotation, and manipulation.
 */

#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include "constants.h"
#include "color.h"
#include <stdbool.h>

/**
 * Tetris piece structure
 */
typedef struct {
    piece_type_t type;
    int rotation; // 0, 1, 2, or 3 (90 degree increments)
    int x, y; // Position on the board
    bool active; // Whether this piece is currently in use
    color_t color; // Color for rendering
} tetris_piece_t;

/**
 * Initialize a tetris piece to default empty state
 *
 * @param piece Pointer to the piece to initialize
 */
void tetris_piece_init(tetris_piece_t *piece);

/**
 * Get the shape data for a piece type and rotation
 *
 * @param type Piece type
 * @param rotation Rotation (0-3)
 * @return Pointer to 4x4 array of booleans representing the piece shape
 */
const bool (*tetris_piece_get_shape(piece_type_t type, int rotation))[PIECE_SIZE];

/**
 * Rotate a piece clockwise
 *
 * @param piece Pointer to the piece to rotate
 */
void tetris_piece_rotate_clockwise(tetris_piece_t *piece);

/**
 * Rotate a piece counter-clockwise
 *
 * @param piece Pointer to the piece to rotate
 */
void tetris_piece_rotate_counter_clockwise(tetris_piece_t *piece);

/**
 * Move a piece by the given offset
 *
 * @param piece Pointer to the piece to move
 * @param dx X offset
 * @param dy Y offset
 */
void tetris_piece_move(tetris_piece_t *piece, int dx, int dy);

/**
 * Get the color for a piece type
 *
 * @param type Piece type
 * @return Color for the piece type
 */
color_t tetris_piece_get_color(piece_type_t type);

/**
 * Generate a random piece type
 *
 * @return Random piece type
 */
piece_type_t tetris_piece_random_type(void);

/**
 * Reset a piece with new type and position
 *
 * @param piece Pointer to the piece to reset
 * @param type Type of piece to create
 * @param x Initial x position
 * @param y Initial y position
 */
void tetris_piece_reset(tetris_piece_t *piece, piece_type_t type, int x, int y);

/**
 * Check if a cell in the piece shape is filled
 *
 * @param type Piece type
 * @param rotation Rotation (0-3)
 * @param x X coordinate within the 4x4 piece grid
 * @param y Y coordinate within the 4x4 piece grid
 * @return true if the cell is filled, false otherwise
 */
bool tetris_piece_is_cell_filled(piece_type_t type, int rotation, int x, int y);

#endif // TETRIS_PIECE_H_
