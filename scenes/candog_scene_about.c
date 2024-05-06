#include "../candog_app_i.h"
#include "../lib/can/candog_chip.h"

#define CANDOG_VERSION_APP FAP_VERSION
#define CANDOG_DEVELOPER "Bugfish03"
#define CANDOG_GITHUB "https://github.com/reboot-codes/canDog"
#define CANDOG_NAME "\e#\e!               canDog               \e!\n"
#define CANDOG_BLANK_INV "\e#\e!                                                      \e!\n"

void candog_scene_about_on_enter(void* context) {
    CanDogApp* app = context;
    FuriString* tmp_string = furi_string_alloc();

    widget_add_text_box_element(
        app->widget, 0, 0, 128, 14, AlignCenter, AlignBottom, CANDOG_BLANK_INV, false);
    widget_add_text_box_element(
        app->widget, 0, 2, 128, 14, AlignCenter, AlignBottom, CANDOG_NAME, false);
    furi_string_printf(tmp_string, "\e#%s\n", "Information");
    furi_string_cat_printf(tmp_string, "Version: %s\n", CANDOG_VERSION_APP);
    furi_string_cat_printf(tmp_string, "Developed by: %s\n", CANDOG_DEVELOPER);
    furi_string_cat_printf(tmp_string, "Github: %s\n\n", CANDOG_GITHUB);
    furi_string_cat_printf(tmp_string, "\e#%s\n", "Description");
    furi_string_cat_printf(
        tmp_string,
        "CAN FD sniffer\n"
        "Based on SPI Memory dumper"
        "Originally written by Hedger, ghettorce and x893n,\n"
        "modified by Bugfish03 and\nreboot-codes\n");
    widget_add_text_scroll_element(app->widget, 0, 16, 128, 50, furi_string_get_cstr(tmp_string));

    furi_string_free(tmp_string);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
}

bool candog_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}
void candog_scene_about_on_exit(void* context) {
    CanDogApp* app = context;
    widget_reset(app->widget);
}
