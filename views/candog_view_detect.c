#include "candog_view_detect.h"
#include "candog_icons.h"
#include <gui/elements.h>

struct CanDogDetectView {
    View* view;
    IconAnimation* icon;
    CanDogDetectViewCallback callback;
    void* cb_ctx;
};

typedef struct {
    IconAnimation* icon;
} CanDogDetectViewModel;

View* candog_view_detect_get_view(CanDogDetectView* app) {
    return app->view;
}

static void candog_view_detect_draw_callback(Canvas* canvas, void* context) {
    CanDogDetectViewModel* model = context;
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_icon_animation(canvas, 0, 0, model->icon);
    canvas_draw_str_aligned(canvas, 64, 26, AlignLeft, AlignCenter, "Detecting");
    canvas_draw_str_aligned(canvas, 64, 36, AlignLeft, AlignCenter, "SPI chip...");
}

static void candog_view_detect_enter_callback(void* context) {
    CanDogDetectView* app = context;
    with_view_model(
        app->view, CanDogDetectViewModel * model, { icon_animation_start(model->icon); }, false);
}

static void candog_view_detect_exit_callback(void* context) {
    CanDogDetectView* app = context;
    with_view_model(
        app->view, CanDogDetectViewModel * model, { icon_animation_stop(model->icon); }, false);
}

CanDogDetectView* candog_view_detect_alloc() {
    CanDogDetectView* app = malloc(sizeof(CanDogDetectView));
    app->view = view_alloc();
    view_set_context(app->view, app);
    view_allocate_model(app->view, ViewModelTypeLocking, sizeof(CanDogDetectViewModel));
    with_view_model(
        app->view,
        CanDogDetectViewModel * model,
        {
            model->icon = icon_animation_alloc(&A_ChipLooking_64x64);
            view_tie_icon_animation(app->view, model->icon);
        },
        false);
    view_set_draw_callback(app->view, candog_view_detect_draw_callback);
    view_set_enter_callback(app->view, candog_view_detect_enter_callback);
    view_set_exit_callback(app->view, candog_view_detect_exit_callback);
    return app;
}

void candog_view_detect_free(CanDogDetectView* app) {
    with_view_model(app->view, CanDogDetectViewModel * model, { icon_animation_free(model->icon); }, false);
    view_free(app->view);
    free(app);
}
