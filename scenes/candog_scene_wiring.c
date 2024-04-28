#include "../candog_app_i.h"
#include "../lib/spi/candog_chip.h"

void candog_scene_wiring_on_enter(void* context) {
    CanDogApp* app = context;
    widget_add_icon_element(app->widget, 0, 0, &I_Wiring_SPI_128x64);
    view_dispatcher_switch_to_view(app->view_dispatcher, CanDogViewWidget);
}

bool candog_scene_wiring_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}
void candog_scene_wiring_on_exit(void* context) {
    CanDogApp* app = context;
    widget_reset(app->widget);
}
