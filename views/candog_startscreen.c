#include "../candog.h"
#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/elements.h>

struct CandogStartscreen {
    View* view;
    CandogStartscreenCallback callback;
    void* context;
};

typedef struct {
    int some_value;
} CandogStartscreenModel;

void candog_startscreen_set_callback(
    CandogStartscreen* instance,
    CandogStartscreenCallback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void candog_startscreen_draw(Canvas* canvas, CandogStartscreenModel* model) {
    UNUSED(model);
    char buffer[64];
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignTop, "Start Screen");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 22, AlignCenter, AlignTop, "Explain your app");
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, "on this screen");
    snprintf(buffer, sizeof(buffer), "Version: %s", CANDOG_VERSION);
    canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignTop, buffer);
    elements_button_center(canvas, "Start");
}

static void candog_startscreen_model_init(CandogStartscreenModel* const model) {
    model->some_value = 1;
}

bool candog_startscreen_input(InputEvent* event, void* context) {
    furi_assert(context);
    CandogStartscreen* instance = context;
    if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                instance->view,
                CandogStartscreenModel * model,
                {
                    UNUSED(model);
                    instance->callback(CandogCustomEventStartscreenBack, instance->context);
                },
                true);
            break;
        case InputKeyLeft:
        case InputKeyRight:
        case InputKeyUp:
        case InputKeyDown:
        case InputKeyOk:
            with_view_model(
                instance->view,
                CandogStartscreenModel * model,
                {
                    UNUSED(model);
                    instance->callback(CandogCustomEventStartscreenOk, instance->context);
                },
                true);
            break;
        case InputKeyMAX:
            break;
        }
    }
    return true;
}

void candog_startscreen_exit(void* context) {
    furi_assert(context);
}

void candog_startscreen_enter(void* context) {
    furi_assert(context);
    CandogStartscreen* instance = (CandogStartscreen*)context;
    with_view_model(
        instance->view,
        CandogStartscreenModel * model,
        { candog_startscreen_model_init(model); },
        true);
}

CandogStartscreen* candog_startscreen_alloc() {
    CandogStartscreen* instance = malloc(sizeof(CandogStartscreen));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(CandogStartscreenModel));
    view_set_context(instance->view, instance); // furi_assert crashes in events without this
    view_set_draw_callback(instance->view, (ViewDrawCallback)candog_startscreen_draw);
    view_set_input_callback(instance->view, candog_startscreen_input);
    //view_set_enter_callback(instance->view, candog_startscreen_enter);
    //view_set_exit_callback(instance->view, candog_startscreen_exit);

    with_view_model(
        instance->view,
        CandogStartscreenModel * model,
        { candog_startscreen_model_init(model); },
        true);

    return instance;
}

void candog_startscreen_free(CandogStartscreen* instance) {
    furi_assert(instance);

    with_view_model(instance->view, CandogStartscreenModel * model, { UNUSED(model); }, true);
    view_free(instance->view);
    free(instance);
}

View* candog_startscreen_get_view(CandogStartscreen* instance) {
    furi_assert(instance);
    return instance->view;
}
