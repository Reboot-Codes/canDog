#include "candog_worker_i.h"

typedef enum {
    CanDogEventStopThread = (1 << 0),
    CanDogEventChipDetect = (1 << 1),
    CanDogEventRead = (1 << 2),
    CanDogEventVerify = (1 << 3),
    CanDogEventErase = (1 << 4),
    CanDogEventWrite = (1 << 5),
    CanDogEventAll =
    (CanDogEventStopThread | CanDogEventChipDetect | CanDogEventRead | CanDogEventVerify |
        CanDogEventErase | CanDogEventWrite)
} CanDogEventEventType;

static int32_t candog_worker_thread(void* thread_context);

CanDogWorker* candog_worker_alloc() {
    CanDogWorker* worker = malloc(sizeof(CanDogWorker));
    worker->callback = NULL;
    worker->thread = furi_thread_alloc();
    worker->mode_index = CanDogWorkerModeIdle;
    furi_thread_set_name(worker->thread, "CanDogWorker");
    furi_thread_set_callback(worker->thread, candog_worker_thread);
    furi_thread_set_context(worker->thread, worker);
    furi_thread_set_stack_size(worker->thread, 10240);
    return worker;
}

void candog_worker_free(CanDogWorker* worker) {
    furi_thread_free(worker->thread);
    free(worker);
}

bool candog_worker_check_for_stop(CanDogWorker* worker) {
    UNUSED(worker);
    uint32_t flags = furi_thread_flags_get();
    return (flags & CanDogEventStopThread);
}

static int32_t candog_worker_thread(void* thread_context) {
    CanDogWorker* worker = thread_context;
    while (true) {
        uint32_t flags = furi_thread_flags_wait(CanDogEventAll, FuriFlagWaitAny, FuriWaitForever);
        if (flags != (unsigned)FuriFlagErrorTimeout) {
            if (flags & CanDogEventStopThread) break;
            if (flags & CanDogEventChipDetect) worker->mode_index = CanDogWorkerModeChipDetect;
            if (flags & CanDogEventRead) worker->mode_index = CanDogWorkerModeRead;
            if (flags & CanDogEventVerify) worker->mode_index = CanDogWorkerModeVerify;
            if (flags & CanDogEventErase) worker->mode_index = CanDogWorkerModeErase;
            if (flags & CanDogEventWrite) worker->mode_index = CanDogWorkerModeWrite;
            if (candog_worker_modes[worker->mode_index].process) {
                candog_worker_modes[worker->mode_index].process(worker);
            }
            worker->mode_index = CanDogWorkerModeIdle;
        }
    }
    return 0;
}

void candog_worker_start_thread(CanDogWorker* worker) {
    furi_thread_start(worker->thread);
}

void candog_worker_stop_thread(CanDogWorker* worker) {
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventStopThread);
    furi_thread_join(worker->thread);
}

void candog_worker_chip_detect_start(
    CanDogChip* chip_info,
    found_chips_t* found_chips,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context) {
    furi_check(worker->mode_index == CanDogWorkerModeIdle);
    worker->callback = callback;
    worker->cb_ctx = context;
    worker->chip_info = chip_info;
    worker->found_chips = found_chips;
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventChipDetect);
}

void candog_worker_read_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context) {
    furi_check(worker->mode_index == CanDogWorkerModeIdle);
    worker->callback = callback;
    worker->cb_ctx = context;
    worker->chip_info = chip_info;
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventRead);
}

void candog_worker_verify_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context) {
    furi_check(worker->mode_index == CanDogWorkerModeIdle);
    worker->callback = callback;
    worker->cb_ctx = context;
    worker->chip_info = chip_info;
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventVerify);
}

void candog_worker_erase_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context) {
    furi_check(worker->mode_index == CanDogWorkerModeIdle);
    worker->callback = callback;
    worker->cb_ctx = context;
    worker->chip_info = chip_info;
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventErase);
}

void candog_worker_write_start(
    CanDogChip* chip_info,
    CanDogWorker* worker,
    CanDogWorkerCallback callback,
    void* context) {
    furi_check(worker->mode_index == CanDogWorkerModeIdle);
    worker->callback = callback;
    worker->cb_ctx = context;
    worker->chip_info = chip_info;
    furi_thread_flags_set(furi_thread_get_id(worker->thread), CanDogEventWrite);
}
