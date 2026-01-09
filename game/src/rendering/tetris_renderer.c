/**
 * @file tetris_renderer.c
 * @brief Tetris game rendering system implementation
 */

#include "tetris_renderer.h"
#include "drawing_primitives.h"
#include "tetris_collision.h"
#include "constants.h"
#include "clock.h"
#include "frame.h"
#include "text.h"
#include "arcade_font.h"
#include "bitmap_font.h"
#include "geometry.h"
#include "texture.h"
#include <stdio.h>

// Color constants
static const color_t BORDER_COLOR = COLOR(0, 0, 255); // Blue border
static const color_t GRID_COLOR = GRAY(64); // Dark gray grid
static const int GHOST_ALPHA = 128; // Semi-transparent ghost piece
static const color_t UI_BOX_COLOR = GRAY(64); // Semi-transparent dark gray for UI boxes

bool tetris_renderer_init(const graphics_context_t *graphics_context) {
    (void)graphics_context; // Not used for basic renderer
    return true;
}

void tetris_renderer_render_game(const game_t *game, const graphics_context_t *graphics_context) {
    if (!game || !graphics_context) {
        return;
    }
    
    // Clear screen with background color
    clear_frame((graphics_context_t*)graphics_context);
    
    // Render background image
    tetris_renderer_render_background(game, graphics_context);
    
    // Render game board and border
    tetris_renderer_render_board(game, graphics_context);
    
    // Render placed pieces
    tetris_renderer_render_placed_pieces(&game->board, graphics_context);
    
    // Render line clear effect if active
    if (game->line_clear_active) {
        tetris_renderer_render_line_clear_effect(game, graphics_context);
    }
    
    // Render current piece if active
    if (game->current_piece_type != PIECE_EMPTY) {
        tetris_renderer_render_ghost_piece(game, graphics_context);
        tetris_renderer_render_current_piece(game, graphics_context);
    }
    
    // Render next piece preview
    tetris_renderer_render_next_piece(game, graphics_context);
    
    // Render UI elements
    tetris_renderer_render_ui(game, graphics_context);
    
    // Render countdown if active
    if (game->show_countdown) {
        tetris_renderer_render_countdown(game, graphics_context);
    }
}

void tetris_renderer_render_board(const game_t *game, const graphics_context_t *graphics_context) {
    (void)game; // Board rendering doesn't need game state
    
    // Render semi-transparent background for playfield
    tetris_renderer_render_playfield_background(graphics_context);
    
    tetris_renderer_render_board_border(graphics_context);
    tetris_renderer_render_board_grid(graphics_context);
}

void tetris_renderer_render_board_border(const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    int board_screen_width = BOARD_WIDTH * CELL_SIZE;
    int board_screen_height = BOARD_HEIGHT * CELL_SIZE;
    
    // Draw border around the game board
    // Top border
    draw_line((graphics_context_t*)graphics_context, 
              BOARD_OFFSET_X - BORDER_SIZE, BOARD_OFFSET_Y - BORDER_SIZE,
              BOARD_OFFSET_X + board_screen_width + BORDER_SIZE, BOARD_OFFSET_Y - BORDER_SIZE,
              BORDER_COLOR);
    
    // Bottom border  
    draw_line((graphics_context_t*)graphics_context,
              BOARD_OFFSET_X - BORDER_SIZE, BOARD_OFFSET_Y + board_screen_height + BORDER_SIZE,
              BOARD_OFFSET_X + board_screen_width + BORDER_SIZE, BOARD_OFFSET_Y + board_screen_height + BORDER_SIZE,
              BORDER_COLOR);
    
    // Left border
    draw_line((graphics_context_t*)graphics_context,
              BOARD_OFFSET_X - BORDER_SIZE, BOARD_OFFSET_Y - BORDER_SIZE,
              BOARD_OFFSET_X - BORDER_SIZE, BOARD_OFFSET_Y + board_screen_height + BORDER_SIZE,
              BORDER_COLOR);
    
    // Right border
    draw_line((graphics_context_t*)graphics_context,
              BOARD_OFFSET_X + board_screen_width + BORDER_SIZE, BOARD_OFFSET_Y - BORDER_SIZE,
              BOARD_OFFSET_X + board_screen_width + BORDER_SIZE, BOARD_OFFSET_Y + board_screen_height + BORDER_SIZE,
              BORDER_COLOR);
}

