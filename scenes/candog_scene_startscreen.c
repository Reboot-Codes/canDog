#include "../candog.h"
#include "../helpers/candog_custom_event.h"
#include "../views/candog_startscreen.h"

void candog_scene_startscreen_callback(CandogCustomEvent event, void* context) {
    furi_assert(context);
    Candog* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void candog_scene_startscreen_on_enter(void* context) {
    furi_assert(context);
    Candog* app = context;
    candog_startscreen_set_callback(
        app->candog_startscreen, candog_scene_startscreen_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, CandogViewIdStartscreen);
}

bool candog_scene_startscreen_on_event(void* context, SceneManagerEvent event) {
    Candog* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case CandogCustomEventStartscreenLeft:
        case CandogCustomEventStartscreenRight:
            break;
        case CandogCustomEventStartscreenUp:
        case CandogCustomEventStartscreenDown:
            break;
        case CandogCustomEventStartscreenOk:
            scene_manager_next_scene(app->scene_manager, CandogSceneMenu);
            consumed = true;
            break;
        case CandogCustomEventStartscreenBack:
            notification_message(app->notification, &sequence_reset_red);
            notification_message(app->notification, &sequence_reset_green);
            notification_message(app->notification, &sequence_reset_blue);
            if(!scene_manager_search_and_switch_to_previous_scene(
                   app->scene_manager, CandogSceneStartscreen)) {
                scene_manager_stop(app->scene_manager);
                view_dispatcher_stop(app->view_dispatcher);
            }
            consumed = true;
            break;
        }
    }

    return consumed;
}

void candog_scene_startscreen_on_exit(void* context) {
    Candog* app = context;
    UNUSED(app);
}