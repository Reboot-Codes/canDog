#include "../candog.h"
#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/elements.h>
#include <dolphin/dolphin.h>

struct CandogScene1 {
    View* view;
    CandogScene1Callback callback;
    void* context;
};

typedef struct {
    int some_value;
} CandogScene1Model;

void candog_scene_1_set_callback(
    CandogScene1* instance,
    CandogScene1Callback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void candog_scene_1_draw(Canvas* canvas, CandogScene1Model* model) {
    UNUSED(model);
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 0, 10, AlignLeft, AlignTop, "This is Scene 1");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 0, 22, AlignLeft, AlignTop, "An empty scene to be");
    canvas_draw_str_aligned(canvas, 0, 32, AlignLeft, AlignTop, "used as candog");
}

static void candog_scene_1_model_init(CandogScene1Model* const model) {
    model->some_value = 1;
}

bool candog_scene_1_input(InputEvent* event, void* context) {
    furi_assert(context);
    CandogScene1* instance = context;
    if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                instance->view,
                CandogScene1Model * model,
                {
                    UNUSED(model);
                    instance->callback(CandogCustomEventScene1Back, instance->context);
                },
                true);
            break;
        case InputKeyLeft:
        case InputKeyRight:
        case InputKeyUp:
        case InputKeyDown:
        case InputKeyOk:
            with_view_model(instance->view, CandogScene1Model * model, { UNUSED(model); }, true);
            break;
        case InputKeyMAX:
            break;
        }
    }
    return true;
}

void candog_scene_1_exit(void* context) {
    furi_assert(context);
}

void candog_scene_1_enter(void* context) {
    furi_assert(context);
    CandogScene1* instance = (CandogScene1*)context;
    with_view_model(
        instance->view, CandogScene1Model * model, { candog_scene_1_model_init(model); }, true);
}

CandogScene1* candog_scene_1_alloc() {
    CandogScene1* instance = malloc(sizeof(CandogScene1));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(CandogScene1Model));
    view_set_context(instance->view, instance); // furi_assert crashes in events without this
    view_set_draw_callback(instance->view, (ViewDrawCallback)candog_scene_1_draw);
    view_set_input_callback(instance->view, candog_scene_1_input);
    view_set_enter_callback(instance->view, candog_scene_1_enter);
    view_set_exit_callback(instance->view, candog_scene_1_exit);

    with_view_model(
        instance->view, CandogScene1Model * model, { candog_scene_1_model_init(model); }, true);

    return instance;
}

void candog_scene_1_free(CandogScene1* instance) {
    furi_assert(instance);

    with_view_model(instance->view, CandogScene1Model * model, { UNUSED(model); }, true);
    view_free(instance->view);
    free(instance);
}

View* candog_scene_1_get_view(CandogScene1* instance) {
    furi_assert(instance);
    return instance->view;
}