void tetris_renderer_render_board_grid(const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    // Draw vertical grid lines
    for (int x = 1; x < BOARD_WIDTH; x++) {
        int screen_x = BOARD_OFFSET_X + x * CELL_SIZE;
        draw_line((graphics_context_t*)graphics_context,
                  screen_x, BOARD_OFFSET_Y,
                  screen_x, BOARD_OFFSET_Y + BOARD_HEIGHT * CELL_SIZE,
                  GRID_COLOR);
    }
    
    // Draw horizontal grid lines
    for (int y = 1; y < BOARD_HEIGHT; y++) {
        int screen_y = BOARD_OFFSET_Y + y * CELL_SIZE;
        draw_line((graphics_context_t*)graphics_context,
                  BOARD_OFFSET_X, screen_y,
                  BOARD_OFFSET_X + BOARD_WIDTH * CELL_SIZE, screen_y,
                  GRID_COLOR);
    }
}

void tetris_renderer_render_placed_pieces(const game_board_t *board, 
                                         const graphics_context_t *graphics_context) {
    if (!board || !graphics_context) {
        return;
    }
    
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game_board_is_cell_filled(board, x, y)) {
                int screen_x, screen_y;
                tetris_renderer_board_to_screen(x, y, &screen_x, &screen_y);
                
                color_t cell_color = game_board_get_cell_color(board, x, y);
                color_t border_color = COLOR(255, 255, 255); // White border
                
                tetris_renderer_render_cell(screen_x, screen_y, CELL_SIZE, 
                                           cell_color, border_color, graphics_context);
            }
        }
    }
}

void tetris_renderer_render_current_piece(const game_t *game, 
                                         const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || game->current_piece_type == PIECE_EMPTY) {
        return;
    }
    
    color_t piece_color = tetris_piece_get_color(game->current_piece_type);
    color_t border_color = COLOR(255, 255, 255); // White border
    
    // Render each cell of the piece
    for (int py = 0; py < PIECE_SIZE; py++) {
        for (int px = 0; px < PIECE_SIZE; px++) {
            if (tetris_piece_is_cell_filled(game->current_piece_type, 
                                           game->current_piece_rotation, px, py)) {
                int board_x = game->current_piece_x + px;
                int board_y = game->current_piece_y + py;
                
                if (game_board_is_position_valid(board_x, board_y)) {
                    int screen_x, screen_y;
                    tetris_renderer_board_to_screen(board_x, board_y, &screen_x, &screen_y);
                    
                    tetris_renderer_render_cell(screen_x, screen_y, CELL_SIZE, 
                                               piece_color, border_color, graphics_context);
                }
            }
        }
    }
}

