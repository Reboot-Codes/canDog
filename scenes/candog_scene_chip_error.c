#include "../candog_app_i.h"

static void
candog_scene_chip_error_widget_callback(GuiButtonType result, InputType type, void* context) {
    CanDogApp* app = context;
    if (type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void candog_scene_chip_error_on_enter(void* context) {
    CanDogApp* app = context;
    widget_add_button_element(app->widget, GuiButtonTypeLeft, "Back", candog_scene_chip_error_widget_callback, app);
    widget_add_string_element(app->widget, 85, 15, AlignCenter, AlignBottom, FontPrimary, "SPI chip error");
    widget_add_string_multiline_element(app->widget, 85, 52, AlignCenter, AlignBottom, FontSecondary, "Error while\ncommunicating\nwith chip");
    widget_add_icon_element(app->widget, 5, 6, &I_Dip8_32x36);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
}

static void candog_scene_chip_error_set_previous_scene(CanDogApp* app) {
    uint32_t scene = CanDogSceneChipDetect;
    if (app->mode == CanDogModeRead || app->mode == CanDogModeErase) scene = CanDogSceneStart;
    scene_manager_search_and_switch_to_previous_scene(app->scene_manager, scene);
}

bool candog_scene_chip_error_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeBack) {
        success = true;
        candog_scene_chip_error_set_previous_scene(app);
    }
    else if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == GuiButtonTypeLeft) {
            candog_scene_chip_error_set_previous_scene(app);
        }
    }
    return success;
}
void candog_scene_chip_error_on_exit(void* context) {
    CanDogApp* app = context;
    widget_reset(app->widget);
}
