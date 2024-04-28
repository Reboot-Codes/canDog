#include "../candog_app_i.h"

static void candog_scene_verify_error_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    CanDogApp* app = context;
    if (type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void candog_scene_verify_error_on_enter(void* context) {
    CanDogApp* app = context;
    widget_add_button_element(app->widget, GuiButtonTypeLeft, "Back", candog_scene_verify_error_widget_callback, app);
    widget_add_string_element(app->widget, 64, 9, AlignCenter, AlignBottom, FontPrimary, "Verification error");
    widget_add_string_element(app->widget, 64, 21, AlignCenter, AlignBottom, FontSecondary, "Data mismatch");
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
}

bool candog_scene_verify_error_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeBack) {
        success = true;
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, CanDogSceneChipDetect);
    }
    else if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == GuiButtonTypeLeft) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, CanDogSceneChipDetect);
        }
    }
    return success;
}
void candog_scene_verify_error_on_exit(void* context) {
    CanDogApp* app = context;
    widget_reset(app->widget);
}
