/**
 * @file pentomino.c
 * @brief Implementation of pentomino coordinate system
 */

#include "pentomino.h"
#include <string.h>

// Pentomino definitions with exact predefined rotations
static const pentomino_def_t pentomino_defs[NUM_PIECE_TYPES] = {
    [PIECE_I] = {
        .rotations = {
            { {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0} },  // 0°
            { {0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2} },  // 90°
            { {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0} },  // 180° (same as 0°)
            { {0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2} }   // 270° (same as 90°)
        },
        .unique_rotations = 2
    },
    [PIECE_X] = {
        .rotations = {
            { {0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1} },  // 0°
            { {0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1} },  // 90° (same)
            { {0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1} },  // 180° (same)
            { {0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1} }   // 270° (same)
        },
        .unique_rotations = 1
    },
    [PIECE_T] = {
        .rotations = {
            { {-1, 0}, {0, 0}, {1, 0}, {0, 1}, {0, 2} },   // 0°
            { {0, -1}, {0, 0}, {0, 1}, {-1, 0}, {-2, 0} }, // 90°
            { {1, 0}, {0, 0}, {-1, 0}, {0, -1}, {0, -2} }, // 180°
            { {0, 1}, {0, 0}, {0, -1}, {1, 0}, {2, 0} }    // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_U] = {
        .rotations = {
            { {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} },  // 0°
            { {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1} }, // 90°
            { {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1} }, // 180°
            { {0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1} }   // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_V] = {
        .rotations = {
            { {-1, 0}, {-1, 1}, {-1, 2}, {0, 2}, {1, 2} }, // 0°
            { {0, -1}, {-1, -1}, {-2, -1}, {-2, 0}, {-2, 1} }, // 90°
            { {1, 0}, {1, -1}, {1, -2}, {0, -2}, {-1, -2} }, // 180°
            { {0, 1}, {1, 1}, {2, 1}, {2, 0}, {2, -1} }    // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_W] = {
        .rotations = {
            { {-1, 0}, {-1, 1}, {0, 1}, {0, 2}, {1, 2} },  // 0°
            { {0, -1}, {-1, -1}, {-1, 0}, {-2, 0}, {-2, 1} }, // 90°
            { {1, 0}, {1, -1}, {0, -1}, {0, -2}, {-1, -2} }, // 180°
            { {0, 1}, {1, 1}, {1, 0}, {2, 0}, {2, -1} }    // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_L] = {
        .rotations = {
            { {0, -2}, {0, -1}, {0, 0}, {0, 1}, {1, 1} },  // 0°
            { {2, 0}, {1, 0}, {0, 0}, {-1, 0}, {-1, 1} },  // 90°
            { {0, 2}, {0, 1}, {0, 0}, {0, -1}, {-1, -1} }, // 180°
            { {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {1, -1} } // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_L_MIRROR] = {
        .rotations = {
            { {0, -2}, {0, -1}, {0, 0}, {0, 1}, {-1, 1} }, // 0°
            { {2, 0}, {1, 0}, {0, 0}, {-1, 0}, {-1, -1} }, // 90°
            { {0, 2}, {0, 1}, {0, 0}, {0, -1}, {1, -1} },  // 180°
            { {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {1, 1} }  // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_N] = {
        .rotations = {
            { {1, -1}, {0, 0}, {1, 0}, {0, 1}, {0, 2} },   // 0°
            { {1, 1}, {0, 0}, {0, 1}, {-1, 0}, {-2, 0} },  // 90°
            { {-1, 1}, {0, 0}, {-1, 0}, {0, -1}, {0, -2} }, // 180°
            { {-1, -1}, {0, 0}, {0, -1}, {1, 0}, {2, 0} }  // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_N_MIRROR] = {
        .rotations = {
            { {-1, -1}, {-1, 0}, {0, 0}, {0, 1}, {0, 2} }, // 0°
            { {1, -1}, {0, -1}, {0, 0}, {-1, 0}, {-2, 0} }, // 90°
            { {1, 1}, {1, 0}, {0, 0}, {0, -1}, {0, -2} },  // 180°
            { {-1, 1}, {0, 1}, {0, 0}, {1, 0}, {2, 0} }   // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_Y] = {
        .rotations = {
            { {0, -2}, {0, -1}, {0, 0}, {1, 0}, {0, 1} },  // 0°
            { {2, 0}, {1, 0}, {0, 0}, {0, 1}, {-1, 0} },   // 90°
            { {0, 2}, {0, 1}, {0, 0}, {-1, 0}, {0, -1} },  // 180°
            { {-2, 0}, {-1, 0}, {0, 0}, {0, -1}, {1, 0} } // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_Y_MIRROR] = {
        .rotations = {
            { {0, -2}, {0, -1}, {-1, 0}, {0, 0}, {0, 1} }, // 0°
            { {2, 0}, {1, 0}, {0, -1}, {0, 0}, {-1, 0} },  // 90°
            { {0, 2}, {0, 1}, {1, 0}, {0, 0}, {0, -1} },   // 180°
            { {-2, 0}, {-1, 0}, {0, 1}, {0, 0}, {1, 0} }  // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_P] = {
        .rotations = {
            { {0, -1}, {1, -1}, {0, 0}, {1, 0}, {0, 1} },  // 0°
            { {1, 0}, {1, 1}, {0, 0}, {0, 1}, {-1, 0} },   // 90°
            { {0, 1}, {-1, 1}, {0, 0}, {-1, 0}, {0, -1} }, // 180°
            { {-1, 0}, {-1, -1}, {0, 0}, {0, -1}, {1, 0} } // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_P_MIRROR] = {
        .rotations = {
            { {-1, -1}, {0, -1}, {-1, 0}, {0, 0}, {0, 1} }, // 0°
            { {1, -1}, {1, 0}, {0, -1}, {0, 0}, {-1, 0} },  // 90°
            { {1, 1}, {0, 1}, {1, 0}, {0, 0}, {0, -1} },   // 180°
            { {-1, 1}, {-1, 0}, {0, 1}, {0, 0}, {1, 0} }   // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_F] = {
        .rotations = {
            { {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {0, 1} }, // 0°
            { {1, 0}, {1, 1}, {0, -1}, {0, 0}, {-1, 0} },  // 90°
            { {0, 1}, {-1, 1}, {1, 0}, {0, 0}, {0, -1} },  // 180°
            { {-1, 0}, {-1, -1}, {0, 1}, {0, 0}, {1, 0} } // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_F_MIRROR] = {
        .rotations = {
            { {-1, -1}, {0, -1}, {0, 0}, {1, 0}, {0, 1} }, // 0°
            { {1, -1}, {1, 0}, {0, 0}, {0, 1}, {-1, 0} },  // 90°
            { {1, 1}, {0, 1}, {0, 0}, {-1, 0}, {0, -1} },  // 180°
            { {-1, 1}, {-1, 0}, {0, 0}, {0, -1}, {1, 0} } // 270°
        },
        .unique_rotations = 4
    },
    [PIECE_Z] = {
        .rotations = {
            { {-1, -1}, {0, -1}, {0, 0}, {0, 1}, {1, 1} }, // 0°
            { {1, -1}, {1, 0}, {0, 0}, {-1, 0}, {-1, 1} }, // 90°
            { {-1, -1}, {0, -1}, {0, 0}, {0, 1}, {1, 1} }, // 180° (same as 0°)
            { {1, -1}, {1, 0}, {0, 0}, {-1, 0}, {-1, 1} }  // 270° (same as 90°)
        },
        .unique_rotations = 2
    },
    [PIECE_Z_MIRROR] = {
        .rotations = {
            { {1, -1}, {0, -1}, {0, 0}, {0, 1}, {-1, 1} }, // 0°
            { {1, 1}, {1, 0}, {0, 0}, {-1, 0}, {-1, -1} }, // 90°
            { {1, -1}, {0, -1}, {0, 0}, {0, 1}, {-1, 1} }, // 180° (same as 0°)
            { {1, 1}, {1, 0}, {0, 0}, {-1, 0}, {-1, -1} }  // 270° (same as 90°)
        },
        .unique_rotations = 2
    }
};

const pentomino_def_t* get_pentomino_def(piece_type_t type) {
    if (type >= NUM_PIECE_TYPES || type == PIECE_EMPTY) {
        return NULL;
    }
    return &pentomino_defs[type];
}

void coords_to_grid(const coord_t coords[5], bool grid[PIECE_SIZE][PIECE_SIZE]) {
    // Clear the grid first
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            grid[y][x] = false;
        }
    }
    
    // Convert coordinates to grid positions
    // Grid center is at (2, 2) in a 5x5 grid
    const int center_x = 2;
    const int center_y = 2;
    
    for (int i = 0; i < 5; i++) {
        int grid_x = coords[i].x + center_x;
        int grid_y = coords[i].y + center_y;
        
        // Ensure coordinates are within bounds
        if (grid_x >= 0 && grid_x < PIECE_SIZE && grid_y >= 0 && grid_y < PIECE_SIZE) {
            grid[grid_y][grid_x] = true;
        }
    }
}

void rotate_coords_clockwise(const coord_t input[5], coord_t output[5]) {
    for (int i = 0; i < 5; i++) {
        // 90-degree clockwise rotation: (x, y) -> (y, -x)
        output[i].x = input[i].y;
        output[i].y = -input[i].x;
    }
}

void generate_pentomino_rotations(piece_type_t type, bool rotations[4][PIECE_SIZE][PIECE_SIZE]) {
    const pentomino_def_t* def = get_pentomino_def(type);
    if (!def) {
        // Clear all rotations for invalid types
        for (int r = 0; r < 4; r++) {
            for (int y = 0; y < PIECE_SIZE; y++) {
                for (int x = 0; x < PIECE_SIZE; x++) {
                    rotations[r][y][x] = false;
                }
            }
        }
        return;
    }
    
    // Use predefined rotations directly
    for (int rotation = 0; rotation < 4; rotation++) {
        coords_to_grid(def->rotations[rotation], rotations[rotation]);
    }
}
