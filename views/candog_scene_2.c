#include "../candog.h"
#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/elements.h>
#include <dolphin/dolphin.h>
#include "../helpers/candog_haptic.h"
#include "../helpers/candog_speaker.h"
#include "../helpers/candog_led.h"

struct CandogScene2 {
    View* view;
    CandogScene2Callback callback;
    void* context;
};

typedef struct {
    int screen_text;
} CandogScene2Model;

char buttonText[11][14] = {
    "",
    "Press Up",
    "Press Down",
    "Press Left",
    "Press Right",
    "Press Ok",
    "Release Up",
    "Release Down",
    "Release Left",
    "Release Right",
    "Release Ok",
};

void candog_scene_2_set_callback(
    CandogScene2* instance,
    CandogScene2Callback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void candog_scene_2_draw(Canvas* canvas, CandogScene2Model* model) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 0, 10, AlignLeft, AlignTop, "Scene 2: Input Examples");
    canvas_set_font(canvas, FontSecondary);
    char* strInput = malloc(15);
    strcpy(strInput, buttonText[model->screen_text]);
    canvas_draw_str_aligned(canvas, 0, 22, AlignLeft, AlignTop, strInput);
    free(strInput);
}

static void candog_scene_2_model_init(CandogScene2Model* const model) {
    model->screen_text = 0;
}

bool candog_scene_2_input(InputEvent* event, void* context) {
    furi_assert(context);
    CandogScene2* instance = context;
    if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    UNUSED(model);
                    candog_stop_all_sound(instance->context);
                    instance->callback(CandogCustomEventScene2Back, instance->context);
                    candog_play_long_bump(instance->context);
                },
                true);
            break;
        case InputKeyUp:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 6;
                    candog_play_bad_bump(instance->context);
                    candog_stop_all_sound(instance->context);
                    candog_led_set_rgb(instance->context, 255, 0, 255);
                },
                true);
            break;
        case InputKeyDown:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 7;
                    candog_play_bad_bump(instance->context);
                    candog_stop_all_sound(instance->context);
                    candog_led_set_rgb(instance->context, 255, 255, 0);
                },
                true);
            break;
        case InputKeyLeft:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 8;
                    candog_play_bad_bump(instance->context);
                    candog_stop_all_sound(instance->context);
                    candog_led_set_rgb(instance->context, 0, 255, 255);
                },
                true);
            break;
        case InputKeyRight:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 9;
                    candog_play_bad_bump(instance->context);
                    candog_stop_all_sound(instance->context);
                    candog_led_set_rgb(instance->context, 255, 0, 0);
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 10;
                    candog_play_bad_bump(instance->context);
                    candog_stop_all_sound(instance->context);
                    candog_led_set_rgb(instance->context, 255, 255, 255);
                },
                true);
            break;
        case InputKeyMAX:
            break;
        }
    } else if(event->type == InputTypePress) {
        switch(event->key) {
        case InputKeyUp:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 1;
                    candog_play_happy_bump(instance->context);
                    candog_play_input_sound(instance->context);
                },
                true);
            break;
        case InputKeyDown:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 2;
                    candog_play_happy_bump(instance->context);
                    candog_play_input_sound(instance->context);
                },
                true);
            break;
        case InputKeyLeft:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 3;
                    candog_play_happy_bump(instance->context);
                    candog_play_input_sound(instance->context);
                },
                true);
            break;
        case InputKeyRight:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 4;
                    candog_play_happy_bump(instance->context);
                    candog_play_input_sound(instance->context);
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                instance->view,
                CandogScene2Model * model,
                {
                    model->screen_text = 5;
                    candog_play_happy_bump(instance->context);
                    candog_play_input_sound(instance->context);
                },
                true);
            break;
        case InputKeyBack:
        case InputKeyMAX:
            break;
        }
    }

    return true;
}

void candog_scene_2_exit(void* context) {
    furi_assert(context);
    Candog* app = context;
    candog_stop_all_sound(app);
    //candog_led_reset(app);
}

void candog_scene_2_enter(void* context) {
    furi_assert(context);
    dolphin_deed(DolphinDeedPluginStart);
}

CandogScene2* candog_scene_2_alloc() {
    CandogScene2* instance = malloc(sizeof(CandogScene2));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(CandogScene2Model));
    view_set_context(instance->view, instance);
    view_set_draw_callback(instance->view, (ViewDrawCallback)candog_scene_2_draw);
    view_set_input_callback(instance->view, candog_scene_2_input);
    //view_set_enter_callback(instance->view, candog_scene_2_enter);
    view_set_exit_callback(instance->view, candog_scene_2_exit);

    with_view_model(
        instance->view, CandogScene2Model * model, { candog_scene_2_model_init(model); }, true);

    return instance;
}

void candog_scene_2_free(CandogScene2* instance) {
    furi_assert(instance);

    view_free(instance->view);
    free(instance);
}

View* candog_scene_2_get_view(CandogScene2* instance) {
    furi_assert(instance);

    return instance->view;
}
