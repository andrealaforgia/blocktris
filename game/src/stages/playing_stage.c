/**
 * @file playing_stage.c
 * @brief BlockTris playing stage implementation
 */

#include "playing_stage.h"
#include "keyboard.h"
#include "events.h"
#include "blocktris_renderer.h"
#include "blocktris_collision.h"
#include "clock.h"
#include "constants.h"
#include "frame.h"
#include <stdlib.h>

stage_ptr create_playing_stage_instance(void) {
    stage_ptr stage = malloc(sizeof(stage_t));
    if (!stage) {
        return NULL;
    }
    
    stage->state = NULL;
    stage->init = playing_stage_init;
    stage->update = playing_stage_update;
    stage->cleanup = playing_stage_cleanup;
    stage->name = "Playing Stage";
    
    return stage;
}

void playing_stage_init(stage_t *stage, game_ptr game) {
    if (!stage || !game) {
        return;
    }
    
    playing_stage_state_t *state = malloc(sizeof(playing_stage_state_t));
    if (!state) {
        return;
    }
    
    state->game = game;
    state->game_over_requested = false;
    state->last_fall_time = get_clock_ticks_ms();
    
    // Initialize controller
    blocktris_controller_init(&state->controller);
    
    stage->state = state;
    
    // Reset game state for new game
    game_reset(game);
    game->current_screen = SCREEN_PLAYING;
    
    // Don't spawn first piece immediately - wait for big "3" to finish
}

game_stage_action_t playing_stage_update(stage_t *stage) {
    if (!stage || !stage->state) {
        return QUIT;
    }
    
    playing_stage_state_t *state = (playing_stage_state_t *)stage->state;
    game_ptr game = state->game;
    
    // Handle SDL events
    handle_events(&game->event_system);
    
    // Update keyboard state
    game->keyboard_state.keys = SDL_GetKeyboardState(NULL);
    
    // Check for quit
    if (is_esc_key_pressed(&game->keyboard_state)) {
        return QUIT;
    }
    
    // Check for pause
    if (is_s_key_pressed(&game->keyboard_state)) {
        game->paused = !game->paused;
        game->current_screen = game->paused ? SCREEN_PAUSED : SCREEN_PLAYING;
    }
    
    // Handle countdown display if active
    if (game->show_countdown) {
        playing_stage_update_countdown(state);
    }
    
    // Only update game logic if not paused and countdown is not showing
    if (!game->paused && !game->show_countdown) {
        // Update controller
        blocktris_controller_update(&state->controller, game, &game->keyboard_state);
        
        // Update game logic
        playing_stage_update_game_logic(state);
        
        // Check for game over
        if (game_board_is_game_over(&game->board)) {
            state->game_over_requested = true;
            game->current_screen = SCREEN_GAME_OVER;
            return PROGRESS;
        }
    }
    
    // Render game
    blocktris_renderer_render_game(game, &game->graphics_context);
    
    // If paused, draw pause indicator
    if (game->paused) {
        color_t white = COLOR(255, 255, 255);
        draw_line(&game->graphics_context, 350, 280, 450, 280, white);
        draw_line(&game->graphics_context, 350, 300, 450, 300, white);
        draw_line(&game->graphics_context, 350, 320, 450, 320, white);
    }
    
    render_frame(&game->graphics_context);
    
    return PROGRESS;
}

void playing_stage_cleanup(stage_t *stage) {
    if (!stage) {
        return;
    }
    
    if (stage->state) {
        free(stage->state);
        stage->state = NULL;
    }
}

void playing_stage_update_game_logic(playing_stage_state_t *state) {
    if (!state) {
        return;
    }
    
    // Update line clear effect
    playing_stage_update_line_clear_effect(state);
    
    // Don't update falling pieces during line clear effect
    if (state->game->line_clear_active) {
        return;
    }
    
    // Handle piece falling
    playing_stage_handle_piece_fall(state);
}

void playing_stage_handle_piece_fall(playing_stage_state_t *state) {
    if (!state || state->game->current_piece_type == PIECE_EMPTY) {
        return;
    }
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    
    // Check if piece should fall
    if (current_time - state->last_fall_time >= (timestamp_ms_t)state->game->fall_speed) {
        if (blocktris_collision_can_fall(&state->game->board, 
                                     state->game->current_piece_type,
                                     state->game->current_piece_rotation,
                                     state->game->current_piece_x,
                                     state->game->current_piece_y)) {
            // Piece can fall
            state->game->current_piece_y++;
            state->last_fall_time = current_time;
        } else {
            // Piece has landed
            playing_stage_handle_piece_placement(state);
        }
    }
}

