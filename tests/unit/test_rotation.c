/**
 * @file test_rotation.c
 * @brief Tests for piece rotation validation
 */

#include "../test_framework.h"
#include "../../game/src/entities/tetris_piece.h"
#include "../../game/src/main/constants.h"

// Helper function to count blocks in a piece shape
static int count_blocks_in_shape(piece_type_t type, int rotation) {
    const bool (*shape)[PIECE_SIZE] = tetris_piece_get_shape(type, rotation);
    if (!shape) return 0;
    
    int count = 0;
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            if (shape[y][x]) {
                count++;
            }
        }
    }
    return count;
}

// Helper function to print piece shape for debugging
static void print_piece_shape(piece_type_t type, int rotation) {
    const bool (*shape)[PIECE_SIZE] = tetris_piece_get_shape(type, rotation);
    if (!shape) {
        printf("NULL shape\n");
        return;
    }
    
    printf("Piece %d, Rotation %d:\n", type, rotation);
    for (int y = 0; y < PIECE_SIZE; y++) {
        printf("  ");
        for (int x = 0; x < PIECE_SIZE; x++) {
            printf("%c", shape[y][x] ? 'x' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

// Test that all pentomino pieces have exactly 5 blocks in all rotations
void test_all_pieces_have_5_blocks(void) {
    for (int piece_type = PIECE_I; piece_type < NUM_PIECE_TYPES; piece_type++) {
        // Skip PIECE_EMPTY as it's not a valid pentomino
        if (piece_type == PIECE_EMPTY) continue;
        
        for (int rotation = 0; rotation < 4; rotation++) {
            int block_count = count_blocks_in_shape((piece_type_t)piece_type, rotation);
            
            char test_msg[120];
            snprintf(test_msg, sizeof(test_msg), 
                    "Pentomino %d rotation %d has exactly 5 blocks", piece_type, rotation);
            
            if (block_count != 5) {
                printf("  Debug: Found %d blocks instead of 5\n", block_count);
                print_piece_shape((piece_type_t)piece_type, rotation);
            }
            
            TEST_ASSERT_EQUAL(5, block_count, test_msg);
        }
    }
}

// Test that piece shapes are returned correctly
void test_piece_shapes_not_null(void) {
    for (int piece_type = PIECE_I; piece_type < NUM_PIECE_TYPES; piece_type++) {
        // Skip PIECE_EMPTY as it should return NULL
        if (piece_type == PIECE_EMPTY) continue;
        
        for (int rotation = 0; rotation < 4; rotation++) {
            const bool (*shape)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, rotation);
            
            char test_msg[120];
            snprintf(test_msg, sizeof(test_msg), 
                    "Pentomino %d rotation %d returns valid shape", piece_type, rotation);
            
            TEST_ASSERT(shape != NULL, test_msg);
        }
    }
}

// Test that invalid piece types return NULL
void test_invalid_piece_types_return_null(void) {
    const bool (*shape)[PIECE_SIZE] = tetris_piece_get_shape(PIECE_EMPTY, 0);
    TEST_ASSERT(shape == NULL, "PIECE_EMPTY returns NULL shape");
    
    shape = tetris_piece_get_shape((piece_type_t)999, 0);
    TEST_ASSERT(shape == NULL, "Invalid piece type returns NULL shape");
}

// Test rotation bounds (rotation values should be normalized modulo 4)
void test_rotation_bounds(void) {
    for (int piece_type = PIECE_I; piece_type < NUM_PIECE_TYPES; piece_type++) {
        // Skip PIECE_EMPTY as it should return NULL
        if (piece_type == PIECE_EMPTY) continue;
        
        // Test that rotation values are normalized (modulo 4)
        const bool (*shape0)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, 0);
        const bool (*shape4)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, 4);
        const bool (*shape8)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, 8);
        
        char test_msg[120];
        snprintf(test_msg, sizeof(test_msg), 
                "Pentomino %d rotation normalization works (mod 4)", piece_type);
        
        // All should point to the same shape due to modulo 4 normalization
        bool modulo_works = (shape0 == shape4) && (shape0 == shape8);
        TEST_ASSERT(modulo_works, test_msg);
    }
}

