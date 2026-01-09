/**
 * @file menu_stage.h
 * @brief BlockTris menu stage
 *
 * Handles the main menu screen where players can start a new game.
 */

#ifndef BLOCKTRIS_MENU_STAGE_H_
#define BLOCKTRIS_MENU_STAGE_H_

#include "stage.h"

/**
 * Menu stage state
 */
typedef struct {
    game_ptr game; // Reference to game context
    bool start_game_requested;
} menu_stage_state_t;

typedef menu_stage_state_t *menu_stage_state_ptr;

/**
 * Initialize menu stage
 */
void menu_stage_init(stage_t *stage, game_ptr game);

/**
 * Update menu stage
 */
game_stage_action_t menu_stage_update(stage_t *stage);

/**
 * Cleanup menu stage
 */
void menu_stage_cleanup(stage_t *stage);

#endif // BLOCKTRIS_MENU_STAGE_H_
