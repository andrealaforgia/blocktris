/**
 * @file tetris_renderer.h
 * @brief Tetris game rendering system
 *
 * Handles all rendering operations for the Tetris game including
 * game board, pieces, UI elements, and visual effects.
 */

#ifndef TETRIS_RENDERER_H_
#define TETRIS_RENDERER_H_

#include "game.h"
#include "graphics.h"
#include "color.h"

/**
 * Initialize the Tetris renderer
 *
 * @param graphics_context Pointer to graphics context
 * @return true if initialization successful, false otherwise
 */
bool tetris_renderer_init(const graphics_context_t *graphics_context);

/**
 * Render the entire game
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_game(const game_t *game, const graphics_context_t *graphics_context);

/**
 * Render the game board with border
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_board(const game_t *game, const graphics_context_t *graphics_context);

/**
 * Render the game board border
 *
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_board_border(const graphics_context_t *graphics_context);

/**
 * Render the game board grid
 *
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_board_grid(const graphics_context_t *graphics_context);

/**
 * Render placed pieces on the board
 *
 * @param board Pointer to game board
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_placed_pieces(const game_board_t *board, 
                                         const graphics_context_t *graphics_context);

/**
 * Render the current falling piece
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_current_piece(const game_t *game, 
                                         const graphics_context_t *graphics_context);

/**
 * Render the ghost piece (preview of where piece will land)
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_ghost_piece(const game_t *game, 
                                       const graphics_context_t *graphics_context);

/**
 * Render the next piece preview
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_next_piece(const game_t *game, 
                                      const graphics_context_t *graphics_context);

/**
 * Render a single piece at specified position
 *
 * @param piece_type Type of piece to render
 * @param rotation Rotation of the piece
 * @param x Screen x coordinate
 * @param y Screen y coordinate
 * @param cell_size Size of each cell
 * @param color Color to render the piece
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_piece_at_position(piece_type_t piece_type, int rotation,
                                             int x, int y, int cell_size, color_t color,
                                             const graphics_context_t *graphics_context);

/**
 * Render a single cell (filled rectangle with border)
 *
 * @param x Screen x coordinate
 * @param y Screen y coordinate
 * @param size Size of the cell
 * @param fill_color Fill color
 * @param border_color Border color
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_cell(int x, int y, int size, color_t fill_color, 
                                color_t border_color, const graphics_context_t *graphics_context);

/**
 * Render game UI elements (score, level, lines)
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_ui(const game_t *game, const graphics_context_t *graphics_context);

/**
 * Render line clear effect
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_line_clear_effect(const game_t *game, 
                                             const graphics_context_t *graphics_context);

/**
 * Convert board coordinates to screen coordinates
 *
 * @param board_x Board x coordinate
 * @param board_y Board y coordinate
 * @param screen_x Pointer to output screen x coordinate
 * @param screen_y Pointer to output screen y coordinate
 */
void tetris_renderer_board_to_screen(int board_x, int board_y, int *screen_x, int *screen_y);

/**
 * Get color with alpha transparency
 *
 * @param base_color Base color
 * @param alpha Alpha value (0-255)
 * @return Color with alpha applied
 */
color_t tetris_renderer_get_alpha_color(color_t base_color, int alpha);

/**
 * Render background image
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_background(const game_t *game, const graphics_context_t *graphics_context);

/**
 * Render semi-transparent background for playfield
 *
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_playfield_background(const graphics_context_t *graphics_context);

/**
 * Render semi-transparent background for next piece box
 *
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_next_piece_background(const graphics_context_t *graphics_context);

/**
 * Render semi-transparent background for score box
 *
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_score_background(const graphics_context_t *graphics_context);

/**
 * Render countdown display before game starts (3, 2)
 *
 * @param game Pointer to game state
 * @param graphics_context Pointer to graphics context
 */
void tetris_renderer_render_countdown(const game_t *game, const graphics_context_t *graphics_context);

#endif // TETRIS_RENDERER_H_
