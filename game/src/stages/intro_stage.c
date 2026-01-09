/**
 * @file intro_stage.c
 * @brief Intro stage implementation
 */

#include "intro_stage.h"
#include "constants.h"
#include "arcade_font.h"
#include "graphics.h"
#include "geometry.h"
#include "keyboard.h"
#include "clock.h"
#include "events.h"
#include <stdlib.h>

stage_ptr create_intro_stage_instance(void) {
    stage_ptr stage = malloc(sizeof(stage_t));
    if (!stage) {
        return NULL;
    }
    
    stage->state = NULL;
    stage->init = intro_stage_init;
    stage->update = intro_stage_update;
    stage->cleanup = intro_stage_cleanup;
    stage->name = "Intro Stage";
    
    return stage;
}

void intro_stage_init(stage_t *stage, game_ptr game) {
    if (!stage || !game) {
        return;
    }
    
    intro_stage_state_t *state = malloc(sizeof(intro_stage_state_t));
    if (!state) {
        return;
    }
    
    state->game = game;
    state->start_time = get_clock_ticks_ms();
    state->start_requested = false;
    
    stage->state = state;
    
    // Clear screen to black
    SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->graphics_context.renderer);
}

game_stage_action_t intro_stage_update(stage_t *stage) {
    if (!stage || !stage->state) {
        return QUIT;
    }
    
    intro_stage_state_t *state = (intro_stage_state_t *)stage->state;
    game_ptr game = state->game;
    
    if (!game) {
        return QUIT;
    }
    
    // Handle SDL events
    handle_events(&game->event_system);
    
    // Update keyboard state
    game->keyboard_state.keys = SDL_GetKeyboardState(NULL);
    
    // Calculate elapsed time 
    timestamp_ms_t current_time = get_clock_ticks_ms();
    timestamp_ms_t elapsed = current_time - state->start_time;
    
    // Allow manual advancement with space key or auto-advance after 3 seconds
    if (is_space_key_pressed(&game->keyboard_state) || elapsed >= 3000) {
        game->current_screen = SCREEN_PLAYING;
        return PROGRESS;
    }
    
    // Clear screen to black
    SDL_SetRenderDrawColor(game->graphics_context.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->graphics_context.renderer);
    
    // Render "BLOCKTRIS" title - occupies 80% of window width
    const char* title_text = "BLOCKTRIS";
    int target_title_width = (int)(LOGICAL_WIDTH * 0.8);
    
    // Calculate scale needed to achieve 80% width
    int base_title_width = get_arcade_text_width_scaled(&game->arcade_font, title_text, 1);
    int title_scale = target_title_width / base_title_width;
    if (title_scale < 1) title_scale = 1; // Minimum scale
    
    int actual_title_width = get_arcade_text_width_scaled(&game->arcade_font, title_text, title_scale);
    int title_x = (LOGICAL_WIDTH - actual_title_width) / 2;
    int title_y = LOGICAL_HEIGHT / 4;
    
    render_arcade_text_scaled(&game->arcade_font, &game->graphics_context, 
                              title_text, title_x, title_y, FONT_COLOR_CYAN, title_scale);
    
    // Render flashing "PRESS SPACE TO START" text - occupies 90% of window width
    const char* start_text = "PRESS SPACE TO START";
    int target_start_width = (int)(LOGICAL_WIDTH * 0.9);
    
    // Calculate scale needed to achieve 90% width
    int base_start_width = get_arcade_text_width_scaled(&game->arcade_font, start_text, 1);
    int start_scale = target_start_width / base_start_width;
    if (start_scale < 1) start_scale = 1; // Minimum scale
    
    int actual_start_width = get_arcade_text_width_scaled(&game->arcade_font, start_text, start_scale);
    int start_height = 7 * start_scale;
    int start_x = (LOGICAL_WIDTH - actual_start_width) / 2; // Horizontal center
    int start_y = (LOGICAL_HEIGHT - start_height) / 2; // Vertical center
    
    // Flash the start text every 500ms
    bool show_start_text = (elapsed / 500) % 2 == 0;
    if (show_start_text) {
        render_arcade_text_scaled(&game->arcade_font, &game->graphics_context, 
                                  start_text, start_x, start_y, FONT_COLOR_WHITE, start_scale);
    }
    
    // Present the rendered frame
    SDL_RenderPresent(game->graphics_context.renderer);
    
    return PROGRESS;
}

void intro_stage_cleanup(stage_t *stage) {
    if (!stage || !stage->state) {
        return;
    }
    
    free(stage->state);
    stage->state = NULL;
}
