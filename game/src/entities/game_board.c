/**
 * @file game_board.c
 * @brief Tetris game board management implementation
 */

#include "game_board.h"
#include "tetris_piece.h"
#include <string.h>

void game_board_init(game_board_t *board) {
    if (!board) {
        return;
    }
    
    game_board_reset(board);
}

void game_board_reset(game_board_t *board) {
    if (!board) {
        return;
    }
    
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board->cells[y][x].filled = false;
            board->cells[y][x].piece_type = PIECE_EMPTY;
            board->cells[y][x].color = COLOR_BLACK;
        }
    }
}

void game_board_set_cell(game_board_t *board, int x, int y, piece_type_t piece_type, color_t color) {
    if (!board || !game_board_is_position_valid(x, y)) {
        return;
    }
    
    board->cells[y][x].filled = true;
    board->cells[y][x].piece_type = piece_type;
    board->cells[y][x].color = color;
}

void game_board_clear_cell(game_board_t *board, int x, int y) {
    if (!board || !game_board_is_position_valid(x, y)) {
        return;
    }
    
    board->cells[y][x].filled = false;
    board->cells[y][x].piece_type = PIECE_EMPTY;
    board->cells[y][x].color = COLOR_BLACK;
}

bool game_board_is_cell_filled(const game_board_t *board, int x, int y) {
    if (!board || !game_board_is_position_valid(x, y)) {
        return true; // Consider out-of-bounds as filled
    }
    
    return board->cells[y][x].filled;
}

bool game_board_is_position_valid(int x, int y) {
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}

bool game_board_is_line_complete(const game_board_t *board, int y) {
    if (!board || y < 0 || y >= BOARD_HEIGHT) {
        return false;
    }
    
    for (int x = 0; x < BOARD_WIDTH; x++) {
        if (!board->cells[y][x].filled) {
            return false;
        }
    }
    
    return true;
}

void game_board_clear_line(game_board_t *board, int y) {
    if (!board || y < 0 || y >= BOARD_HEIGHT) {
        return;
    }
    
    // Move all lines above down by one
    for (int line = y; line > 0; line--) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            board->cells[line][x] = board->cells[line - 1][x];
        }
    }
    
    // Clear the top line
    for (int x = 0; x < BOARD_WIDTH; x++) {
        board->cells[0][x].filled = false;
        board->cells[0][x].piece_type = PIECE_EMPTY;
        board->cells[0][x].color = COLOR_BLACK;
    }
}

int game_board_find_complete_lines(const game_board_t *board, int lines[4]) {
    if (!board || !lines) {
        return 0;
    }
    
    int count = 0;
    
    // Check from bottom to top
    for (int y = BOARD_HEIGHT - 1; y >= 0 && count < 4; y--) {
        if (game_board_is_line_complete(board, y)) {
            lines[count] = y;
            count++;
        }
    }
    
    return count;
}

void game_board_clear_lines(game_board_t *board, const int lines[], int num_lines) {
    if (!board || !lines || num_lines <= 0) {
        return;
    }
    
    // Sort lines in descending order to clear from bottom to top
    int sorted_lines[4];
    for (int i = 0; i < num_lines; i++) {
        sorted_lines[i] = lines[i];
    }
    
    // Simple bubble sort for small array
    for (int i = 0; i < num_lines - 1; i++) {
        for (int j = 0; j < num_lines - i - 1; j++) {
            if (sorted_lines[j] < sorted_lines[j + 1]) {
                int temp = sorted_lines[j];
                sorted_lines[j] = sorted_lines[j + 1];
                sorted_lines[j + 1] = temp;
            }
        }
    }
    
    // Clear lines from bottom to top
    for (int i = 0; i < num_lines; i++) {
        game_board_clear_line(board, sorted_lines[i]);
    }
}

void game_board_place_piece(game_board_t *board, piece_type_t piece_type, 
                           int piece_rotation, int piece_x, int piece_y) {
    if (!board) {
        return;
    }
    
    color_t piece_color = tetris_piece_get_color(piece_type);
    
    // Place each filled cell of the piece on the board
    for (int py = 0; py < PIECE_SIZE; py++) {
        for (int px = 0; px < PIECE_SIZE; px++) {
            if (tetris_piece_is_cell_filled(piece_type, piece_rotation, px, py)) {
                int board_x = piece_x + px;
                int board_y = piece_y + py;
                
                if (game_board_is_position_valid(board_x, board_y)) {
                    game_board_set_cell(board, board_x, board_y, piece_type, piece_color);
                }
            }
        }
    }
}

color_t game_board_get_cell_color(const game_board_t *board, int x, int y) {
    if (!board || !game_board_is_position_valid(x, y)) {
        return COLOR_BLACK;
    }
    
    return board->cells[y][x].color;
}

bool game_board_is_game_over(const game_board_t *board) {
    if (!board) {
        return true;
    }
    
    // Check if any cell in the top few rows is filled
    for (int y = 0; y < 4; y++) { // Check top 4 rows for game over
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->cells[y][x].filled) {
                return true;
            }
        }
    }
    
    return false;
}
