#include "../candog.h"

enum SubmenuIndex {
    SubmenuIndexSniff = 10,
    SubmenuIndexReplay,
    SubmenuIndexCredits,
    SubmenuIndexExport,
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
        app->submenu, "Start sniffing", SubmenuIndexSniff, candog_scene_menu_submenu_callback, app);
    submenu_add_item(
        app->submenu,
        "Replay Capture",
        SubmenuIndexReplay,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Export Capture",
        SubmenuIndexExport,
        candog_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu, "Settings", SubmenuIndexSettings, candog_scene_menu_submenu_callback, app);
    submenu_add_item(
        app->submenu, "Credits", SubmenuIndexCredits, candog_scene_menu_submenu_callback, app);

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
        if(event.event == SubmenuIndexSniff) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexSniff);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_1);
            return true;

        } else if(event.event == SubmenuIndexReplay) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexReplay);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_2);
            return true;

        } else if(event.event == SubmenuIndexCredits) {
            scene_manager_set_scene_state(
                app->scene_manager, CandogSceneMenu, SubmenuIndexCredits);
            scene_manager_next_scene(app->scene_manager, CandogSceneScene_3);

        } else if(event.event == SubmenuIndexExport) {
            scene_manager_set_scene_state(app->scene_manager, CandogSceneMenu, SubmenuIndexExport);
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