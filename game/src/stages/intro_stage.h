/**
 * @file intro_stage.h
 * @brief Blocktris intro stage
 *
 * Handles the intro screen showing the game title and start prompt.
 */

#ifndef BLOCKTRIS_INTRO_STAGE_H_
#define BLOCKTRIS_INTRO_STAGE_H_

#include "stage.h"

/**
 * Intro stage state
 */
typedef struct {
    game_ptr game; // Reference to game context
    timestamp_ms_t start_time;
    bool start_requested;
} intro_stage_state_t;

typedef intro_stage_state_t *intro_stage_state_ptr;

/**
 * Initialize intro stage
 */
void intro_stage_init(stage_t *stage, game_ptr game);

/**
 * Update intro stage
 */
game_stage_action_t intro_stage_update(stage_t *stage);

/**
 * Cleanup intro stage
 */
void intro_stage_cleanup(stage_t *stage);

#endif // BLOCKTRIS_INTRO_STAGE_H_
