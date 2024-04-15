#pragma once

#include <gui/view.h>
#include "../helpers/candog_custom_event.h"

typedef struct CandogScene2 CandogScene2;

typedef void (*CandogScene2Callback)(CandogCustomEvent event, void* context);

void candog_scene_2_set_callback(
    CandogScene2* instance,
    CandogScene2Callback callback,
    void* context);

CandogScene2* candog_scene_2_alloc();

void candog_scene_2_free(CandogScene2* candog_static);

View* candog_scene_2_get_view(CandogScene2* boilerpate_static);
