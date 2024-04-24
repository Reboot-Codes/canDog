#include "candog.h"

bool candog_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    Candog* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

void candog_tick_event_callback(void* context) {
    furi_assert(context);
    Candog* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

//leave app if back button pressed
bool candog_navigation_event_callback(void* context) {
    furi_assert(context);
    Candog* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

Candog* candog_app_alloc() {
    Candog* app = malloc(sizeof(Candog));
    app->gui = furi_record_open(RECORD_GUI);
    app->notification = furi_record_open(RECORD_NOTIFICATION);

    //Turn backlight on, believe me this makes testing your app easier
    notification_message(app->notification, &sequence_display_backlight_on);

    //Scene additions
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    app->scene_manager = scene_manager_alloc(&candog_scene_handlers, app);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, candog_navigation_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, candog_tick_event_callback, 100);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, candog_custom_event_callback);
    app->submenu = submenu_alloc();

    // Set defaults, in case no config loaded
    app->haptic = 1;
    app->speaker = 1;
    app->led = 1;
    app->save_settings = 1;

    // Used for File Browser
    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->file_path = furi_string_alloc();

    // Load configs
    candog_read_settings(app);

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdMenu, submenu_get_view(app->submenu));
    app->candog_startscreen = candog_startscreen_alloc();

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdStartscreen, candog_startscreen_get_view(app->candog_startscreen));
    app->candog_scene_1 = candog_scene_1_alloc();

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdScene1, candog_scene_1_get_view(app->candog_scene_1));
    app->candog_scene_2 = candog_scene_2_alloc();

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdScene2, candog_scene_2_get_view(app->candog_scene_2));
    app->button_menu = button_menu_alloc();

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdScene3, button_menu_get_view(app->button_menu));
    app->variable_item_list = variable_item_list_alloc();

    view_dispatcher_add_view(app->view_dispatcher, CandogViewIdSettings, variable_item_list_get_view(app->variable_item_list));

    //End Scene Additions

    return app;
}

void candog_app_free(Candog* app) {
    furi_assert(app);

    // Scene manager
    scene_manager_free(app->scene_manager);

    // View Dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdMenu);
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdScene1);
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdScene2);
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdScene3);
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdSettings);
    view_dispatcher_remove_view(app->view_dispatcher, CandogViewIdStartscreen);
    submenu_free(app->submenu);
    variable_item_list_free(app->variable_item_list);
    candog_scene_1_free(app->candog_scene_1);
    candog_scene_2_free(app->candog_scene_2);
    candog_startscreen_free(app->candog_startscreen);

    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);

    app->gui = NULL;
    app->notification = NULL;

    // Close File Browser
    furi_record_close(RECORD_DIALOGS);
    furi_string_free(app->file_path);

    //Remove whatever is left
    free(app);
}

int32_t candog_app(void* p) {
    UNUSED(p);
    Candog* app = candog_app_alloc();

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    scene_manager_next_scene(app->scene_manager, CandogSceneMenu); //Start with start screen

    furi_hal_power_suppress_charge_enter();

    view_dispatcher_run(app->view_dispatcher);

    candog_save_settings(app);

    furi_hal_power_suppress_charge_exit();
    candog_app_free(app);

    return 0;
}