void playing_stage_spawn_new_piece(playing_stage_state_t *state) {
    if (!state) {
        return;
    }
    
    game_ptr game = state->game;
    
    // If we have a next piece, use it, otherwise generate random
    if (game->next_piece_type != PIECE_EMPTY) {
        game->current_piece_type = game->next_piece_type;
    } else {
        game->current_piece_type = blocktris_piece_random_type();
    }
    
    // Generate next piece
    game->next_piece_type = blocktris_piece_random_type();
    
    // Reset piece position and rotation
    game->current_piece_x = BOARD_WIDTH / 2 - 2;
    game->current_piece_y = 0;
    game->current_piece_rotation = 0;
    
    // Reset fall speed if it was modified by soft drop
    game->fall_speed = INITIAL_FALL_SPEED - (game->level - 1) * SPEED_INCREASE_PER_LEVEL;
    if (game->fall_speed < FAST_FALL_SPEED) {
        game->fall_speed = FAST_FALL_SPEED;
    }
}

void playing_stage_handle_piece_placement(playing_stage_state_t *state) {
    if (!state) {
        return;
    }
    
    game_ptr game = state->game;
    
    // Place piece on board
    game_board_place_piece(&game->board, 
                          game->current_piece_type,
                          game->current_piece_rotation,
                          game->current_piece_x,
                          game->current_piece_y);
    
    // Check for line clearing
    playing_stage_handle_line_clearing(state);
    
    // Spawn new piece
    playing_stage_spawn_new_piece(state);
}

void playing_stage_update_countdown(playing_stage_state_t *state) {
    if (!state || !state->game->show_countdown) {
        return;
    }
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    timestamp_ms_t elapsed = current_time - state->game->countdown_start_time;
    
    // Total countdown duration: "3", "2", "1", "GO!" (each 0.5s) = 2 seconds total
    const int total_countdown_duration_ms = 2000;
    
    if (elapsed >= total_countdown_duration_ms) {
        // Countdown finished - hide countdown and start the game
        state->game->show_countdown = false;
        playing_stage_spawn_new_piece(state);
    }
}

void playing_stage_handle_line_clearing(playing_stage_state_t *state) {
    if (!state) {
        return;
    }
    
    game_ptr game = state->game;
    
    // Find complete lines
    int complete_lines[4];
    int num_lines = game_board_find_complete_lines(&game->board, complete_lines);
    
    if (num_lines > 0) {
        // Set up line clear effect
        game->line_clear_active = true;
        game->line_clear_start_time = get_clock_ticks_ms();
        game->num_lines_to_clear = num_lines;
        
        for (int i = 0; i < num_lines; i++) {
            game->lines_to_clear[i] = complete_lines[i];
        }
        
        // Update score based on number of lines
        int points = 0;
        switch (num_lines) {
            case 1:
                points = POINTS_SINGLE_LINE;
                break;
            case 2:
                points = POINTS_DOUBLE_LINE;
                break;
            case 3:
                points = POINTS_TRIPLE_LINE;
                break;
            case 4:
                points = POINTS_TETRIS;
                break;
        }
        game->score += points * game->level;
        
        // Update lines cleared count
        game->lines_cleared += num_lines;
        
        // Update level (every 10 lines)
        game->level = (game->lines_cleared / 10) + 1;
    }
}

void playing_stage_update_line_clear_effect(playing_stage_state_t *state) {
    if (!state || !state->game->line_clear_active) {
        return;
    }
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    timestamp_ms_t elapsed = current_time - state->game->line_clear_start_time;
    
    if (elapsed >= LINE_CLEAR_DELAY) {
        // Clear the lines and end effect
        game_board_clear_lines(&state->game->board, 
                              state->game->lines_to_clear, 
                              state->game->num_lines_to_clear);
        
        // Reset line clear state
        state->game->line_clear_active = false;
        state->game->num_lines_to_clear = 0;
        for (int i = 0; i < 4; i++) {
            state->game->lines_to_clear[i] = -1;
        }
    }
}

