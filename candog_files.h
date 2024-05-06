#pragma once
#include "candog_app.h"

bool candog_file_select(CanDogApp* app);
bool candog_file_create(CanDogApp* app, const char* file_name);
bool candog_file_delete(CanDogApp* app);
bool candog_file_create_open(CanDogApp* app);
bool candog_file_open(CanDogApp* app);
bool candog_file_write_block(CanDogApp* app, uint8_t* data, size_t size);
bool candog_file_read_block(CanDogApp* app, uint8_t* data, size_t size);
void candog_file_close(CanDogApp* app);
void candog_file_show_storage_error(CanDogApp* app, const char* error_text);
size_t candog_file_get_size(CanDogApp* app);
