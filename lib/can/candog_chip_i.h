#pragma once

#include <furi.h>
#include "candog_chip.h"

typedef enum {
    CanDogChipVendorUnknown,
    CanDogChipVendorADESTO,
    CanDogChipVendorAMIC,
    CanDogChipVendorBoya,
    CanDogChipVendorEON,
    CanDogChipVendorPFLASH,
    CanDogChipVendorTERRA,
    CanDogChipVendorGeneralplus,
    CanDogChipVendorDEUTRON,
    CanDogChipVendorEFST,
    CanDogChipVendorEXCELSEMI,
    CanDogChipVendorFIDELIX,
    CanDogChipVendorGIGADEVICE,
    CanDogChipVendorICE,
    CanDogChipVendorINTEL,
    CanDogChipVendorKHIC,
    CanDogChipVendorMACRONIX,
    CanDogChipVendorMICRON,
    CanDogChipVendorMSHINE,
    CanDogChipVendorNANTRONICS,
    CanDogChipVendorNEXFLASH,
    CanDogChipVendorNUMONYX,
    CanDogChipVendorPCT,
    CanDogChipVendorSPANSION,
    CanDogChipVendorSST,
    CanDogChipVendorST,
    CanDogChipVendorWINBOND,
    CanDogChipVendorZEMPRO,
    CanDogChipVendorZbit,
    CanDogChipVendorBerg_Micro,
    CanDogChipVendorATMEL,
    CanDogChipVendorACE,
    CanDogChipVendorATO,
    CanDogChipVendorDOUQI,
    CanDogChipVendorFremont,
    CanDogChipVendorFudan,
    CanDogChipVendorGenitop,
    CanDogChipVendorParagon,
    CanDogChipVendorXTX,
    CanDogChipVendorXMC
} CanDogChipVendor;

typedef enum {
    CanDogChipCMDReadJEDECChipID = 0x9F,
    CanDogChipCMDReadData = 0x03,
    CanDogChipCMDChipErase = 0xC7,
    CanDogChipCMDWriteEnable = 0x06,
    CanDogChipCMDWriteDisable = 0x04,
    CanDogChipCMDReadStatus = 0x05,
    CanDogChipCMDWriteData = 0x02,
    CanDogChipCMDReleasePowerDown = 0xAB
} CanDogChipCMD;

enum CanDogChipStatusBit {
    CanDogChipStatusBitBusy = (0x01 << 0),
    CanDogChipStatusBitWriteEnabled = (0x01 << 1),
    CanDogChipStatusBitBitProtection1 = (0x01 << 2),
    CanDogChipStatusBitBitProtection2 = (0x01 << 3),
    CanDogChipStatusBitBitProtection3 = (0x01 << 4),
    CanDogChipStatusBitTopBottomProtection = (0x01 << 5),
    CanDogChipStatusBitSectorProtect = (0x01 << 6),
    CanDogChipStatusBitRegisterProtect = (0x01 << 7)
};

typedef struct {
    const char* vendor_name;
    CanDogChipVendor vendor_enum;
} CanDogChipVendorName;

struct CanDogChip {
    uint8_t vendor_id;
    uint8_t type_id;
    uint8_t capacity_id;
    const char* model_name;
    size_t size;
    size_t page_size;
    CanDogChipVendor vendor_enum;
    CanDogChipWriteMode write_mode;
};

extern const CanDogChip CanDogChips[];
