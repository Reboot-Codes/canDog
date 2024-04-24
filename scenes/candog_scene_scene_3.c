#include "../candog.h"
#include "../helpers/candog_custom_event.h"
#include "../helpers/candog_haptic.h"
#include "../helpers/candog_led.h"

// Credits

typedef enum {
    ButtonIndexControl3 = -3,
    ButtonIndexControl2 = -2,
    ButtonIndexControl1 = -1,
    ButtonIndexButton1 = 0,
    ButtonIndexButton2 = 1,
    ButtonIndexButton3 = 2,
} ButtonIndex;

static void candog_scene_3_callback(void* context, int32_t index, InputType type) {
    Candog* app = context;

    uint16_t custom_type;
    if (type == InputTypePress) {
        custom_type = CandogCustomEventMenuSelected;
    }
    else if (type == InputTypeRelease) {
        custom_type = CandogCustomEventMenuVoid;
    }
    else if (type == InputTypeShort) {
        //somehow ButtonMenuItemTypeCommon uses InputTypeShort
        custom_type = CandogCustomEventMenuSelected;
    }
    else {
        furi_crash("Unexpected Input Type");
    }
    view_dispatcher_send_custom_event(
        app->view_dispatcher, candog_custom_menu_event_pack(custom_type, index));
}

void candog_scene_scene_3_on_enter(void* context) {
    furi_assert(context);
    Candog* app = context;
    ButtonMenu* button_menu = app->button_menu;
    SceneManager* scene_manager = app->scene_manager;

    button_menu_add_item(button_menu, "Common", ButtonIndexButton1, candog_scene_3_callback, ButtonMenuItemTypeCommon, context);
    button_menu_add_item(button_menu, "Button", ButtonIndexButton2, candog_scene_3_callback, ButtonMenuItemTypeCommon, context);
    button_menu_add_item(button_menu, "Examples", ButtonIndexButton1, candog_scene_3_callback, ButtonMenuItemTypeCommon, context);
    button_menu_add_item(button_menu, "Control", ButtonIndexControl1, candog_scene_3_callback, ButtonMenuItemTypeControl, context);
    button_menu_add_item(button_menu, "Button", ButtonIndexControl2, candog_scene_3_callback, ButtonMenuItemTypeControl, context);
    button_menu_add_item(button_menu, "Examples", ButtonIndexControl3, candog_scene_3_callback, ButtonMenuItemTypeControl, context);

    button_menu_set_header(button_menu, "Button Menu");
    const int16_t button_index = (signed)scene_manager_get_scene_state(app->scene_manager, CandogViewIdScene3);

    button_menu_set_selected_item(button_menu, button_index);
    scene_manager_set_scene_state(scene_manager, CandogSceneScene_3, ButtonIndexButton1);

    view_dispatcher_switch_to_view(app->view_dispatcher, CandogViewIdScene3);
}

bool candog_scene_scene_3_on_event(void* context, SceneManagerEvent event) {
    Candog* app = context;
    bool consumed = false;

    if (event.type == SceneManagerEventTypeCustom) {
        const uint16_t custom_type = candog_custom_menu_event_get_type(event.event);
        const int16_t button_index = candog_custom_menu_event_get_value(event.event);
        if (custom_type == CandogCustomEventMenuSelected) {
            switch (button_index) {
            case ButtonIndexButton1:
                candog_play_happy_bump(app);
                candog_led_set_rgb(app, 255, 0, 0);
                break;
            case ButtonIndexButton2:
                candog_play_happy_bump(app);
                candog_led_set_rgb(app, 0, 255, 0);
                break;
            case ButtonIndexButton3:
                candog_play_happy_bump(app);
                candog_led_set_rgb(app, 0, 0, 255);
                break;
            case ButtonIndexControl1:
                candog_play_bad_bump(app);
                candog_led_set_rgb(app, 255, 0, 255);
                break;
            case ButtonIndexControl2:
                candog_play_bad_bump(app);
                candog_led_set_rgb(app, 255, 255, 0);
                break;
            case ButtonIndexControl3:
                candog_play_bad_bump(app);
                candog_led_set_rgb(app, 0, 255, 255);
                break;
            }
            consumed = true;
        }
    }
    return consumed;
}

void candog_scene_scene_3_on_exit(void* context) {
    Candog* app = context;
    button_menu_reset(app->button_menu);
    notification_message(app->notification, &sequence_reset_red);
    notification_message(app->notification, &sequence_reset_green);
    notification_message(app->notification, &sequence_reset_blue);
}
