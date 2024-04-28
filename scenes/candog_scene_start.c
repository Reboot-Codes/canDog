#include "../candog_app_i.h"

typedef enum {
    CanDogSceneStartSubmenuIndexRead,
    CanDogSceneStartSubmenuIndexSaved,
    CanDogSceneStartSubmenuIndexErase,
    CanDogSceneStartSubmenuIndexWiring,
    CanDogSceneStartSubmenuIndexAbout
} CanDogSceneStartSubmenuIndex;

static void candog_scene_start_submenu_callback(void* context, uint32_t index) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void candog_scene_start_on_enter(void* context) {
    CanDogApp* app = context;
    submenu_add_item(app->submenu, "Read", CanDogSceneStartSubmenuIndexRead, candog_scene_start_submenu_callback, app);
    submenu_add_item(app->submenu, "Saved", CanDogSceneStartSubmenuIndexSaved, candog_scene_start_submenu_callback, app);
    submenu_add_item(app->submenu, "Erase", CanDogSceneStartSubmenuIndexErase, candog_scene_start_submenu_callback, app);
    submenu_add_item(app->submenu, "Wiring", CanDogSceneStartSubmenuIndexWiring, candog_scene_start_submenu_callback, app);
    submenu_add_item(app->submenu, "About", CanDogSceneStartSubmenuIndexAbout, candog_scene_start_submenu_callback, app);
    submenu_set_selected_item(app->submenu, scene_manager_get_scene_state(app->scene_manager, CanDogSceneStart));
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewSubmenu);
}

bool candog_scene_start_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, CanDogSceneStart, event.event);
        if (event.event == CanDogSceneStartSubmenuIndexRead) {
            app->mode = CanDogModeRead;
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipDetect);
            success = true;
        }
        else if (event.event == CanDogSceneStartSubmenuIndexSaved) {
            furi_string_set(app->file_path, STORAGE_APP_DATA_PATH_PREFIX);
            scene_manager_next_scene(app->scene_manager, CanDogSceneSelectFile);
            success = true;
        }
        else if (event.event == CanDogSceneStartSubmenuIndexErase) {
            app->mode = CanDogModeErase;
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipDetect);
            success = true;
        }
        else if (event.event == CanDogSceneStartSubmenuIndexWiring) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneWiring);
            success = true;
        }
        else if (event.event == CanDogSceneStartSubmenuIndexAbout) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneAbout);
            success = true;
        }
    }
    return success;
}

void candog_scene_start_on_exit(void* context) {
    CanDogApp* app = context;
    submenu_reset(app->submenu);
}
