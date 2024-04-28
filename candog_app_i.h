#pragma once

#include <furi.h>
#include <furi_hal_spi.h>
#include <furi_hal_spi_config.h>
#include "candog_app.h"
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/popup.h>
#include <notification/notification_messages.h>
#include <dialogs/dialogs.h>
#include <gui/modules/widget.h>
#include <gui/modules/text_input.h>
#include <storage/storage.h>
#include <toolbox/path.h>
#include <toolbox/name_generator.h>
#include "scenes/candog_scene.h"
#include "lib/spi/candog_worker.h"
#include "candog_icons.h"
#include "views/candog_view_progress.h"
#include "views/candog_view_detect.h"

#define TAG "CanDog"
#define SPI_MEM_FILE_EXTENSION ".bin"
#define SPI_MEM_FILE_PREFIX "CanDog"
#define SPI_MEM_FILE_NAME_SIZE 100
#define SPI_MEM_TEXT_BUFFER_SIZE 128

typedef enum {
    CanDogModeRead,
    CanDogModeWrite,
    CanDogModeCompare,
    CanDogModeErase,
    CanDogModeDelete,
    CanDogModeUnknown
} CanDogMode;

struct CanDogApp {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    Submenu* submenu;
    DialogEx* dialog_ex;
    Popup* popup;
    NotificationApp* notifications;
    FuriString* file_path;
    DialogsApp* dialogs;
    Storage* storage;
    File* file;
    Widget* widget;
    CanDogWorker* worker;
    CanDogChip* chip_info;
    found_chips_t found_chips;
    uint32_t chip_vendor_enum;
    CanDogProgressView* view_progress;
    CanDogDetectView* view_detect;
    TextInput* text_input;
    CanDogMode mode;
    char text_buffer[SPI_MEM_TEXT_BUFFER_SIZE + 1];
};

typedef enum {
    CanDogViewSubmenu,
    CanDogViewDialogEx,
    CanDogViewPopup,
    CanDogViewWidget,
    CanDogViewTextInput,
    CanDogViewProgress,
    CanDogViewDetect
} CanDogView;

typedef enum {
    CanDogCustomEventViewReadCancel,
    CanDogCustomEventViewVerifySkip,
    CanDogCustomEventTextEditResult,
    CanDogCustomEventPopupBack
} CanDogCustomEvent;
