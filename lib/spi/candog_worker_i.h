#pragma once

#include "candog_worker.h"

typedef enum {
    CanDogWorkerModeIdle,
    CanDogWorkerModeChipDetect,
    CanDogWorkerModeRead,
    CanDogWorkerModeVerify,
    CanDogWorkerModeErase,
    CanDogWorkerModeWrite
} CanDogWorkerMode;

struct CanDogWorker {
    CanDogChip* chip_info;
    found_chips_t* found_chips;
    CanDogWorkerMode mode_index;
    CanDogWorkerCallback callback;
    void* cb_ctx;
    FuriThread* thread;
    FuriString* file_name;
};

extern const CanDogWorkerModeType candog_worker_modes[];
