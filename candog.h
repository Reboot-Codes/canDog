#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <dialogs/dialogs.h>
#include <notification/notification_messages.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/scene_manager.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/button_menu.h>
#include <gui/modules/dialog_ex.h>
#include "scenes/candog_scene.h"
#include "views/candog_startscreen.h"
#include "views/candog_scene_1.h"
#include "views/candog_scene_2.h"
#include "helpers/candog_storage.h"
#include "candog_icons.h"

#define TAG "Candog"

#define SUBGHZ_APP_EXTENSION ".sub"
#define SUBGHZ_APP_FOLDER ANY_PATH("subghz")
#define BOILERPLATE_VERSION "1.1"

typedef struct {
    Gui* gui;
    NotificationApp* notification;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    SceneManager* scene_manager;
    VariableItemList* variable_item_list;
    CandogStartscreen* candog_startscreen;
    CandogScene1* candog_scene_1;
    CandogScene2* candog_scene_2;
    DialogsApp* dialogs; // File Browser
    FuriString* file_path; // File Browser
    uint32_t haptic;
    uint32_t speaker;
    uint32_t led;
    uint32_t save_settings;
    ButtonMenu* button_menu; // Button Menu
} Candog;

typedef enum {
    CandogViewIdStartscreen,
    CandogViewIdMenu,
    CandogViewIdScene1,
    CandogViewIdScene2,
    CandogViewIdScene3,
    CandogViewIdScene4,
    CandogViewIdScene5,
    CandogViewIdSettings,
} CandogViewId;

typedef enum {
    CandogHapticOff,
    CandogHapticOn,
} CandogHapticState;

typedef enum {
    CandogSpeakerOff,
    CandogSpeakerOn,
} CandogSpeakerState;

typedef enum {
    CandogLedOff,
    CandogLedOn,
} CandogLedState;

typedef enum {
    CandogSettingsOff,
    CandogSettingsOn,
} CandogSettingsStoreState;
