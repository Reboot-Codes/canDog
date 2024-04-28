#include "../candog_app_i.h"

static void candog_scene_chip_detected_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    CanDogApp* app = context;
    if (type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

static void candog_scene_chip_detected_print_chip_info(Widget* widget, CanDogChip* chip_info) {
    FuriString* tmp_string = furi_string_alloc();
    widget_add_string_element(widget, 40, 12, AlignLeft, AlignTop, FontSecondary, candog_chip_get_vendor_name(chip_info));
    widget_add_string_element(widget, 40, 20, AlignLeft, AlignTop, FontSecondary, candog_chip_get_model_name(chip_info));
    furi_string_printf(tmp_string, "Size: %zu KB", candog_chip_get_size(chip_info) / 1024);
    widget_add_string_element(widget, 40, 28, AlignLeft, AlignTop, FontSecondary, furi_string_get_cstr(tmp_string));
    furi_string_free(tmp_string);
}

static void candog_scene_chip_detect_draw_next_button(CanDogApp* app) {
    FuriString* str = furi_string_alloc();
    if (app->mode == CanDogModeRead) furi_string_printf(str, "%s", "Read");
    if (app->mode == CanDogModeWrite) furi_string_printf(str, "%s", "Write");
    if (app->mode == CanDogModeErase) furi_string_printf(str, "%s", "Erase");
    if (app->mode == CanDogModeCompare) furi_string_printf(str, "%s", "Check");
    widget_add_button_element(app->widget, GuiButtonTypeRight, furi_string_get_cstr(str), candog_scene_chip_detected_widget_callback, app);
    furi_string_free(str);
}

static void candog_scene_chip_detected_set_previous_scene(CanDogApp* app) {
    uint32_t scene = CanDogSceneStart;
    if (app->mode == CanDogModeCompare || app->mode == CanDogModeWrite)
        scene = CanDogSceneSavedFileMenu;
    scene_manager_search_and_switch_to_previous_scene(app->scene_manager, scene);
}

static void candog_scene_chip_detected_set_next_scene(CanDogApp* app) {
    uint32_t scene = CanDogSceneStart;
    if (app->mode == CanDogModeRead) scene = CanDogSceneReadFilename;
    if (app->mode == CanDogModeWrite) scene = CanDogSceneErase;
    if (app->mode == CanDogModeErase) scene = CanDogSceneErase;
    if (app->mode == CanDogModeCompare) scene = CanDogSceneVerify;
    scene_manager_next_scene(app->scene_manager, scene);
}

void candog_scene_chip_detected_on_enter(void* context) {
    CanDogApp* app = context;
    widget_add_button_element(app->widget, GuiButtonTypeLeft, "Retry", candog_scene_chip_detected_widget_callback, app);
    candog_scene_chip_detect_draw_next_button(app);
    widget_add_icon_element(app->widget, 0, 12, &I_Dip8_32x36);
    widget_add_string_element(app->widget, 64, 9, AlignCenter, AlignBottom, FontPrimary, "Detected SPI chip");
    candog_scene_chip_detected_print_chip_info(app->widget, app->chip_info);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
}

bool candog_scene_chip_detected_on_event(void* context, SceneManagerEvent event) {
    CanDogApp* app = context;
    bool success = false;
    if (event.type == SceneManagerEventTypeBack) {
        success = true;
        candog_scene_chip_detected_set_previous_scene(app);
    }
    else if (event.type == SceneManagerEventTypeCustom) {
        success = true;
        if (event.event == GuiButtonTypeLeft) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, CanDogSceneChipDetect);
        }
        else if (event.event == GuiButtonTypeRight) {
            candog_scene_chip_detected_set_next_scene(app);
        }
    }
    return success;
}
void candog_scene_chip_detected_on_exit(void* context) {
    CanDogApp* app = context;
    widget_reset(app->widget);
}