void tetris_renderer_render_ghost_piece(const game_t *game, 
                                       const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || game->current_piece_type == PIECE_EMPTY) {
        return;
    }
    
    // Find where the piece would land
    int ghost_y = tetris_collision_find_drop_position(&game->board, game->current_piece_type,
                                                     game->current_piece_rotation,
                                                     game->current_piece_x, game->current_piece_y);
    
    // Don't render ghost if it's at the same position as current piece
    if (ghost_y == game->current_piece_y) {
        return;
    }
    
    color_t white_outline = COLOR(255, 255, 255); // White outline only
    
    // Render each cell of the ghost piece as white outline only
    for (int py = 0; py < PIECE_SIZE; py++) {
        for (int px = 0; px < PIECE_SIZE; px++) {
            if (tetris_piece_is_cell_filled(game->current_piece_type, 
                                           game->current_piece_rotation, px, py)) {
                int board_x = game->current_piece_x + px;
                int board_y_ghost = ghost_y + py;
                
                if (game_board_is_position_valid(board_x, board_y_ghost)) {
                    int screen_x, screen_y;
                    tetris_renderer_board_to_screen(board_x, board_y_ghost, &screen_x, &screen_y);
                    
                    // Draw only the white border outline (no fill)
                    draw_line((graphics_context_t*)graphics_context, 
                             screen_x, screen_y, screen_x + CELL_SIZE, screen_y, white_outline); // Top
                    draw_line((graphics_context_t*)graphics_context, 
                             screen_x + CELL_SIZE, screen_y, screen_x + CELL_SIZE, screen_y + CELL_SIZE, white_outline); // Right
                    draw_line((graphics_context_t*)graphics_context, 
                             screen_x + CELL_SIZE, screen_y + CELL_SIZE, screen_x, screen_y + CELL_SIZE, white_outline); // Bottom
                    draw_line((graphics_context_t*)graphics_context, 
                             screen_x, screen_y + CELL_SIZE, screen_x, screen_y, white_outline); // Left
                }
            }
        }
    }
}

void tetris_renderer_render_next_piece(const game_t *game, 
                                      const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || game->next_piece_type == PIECE_EMPTY) {
        return;
    }
    
    // Render semi-transparent background for next piece box
    tetris_renderer_render_next_piece_background(graphics_context);
    
    // Draw blue border around next piece area
    color_t border_color = COLOR(0, 100, 255); // Blue border
    int border_thickness = 3;
    
    // Draw thick border
    for (int i = 0; i < border_thickness; i++) {
        draw_line((graphics_context_t*)graphics_context, 
                  NEXT_PIECE_X - i, NEXT_PIECE_Y - i,
                  NEXT_PIECE_X + NEXT_PIECE_SIZE + i, NEXT_PIECE_Y - i, border_color);
        draw_line((graphics_context_t*)graphics_context,
                  NEXT_PIECE_X + NEXT_PIECE_SIZE + i, NEXT_PIECE_Y - i,
                  NEXT_PIECE_X + NEXT_PIECE_SIZE + i, NEXT_PIECE_Y + NEXT_PIECE_SIZE + i, border_color);
        draw_line((graphics_context_t*)graphics_context,
                  NEXT_PIECE_X + NEXT_PIECE_SIZE + i, NEXT_PIECE_Y + NEXT_PIECE_SIZE + i,
                  NEXT_PIECE_X - i, NEXT_PIECE_Y + NEXT_PIECE_SIZE + i, border_color);
        draw_line((graphics_context_t*)graphics_context,
                  NEXT_PIECE_X - i, NEXT_PIECE_Y + NEXT_PIECE_SIZE + i,
                  NEXT_PIECE_X - i, NEXT_PIECE_Y - i, border_color);
    }
    
    // Render "NEXT" text in yellow above the piece using arcade font with larger scale
    const char* next_text = "NEXT";
    int text_x = NEXT_PIECE_X + 5;
    int text_y = NEXT_PIECE_Y - 25; // Moved up slightly for larger text
    render_arcade_text_scaled((arcade_font_ptr)&game->arcade_font,
                             (graphics_context_ptr)graphics_context,
                             next_text, text_x, text_y, FONT_COLOR_YELLOW, 2);
    
    // Calculate piece position to center it in the box
    int piece_cell_size = CELL_SIZE / 2; // Smaller for next piece preview
    int piece_x = NEXT_PIECE_X + (NEXT_PIECE_SIZE - piece_cell_size * PIECE_SIZE) / 2;
    int piece_y = NEXT_PIECE_Y + (NEXT_PIECE_SIZE - piece_cell_size * PIECE_SIZE) / 2;
    
    color_t piece_color = tetris_piece_get_color(game->next_piece_type);
    
    tetris_renderer_render_piece_at_position(game->next_piece_type, 0,
                                            piece_x, piece_y, piece_cell_size,
                                            piece_color, graphics_context);
}

