#include <furi_hal.h>
#include "candog_app_i.h"
#include "candog_files.h"
#include "lib/spi/candog_chip_i.h"

static bool candog_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    CanDogApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool candog_back_event_callback(void* context) {
    furi_assert(context);
    CanDogApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

CanDogApp* candog_alloc(void) {
    CanDogApp* instance = malloc(sizeof(CanDogApp)); //-V799

    instance->file_path = furi_string_alloc_set(STORAGE_APP_DATA_PATH_PREFIX);
    instance->gui = furi_record_open(RECORD_GUI);
    instance->notifications = furi_record_open(RECORD_NOTIFICATION);
    instance->view_dispatcher = view_dispatcher_alloc();
    instance->scene_manager = scene_manager_alloc(&candog_scene_handlers, instance);
    instance->submenu = submenu_alloc();
    instance->dialog_ex = dialog_ex_alloc();
    instance->popup = popup_alloc();
    instance->worker = candog_worker_alloc();
    instance->dialogs = furi_record_open(RECORD_DIALOGS);
    instance->storage = furi_record_open(RECORD_STORAGE);
    instance->widget = widget_alloc();
    instance->chip_info = malloc(sizeof(CanDogChip));
    found_chips_init(instance->found_chips);
    instance->view_progress = candog_view_progress_alloc();
    instance->view_detect = candog_view_detect_alloc();
    instance->text_input = text_input_alloc();
    instance->mode = CanDogModeUnknown;

    // Migrate data from old sd-card folder
    storage_common_migrate(instance->storage, EXT_PATH("canmem"), STORAGE_APP_DATA_PATH_PREFIX);

    view_dispatcher_enable_queue(instance->view_dispatcher);
    view_dispatcher_set_event_callback_context(instance->view_dispatcher, instance);
    view_dispatcher_set_custom_event_callback(instance->view_dispatcher, candog_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(instance->view_dispatcher, candog_back_event_callback);
    view_dispatcher_attach_to_gui(instance->view_dispatcher, instance->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewSubmenu, submenu_get_view(instance->submenu));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewDialogEx, dialog_ex_get_view(instance->dialog_ex));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewPopup, popup_get_view(instance->popup));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewWidget, widget_get_view(instance->widget));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewProgress, candog_view_progress_get_view(instance->view_progress));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewDetect, candog_view_detect_get_view(instance->view_detect));
    view_dispatcher_add_view(instance->view_dispatcher, CanDogViewTextInput, text_input_get_view(instance->text_input));

    furi_hal_power_enable_otg();
    furi_hal_spi_bus_handle_init(&furi_hal_spi_bus_handle_external);
    scene_manager_next_scene(instance->scene_manager, CanDogSceneStart);
    return instance;
} //-V773

void candog_free(CanDogApp* instance) {
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewSubmenu);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewDialogEx);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewPopup);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewWidget);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewProgress);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewDetect);
    view_dispatcher_remove_view(instance->view_dispatcher, CanDogViewTextInput);
    candog_view_progress_free(instance->view_progress);
    candog_view_detect_free(instance->view_detect);
    submenu_free(instance->submenu);
    dialog_ex_free(instance->dialog_ex);
    popup_free(instance->popup);
    widget_free(instance->widget);
    text_input_free(instance->text_input);
    view_dispatcher_free(instance->view_dispatcher);
    scene_manager_free(instance->scene_manager);
    candog_worker_free(instance->worker);
    free(instance->chip_info);
    found_chips_clear(instance->found_chips);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
    furi_string_free(instance->file_path);
    furi_hal_spi_bus_handle_deinit(&furi_hal_spi_bus_handle_external);
    furi_hal_power_disable_otg();
    free(instance);
}

int32_t candog_app(void* p) {
    UNUSED(p);
    CanDogApp* instance = candog_alloc();
    view_dispatcher_run(instance->view_dispatcher);
    candog_free(instance);
    return 0;
}
