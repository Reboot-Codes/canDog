#include "../candog_app_i.h"
#include "../candog_files.h"
#include "../lib/spi/candog_chip.h"
#include "../lib/spi/candog_tools.h"

void candog_scene_read_progress_view_result_callback(void* context) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, CanDogCustomEventViewReadCancel);
}

static void candog_scene_read_callback(void* context, CanDogCustomEventWorker event) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void candog_scene_read_on_enter(void* context) {
    CanDogApp* app = context;
    candog_view_progress_set_read_callback(app->view_progress, candog_scene_read_progress_view_result_callback, app);
    notification_message(app->notifications, &sequence_blink_start_blue);
    candog_view_progress_set_chip_size(app->view_progress, candog_chip_get_size(app->chip_info));
    candog_view_progress_set_block_size(app->view_progress, candog_tools_get_file_max_block_size(app->chip_info));
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewProgress);
    candog_worker_start_thread(app->worker);
    candog_worker_read_start(app->chip_info, app->worker, candog_scene_read_callback, app);
}

bool candog_scene_read_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    UNUSED(app);
    bool success = false;
    if (event.type == SceneManagerEventTypeBack) {
        success = true;
    }
    else if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == CanDogCustomEventViewReadCancel) {
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, CanDogSceneChipDetect);
        }
        else if (event.event == CanDogCustomEventWorkerBlockReaded) {
            candog_view_progress_inc_progress(app->view_progress);
        }
        else if (event.event == CanDogCustomEventWorkerDone) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneVerify);
        }
        else if (event.event == CanDogCustomEventWorkerChipFail) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipError);
        }
        else if (event.event == CanDogCustomEventWorkerFileFail) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneStorageError);
        }
    }
    return success;
}
void candog_scene_read_on_exit(void* context) {
    CanDogApp* app = context;
    candog_worker_stop_thread(app->worker);
    candog_view_progress_reset(app->view_progress);
    notification_message(app->notifications, &sequence_blink_stop);
}
