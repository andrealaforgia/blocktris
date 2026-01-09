/**
 * @file tetris_piece.c
 * @brief Tetris piece entity implementation using pentomino coordinate system
 */

#include "tetris_piece.h"
#include "pentomino.h"
#include <stdlib.h>

// Cache for generated rotations - computed on demand
static bool rotation_cache[NUM_PIECE_TYPES][4][PIECE_SIZE][PIECE_SIZE];
static bool cache_initialized[NUM_PIECE_TYPES] = {false};

// Piece colors for all 18 pentominoes (using available colors)
static const color_t piece_colors[NUM_PIECE_TYPES] = {
    [PIECE_I] = COLOR_RED,
    [PIECE_X] = COLOR_GREEN, 
    [PIECE_T] = 0x0000FF,        // Blue
    [PIECE_U] = COLOR_YELLOW,
    [PIECE_V] = 0xFF00FF,        // Magenta
    [PIECE_W] = 0x00FFFF,        // Cyan
    [PIECE_L] = 0xFFA500,        // Orange
    [PIECE_L_MIRROR] = 0xFFD700, // Gold
    [PIECE_N] = 0xFFC0CB,        // Pink
    [PIECE_N_MIRROR] = 0xFF69B4, // Hot Pink
    [PIECE_Y] = 0x800080,        // Purple
    [PIECE_Y_MIRROR] = 0x9370DB, // Medium Purple
    [PIECE_P] = 0xA52A2A,        // Brown
    [PIECE_P_MIRROR] = 0xD2691E, // Chocolate
    [PIECE_F] = 0x006400,        // Dark Green
    [PIECE_F_MIRROR] = 0x90EE90, // Light Green
    [PIECE_Z] = 0x000080,        // Navy
    [PIECE_Z_MIRROR] = 0x87CEEB  // Sky Blue
};

void tetris_piece_init(tetris_piece_t* piece) {
    piece->type = PIECE_EMPTY;
    piece->x = 0;
    piece->y = 0;
    piece->rotation = 0;
    piece->active = false;
    piece->color = COLOR_WHITE;
}

color_t tetris_piece_get_color(piece_type_t type) {
    if (type >= NUM_PIECE_TYPES || type == PIECE_EMPTY) {
        return COLOR_WHITE;
    }
    return piece_colors[type];
}

const bool (*tetris_piece_get_shape(piece_type_t type, int rotation))[PIECE_SIZE] {
    // Validate inputs
    if (type >= NUM_PIECE_TYPES || type == PIECE_EMPTY || rotation < 0) {
        return NULL;
    }
    
    // Normalize rotation to 0-3 range
    rotation = rotation % 4;
    
    // Initialize cache for this piece type if needed
    if (!cache_initialized[type]) {
        generate_pentomino_rotations(type, rotation_cache[type]);
        cache_initialized[type] = true;
    }
    
    return rotation_cache[type][rotation];
}

void tetris_piece_rotate_clockwise(tetris_piece_t* piece) {
    piece->rotation = (piece->rotation + 1) % 4;
}

void tetris_piece_rotate_counter_clockwise(tetris_piece_t* piece) {
    piece->rotation = (piece->rotation + 3) % 4; // +3 is same as -1 mod 4
}

void tetris_piece_move(tetris_piece_t* piece, int dx, int dy) {
    piece->x += dx;
    piece->y += dy;
}

piece_type_t tetris_piece_random_type(void) {
    return rand() % NUM_PIECE_TYPES;
}

void tetris_piece_reset(tetris_piece_t* piece, piece_type_t type, int x, int y) {
    piece->type = type;
    piece->x = x;
    piece->y = y;
    piece->rotation = 0;
    piece->active = true;
    piece->color = tetris_piece_get_color(type);
}

bool tetris_piece_is_cell_filled(piece_type_t type, int rotation, int x, int y) {
    // Validate coordinates
    if (x < 0 || x >= PIECE_SIZE || y < 0 || y >= PIECE_SIZE) {
        return false;
    }
    
    const bool (*shape)[PIECE_SIZE] = tetris_piece_get_shape(type, rotation);
    if (!shape) {
        return false;
    }
    
    return shape[y][x];
}
