/**
 * @file resource_manager.c
 * @brief Coordinated BlockTris game resource loading and cleanup implementation
 */

#include "resource_manager.h"
#include "constants.h"
#include "graphics_context.h"
#include "audio.h"
#include "drawing_primitives.h"
#include "arcade_font.h"
#include "texture.h"
#include <stdio.h>

// Custom graphics context initialization that gets screen info first, calculates dimensions, then creates properly sized window
bool initialize_game_graphics(game_ptr game) {
    // First initialize just the graphics subsystems to get screen info
    if (!initialize_graphics_subsystems()) {
        return false;
    }
    
    // Get display configuration to determine screen size
    int display = 0, display_mode = 0;
    SDL_DisplayMode sdl_display_mode;
    if (!validate_display_configuration(&display, &display_mode, &sdl_display_mode)) {
        return false;
    }
    
    // Calculate our desired window dimensions
    calculate_window_dimensions(sdl_display_mode.w, sdl_display_mode.h);
    
    // Create window with our calculated dimensions
    game->graphics_context.screen_width = sdl_display_mode.w;
    game->graphics_context.screen_height = sdl_display_mode.h;
    game->graphics_context.screen_center = point(sdl_display_mode.w / 2, sdl_display_mode.h / 2);
    
    game->graphics_context.window = create_application_window("Blocktris", WINDOWED, display,
                                                               window_width, window_height, &sdl_display_mode);
    if (!game->graphics_context.window) {
        return false;
    }
    
    game->graphics_context.renderer = create_application_renderer(game->graphics_context.window, false);
    if (!game->graphics_context.renderer) {
        return false;
    }
    
    return true;
}

bool load_game_resources(game_ptr game) {
    if (!game) {
        return false;
    }
    
    // Initialize graphics with custom window size
    if (!initialize_game_graphics(game)) {
        printf("Failed to initialize graphics context\n");
        return false;
    }
    
    // Initialize audio context  
    game->audio_context = init_audio_context(8, 64); // 8 sounds max, half volume
    
    // Initialize drawing primitives
    init_circle_lookup();
    
    // Load arcade font
    game->arcade_font = load_arcade_font(&game->graphics_context);
    if (!game->arcade_font.bitmap_font.texture.texture) {
        printf("Failed to load arcade font\n");
        return false;
    }
    
    // Load background texture
    game->background_texture = load_texture(game->graphics_context.renderer, "game/assets/images/background.jpg");
    if (!game->background_texture.texture) {
        printf("Failed to load background image\n");
        return false;
    }
    
    return true;
}

void free_game_resources(game_ptr game) {
    if (!game) {
        return;
    }
    
    // Cleanup font
    free_arcade_font(&game->arcade_font);
    
    // Cleanup background texture
    free_texture(&game->background_texture);
    
    // Cleanup audio
    terminate_audio_context(&game->audio_context);
    
    // Cleanup graphics
    terminate_graphics_context(&game->graphics_context);
}
