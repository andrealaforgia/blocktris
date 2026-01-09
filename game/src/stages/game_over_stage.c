/**
 * @file game_over_stage.c
 * @brief BlockTris game over stage implementation
 */

#include "game_over_stage.h"
#include "keyboard.h"
#include "events.h"
#include "blocktris_renderer.h"
#include "drawing_primitives.h"
#include "frame.h"
#include "color.h"
#include "clock.h"
#include <stdlib.h>

stage_ptr create_game_over_stage_instance(void) {
    stage_ptr stage = malloc(sizeof(stage_t));
    if (!stage) {
        return NULL;
    }
    
    stage->state = NULL;
    stage->init = game_over_stage_init;
    stage->update = game_over_stage_update;
    stage->cleanup = game_over_stage_cleanup;
    stage->name = "Game Over Stage";
    
    return stage;
}

void game_over_stage_init(stage_t *stage, game_ptr game) {
    if (!stage || !game) {
        return;
    }
    
    game_over_stage_state_t *state = malloc(sizeof(game_over_stage_state_t));
    if (!state) {
        return;
    }
    
    state->game = game;
    state->restart_requested = false;
    state->quit_requested = false;
    state->stage_start_time = get_clock_ticks_ms();
    
    stage->state = state;
    
    // Set game screen
    game->current_screen = SCREEN_GAME_OVER;
}

game_stage_action_t game_over_stage_update(stage_t *stage) {
    if (!stage || !stage->state) {
        return QUIT;
    }
    
    game_over_stage_state_t *state = (game_over_stage_state_t *)stage->state;
    game_ptr game = state->game;
    
    // Handle SDL events
    handle_events(&game->event_system);
    
    // Update keyboard state
    game->keyboard_state.keys = SDL_GetKeyboardState(NULL);
    
    // Check for quit
    if (is_esc_key_pressed(&game->keyboard_state)) {
        return QUIT;
    }
    
    // Check for restart - go back to intro screen
    if (is_space_key_pressed(&game->keyboard_state) || is_return_key_pressed(&game->keyboard_state)) {
        state->restart_requested = true;
        game->current_screen = SCREEN_INTRO;
        return PROGRESS;
    }
    
    // Render game over screen
    clear_frame(&game->graphics_context);
    
    // First render the final game state
    blocktris_renderer_render_board(game, &game->graphics_context);
    blocktris_renderer_render_placed_pieces(&game->board, &game->graphics_context);
    
    // Calculate animation progress
    timestamp_ms_t current_time = get_clock_ticks_ms();
    timestamp_ms_t elapsed = current_time - state->stage_start_time;
    
    // Animation parameters
    const int animation_duration = 1000; // 1 second to slide up
    float progress = (float)elapsed / animation_duration;
    if (progress > 1.0f) progress = 1.0f;
    
    // "GAME OVER" text - slides up from below screen to center
    const char* game_over_text = "GAME OVER";
    int scale = 6; // Large scale for dramatic effect
    int text_width = get_arcade_text_width_scaled(&game->arcade_font, game_over_text, scale);
    int text_height = 7 * scale;
    
    // Start below screen, animate to center
    int start_y = LOGICAL_HEIGHT + text_height; // Start below visible area
    int end_y = (LOGICAL_HEIGHT - text_height) / 2; // Center of screen
    int current_y = start_y - (int)((start_y - end_y) * progress);
    int text_x = (LOGICAL_WIDTH - text_width) / 2; // Centered horizontally
    
    // Draw "GAME OVER" text with red color
    render_arcade_text_scaled(&game->arcade_font, &game->graphics_context,
                             game_over_text, text_x, current_y, FONT_COLOR_RED, scale);
    
    // Show restart instructions only after animation completes
    if (progress >= 1.0f) {
        // Flashing "PRESS SPACE TO RESTART" text
        const char* restart_text = "PRESS SPACE TO RESTART";
        int restart_scale = 3;
        int restart_width = get_arcade_text_width_scaled(&game->arcade_font, restart_text, restart_scale);
        int restart_height = 7 * restart_scale;
        int restart_x = (LOGICAL_WIDTH - restart_width) / 2;
        int restart_y = current_y + text_height + 40; // Below "GAME OVER"
        
        // Flash every 500ms
        bool show_restart = (elapsed / 500) % 2 == 0;
        if (show_restart) {
            render_arcade_text_scaled(&game->arcade_font, &game->graphics_context,
                                     restart_text, restart_x, restart_y, FONT_COLOR_YELLOW, restart_scale);
        }
        
        // Show final score
        char score_text[64];
        snprintf(score_text, sizeof(score_text), "FINAL SCORE: %d", game->score);
        int score_scale = 2;
        int score_width = get_arcade_text_width_scaled(&game->arcade_font, score_text, score_scale);
        int score_x = (LOGICAL_WIDTH - score_width) / 2;
        int score_y = restart_y + restart_height + 30;
        
        render_arcade_text_scaled(&game->arcade_font, &game->graphics_context,
                                 score_text, score_x, score_y, FONT_COLOR_WHITE, score_scale);
    }
    
    render_frame(&game->graphics_context);
    
    return PROGRESS;
}

void game_over_stage_cleanup(stage_t *stage) {
    if (!stage) {
        return;
    }
    
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}
