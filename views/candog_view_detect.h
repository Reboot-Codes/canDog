#pragma once
#include <gui/view.h>

typedef struct CanDogDetectView CanDogDetectView;
typedef void (*CanDogDetectViewCallback)(void* context);

View* candog_view_detect_get_view(CanDogDetectView* app);
CanDogDetectView* candog_view_detect_alloc();
void candog_view_detect_free(CanDogDetectView* app);
