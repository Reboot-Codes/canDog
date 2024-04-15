#include "../candog.h"

enum SubmenuIndex {
    SubmenuIndexScene1 = 10,
    SubmenuIndexScene2,
    SubmenuIndexScene3,
    SubmenuIndexScene4,
    SubmenuIndexScene5,
    SubmenuIndexSettings,
};

void candog_scene_menu_submenu_callback(void* context, uint32_t index) {
    Candog* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void candog_scene_menu_on_enter(void* context) {
    Candog* app = context;

    submenu_add_item(
        app->submenu,
        "Scene 1 (empty)",
        SubmenuIndexScene1,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Scene 2 (Inputs/Effects)",
        SubmenuIndexScene2,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Scene 3 (Buttonmenu)",
        SubmenuIndexScene3,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Scene 4 (File Browser)",
        SubmenuIndexScene4,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu, "Settings", SubmenuIndexSettings, candog_scene_menu_submenu_callback, app);

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, CandogSceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, CandogViewIdMenu);
}

bool candog_scene_menu_on_event(void* context, SceneManagerEvent event) {
    Candog* app = context;
    UNUSED(app);
    if(event.type == SceneManagerEventTypeBack) {
        //exit app
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexScene1) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexScene1);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_1);
            return true;
        } else if(event.event == SubmenuIndexScene2) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexScene2);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_2);
            return true;
        } else if(event.event == SubmenuIndexScene3) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexScene3);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_3);
        } else if(event.event == SubmenuIndexScene4) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexScene4);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_4);
        } else if(event.event == SubmenuIndexSettings) {
            scene_manager_set_scene_state(
                app->scene_manager, CandogSceneMenu, SubmenuIndexSettings);
            scene_manager_next_scene(app->scene_manager, CandogSceneSettings);
            return true;
        }
    }
    return false;
}

void candog_scene_menu_on_exit(void* context) {
    Candog* app = context;
    submenu_reset(app->submenu);
}