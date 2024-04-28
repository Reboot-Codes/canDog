#include "../candog_app_i.h"
#include "../candog_files.h"

void candog_scene_select_file_on_enter(void* context) {
    CanDogApp* app = context;
    if (candog_file_select(app)) {
        scene_manager_set_scene_state(app->scene_manager, CanDogSceneSavedFileMenu, 0);
        scene_manager_next_scene(app->scene_manager, CanDogSceneSavedFileMenu);
    }
    else {
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, CanDogSceneStart);
    }
}

bool candog_scene_select_file_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void candog_scene_select_file_on_exit(void* context) {
    UNUSED(context);
}
