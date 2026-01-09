/**
 * @file tetris_collision.c
 * @brief Tetris collision detection system implementation
 */

#include "tetris_collision.h"

// Wall kick test offsets for different piece types and rotations
// Standard SRS (Super Rotation System) wall kicks for most pieces
static const int WALL_KICK_OFFSETS_JLSTZ[4][5][2] = {
    // From 0 to 1
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
    // From 1 to 2  
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
    // From 2 to 3
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
    // From 3 to 0
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
};

// I-piece has different wall kick rules
static const int WALL_KICK_OFFSETS_I[4][5][2] = {
    // From 0 to 1
    {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
    // From 1 to 2
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
    // From 2 to 3
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
    // From 3 to 0
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}
};

bool tetris_collision_can_place_piece(const game_board_t *board, piece_type_t piece_type,
                                     int piece_rotation, int piece_x, int piece_y) {
    if (!board) {
        return false;
    }
    
    // Check each cell of the piece
    for (int py = 0; py < PIECE_SIZE; py++) {
        for (int px = 0; px < PIECE_SIZE; px++) {
            if (tetris_piece_is_cell_filled(piece_type, piece_rotation, px, py)) {
                int board_x = piece_x + px;
                int board_y = piece_y + py;
                
                // Check bounds
                if (board_x < 0 || board_x >= BOARD_WIDTH || 
                    board_y < 0 || board_y >= BOARD_HEIGHT) {
                    return false;
                }
                
                // Check if board cell is already occupied
                if (game_board_is_cell_filled(board, board_x, board_y)) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

bool tetris_collision_can_move_piece(const game_board_t *board, piece_type_t piece_type,
                                    int piece_rotation, int piece_x, int piece_y, 
                                    int dx, int dy) {
    return tetris_collision_can_place_piece(board, piece_type, piece_rotation,
                                           piece_x + dx, piece_y + dy);
}

bool tetris_collision_can_rotate_piece(const game_board_t *board, piece_type_t piece_type,
                                      int current_rotation, int new_rotation,
                                      int piece_x, int piece_y) {
    (void)current_rotation; // Basic rotation test doesn't use current rotation
    
    return tetris_collision_can_place_piece(board, piece_type, new_rotation,
                                           piece_x, piece_y);
}

bool tetris_collision_can_fall(const game_board_t *board, piece_type_t piece_type,
                              int piece_rotation, int piece_x, int piece_y) {
    return tetris_collision_can_move_piece(board, piece_type, piece_rotation,
                                          piece_x, piece_y, 0, 1);
}

int tetris_collision_find_drop_position(const game_board_t *board, piece_type_t piece_type,
                                       int piece_rotation, int piece_x, int start_y) {
    int y = start_y;
    
    // Move down until we hit something
    while (tetris_collision_can_place_piece(board, piece_type, piece_rotation, piece_x, y + 1)) {
        y++;
    }
    
    return y;
}

static bool test_wall_kick_position(const game_board_t *board, piece_type_t piece_type,
                                   int new_rotation, int test_x, int test_y) {
    return tetris_collision_can_place_piece(board, piece_type, new_rotation, test_x, test_y);
}

bool tetris_collision_wall_kick_test(const game_board_t *board, piece_type_t piece_type,
                                    int current_rotation, int new_rotation,
                                    int *piece_x, int *piece_y) {
    if (!board || !piece_x || !piece_y) {
        return false;
    }
    
    // Check if piece can be placed in current position first
    if (tetris_collision_can_place_piece(board, piece_type, new_rotation, *piece_x, *piece_y)) {
        return true;
    }
    
    // Determine which wall kick table to use
    const int (*wall_kick_table)[5][2];
    if (piece_type == PIECE_I) {
        wall_kick_table = WALL_KICK_OFFSETS_I;
    } else {
        wall_kick_table = WALL_KICK_OFFSETS_JLSTZ;
    }
    
    // Determine kick test index based on rotation transition
    int kick_test_index;
    if (new_rotation == (current_rotation + 1) % 4) {
        // Clockwise rotation
        kick_test_index = current_rotation;
    } else if (new_rotation == (current_rotation + 3) % 4) {
        // Counter-clockwise rotation - use reverse table
        kick_test_index = new_rotation;
    }
    
    // Try each wall kick offset
    for (int i = 0; i < 5; i++) {
        int test_x = *piece_x + wall_kick_table[kick_test_index][i][0];
        int test_y = *piece_y + wall_kick_table[kick_test_index][i][1];
        
        if (test_wall_kick_position(board, piece_type, new_rotation, test_x, test_y)) {
            // Update position if kick was successful
            *piece_x = test_x;
            *piece_y = test_y;
            return true;
        }
    }
    
    return false;
}
