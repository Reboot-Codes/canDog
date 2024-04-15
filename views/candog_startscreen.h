#pragma once

#include <gui/view.h>
#include "../helpers/candog_custom_event.h"

typedef struct CandogStartscreen CandogStartscreen;

typedef void (*CandogStartscreenCallback)(CandogCustomEvent event, void* context);

void candog_startscreen_set_callback(
    CandogStartscreen* candog_startscreen,
    CandogStartscreenCallback callback,
    void* context);

View* candog_startscreen_get_view(CandogStartscreen* candog_static);

CandogStartscreen* candog_startscreen_alloc();

void candog_startscreen_free(CandogStartscreen* candog_static);