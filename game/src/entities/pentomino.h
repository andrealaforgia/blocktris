/**
 * @file pentomino.h
 * @brief Pentomino coordinate definitions and conversion functions
 */

#ifndef PENTOMINO_H
#define PENTOMINO_H

#include <stdbool.h>
#include "../main/constants.h"

// Coordinate structure for pentomino pieces
typedef struct {
    int x;
    int y;
} coord_t;

// Pentomino definition structure with predefined rotations
typedef struct {
    coord_t rotations[4][5];  // Up to 4 rotations, each with 5 coordinates
    int unique_rotations;     // How many unique rotations (1, 2, or 4)
} pentomino_def_t;

// Function to convert coordinates to 5x5 grid
void coords_to_grid(const coord_t coords[5], bool grid[PIECE_SIZE][PIECE_SIZE]);

// Function to rotate coordinates 90 degrees clockwise
void rotate_coords_clockwise(const coord_t input[5], coord_t output[5]);

// Get pentomino definition for a piece type
const pentomino_def_t* get_pentomino_def(piece_type_t type);

// Generate all 4 rotations of a pentomino
void generate_pentomino_rotations(piece_type_t type, bool rotations[4][PIECE_SIZE][PIECE_SIZE]);

#endif // PENTOMINO_H
