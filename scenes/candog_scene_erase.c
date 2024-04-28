#include "../candog_app_i.h"

static void
candog_scene_erase_widget_callback(GuiButtonType result, InputType type, void* context) {
    CanDogApp* app = context;
    if (type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

static void candog_scene_erase_callback(void* context, CanDogCustomEventWorker event) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void candog_scene_erase_on_enter(void* context) {
    CanDogApp* app = context;
    widget_add_button_element(app->widget, GuiButtonTypeLeft, "Cancel", candog_scene_erase_widget_callback, app);
    widget_add_string_element(app->widget, 64, 15, AlignCenter, AlignBottom, FontPrimary, "Erasing SPI chip");
    widget_add_string_element(app->widget, 64, 27, AlignCenter, AlignBottom, FontSecondary, "Please be patient");
    notification_message(app->notifications, &sequence_blink_start_magenta);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
    candog_worker_start_thread(app->worker);
    candog_worker_erase_start(app->chip_info, app->worker, candog_scene_erase_callback, app);
}

static void candog_scene_erase_set_previous_scene(CanDogApp* app) {
    uint32_t scene = CanDogSceneStart;
    if (app->mode == CanDogModeWrite) scene = CanDogSceneSavedFileMenu;
    scene_manager_search_and_switch_to_previous_scene(app->scene_manager, scene);
}

static void candog_scene_erase_set_next_scene(CanDogApp* app) {
    uint32_t scene = CanDogSceneSuccess;
    if (app->mode == CanDogModeWrite) scene = CanDogSceneWrite;
    scene_manager_next_scene(app->scene_manager, scene);
}

bool candog_scene_erase_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeBack) {
        success = true;
        candog_scene_erase_set_previous_scene(app);
    }
    else if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == GuiButtonTypeLeft) {
            scene_manager_previous_scene(app->scene_manager);
        }
        else if (event.event == CanDogCustomEventWorkerDone) {
            candog_scene_erase_set_next_scene(app);
        }
        else if (event.event == CanDogCustomEventWorkerChipFail) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipError);
        }
    }
    return success;
}
void candog_scene_erase_on_exit(void* context) {
    CanDogApp* app = context;
    candog_worker_stop_thread(app->worker);
    notification_message(app->notifications, &sequence_blink_stop);
    widget_reset(app->widget);
}
