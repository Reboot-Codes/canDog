#include "../candog.h"
#include "../helpers/candog_custom_event.h"
#include "../views/candog_scene_1.h"

// Start sniffing

void candog_scene_1_callback(CandogCustomEvent event, void* context) {
    furi_assert(context);
    Candog* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

void candog_scene_scene_1_on_enter(void* context) {
    furi_assert(context);
    Candog* app = context;
    candog_scene_1_set_callback(app->candog_scene_1, candog_scene_1_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, CandogViewIdScene1);
}

bool candog_scene_scene_1_on_event(void* context, SceneManagerEvent event) {
    Candog* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case CandogCustomEventScene1Left:
        case CandogCustomEventScene1Right:
            break;
        case CandogCustomEventScene1Up:
        case CandogCustomEventScene1Down:
            break;
        case CandogCustomEventScene1Back:
            notification_message(app->notification, &sequence_reset_red);
            notification_message(app->notification, &sequence_reset_green);
            notification_message(app->notification, &sequence_reset_blue);
            if(!scene_manager_search_and_switch_to_previous_scene(
                   app->scene_manager, CandogSceneMenu)) {
                scene_manager_stop(app->scene_manager);
                view_dispatcher_stop(app->view_dispatcher);
            }
            consumed = true;
            break;
        }
    }

    return consumed;
}

void candog_scene_scene_1_on_exit(void* context) {
    Candog* app = context;
    UNUSED(app);
}