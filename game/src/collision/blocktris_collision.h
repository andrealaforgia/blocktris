/**
 * @file blocktris_collision.h
 * @brief BlockTris collision detection system
 *
 * Provides collision detection functions for BlockTris piece movement,
 * rotation, and placement validation.
 */

#ifndef BLOCKTRIS_COLLISION_H_
#define BLOCKTRIS_COLLISION_H_

#include "game_board.h"
#include "blocktris_piece.h"
#include <stdbool.h>

/**
 * Check if a piece can be placed at a specific position and rotation
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece to check
 * @param piece_rotation Rotation of the piece
 * @param piece_x X position to check
 * @param piece_y Y position to check
 * @return true if piece can be placed, false if collision detected
 */
bool blocktris_collision_can_place_piece(const game_board_t *board, piece_type_t piece_type,
                                     int piece_rotation, int piece_x, int piece_y);

/**
 * Check if a piece can move in a specific direction
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece
 * @param piece_rotation Current rotation of the piece
 * @param piece_x Current X position
 * @param piece_y Current Y position
 * @param dx X offset to test
 * @param dy Y offset to test
 * @return true if piece can move, false if collision detected
 */
bool blocktris_collision_can_move_piece(const game_board_t *board, piece_type_t piece_type,
                                    int piece_rotation, int piece_x, int piece_y, 
                                    int dx, int dy);

/**
 * Check if a piece can be rotated to a new rotation
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece
 * @param current_rotation Current rotation of the piece
 * @param new_rotation New rotation to test
 * @param piece_x X position of the piece
 * @param piece_y Y position of the piece
 * @return true if piece can be rotated, false if collision detected
 */
bool blocktris_collision_can_rotate_piece(const game_board_t *board, piece_type_t piece_type,
                                      int current_rotation, int new_rotation,
                                      int piece_x, int piece_y);

/**
 * Check if a piece can fall down (move down by 1)
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece
 * @param piece_rotation Rotation of the piece
 * @param piece_x X position of the piece
 * @param piece_y Y position of the piece
 * @return true if piece can fall, false if it has landed
 */
bool blocktris_collision_can_fall(const game_board_t *board, piece_type_t piece_type,
                              int piece_rotation, int piece_x, int piece_y);

/**
 * Find the lowest position where a piece can be placed (for hard drop)
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece
 * @param piece_rotation Rotation of the piece
 * @param piece_x X position of the piece
 * @param start_y Starting Y position
 * @return Lowest valid Y position for the piece
 */
int blocktris_collision_find_drop_position(const game_board_t *board, piece_type_t piece_type,
                                       int piece_rotation, int piece_x, int start_y);

/**
 * Perform wall kick testing for piece rotation
 * Tests multiple offset positions to see if rotation is possible
 *
 * @param board Pointer to the game board
 * @param piece_type Type of piece
 * @param current_rotation Current rotation
 * @param new_rotation New rotation to test
 * @param piece_x Pointer to X position (may be modified for successful kick)
 * @param piece_y Pointer to Y position (may be modified for successful kick)
 * @return true if rotation is possible (possibly with wall kick), false otherwise
 */
bool blocktris_collision_wall_kick_test(const game_board_t *board, piece_type_t piece_type,
                                    int current_rotation, int new_rotation,
                                    int *piece_x, int *piece_y);

#endif // BLOCKTRIS_COLLISION_H_
