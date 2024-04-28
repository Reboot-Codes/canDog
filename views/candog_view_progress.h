#pragma once
#include <gui/view.h>

typedef struct CanDogProgressView CanDogProgressView;
typedef void (*CanDogProgressViewCallback)(void* context);

View* candog_view_progress_get_view(CanDogProgressView* app);
CanDogProgressView* candog_view_progress_alloc();
void candog_view_progress_free(CanDogProgressView* app);
void candog_view_progress_set_read_callback(CanDogProgressView* app, CanDogProgressViewCallback callback, void* cb_ctx);
void candog_view_progress_set_verify_callback(CanDogProgressView* app, CanDogProgressViewCallback callback, void* cb_ctx);
void candog_view_progress_set_write_callback(CanDogProgressView* app, CanDogProgressViewCallback callback, void* cb_ctx);
void candog_view_progress_set_chip_size(CanDogProgressView* app, size_t chip_size);
void candog_view_progress_set_file_size(CanDogProgressView* app, size_t file_size);
void candog_view_progress_set_block_size(CanDogProgressView* app, size_t block_size);
void candog_view_progress_inc_progress(CanDogProgressView* app);
void candog_view_progress_reset(CanDogProgressView* app);
