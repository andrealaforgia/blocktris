/**
 * @file menu_stage.c
 * @brief BlockTris menu stage implementation
 */

#include "menu_stage.h"
#include "keyboard.h"
#include "events.h"
#include "blocktris_renderer.h"
#include "drawing_primitives.h"
#include "frame.h"
#include "clock.h"
#include "arcade_font.h"
#include "geometry.h"
#include <stdlib.h>

stage_ptr create_menu_stage_instance(void) {
    stage_ptr stage = malloc(sizeof(stage_t));
    if (!stage) {
        return NULL;
    }
    
    stage->state = NULL;
    stage->init = menu_stage_init;
    stage->update = menu_stage_update;
    stage->cleanup = menu_stage_cleanup;
    stage->name = "Menu Stage";
    
    return stage;
}

void menu_stage_init(stage_t *stage, game_ptr game) {
    if (!stage || !game) {
        return;
    }
    
    menu_stage_state_t *state = malloc(sizeof(menu_stage_state_t));
    if (!state) {
        return;
    }
    
    state->game = game;
    state->start_game_requested = false;
    
    stage->state = state;
    
    // Set game screen
    game->current_screen = SCREEN_MENU;
}

game_stage_action_t menu_stage_update(stage_t *stage) {
    if (!stage || !stage->state) {
        return QUIT;
    }
    
    menu_stage_state_t *state = (menu_stage_state_t *)stage->state;
    game_ptr game = state->game;
    
    // Handle SDL events
    handle_events(&game->event_system);
    
    // Update keyboard state
    game->keyboard_state.keys = SDL_GetKeyboardState(NULL);
    
    // Check for quit
    if (is_esc_key_pressed(&game->keyboard_state)) {
        return QUIT;
    }
    
    // Check for start game
    if (is_space_key_pressed(&game->keyboard_state) || is_return_key_pressed(&game->keyboard_state)) {
        state->start_game_requested = true;
        game->current_screen = SCREEN_PLAYING;
        return PROGRESS;
    }
    
    // Render menu
    clear_frame(&game->graphics_context);
    
    // Render "BLOCKTRIS" title in cyan
    const char* title_text = "BLOCKTRIS";
    int title_scale = 6;
    int title_width = get_arcade_text_width_scaled(&game->arcade_font, title_text, title_scale);
    int title_x = (LOGICAL_WIDTH - title_width) / 2;
    int title_y = LOGICAL_HEIGHT / 3;
    
    render_arcade_text_scaled(&game->arcade_font, &game->graphics_context, 
                              title_text, title_x, title_y, FONT_COLOR_CYAN, title_scale);
    
    // Calculate blinking for start text (blink every 500ms)
    timestamp_ms_t current_time = get_clock_ticks_ms();
    bool show_text = (current_time / 500) % 2 == 0;
    
    if (show_text) {
        // Render blinking yellow "PRESS SPACE TO START"
        const char* start_text = "PRESS SPACE TO START";
        int start_scale = 3;
        int start_width = get_arcade_text_width_scaled(&game->arcade_font, start_text, start_scale);
        int start_x = (LOGICAL_WIDTH - start_width) / 2;
        int title_height = 7 * title_scale; // Arcade font char height is 7 pixels
        int start_y = title_y + title_height + 40;
        
        render_arcade_text_scaled(&game->arcade_font, &game->graphics_context, 
                                  start_text, start_x, start_y, FONT_COLOR_YELLOW, start_scale);
    }
    
    render_frame(&game->graphics_context);
    
    return PROGRESS;
}

void menu_stage_cleanup(stage_t *stage) {
    if (!stage) {
        return;
    }
    
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}
