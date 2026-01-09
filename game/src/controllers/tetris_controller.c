/**
 * @file tetris_controller.c
 * @brief Tetris game input controller implementation
 */

#include "tetris_controller.h"
#include "tetris_collision.h"
#include "clock.h"
#include "constants.h"

void tetris_controller_init(tetris_controller_t *controller) {
    if (!controller) {
        return;
    }
    
    controller->last_move_time = 0;
    controller->last_rotate_time = 0;
    controller->last_drop_time = 0;
    controller->left_held = false;
    controller->right_held = false;
    controller->down_held = false;
    controller->space_held = false;
    controller->up_held = false;
}

void tetris_controller_update(tetris_controller_t *controller, game_t *game, 
                             keyboard_state_t *keyboard) {
    if (!controller || !game || !keyboard) {
        return;
    }
    
    // Only process input if game is in playing state and not paused
    if (game->current_screen != SCREEN_PLAYING || game->paused) {
        return;
    }
    
    // Handle all input types
    tetris_controller_handle_movement(controller, game, keyboard);
    tetris_controller_handle_rotation(controller, game, keyboard);
    tetris_controller_handle_soft_drop(controller, game, keyboard);
    tetris_controller_handle_hard_drop(controller, game, keyboard);
}

void tetris_controller_handle_movement(tetris_controller_t *controller, game_t *game,
                                      keyboard_state_t *keyboard) {
    if (!controller || !game || !keyboard) {
        return;
    }
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    bool can_move = tetris_controller_can_repeat_input(controller->last_move_time, MOVE_REPEAT_DELAY);
    
    // Handle left movement
    bool left_pressed = is_left_key_pressed(keyboard);
    if (left_pressed && (!controller->left_held || can_move)) {
        if (tetris_controller_move_piece(game, -1, 0)) {
            controller->last_move_time = current_time;
        }
    }
    controller->left_held = left_pressed;
    
    // Handle right movement
    bool right_pressed = is_right_key_pressed(keyboard);
    if (right_pressed && (!controller->right_held || can_move)) {
        if (tetris_controller_move_piece(game, 1, 0)) {
            controller->last_move_time = current_time;
        }
    }
    controller->right_held = right_pressed;
}

void tetris_controller_handle_rotation(tetris_controller_t *controller, game_t *game,
                                      keyboard_state_t *keyboard) {
    if (!controller || !game || !keyboard) {
        return;
    }
    
    timestamp_ms_t current_time = get_clock_ticks_ms();
    bool can_rotate = tetris_controller_can_repeat_input(controller->last_rotate_time, ROTATE_REPEAT_DELAY);
    
    // Handle rotation (up arrow for clockwise)
    bool up_pressed = is_up_key_pressed(keyboard);
    if (up_pressed && (!controller->up_held || can_rotate)) {
        if (tetris_controller_rotate_piece_clockwise(game)) {
            controller->last_rotate_time = current_time;
        }
    }
    controller->up_held = up_pressed;
}

void tetris_controller_handle_soft_drop(tetris_controller_t *controller, game_t *game,
                                       keyboard_state_t *keyboard) {
    if (!controller || !game || !keyboard) {
        return;
    }
    
    // Handle soft drop (down arrow - using space for now since down key not available)
    bool down_pressed = false; // Disabled for now until down key is implemented
    if (down_pressed) {
        // Override fall speed for faster dropping
        game->fall_speed = FAST_FALL_SPEED;
    } else if (controller->down_held) {
        // Reset fall speed when key is released
        game->fall_speed = INITIAL_FALL_SPEED - (game->level - 1) * SPEED_INCREASE_PER_LEVEL;
        if (game->fall_speed < FAST_FALL_SPEED) {
            game->fall_speed = FAST_FALL_SPEED;
        }
    }
    controller->down_held = down_pressed;
}

void tetris_controller_handle_hard_drop(tetris_controller_t *controller, game_t *game,
                                       keyboard_state_t *keyboard) {
    if (!controller || !game || !keyboard) {
        return;
    }
    
    // Handle hard drop (space bar)
    bool space_pressed = is_space_key_pressed(keyboard);
    if (space_pressed && !controller->space_held) {
        int lines_dropped = tetris_controller_hard_drop_piece(game);
        
        // Add score for hard drop
        game->score += lines_dropped * POINTS_HARD_DROP;
    }
    controller->space_held = space_pressed;
}

bool tetris_controller_move_piece(game_t *game, int dx, int dy) {
    if (!game || game->current_piece_type == PIECE_EMPTY) {
        return false;
    }
    
    // Check if movement is valid
    if (tetris_collision_can_move_piece(&game->board, game->current_piece_type,
                                       game->current_piece_rotation,
                                       game->current_piece_x, game->current_piece_y,
                                       dx, dy)) {
        game->current_piece_x += dx;
        game->current_piece_y += dy;
        return true;
    }
    
    return false;
}

bool tetris_controller_rotate_piece_clockwise(game_t *game) {
    if (!game || game->current_piece_type == PIECE_EMPTY) {
        return false;
    }
    
    int new_rotation = (game->current_piece_rotation + 1) % 4;
    int test_x = game->current_piece_x;
    int test_y = game->current_piece_y;
    
    // Try wall kick
    if (tetris_collision_wall_kick_test(&game->board, game->current_piece_type,
                                       game->current_piece_rotation, new_rotation,
                                       &test_x, &test_y)) {
        game->current_piece_rotation = new_rotation;
        game->current_piece_x = test_x;
        game->current_piece_y = test_y;
        return true;
    }
    
    return false;
}

bool tetris_controller_rotate_piece_counter_clockwise(game_t *game) {
    if (!game || game->current_piece_type == PIECE_EMPTY) {
        return false;
    }
    
    int new_rotation = (game->current_piece_rotation + 3) % 4; // +3 is same as -1 mod 4
    int test_x = game->current_piece_x;
    int test_y = game->current_piece_y;
    
    // Try wall kick
    if (tetris_collision_wall_kick_test(&game->board, game->current_piece_type,
                                       game->current_piece_rotation, new_rotation,
                                       &test_x, &test_y)) {
        game->current_piece_rotation = new_rotation;
        game->current_piece_x = test_x;
        game->current_piece_y = test_y;
        return true;
    }
    
    return false;
}

int tetris_controller_hard_drop_piece(game_t *game) {
    if (!game || game->current_piece_type == PIECE_EMPTY) {
        return 0;
    }
    
    int start_y = game->current_piece_y;
    int drop_y = tetris_collision_find_drop_position(&game->board, game->current_piece_type,
                                                    game->current_piece_rotation,
                                                    game->current_piece_x, start_y);
    
    game->current_piece_y = drop_y;
    
    // Return number of lines dropped
    return drop_y - start_y;
}

bool tetris_controller_can_repeat_input(timestamp_ms_t last_time, int delay) {
    timestamp_ms_t current_time = get_clock_ticks_ms();
    return (current_time - last_time) >= (timestamp_ms_t)delay;
}
