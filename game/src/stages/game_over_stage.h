/**
 * @file game_over_stage.h
 * @brief BlockTris game over stage
 *
 * Handles the game over screen where players can restart or quit.
 */

#ifndef BLOCKTRIS_GAME_OVER_STAGE_H_
#define BLOCKTRIS_GAME_OVER_STAGE_H_

#include "stage.h"

/**
 * Game over stage state
 */
typedef struct {
    game_ptr game; // Reference to game context
    bool restart_requested;
    bool quit_requested;
    timestamp_ms_t stage_start_time;
} game_over_stage_state_t;

typedef game_over_stage_state_t *game_over_stage_state_ptr;

/**
 * Initialize game over stage
 */
void game_over_stage_init(stage_t *stage, game_ptr game);

/**
 * Update game over stage
 */
game_stage_action_t game_over_stage_update(stage_t *stage);

/**
 * Cleanup game over stage
 */
void game_over_stage_cleanup(stage_t *stage);

#endif // BLOCKTRIS_GAME_OVER_STAGE_H_
