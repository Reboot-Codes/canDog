#pragma once

#include "candog_chip.h"

#define SPI_MEM_SPI_TIMEOUT 1000
#define SPI_MEM_MAX_BLOCK_SIZE 256
#define SPI_MEM_FILE_BUFFER_SIZE 4096

bool candog_tools_read_chip_info(CanDogChip* chip);
bool candog_tools_read_block(CanDogChip* chip, size_t offset, uint8_t* data, size_t block_size);
size_t candog_tools_get_file_max_block_size(CanDogChip* chip);
CanDogChipStatus candog_tools_get_chip_status(CanDogChip* chip);
bool candog_tools_erase_chip(CanDogChip* chip);
bool candog_tools_write_bytes(CanDogChip* chip, size_t offset, uint8_t* data, size_t block_size);
