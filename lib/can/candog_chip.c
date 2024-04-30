#include "candog_chip_i.h"

const CanDogChipVendorName candog_chip_vendor_names[] = {
    {"Adesto", CanDogChipVendorADESTO},
    {"AMIC", CanDogChipVendorAMIC},
    {"Boya", CanDogChipVendorBoya},
    {"EON", CanDogChipVendorEON},
    {"PFlash", CanDogChipVendorPFLASH},
    {"Terra", CanDogChipVendorTERRA},
    {"Generalplus", CanDogChipVendorGeneralplus},
    {"Deutron", CanDogChipVendorDEUTRON},
    {"EFST", CanDogChipVendorEFST},
    {"Excel Semi.", CanDogChipVendorEXCELSEMI},
    {"Fidelix", CanDogChipVendorFIDELIX},
    {"GigaDevice", CanDogChipVendorGIGADEVICE},
    {"ICE", CanDogChipVendorICE},
    {"Intel", CanDogChipVendorINTEL},
    {"KHIC", CanDogChipVendorKHIC},
    {"Macronix", CanDogChipVendorMACRONIX},
    {"Micron", CanDogChipVendorMICRON},
    {"Mshine", CanDogChipVendorMSHINE},
    {"Nantronics", CanDogChipVendorNANTRONICS},
    {"Nexflash", CanDogChipVendorNEXFLASH},
    {"Numonyx", CanDogChipVendorNUMONYX},
    {"PCT", CanDogChipVendorPCT},
    {"Spansion", CanDogChipVendorSPANSION},
    {"SST", CanDogChipVendorSST},
    {"ST", CanDogChipVendorST},
    {"Winbond", CanDogChipVendorWINBOND},
    {"Zempro", CanDogChipVendorZEMPRO},
    {"Zbit", CanDogChipVendorZbit},
    {"Berg Micro.", CanDogChipVendorBerg_Micro},
    {"Atmel", CanDogChipVendorATMEL},
    {"ACE", CanDogChipVendorACE},
    {"ATO", CanDogChipVendorATO},
    {"Douqi", CanDogChipVendorDOUQI},
    {"Fremont", CanDogChipVendorFremont},
    {"Fudan", CanDogChipVendorFudan},
    {"Genitop", CanDogChipVendorGenitop},
    {"Paragon", CanDogChipVendorParagon},
    {"XTX", CanDogChipVendorXTX},
    {"XMC", CanDogChipVendorXMC},
    {"Unknown", CanDogChipVendorUnknown} };

static const char* candog_chip_search_vendor_name(CanDogChipVendor vendor_enum) {
    const CanDogChipVendorName* vendor = candog_chip_vendor_names;
    while (vendor->vendor_enum != CanDogChipVendorUnknown && vendor->vendor_enum != vendor_enum)
        vendor++;
    return vendor->vendor_name;
}

bool candog_chip_find_all(CanDogChip* chip_info, found_chips_t found_chips) {
    const CanDogChip* chip_info_arr;
    found_chips_reset(found_chips);
    for (chip_info_arr = CanDogChips; chip_info_arr->model_name != NULL; chip_info_arr++) {
        if (chip_info->vendor_id != chip_info_arr->vendor_id) continue;
        if (chip_info->type_id != chip_info_arr->type_id) continue;
        if (chip_info->capacity_id != chip_info_arr->capacity_id) continue;
        found_chips_push_back(found_chips, chip_info_arr);
    }
    if (found_chips_size(found_chips)) return true;
    return false;
}

void candog_chip_copy_chip_info(CanDogChip* dest, const CanDogChip* src) {
    memcpy(dest, src, sizeof(CanDogChip));
}

size_t candog_chip_get_size(CanDogChip* chip) {
    return (chip->size);
}

const char* candog_chip_get_vendor_name(const CanDogChip* chip) {
    return (candog_chip_search_vendor_name(chip->vendor_enum));
}

const char* candog_chip_get_vendor_name_by_enum(uint32_t vendor_enum) {
    return (candog_chip_search_vendor_name(vendor_enum));
}

const char* candog_chip_get_model_name(const CanDogChip* chip) {
    return (chip->model_name);
}

uint8_t candog_chip_get_vendor_id(CanDogChip* chip) {
    return (chip->vendor_id);
}

uint8_t candog_chip_get_type_id(CanDogChip* chip) {
    return (chip->type_id);
}

uint8_t candog_chip_get_capacity_id(CanDogChip* chip) {
    return (chip->capacity_id);
}

CanDogChipWriteMode candog_chip_get_write_mode(CanDogChip* chip) {
    return (chip->write_mode);
}

size_t candog_chip_get_page_size(CanDogChip* chip) {
    return (chip->page_size);
}

uint32_t candog_chip_get_vendor_enum(const CanDogChip* chip) {
    return ((uint32_t)chip->vendor_enum);
}
