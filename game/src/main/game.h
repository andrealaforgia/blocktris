/**
 * @file game.h
 * @brief Core Tetris game state and lifecycle management
 *
 * Provides the central game structure and lifecycle functions (init/terminate).
 * This is the main entry point for game initialization and cleanup.
 */

#ifndef TETRIS_GAME_H_
#define TETRIS_GAME_H_

#include "types.h"
#include <stdbool.h>

#include "audio.h"
#include "event_system.h"
#include "graphics.h"
#include "keyboard.h"
#include "object_pool.h"
#include "constants.h"
#include "arcade_font.h"
#include "texture.h"

// Entity modules
#include "tetris_piece.h"
#include "game_board.h"

// Forward declarations for stage system
typedef struct stage_t stage_t;

/**
 * Stage action returned by stage update functions
 */
typedef enum {
    PROGRESS, // Continue with current stage
    QUIT      // Quit the game
} game_stage_action_t;

/**
 * Game screen states
 */
typedef enum { 
    SCREEN_INTRO,
    SCREEN_MENU, 
    SCREEN_PLAYING, 
    SCREEN_GAME_OVER, 
    SCREEN_PAUSED 
} game_screen_t;

/**
 * Main Tetris game state structure
 * Contains all game data including graphics, audio, entities, and game state
 */
typedef struct {
    // Core systems
    graphics_context_t graphics_context;
    audio_context_t audio_context;
    event_system_t event_system;
    keyboard_state_t keyboard_state;
    arcade_font_t arcade_font;
    texture_t background_texture;

    // Game state
    bool running;
    bool paused;
    game_screen_t current_screen;
    stage_t *current_stage; // Current active stage (for stage system)

    // Game entities
    game_board_t board;
    
    // Object pools for efficient entity management
    object_pool_t piece_pool;

    // Game statistics
    int score;
    int level;
    int lines_cleared;
    
    // Timing
    timestamp_ms_t last_fall_time;
    timestamp_ms_t last_move_time;
    timestamp_ms_t last_rotate_time;
    int fall_speed;
    
    // Current piece state
    int current_piece_x;
    int current_piece_y;
    int current_piece_rotation;
    piece_type_t current_piece_type;
    piece_type_t next_piece_type;
    
    // Game board state for visual effects
    bool line_clear_active;
    int lines_to_clear[4]; // Maximum 4 lines can be cleared at once
    int num_lines_to_clear;
    timestamp_ms_t line_clear_start_time;
    
    // Countdown sequence at game start (3, 2)
    bool show_countdown;
    timestamp_ms_t countdown_start_time;
} game_t;

// Pointer typedef for game
typedef game_t *game_ptr;

/**
 * Initialize the Tetris game
 * Sets up graphics, audio, loads resources, and initializes game state
 *
 * @param game Pointer to game structure to initialize
 * @return true if initialization successful, false otherwise
 */
bool game_init(game_t *game);

/**
 * Clean up and terminate the game
 * Frees all resources and shuts down SDL subsystems
 *
 * @param game Pointer to game structure to terminate
 */
void game_terminate(game_t *game);

/**
 * Reset game state for a new game
 *
 * @param game Pointer to game structure to reset
 */
void game_reset(game_t *game);

/**
 * Handle SDL events and update event system
 *
 * @param event_system Pointer to event system
 */
void handle_events(event_system_t *event_system);

#endif // TETRIS_GAME_H_