void tetris_renderer_render_piece_at_position(piece_type_t piece_type, int rotation,
                                             int x, int y, int cell_size, color_t color,
                                             const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    color_t border_color = COLOR(255, 255, 255);
    
    for (int py = 0; py < PIECE_SIZE; py++) {
        for (int px = 0; px < PIECE_SIZE; px++) {
            if (tetris_piece_is_cell_filled(piece_type, rotation, px, py)) {
                int cell_x = x + px * cell_size;
                int cell_y = y + py * cell_size;
                
                tetris_renderer_render_cell(cell_x, cell_y, cell_size, 
                                           color, border_color, graphics_context);
            }
        }
    }
}

void tetris_renderer_render_cell(int x, int y, int size, color_t fill_color, 
                                color_t border_color, const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    // Fill the cell
    const SDL_Point points[4] = {
        {x, y},
        {x + size, y},
        {x + size, y + size},
        {x, y + size}
    };
    draw_filled_polygon((graphics_context_t*)graphics_context, points, 4, fill_color);
    
    // Draw border
    draw_line((graphics_context_t*)graphics_context, x, y, x + size, y, border_color); // Top
    draw_line((graphics_context_t*)graphics_context, x + size, y, x + size, y + size, border_color); // Right
    draw_line((graphics_context_t*)graphics_context, x + size, y + size, x, y + size, border_color); // Bottom
    draw_line((graphics_context_t*)graphics_context, x, y + size, x, y, border_color); // Left
}

void tetris_renderer_render_ui(const game_t *game, const graphics_context_t *graphics_context) {
    if (!game || !graphics_context) {
        return;
    }
    
    // Render semi-transparent background for score box
    tetris_renderer_render_score_background(graphics_context);
    
    // Render score in a bordered box
    color_t border_color = COLOR(255, 255, 255); // White border
    // Removed unused text_color variable
    int box_width = NEXT_PIECE_SIZE;
    int box_height = 60;
    
    // Draw score box border
    draw_line((graphics_context_t*)graphics_context, 
              SCORE_X, SCORE_Y, SCORE_X + box_width, SCORE_Y, border_color);
    draw_line((graphics_context_t*)graphics_context,
              SCORE_X + box_width, SCORE_Y, SCORE_X + box_width, SCORE_Y + box_height, border_color);
    draw_line((graphics_context_t*)graphics_context,
              SCORE_X + box_width, SCORE_Y + box_height, SCORE_X, SCORE_Y + box_height, border_color);
    draw_line((graphics_context_t*)graphics_context,
              SCORE_X, SCORE_Y + box_height, SCORE_X, SCORE_Y, border_color);
    
    // Render "SCORE" label using arcade font
    const char* score_label = "SCORE";
    render_arcade_text_scaled((arcade_font_ptr)&game->arcade_font,
                             (graphics_context_ptr)graphics_context,
                             score_label, SCORE_X + 10, SCORE_Y + 10, FONT_COLOR_YELLOW, 2);
    
    // Render score value using arcade font
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "%d", game->score);
    render_arcade_text_scaled((arcade_font_ptr)&game->arcade_font,
                             (graphics_context_ptr)graphics_context,
                             score_text, SCORE_X + 10, SCORE_Y + 35, FONT_COLOR_WHITE, 2);
}

