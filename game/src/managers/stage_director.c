/**
 * @file stage_director.c
 * @brief Stage director implementation for managing game stages
 */

#include "stage_director.h"
#include "intro_stage.h"
#include "menu_stage.h"
#include "playing_stage.h"
#include "game_over_stage.h"
#include <string.h>

/**
 * Register a stage with the director
 */
static bool register_stage(stage_director_ptr director, game_screen_t screen_type, 
                          stage_ptr (*create_fn)(void)) {
    if (!director || director->stage_count >= MAX_STAGES) {
        return false;
    }
    
    director->stages[director->stage_count].screen_type = screen_type;
    director->stages[director->stage_count].create_stage_fn = create_fn;
    director->stages[director->stage_count].instance = NULL;
    director->stage_count++;
    
    return true;
}

/**
 * Find stage by screen type
 */
static stage_registry_entry_t* find_stage_entry(stage_director_ptr director, game_screen_t screen_type) {
    if (!director) {
        return NULL;
    }
    
    for (size_t i = 0; i < director->stage_count; i++) {
        if (director->stages[i].screen_type == screen_type) {
            return &director->stages[i];
        }
    }
    
    return NULL;
}

/**
 * Get or create stage instance
 */
static stage_ptr get_stage_instance(stage_registry_entry_t* entry) {
    if (!entry) {
        return NULL;
    }
    
    if (!entry->instance && entry->create_stage_fn) {
        entry->instance = entry->create_stage_fn();
    }
    
    return entry->instance;
}

bool stage_director_init(stage_director_ptr director, game_ptr game) {
    if (!director || !game) {
        return false;
    }
    
    // Initialize director state
    director->stage_count = 0;
    director->current_stage = NULL;
    director->previous_screen = SCREEN_INTRO;
    
    // Register all stages
    if (!register_stage(director, SCREEN_INTRO, create_intro_stage_instance)) {
        return false;
    }
    
    if (!register_stage(director, SCREEN_MENU, create_menu_stage_instance)) {
        return false;
    }
    
    if (!register_stage(director, SCREEN_PLAYING, create_playing_stage_instance)) {
        return false;
    }
    
    if (!register_stage(director, SCREEN_GAME_OVER, create_game_over_stage_instance)) {
        return false;
    }
    
    // Initialize with intro stage
    stage_registry_entry_t* intro_entry = find_stage_entry(director, SCREEN_INTRO);
    if (!intro_entry) {
        return false;
    }
    
    director->current_stage = get_stage_instance(intro_entry);
    if (!director->current_stage) {
        return false;
    }
    
    // Initialize the stage
    if (director->current_stage->init) {
        director->current_stage->init(director->current_stage, game);
    }
    
    return true;
}

game_stage_action_t stage_director_update(stage_director_ptr director, game_ptr game) {
    if (!director || !game || !director->current_stage) {
        return QUIT;
    }
    
    // Update current stage
    game_stage_action_t action = PROGRESS;
    if (director->current_stage->update) {
        action = director->current_stage->update(director->current_stage);
    }
    
    // Check for stage transition
    if (game->current_screen != director->previous_screen) {
        // Find new stage
        stage_registry_entry_t* new_entry = find_stage_entry(director, game->current_screen);
        if (!new_entry) {
            // Unknown screen type, stay on current stage
            game->current_screen = director->previous_screen;
            return action;
        }
        
        // Get new stage instance
        stage_ptr new_stage = get_stage_instance(new_entry);
        if (!new_stage) {
            // Failed to create new stage, stay on current
            game->current_screen = director->previous_screen;
            return action;
        }
        
        // Cleanup current stage if it's different
        if (director->current_stage != new_stage && director->current_stage->cleanup) {
            director->current_stage->cleanup(director->current_stage);
        }
        
        // Switch to new stage
        director->current_stage = new_stage;
        director->previous_screen = game->current_screen;
        
        // Initialize new stage
        if (director->current_stage->init) {
            director->current_stage->init(director->current_stage, game);
        }
    }
    
    return action;
}

void stage_director_cleanup(stage_director_ptr director) {
    if (!director) {
        return;
    }
    
    // Cleanup all stage instances
    for (size_t i = 0; i < director->stage_count; i++) {
        if (director->stages[i].instance) {
            destroy_stage(director->stages[i].instance);
            director->stages[i].instance = NULL;
        }
    }
    
    director->stage_count = 0;
    director->current_stage = NULL;
}
