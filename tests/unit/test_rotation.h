/**
 * @file test_rotation.h
 * @brief Header for rotation validation tests
 */

#ifndef TEST_ROTATION_H
#define TEST_ROTATION_H

// Test function declarations
void test_all_pieces_have_5_blocks(void);
void test_piece_shapes_not_null(void);
void test_invalid_piece_types_return_null(void);
void test_rotation_bounds(void);
void test_piece_specific_shapes(void);
void test_rotations_are_different(void);
void test_tetris_piece_is_cell_filled(void);

// Main test runner function
void run_rotation_tests(void);

#endif // TEST_ROTATION_H
