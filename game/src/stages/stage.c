/**
 * @file stage.c
 * @brief Common stage operations implementation
 */

#include "stage.h"
#include <stdlib.h>

void destroy_stage(stage_ptr stage) {
    if (!stage) {
        return;
    }
    
    if (stage->cleanup) {
        stage->cleanup(stage);
    }
    
    free(stage);
}
