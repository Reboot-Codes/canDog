#pragma once

#include <gui/view.h>
#include "../helpers/candog_custom_event.h"

typedef struct CandogScene1 CandogScene1;

typedef void (*CandogScene1Callback)(CandogCustomEvent event, void* context);

void candog_scene_1_set_callback(
    CandogScene1* candog_scene_1,
    CandogScene1Callback callback,
    void* context);

View* candog_scene_1_get_view(CandogScene1* candog_static);

CandogScene1* candog_scene_1_alloc();

void candog_scene_1_free(CandogScene1* candog_static);