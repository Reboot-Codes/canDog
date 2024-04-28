#include "candog_view_progress.h"
#include <gui/elements.h>

struct CanDogProgressView {
    View* view;
    CanDogProgressViewCallback callback;
    void* cb_ctx;
};

typedef enum {
    CanDogProgressViewTypeRead,
    CanDogProgressViewTypeVerify,
    CanDogProgressViewTypeWrite,
    CanDogProgressViewTypeUnknown
} CanDogProgressViewType;

typedef struct {
    size_t chip_size;
    size_t file_size;
    size_t blocks_written;
    size_t block_size;
    float progress;
    CanDogProgressViewType view_type;
} CanDogProgressViewModel;

View* candog_view_progress_get_view(CanDogProgressView* app) {
    return app->view;
}

static void candog_view_progress_draw_progress(Canvas* canvas, float progress) {
    FuriString* progress_str = furi_string_alloc();
    if (progress > 1.0) progress = 1.0;
    furi_string_printf(progress_str, "%d %%", (int)(progress * 100));
    elements_progress_bar(canvas, 13, 35, 100, progress);
    canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignTop, furi_string_get_cstr(progress_str));
    furi_string_free(progress_str);
}

static void
candog_view_progress_read_draw_callback(Canvas* canvas, CanDogProgressViewModel* model) {
    canvas_draw_str_aligned(canvas, 64, 4, AlignCenter, AlignTop, "Reading dump");
    candog_view_progress_draw_progress(canvas, model->progress);
    elements_button_left(canvas, "Cancel");
}

static void
candog_view_progress_draw_size_warning(Canvas* canvas, CanDogProgressViewModel* model) {
    if (model->file_size > model->chip_size) {
        canvas_draw_str_aligned(canvas, 64, 13, AlignCenter, AlignTop, "Size clamped to chip!");
    }
    if (model->chip_size > model->file_size) {
        canvas_draw_str_aligned(canvas, 64, 13, AlignCenter, AlignTop, "Size clamped to file!");
    }
}

static void
candog_view_progress_verify_draw_callback(Canvas* canvas, CanDogProgressViewModel* model) {
    canvas_draw_str_aligned(canvas, 64, 2, AlignCenter, AlignTop, "Verifying dump");
    candog_view_progress_draw_size_warning(canvas, model);
    candog_view_progress_draw_progress(canvas, model->progress);
    elements_button_center(canvas, "Skip");
}

static void
candog_view_progress_write_draw_callback(Canvas* canvas, CanDogProgressViewModel* model) {
    canvas_draw_str_aligned(canvas, 64, 4, AlignCenter, AlignTop, "Writing dump");
    candog_view_progress_draw_size_warning(canvas, model);
    candog_view_progress_draw_progress(canvas, model->progress);
    elements_button_left(canvas, "Cancel");
}

static void candog_view_progress_draw_callback(Canvas* canvas, void* context) {
    CanDogProgressViewModel* model = context;
    CanDogProgressViewType view_type = model->view_type;
    if (view_type == CanDogProgressViewTypeRead) {
        candog_view_progress_read_draw_callback(canvas, model);
    }
    else if (view_type == CanDogProgressViewTypeVerify) {
        candog_view_progress_verify_draw_callback(canvas, model);
    }
    else if (view_type == CanDogProgressViewTypeWrite) {
        candog_view_progress_write_draw_callback(canvas, model);
    }
}

static bool
candog_view_progress_read_write_input_callback(InputEvent* event, CanDogProgressView* app) {
    bool success = false;
    if (event->type == InputTypeShort && event->key == InputKeyLeft) {
        if (app->callback) {
            app->callback(app->cb_ctx);
        }
        success = true;
    }
    return success;
}

static bool
candog_view_progress_verify_input_callback(InputEvent* event, CanDogProgressView* app) {
    bool success = false;
    if (event->type == InputTypeShort && event->key == InputKeyOk) {
        if (app->callback) {
            app->callback(app->cb_ctx);
        }
        success = true;
    }
    return success;
}

