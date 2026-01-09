/**
 * @file tetris_score.c
 * @brief Tetris scoring system implementation
 */

#include "tetris_score.h"
#include "constants.h"
#include <stdio.h>

void tetris_score_add_line_clear(game_t *game, int lines_cleared) {
    if (!game || lines_cleared < 1 || lines_cleared > 4) {
        return;
    }
    
    int base_points = 0;
    
    switch (lines_cleared) {
        case 1:
            base_points = POINTS_SINGLE_LINE;
            break;
        case 2:
            base_points = POINTS_DOUBLE_LINE;
            break;
        case 3:
            base_points = POINTS_TRIPLE_LINE;
            break;
        case 4:
            base_points = POINTS_TETRIS;
            break;
    }
    
    // Apply level multiplier
    int multiplier = tetris_score_get_level_multiplier(game->level);
    game->score += base_points * multiplier;
    
    // Update lines cleared count
    game->lines_cleared += lines_cleared;
    
    // Update level
    tetris_score_update_level(game);
}

void tetris_score_add_soft_drop(game_t *game, int cells_dropped) {
    if (!game || cells_dropped < 1) {
        return;
    }
    
    game->score += cells_dropped * POINTS_SOFT_DROP;
}

void tetris_score_add_hard_drop(game_t *game, int cells_dropped) {
    if (!game || cells_dropped < 1) {
        return;
    }
    
    game->score += cells_dropped * POINTS_HARD_DROP;
}

void tetris_score_update_level(game_t *game) {
    if (!game) {
        return;
    }
    
    // Level up every 10 lines
    int new_level = (game->lines_cleared / 10) + 1;
    
    if (new_level != game->level) {
        game->level = new_level;
        
        // Update fall speed
        game->fall_speed = tetris_score_calculate_fall_speed(game->level);
    }
}

int tetris_score_calculate_fall_speed(int level) {
    int speed = INITIAL_FALL_SPEED - ((level - 1) * SPEED_INCREASE_PER_LEVEL);
    
    // Minimum fall speed
    if (speed < FAST_FALL_SPEED) {
        speed = FAST_FALL_SPEED;
    }
    
    return speed;
}

int tetris_score_get_level_multiplier(int level) {
    // Level multiplier increases with level
    return level;
}

void tetris_score_reset(game_t *game) {
    if (!game) {
        return;
    }
    
    game->score = 0;
    game->level = 1;
    game->lines_cleared = 0;
    game->fall_speed = INITIAL_FALL_SPEED;
}

void tetris_score_format_display(int score, char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        return;
    }
    
    snprintf(buffer, buffer_size, "%08d", score);
}