void tetris_renderer_render_line_clear_effect(const game_t *game, 
                                             const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || !game->line_clear_active) {
        return;
    }
    
    timestamp_ms_t elapsed = get_clock_ticks_ms() - game->line_clear_start_time;
    if (elapsed > LINE_CLEAR_DELAY) {
        return;
    }
    
    // Create flashing effect
    int flash_interval = 100;
    bool flash_on = (elapsed / flash_interval) % 2 == 0;
    
    if (flash_on) {
        color_t flash_color = COLOR(255, 255, 255); // White flash
        
        for (int i = 0; i < game->num_lines_to_clear; i++) {
            int line = game->lines_to_clear[i];
            if (line >= 0 && line < BOARD_HEIGHT) {
                int screen_x, screen_y;
                tetris_renderer_board_to_screen(0, line, &screen_x, &screen_y);
                
                // Draw line across the entire width
                draw_thick_line((graphics_context_t*)graphics_context,
                               screen_x, screen_y + CELL_SIZE / 2,
                               screen_x + BOARD_WIDTH * CELL_SIZE, screen_y + CELL_SIZE / 2,
                               flash_color);
            }
        }
    }
}

void tetris_renderer_board_to_screen(int board_x, int board_y, int *screen_x, int *screen_y) {
    if (screen_x) {
        *screen_x = BOARD_OFFSET_X + board_x * CELL_SIZE;
    }
    if (screen_y) {
        *screen_y = BOARD_OFFSET_Y + board_y * CELL_SIZE;
    }
}

color_t tetris_renderer_get_alpha_color(color_t base_color, int alpha) {
    // Since the color system doesn't support alpha, create a dimmed version
    // by reducing the intensity of the color based on alpha value
    int r = R(base_color);
    int g = G(base_color);
    int b = B(base_color);
    
    // Apply alpha by scaling the RGB values (alpha 0-255)
    r = (r * alpha) / 255;
    g = (g * alpha) / 255;
    b = (b * alpha) / 255;
    
    return COLOR(r, g, b);
}


void tetris_renderer_render_background(const game_t *game, const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || !game->background_texture.texture) {
        return;
    }
    
    // Create destination rectangle to cover the entire screen
    rect_t dst_rect = make_rect(0, 0, graphics_context->screen_width, graphics_context->screen_height);
    
    // Render the background texture scaled to fit the entire screen
    render_sprite((graphics_context_ptr)graphics_context, (texture_ptr)&game->background_texture, NULL, &dst_rect);
}

void tetris_renderer_render_playfield_background(const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    // Create semi-transparent dark gray background for the entire playfield area
    int padding = 10;
    int x = BOARD_OFFSET_X - padding;
    int y = BOARD_OFFSET_Y - padding;
    int width = BOARD_WIDTH * CELL_SIZE + padding * 2;
    int height = BOARD_HEIGHT * CELL_SIZE + padding * 2;
    
    // Draw filled rectangle using horizontal lines
    color_t semi_transparent_gray = COLOR(32, 32, 32); // Darker gray for better visibility
    for (int line_y = y; line_y < y + height; line_y++) {
        draw_line((graphics_context_t*)graphics_context, x, line_y, x + width, line_y, semi_transparent_gray);
    }
}

void tetris_renderer_render_next_piece_background(const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    // Create semi-transparent dark gray background for next piece box
    int padding = 5;
    int x = NEXT_PIECE_X - padding;
    int y = NEXT_PIECE_Y - 30; // Include space for "NEXT" text
    int width = NEXT_PIECE_SIZE + padding * 2;
    int height = NEXT_PIECE_SIZE + 35; // Extra height for text
    
    // Draw filled rectangle using horizontal lines
    color_t semi_transparent_gray = COLOR(32, 32, 32);
    for (int line_y = y; line_y < y + height; line_y++) {
        draw_line((graphics_context_t*)graphics_context, x, line_y, x + width, line_y, semi_transparent_gray);
    }
}

