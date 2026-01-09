/**
 * @file stage.h
 * @brief Tetris game stage abstraction and lifecycle management
 *
 * Defines the stage interface for different game screens like menu,
 * playing, and game over. Provides factory functions and common operations
 * for creating and managing stage transitions using function pointers.
 */

#ifndef TETRIS_STAGE_H_
#define TETRIS_STAGE_H_

#include "game.h"

// game_ptr is defined in game.h

struct stage_t {
    void *state; // Stage-specific state

    void (*init)(stage_t *stage, game_ptr game);
    game_stage_action_t (*update)(stage_t *stage);
    void (*cleanup)(stage_t *stage);

    const char *name; // For debugging
};

typedef stage_t *stage_ptr;

// Factory functions for creating stages
stage_ptr create_intro_stage_instance(void);
stage_ptr create_menu_stage_instance(void);
stage_ptr create_playing_stage_instance(void);
stage_ptr create_game_over_stage_instance(void);
stage_ptr create_paused_stage_instance(void);

// Common stage operations
void destroy_stage(stage_ptr stage);

#endif // TETRIS_STAGE_H_
