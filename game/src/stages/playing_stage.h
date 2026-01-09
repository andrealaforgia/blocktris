/**
 * @file playing_stage.h
 * @brief Tetris playing stage
 *
 * Handles the main game play where the actual Tetris game runs.
 */

#ifndef TETRIS_PLAYING_STAGE_H_
#define TETRIS_PLAYING_STAGE_H_

#include "stage.h"
#include "tetris_controller.h"

/**
 * Playing stage state
 */
typedef struct {
    game_ptr game; // Reference to game context
    tetris_controller_t controller;
    timestamp_ms_t last_fall_time;
    bool game_over_requested;
} playing_stage_state_t;

/**
 * Initialize playing stage
 */
void playing_stage_init(stage_t *stage, game_ptr game);

/**
 * Update playing stage
 */
game_stage_action_t playing_stage_update(stage_t *stage);

/**
 * Cleanup playing stage
 */
void playing_stage_cleanup(stage_t *stage);

/**
 * Update game logic (piece falling, line clearing, etc.)
 */
void playing_stage_update_game_logic(playing_stage_state_t *state);

/**
 * Handle piece falling
 */
void playing_stage_handle_piece_fall(playing_stage_state_t *state);

/**
 * Spawn a new piece
 */
void playing_stage_spawn_new_piece(playing_stage_state_t *state);

/**
 * Handle piece placement when it can't fall further
 */
void playing_stage_handle_piece_placement(playing_stage_state_t *state);

/**
 * Check and handle line clearing
 */
void playing_stage_handle_line_clearing(playing_stage_state_t *state);

/**
 * Update line clear effect
 */
void playing_stage_update_line_clear_effect(playing_stage_state_t *state);

/**
 * Update countdown display before game starts (3, 2)
 */
void playing_stage_update_countdown(playing_stage_state_t *state);

#endif // TETRIS_PLAYING_STAGE_H_
