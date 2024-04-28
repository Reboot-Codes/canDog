#pragma once

#include <furi.h>
#include "candog_chip.h"

typedef struct CanDogWorker CanDogWorker;

typedef struct {
    void (* const process)(CanDogWorker* worker);
} CanDogWorkerModeType;

typedef enum {
    CanDogCustomEventWorkerChipIdentified,
    CanDogCustomEventWorkerChipUnknown,
    CanDogCustomEventWorkerBlockReaded,
    CanDogCustomEventWorkerChipFail,
    CanDogCustomEventWorkerFileFail,
    CanDogCustomEventWorkerDone,
    CanDogCustomEventWorkerVerifyFail,
} CanDogCustomEventWorker;

typedef void (*CanDogWorkerCallback)(void* context, CanDogCustomEventWorker event);

CanDogWorker* candog_worker_alloc();
void candog_worker_free(CanDogWorker* worker);
void candog_worker_start_thread(CanDogWorker* worker);
void candog_worker_stop_thread(CanDogWorker* worker);
bool candog_worker_check_for_stop(CanDogWorker* worker);
void candog_worker_chip_detect_start(
    CanDogChip* chip_info,
    found_chips_t* found_chips,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context);
void candog_worker_read_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context);
void candog_worker_verify_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context);
void candog_worker_erase_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context);
void candog_worker_write_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context);
