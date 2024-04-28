#include "../candog_app_i.h"

static void candog_scene_chip_detect_callback(void* context, CanDogCustomEventWorker event) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void candog_scene_chip_detect_on_enter(void* context) {
    CanDogApp* app = context;
    notification_message(app->notifications, &sequence_blink_start_yellow);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewDetect);
    candog_worker_start_thread(app->worker);
    candog_worker_chip_detect_start(app->chip_info, &app->found_chips, app->worker, candog_scene_chip_detect_callback, app);
}

bool candog_scene_chip_detect_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == CanDogCustomEventWorkerChipIdentified) {
            scene_manager_set_scene_state(app->scene_manager, CanDogSceneSelectVendor, 0);
            scene_manager_next_scene(app->scene_manager, CanDogSceneSelectVendor);
        }
        else if (event.event == CanDogCustomEventWorkerChipUnknown) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipDetectFail);
        }
    }
    return success;
}

void candog_scene_chip_detect_on_exit(void* context) {
    CanDogApp* app = context;
    candog_worker_stop_thread(app->worker);
    notification_message(app->notifications, &sequence_blink_stop);
    popup_reset(app->popup);
}