static bool candog_view_progress_input_callback(InputEvent* event, void* context) {
    CanDogProgressView* app = context;
    bool success = false;
    CanDogProgressViewType view_type;
    with_view_model(
        app->view, CanDogProgressViewModel * model, { view_type = model->view_type; }, true);
    if (view_type == CanDogProgressViewTypeRead) {
        success = candog_view_progress_read_write_input_callback(event, app);
    }
    else if (view_type == CanDogProgressViewTypeVerify) {
        success = candog_view_progress_verify_input_callback(event, app);
    }
    else if (view_type == CanDogProgressViewTypeWrite) {
        success = candog_view_progress_read_write_input_callback(event, app);
    }
    return success;
}

CanDogProgressView* candog_view_progress_alloc() {
    CanDogProgressView* app = malloc(sizeof(CanDogProgressView));
    app->view = view_alloc();
    view_allocate_model(app->view, ViewModelTypeLocking, sizeof(CanDogProgressViewModel));
    view_set_context(app->view, app);
    view_set_draw_callback(app->view, candog_view_progress_draw_callback);
    view_set_input_callback(app->view, candog_view_progress_input_callback);
    candog_view_progress_reset(app);
    return app;
}

void candog_view_progress_free(CanDogProgressView* app) {
    view_free(app->view);
    free(app);
}

void candog_view_progress_set_read_callback(
    CanDogProgressView* app,
    CanDogProgressViewCallback callback,
    void* cb_ctx) {
    app->callback = callback;
    app->cb_ctx = cb_ctx;
    with_view_model(app->view, CanDogProgressViewModel * model, { model->view_type = CanDogProgressViewTypeRead; }, true);
}

void candog_view_progress_set_verify_callback(
    CanDogProgressView* app,
    CanDogProgressViewCallback callback,
    void* cb_ctx) {
    app->callback = callback;
    app->cb_ctx = cb_ctx;
    with_view_model(app->view, CanDogProgressViewModel * model, { model->view_type = CanDogProgressViewTypeVerify; }, true);
}

void candog_view_progress_set_write_callback(
    CanDogProgressView* app,
    CanDogProgressViewCallback callback,
    void* cb_ctx) {
    app->callback = callback;
    app->cb_ctx = cb_ctx;
    with_view_model(app->view, CanDogProgressViewModel * model, { model->view_type = CanDogProgressViewTypeWrite; }, true);
}

void candog_view_progress_set_chip_size(CanDogProgressView* app, size_t chip_size) {
    with_view_model(
        app->view, CanDogProgressViewModel * model, { model->chip_size = chip_size; }, true);
}

void candog_view_progress_set_file_size(CanDogProgressView* app, size_t file_size) {
    with_view_model(
        app->view, CanDogProgressViewModel * model, { model->file_size = file_size; }, true);
}

void candog_view_progress_set_block_size(CanDogProgressView* app, size_t block_size) {
    with_view_model(
        app->view, CanDogProgressViewModel * model, { model->block_size = block_size; }, true);
}

static size_t candog_view_progress_set_total_size(CanDogProgressViewModel* model) {
    size_t total_size = model->chip_size;
    if ((model->chip_size > model->file_size) && model->view_type != CanDogProgressViewTypeRead) {
        total_size = model->file_size;
    }
    return total_size;
}

void candog_view_progress_inc_progress(CanDogProgressView* app) {
    with_view_model(
        app->view,
        CanDogProgressViewModel * model,
        {
            size_t total_size = candog_view_progress_set_total_size(model);
            if (total_size == 0) total_size = 1;
            model->blocks_written++;
            model->progress = ((float)model->block_size * (float)model->blocks_written) / ((float)total_size);
        },
        true);
}

void candog_view_progress_reset(CanDogProgressView* app) {
    with_view_model(
        app->view,
        CanDogProgressViewModel * model,
        {
            model->blocks_written = 0;
            model->block_size = 0;
            model->chip_size = 0;
            model->file_size = 0;
            model->progress = 0;
            model->view_type = CanDogProgressViewTypeUnknown;
        },
        true);
}
