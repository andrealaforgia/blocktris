/**
 * @file blocktris_score.h
 * @brief BlockTris scoring system
 *
 * Handles score calculation, level progression, and statistics tracking
 * for the BlockTris game.
 */

#ifndef BLOCKTRIS_SCORE_H_
#define BLOCKTRIS_SCORE_H_

#include "game.h"

/**
 * Update score for line clears
 *
 * @param game Pointer to game state
 * @param lines_cleared Number of lines cleared (1-4)
 */
void blocktris_score_add_line_clear(game_t *game, int lines_cleared);

/**
 * Update score for soft drop
 *
 * @param game Pointer to game state
 * @param cells_dropped Number of cells dropped
 */
void blocktris_score_add_soft_drop(game_t *game, int cells_dropped);

/**
 * Update score for hard drop
 *
 * @param game Pointer to game state
 * @param cells_dropped Number of cells dropped
 */
void blocktris_score_add_hard_drop(game_t *game, int cells_dropped);

/**
 * Update level based on lines cleared
 *
 * @param game Pointer to game state
 */
void blocktris_score_update_level(game_t *game);

/**
 * Calculate fall speed based on level
 *
 * @param level Current level
 * @return Fall speed in milliseconds
 */
int blocktris_score_calculate_fall_speed(int level);

/**
 * Get score multiplier for level
 *
 * @param level Current level
 * @return Score multiplier
 */
int blocktris_score_get_level_multiplier(int level);

/**
 * Reset score and statistics
 *
 * @param game Pointer to game state
 */
void blocktris_score_reset(game_t *game);

/**
 * Format score for display
 *
 * @param score Score value
 * @param buffer Output buffer
 * @param buffer_size Size of output buffer
 */
void blocktris_score_format_display(int score, char *buffer, size_t buffer_size);

#endif // BLOCKTRIS_SCORE_H_
