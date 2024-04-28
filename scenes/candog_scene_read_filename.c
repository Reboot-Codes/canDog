#include "../candog_app_i.h"
#include "../candog_files.h"

void candog_scene_read_filename_view_result_callback(void* context) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, CanDogCustomEventTextEditResult);
}

void candog_scene_read_set_random_filename(CanDogApp* app) {
    if (furi_string_end_with(app->file_path, SPI_MEM_FILE_EXTENSION)) {
        size_t filename_start = furi_string_search_rchar(app->file_path, '/');
        furi_string_left(app->file_path, filename_start);
    }
    name_generator_make_auto(app->text_buffer, SPI_MEM_TEXT_BUFFER_SIZE, SPI_MEM_FILE_PREFIX);
}

void candog_scene_read_filename_on_enter(void* context) {
    CanDogApp* app = context;
    candog_scene_read_set_random_filename(app);
    text_input_set_header_text(app->text_input, "Name the dump");
    text_input_set_result_callback(app->text_input, candog_scene_read_filename_view_result_callback, app, app->text_buffer, SPI_MEM_FILE_NAME_SIZE, true);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewTextInput);
}

bool candog_scene_read_filename_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    UNUSED(app);
    bool success = false;
    if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == CanDogCustomEventTextEditResult) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneRead);
        }
    }
    return success;
}
void candog_scene_read_filename_on_exit(void* context) {
    CanDogApp* app = context;
    text_input_reset(app->text_input);
}
