#pragma once

#include <furi.h>
#include <m-array.h>

typedef struct CanDogChip CanDogChip;

ARRAY_DEF(found_chips, const CanDogChip*, M_POD_OPLIST)

typedef enum {
    CanDogChipStatusBusy,
    CanDogChipStatusIdle,
    CanDogChipStatusError
} CanDogChipStatus;

typedef enum {
    CanDogChipWriteModeUnknown = 0,
    CanDogChipWriteModePage = (0x01 << 0),
    CanDogChipWriteModeAAIByte = (0x01 << 1),
    CanDogChipWriteModeAAIWord = (0x01 << 2),
} CanDogChipWriteMode;

const char* candog_chip_get_vendor_name(const CanDogChip* chip);
const char* candog_chip_get_model_name(const CanDogChip* chip);
size_t candog_chip_get_size(CanDogChip* chip);
uint8_t candog_chip_get_vendor_id(CanDogChip* chip);
uint8_t candog_chip_get_type_id(CanDogChip* chip);
uint8_t candog_chip_get_capacity_id(CanDogChip* chip);
CanDogChipWriteMode candog_chip_get_write_mode(CanDogChip* chip);
size_t candog_chip_get_page_size(CanDogChip* chip);
bool candog_chip_find_all(CanDogChip* chip_info, found_chips_t found_chips);
void candog_chip_copy_chip_info(CanDogChip* dest, const CanDogChip* src);
uint32_t candog_chip_get_vendor_enum(const CanDogChip* chip);
const char* candog_chip_get_vendor_name_by_enum(uint32_t vendor_enum);
