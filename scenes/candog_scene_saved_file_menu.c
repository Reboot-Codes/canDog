#include "../candog_app_i.h"

typedef enum {
    CanDogSceneSavedFileMenuSubmenuIndexWrite,
    CanDogSceneSavedFileMenuSubmenuIndexCompare,
    CanDogSceneSavedFileMenuSubmenuIndexInfo,
    CanDogSceneSavedFileMenuSubmenuIndexDelete,
} CanDogSceneSavedFileMenuSubmenuIndex;

static void candog_scene_saved_file_menu_submenu_callback(void* context, uint32_t index) {
    CanDogApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void candog_scene_saved_file_menu_on_enter(void* context) {
    CanDogApp* app = context;
    submenu_add_item(app->submenu, "Write", CanDogSceneSavedFileMenuSubmenuIndexWrite, candog_scene_saved_file_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Compare", CanDogSceneSavedFileMenuSubmenuIndexCompare, candog_scene_saved_file_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Info", CanDogSceneSavedFileMenuSubmenuIndexInfo, candog_scene_saved_file_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Delete", CanDogSceneSavedFileMenuSubmenuIndexDelete, candog_scene_saved_file_menu_submenu_callback, app);
    submenu_set_selected_item(app->submenu, scene_manager_get_scene_state(app->scene_manager, CanDogSceneSavedFileMenu));
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewSubmenu);
}

bool candog_scene_saved_file_menu_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, CanDogSceneSavedFileMenu, event.event);
        if (event.event == CanDogSceneSavedFileMenuSubmenuIndexWrite) {
            app->mode = CanDogModeWrite;
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipDetect);
            success = true;
        }
        if (event.event == CanDogSceneSavedFileMenuSubmenuIndexCompare) {
            app->mode = CanDogModeCompare;
            scene_manager_next_scene(app->scene_manager, CanDogSceneChipDetect);
            success = true;
        }
        if (event.event == CanDogSceneSavedFileMenuSubmenuIndexDelete) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneDeleteConfirm);
            success = true;
        }
        if (event.event == CanDogSceneSavedFileMenuSubmenuIndexInfo) {
            scene_manager_next_scene(app->scene_manager, CanDogSceneFileInfo);
            success = true;
        }
    }
    return success;
}

void candog_scene_saved_file_menu_on_exit(void* context) {
    CanDogApp* app = context;
    submenu_reset(app->submenu);
}
