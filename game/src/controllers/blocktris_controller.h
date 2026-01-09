/**
 * @file blocktris_controller.h
 * @brief BlockTris game input controller
 *
 * Handles input processing for BlockTris game including piece movement,
 * rotation, and special actions like hard drop and pause.
 */

#ifndef BLOCKTRIS_CONTROLLER_H_
#define BLOCKTRIS_CONTROLLER_H_

#include "game.h"
#include "keyboard.h"
#include <stdbool.h>

/**
 * BlockTris input controller state
 */
typedef struct {
    timestamp_ms_t last_move_time;
    timestamp_ms_t last_rotate_time;
    timestamp_ms_t last_drop_time;
    bool left_held;
    bool right_held;
    bool down_held;
    bool space_held;
    bool up_held;
} blocktris_controller_t;

typedef blocktris_controller_t *blocktris_controller_ptr;

/**
 * Initialize the BlockTris controller
 *
 * @param controller Pointer to controller to initialize
 */
void blocktris_controller_init(blocktris_controller_t *controller);

/**
 * Update the controller state and process input
 *
 * @param controller Pointer to controller
 * @param game Pointer to game state
 * @param keyboard Pointer to keyboard state
 */
void blocktris_controller_update(blocktris_controller_t *controller, game_t *game, 
                             keyboard_state_t *keyboard);

/**
 * Handle piece movement input
 *
 * @param controller Pointer to controller
 * @param game Pointer to game state
 * @param keyboard Pointer to keyboard state
 */
void blocktris_controller_handle_movement(blocktris_controller_t *controller, game_t *game,
                                      keyboard_state_t *keyboard);

/**
 * Handle piece rotation input
 *
 * @param controller Pointer to controller
 * @param game Pointer to game state
 * @param keyboard Pointer to keyboard state
 */
void blocktris_controller_handle_rotation(blocktris_controller_t *controller, game_t *game,
                                      keyboard_state_t *keyboard);

/**
 * Handle soft drop input (down arrow)
 *
 * @param controller Pointer to controller
 * @param game Pointer to game state
 * @param keyboard Pointer to keyboard state
 */
void blocktris_controller_handle_soft_drop(blocktris_controller_t *controller, game_t *game,
                                       const keyboard_state_t *keyboard);

/**
 * Handle hard drop input (space bar)
 *
 * @param controller Pointer to controller
 * @param game Pointer to game state
 * @param keyboard Pointer to keyboard state
 */
void blocktris_controller_handle_hard_drop(blocktris_controller_t *controller, game_t *game,
                                       keyboard_state_t *keyboard);

/**
 * Attempt to move the current piece
 *
 * @param game Pointer to game state
 * @param dx X offset
 * @param dy Y offset
 * @return true if piece was moved, false if blocked
 */
bool blocktris_controller_move_piece(game_t *game, int dx, int dy);

/**
 * Attempt to rotate the current piece clockwise
 *
 * @param game Pointer to game state
 * @return true if piece was rotated, false if blocked
 */
bool blocktris_controller_rotate_piece_clockwise(game_t *game);

/**
 * Attempt to rotate the current piece counter-clockwise
 *
 * @param game Pointer to game state
 * @return true if piece was rotated, false if blocked
 */
bool blocktris_controller_rotate_piece_counter_clockwise(game_t *game);

/**
 * Perform hard drop (drop piece to bottom)
 *
 * @param game Pointer to game state
 * @return Number of lines dropped
 */
int blocktris_controller_hard_drop_piece(game_t *game);

/**
 * Check if enough time has passed for repeated input
 *
 * @param last_time Last time the input was processed
 * @param delay Delay between repeated inputs
 * @return true if enough time has passed
 */
bool blocktris_controller_can_repeat_input(timestamp_ms_t last_time, int delay);

#endif // BLOCKTRIS_CONTROLLER_H_