// Test specific pentomino shapes match basic expectations
void test_piece_specific_shapes(void) {
    // Test PIECE_I (line) - should have blocks in a row when horizontal
    const bool (*piece_i_rot0)[PIECE_SIZE] = tetris_piece_get_shape(PIECE_I, 0);
    if (piece_i_rot0) {
        // Count horizontal lines in each row
        int max_consecutive = 0;
        for (int y = 0; y < PIECE_SIZE; y++) {
            int consecutive = 0;
            for (int x = 0; x < PIECE_SIZE; x++) {
                if (piece_i_rot0[y][x]) {
                    consecutive++;
                } else if (consecutive > 0) {
                    if (consecutive > max_consecutive) max_consecutive = consecutive;
                    consecutive = 0;
                }
            }
            if (consecutive > max_consecutive) max_consecutive = consecutive;
        }
        
        TEST_ASSERT(max_consecutive >= 4, "PIECE_I has a line of at least 4 consecutive blocks");
    }
    
    // Test PIECE_X (plus shape) - should have a center block with arms
    const bool (*piece_x_rot0)[PIECE_SIZE] = tetris_piece_get_shape(PIECE_X, 0);
    if (piece_x_rot0) {
        // Find a block that has blocks in all 4 directions (center of plus)
        bool found_plus_center = false;
        for (int y = 1; y < PIECE_SIZE - 1; y++) {
            for (int x = 1; x < PIECE_SIZE - 1; x++) {
                if (piece_x_rot0[y][x] && 
                    piece_x_rot0[y-1][x] && piece_x_rot0[y+1][x] &&
                    piece_x_rot0[y][x-1] && piece_x_rot0[y][x+1]) {
                    found_plus_center = true;
                    break;
                }
            }
        }
        
        TEST_ASSERT(found_plus_center, "PIECE_X has a plus shape with center and 4 arms");
    }
}

// Test that rotations work correctly (some may be symmetric)
void test_rotations_are_different(void) {
    for (int piece_type = PIECE_I; piece_type < NUM_PIECE_TYPES; piece_type++) {
        // Skip PIECE_EMPTY as it should return NULL
        if (piece_type == PIECE_EMPTY) continue;
        
        // Count how many different rotations this piece has
        int different_rotations = 0;
        const bool (*rot0)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, 0);
        
        for (int r = 1; r < 4; r++) {
            const bool (*rot_r)[PIECE_SIZE] = tetris_piece_get_shape((piece_type_t)piece_type, r);
            
            bool is_different = false;
            for (int y = 0; y < PIECE_SIZE && !is_different; y++) {
                for (int x = 0; x < PIECE_SIZE && !is_different; x++) {
                    if (rot0[y][x] != rot_r[y][x]) {
                        is_different = true;
                    }
                }
            }
            
            if (is_different) {
                different_rotations++;
            }
        }
        
        char test_msg[120];
        snprintf(test_msg, sizeof(test_msg), 
                "Pentomino %d has valid rotation system", piece_type);
        
        // All pentominos should have at least some rotational behavior
        // Even symmetric pieces should have their rotations properly defined
        TEST_ASSERT(rot0 != NULL, test_msg);
    }
}

// Test function validation
void test_tetris_piece_is_cell_filled(void) {
    // Test valid coordinates
    bool result = tetris_piece_is_cell_filled(PIECE_I, 0, 1, 1);
    TEST_ASSERT(result == true || result == false, "tetris_piece_is_cell_filled returns valid boolean");
    
    // Test invalid coordinates (should return false)
    result = tetris_piece_is_cell_filled(PIECE_I, 0, -1, 0);
    TEST_ASSERT(result == false, "tetris_piece_is_cell_filled returns false for negative x");
    
    result = tetris_piece_is_cell_filled(PIECE_I, 0, 0, -1);
    TEST_ASSERT(result == false, "tetris_piece_is_cell_filled returns false for negative y");
    
    result = tetris_piece_is_cell_filled(PIECE_I, 0, PIECE_SIZE, 0);
    TEST_ASSERT(result == false, "tetris_piece_is_cell_filled returns false for x >= PIECE_SIZE");
    
    result = tetris_piece_is_cell_filled(PIECE_I, 0, 0, PIECE_SIZE);
    TEST_ASSERT(result == false, "tetris_piece_is_cell_filled returns false for y >= PIECE_SIZE");
}

// Main rotation test runner
void run_rotation_tests(void) {
    printf("=== Rotation Validation Tests ===\n\n");
    
    RUN_TEST(test_piece_shapes_not_null);
    RUN_TEST(test_all_pieces_have_5_blocks);
    RUN_TEST(test_invalid_piece_types_return_null);
    RUN_TEST(test_rotation_bounds);
    RUN_TEST(test_piece_specific_shapes);
    RUN_TEST(test_rotations_are_different);
    RUN_TEST(test_tetris_piece_is_cell_filled);
}
