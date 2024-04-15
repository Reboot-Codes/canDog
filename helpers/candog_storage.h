#pragma once

#include <stdlib.h>
#include <string.h>
#include <storage/storage.h>
#include <flipper_format/flipper_format_i.h>
#include "../candog.h"

#define CANDOG_SETTINGS_FILE_VERSION 1
#define CONFIG_FILE_DIRECTORY_PATH EXT_PATH("apps_data/candog")
#define CANDOG_SETTINGS_SAVE_PATH CONFIG_FILE_DIRECTORY_PATH "/candog.conf"
#define CANDOG_SETTINGS_SAVE_PATH_TMP CANDOG_SETTINGS_SAVE_PATH ".tmp"
#define CANDOG_SETTINGS_HEADER "Candog Config File"
#define CANDOG_SETTINGS_KEY_HAPTIC "Haptic"
#define CANDOG_SETTINGS_KEY_LED "Led"
#define CANDOG_SETTINGS_KEY_SPEAKER "Speaker"
#define CANDOG_SETTINGS_KEY_SAVE_SETTINGS "SaveSettings"

void candog_save_settings(void* context);
void candog_read_settings(void* context);