void tetris_renderer_render_score_background(const graphics_context_t *graphics_context) {
    if (!graphics_context) {
        return;
    }
    
    // Create semi-transparent dark gray background for score box
    int padding = 5;
    int x = SCORE_X - padding;
    int y = SCORE_Y - padding;
    int width = NEXT_PIECE_SIZE + padding * 2;
    int height = 60 + padding * 2;
    
    // Draw filled rectangle using horizontal lines
    color_t semi_transparent_gray = COLOR(32, 32, 32);
    for (int line_y = y; line_y < y + height; line_y++) {
        draw_line((graphics_context_t*)graphics_context, x, line_y, x + width, line_y, semi_transparent_gray);
    }
}

void tetris_renderer_render_countdown(const game_t *game, const graphics_context_t *graphics_context) {
    if (!game || !graphics_context || !game->show_countdown) {
        return;
    }
    
    // Calculate elapsed time since countdown started
    timestamp_ms_t current_time = get_clock_ticks_ms();
    timestamp_ms_t elapsed_time = current_time - game->countdown_start_time;
    
    // Countdown sequence: "3" (2s), "2" (2s), "1" (0.5s), "GO!" (0.5s)
    const char* countdown_text;
    timestamp_ms_t phase_elapsed;
    font_color_t text_color;
    timestamp_ms_t fade_start_time;
    
    if (elapsed_time < 2000) {
        // Phase 1: Show "3" for first 2 seconds
        countdown_text = "3";
        phase_elapsed = elapsed_time;
        text_color = FONT_COLOR_YELLOW;
        fade_start_time = 1500; // Fade in last 0.5 seconds
    } else if (elapsed_time < 4000) {
        // Phase 2: Show "2" for next 2 seconds
        countdown_text = "2";
        phase_elapsed = elapsed_time - 2000;
        text_color = FONT_COLOR_YELLOW;
        fade_start_time = 1500; // Fade in last 0.5 seconds
    } else if (elapsed_time < 4500) {
        // Phase 3: Show "1" for 0.5 seconds
        countdown_text = "1";
        phase_elapsed = elapsed_time - 4000;
        text_color = FONT_COLOR_YELLOW;
        fade_start_time = 250; // No fade for fast transition
    } else if (elapsed_time < 5000) {
        // Phase 4: Show "GO!" for 0.5 seconds
        countdown_text = "GO!";
        phase_elapsed = elapsed_time - 4500;
        text_color = FONT_COLOR_CYAN; // Use cyan as closest to green
        fade_start_time = 250; // No fade for fast transition
    } else {
        // Countdown complete - don't render
        return;
    }
    
    // Make it 2x bigger - scale 40
    int scale = 40;
    
    // Calculate alpha for smooth fade-out effect
    int alpha = 255; // Start fully opaque
    if (phase_elapsed >= fade_start_time) {
        // In fade-out period - linearly decrease alpha
        timestamp_ms_t fade_duration = (strcmp(countdown_text, "1") == 0 || strcmp(countdown_text, "GO!") == 0) ? 250 : 500;
        timestamp_ms_t fade_time = phase_elapsed - fade_start_time;
        if (fade_time >= fade_duration) {
            // This phase fade complete
            alpha = 0;
        } else {
            // Linear fade from 255 to 0
            alpha = 255 - (int)((fade_time * 255) / fade_duration);
            if (alpha < 0) alpha = 0;
        }
    }
    
    // Don't render if alpha is 0
    if (alpha == 0) {
        return;
    }
    
    // Calculate text dimensions for proper centering
    int text_width = get_arcade_text_width_scaled((arcade_font_ptr)&game->arcade_font, countdown_text, scale);
    int text_height = 7 * scale;  // Arcade font character height is 7 pixels
    
    // Position text in exact center of screen
    int text_x = (LOGICAL_WIDTH - text_width) / 2;
    int text_y = (LOGICAL_HEIGHT - text_height) / 2;
    
    render_arcade_text_scaled_alpha((arcade_font_ptr)&game->arcade_font,
                                   (graphics_context_ptr)graphics_context,
                                   countdown_text, text_x, text_y, text_color, scale, alpha);
}
