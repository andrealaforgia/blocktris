/**
 * @file game.c
 * @brief Core Tetris game state and lifecycle management implementation
 */

#include "game.h"
#include "constants.h"
#include "clock.h"
#include "resource_manager.h"
#include "object_pool.h"
#include "events.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



bool game_init(game_t *game) {
    if (!game) {
        return false;
    }
    
    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Load all game resources (graphics, audio, fonts)
    if (!load_game_resources(game)) {
        return false;
    }

    // Initialize event system
    game->event_system = create_event_system();

    // Initialize keyboard state
    game->keyboard_state = init_keyboard_state();

    // Initialize game state
    game->running = true;
    game->paused = false;
    game->current_screen = SCREEN_INTRO;
    game->current_stage = NULL;
    
    // Initialize game board
    game_board_init(&game->board);
    
    // Initialize object pools
    game->piece_pool = create_object_pool(sizeof(tetris_piece_t), MAX_PIECES);
    
    // Initialize game statistics
    game->score = 0;
    game->level = 1;
    game->lines_cleared = 0;
    
    // Initialize timing
    game->last_fall_time = get_clock_ticks_ms();
    game->last_move_time = 0;
    game->last_rotate_time = 0;
    game->fall_speed = INITIAL_FALL_SPEED;
    
    // Initialize piece state
    game->current_piece_x = BOARD_WIDTH / 2;
    game->current_piece_y = 0;
    game->current_piece_rotation = 0;
    game->current_piece_type = PIECE_EMPTY;
    game->next_piece_type = PIECE_EMPTY;
    
    // Initialize line clear state
    game->line_clear_active = false;
    game->num_lines_to_clear = 0;
    game->line_clear_start_time = 0;
    for (int i = 0; i < 4; i++) {
        game->lines_to_clear[i] = -1;
    }
    
    // Initialize countdown display state
    game->show_countdown = false;
    game->countdown_start_time = 0;
    
    return true;
}

void game_terminate(game_t *game) {
    if (!game) {
        return;
    }
    
    // Cleanup object pools
    pool_destroy(&game->piece_pool);
    
    // Free all game resources
    free_game_resources(game);
    
    printf("Tetris game terminated successfully\n");
}

void game_reset(game_t *game) {
    if (!game) {
        return;
    }
    
    // Reset game board
    game_board_reset(&game->board);
    
    // Reset game statistics
    game->score = 0;
    game->level = 1;
    game->lines_cleared = 0;
    
    // Reset timing
    game->last_fall_time = get_clock_ticks_ms();
    game->last_move_time = 0;
    game->last_rotate_time = 0;
    game->fall_speed = INITIAL_FALL_SPEED;
    
    // Reset piece state
    game->current_piece_x = BOARD_WIDTH / 2;
    game->current_piece_y = 0;
    game->current_piece_rotation = 0;
    game->current_piece_type = PIECE_EMPTY;
    game->next_piece_type = PIECE_EMPTY;
    
    // Reset line clear state
    game->line_clear_active = false;
    game->num_lines_to_clear = 0;
    game->line_clear_start_time = 0;
    for (int i = 0; i < 4; i++) {
        game->lines_to_clear[i] = -1;
    }
    
    // Show countdown when game resets
    game->show_countdown = true;
    game->countdown_start_time = get_clock_ticks_ms();
    printf("DEBUG: Countdown started! Show: %d, Start time: %llu\n", 
           game->show_countdown, (unsigned long long)game->countdown_start_time);
    
    game->paused = false;
}

void handle_events(event_system_t *event_system) {
    (void)event_system; // Unused for now
    
    // Simple SDL event polling wrapper
    // Currently just polls events without doing anything specific with them
    // The actual event handling is done by individual stages using keyboard state
    event_t event;
    do {
        event = poll_event();
        // Events are processed but the main input handling is done via keyboard state
        // This function exists to satisfy the interface expected by stages
    } while (event != NO_EVENT);
}
