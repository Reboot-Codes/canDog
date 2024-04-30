#include "candog_worker_i.h"
#include "candog_chip.h"
#include "candog_tools.h"
#include "../../candog_files.h"

static void candog_worker_chip_detect_process(CanDogWorker* worker);
static void candog_worker_read_process(CanDogWorker* worker);
static void candog_worker_verify_process(CanDogWorker* worker);
static void candog_worker_erase_process(CanDogWorker* worker);
static void candog_worker_write_process(CanDogWorker* worker);

const CanDogWorkerModeType candog_worker_modes[] = {
    [CanDogWorkerModeIdle] = {.process = NULL},
    [CanDogWorkerModeChipDetect] = {.process = candog_worker_chip_detect_process},
    [CanDogWorkerModeRead] = {.process = candog_worker_read_process},
    [CanDogWorkerModeVerify] = {.process = candog_worker_verify_process},
    [CanDogWorkerModeErase] = {.process = candog_worker_erase_process},
    [CanDogWorkerModeWrite] = {.process = candog_worker_write_process} };

static void candog_worker_run_callback(CanDogWorker* worker, CanDogCustomEventWorker event) {
    if (worker->callback) {
        worker->callback(worker->cb_ctx, event);
    }
}

static bool candog_worker_await_chip_busy(CanDogWorker* worker) {
    while (true) {
        furi_delay_tick(10); // to give some time to OS
        if (candog_worker_check_for_stop(worker)) return true;
        CanDogChipStatus chip_status = candog_tools_get_chip_status(worker->chip_info);
        if (chip_status == CanDogChipStatusError) return false;
        if (chip_status == CanDogChipStatusBusy) continue;
        return true;
    }
}

static size_t candog_worker_modes_get_total_size(CanDogWorker* worker) {
    size_t chip_size = candog_chip_get_size(worker->chip_info);
    size_t file_size = candog_file_get_size(worker->cb_ctx);
    size_t total_size = chip_size;
    if (chip_size > file_size) total_size = file_size;
    return total_size;
}

// ChipDetect
static void candog_worker_chip_detect_process(CanDogWorker* worker) {
    CanDogCustomEventWorker event;
    while (!candog_tools_read_chip_info(worker->chip_info)) {
        furi_delay_tick(10); // to give some time to OS
        if (candog_worker_check_for_stop(worker)) return;
    }
    if (candog_chip_find_all(worker->chip_info, *worker->found_chips)) {
        event = CanDogCustomEventWorkerChipIdentified;
    }
    else {
        event = CanDogCustomEventWorkerChipUnknown;
    }
    candog_worker_run_callback(worker, event);
}

// Read
static bool candog_worker_read(CanDogWorker* worker, CanDogCustomEventWorker* event) {
    uint8_t data_buffer[SPI_MEM_FILE_BUFFER_SIZE];
    size_t chip_size = candog_chip_get_size(worker->chip_info);
    size_t offset = 0;
    bool success = true;
    while (true) {
        furi_delay_tick(10); // to give some time to OS
        size_t block_size = SPI_MEM_FILE_BUFFER_SIZE;
        if (candog_worker_check_for_stop(worker)) break;
        if (offset >= chip_size) break;
        if ((offset + block_size) > chip_size) block_size = chip_size - offset;
        if (!candog_tools_read_block(worker->chip_info, offset, data_buffer, block_size)) {
            *event = CanDogCustomEventWorkerChipFail;
            success = false;
            break;
        }
        if (!candog_file_write_block(worker->cb_ctx, data_buffer, block_size)) {
            success = false;
            break;
        }
        offset += block_size;
        candog_worker_run_callback(worker, CanDogCustomEventWorkerBlockReaded);
    }
    if (success) *event = CanDogCustomEventWorkerDone;
    return success;
}

static void candog_worker_read_process(CanDogWorker* worker) {
    CanDogCustomEventWorker event = CanDogCustomEventWorkerFileFail;
    do {
        if (!candog_worker_await_chip_busy(worker)) break;
        if (!candog_file_create_open(worker->cb_ctx)) break;
        if (!candog_worker_read(worker, &event)) break;
    } while (0);
    candog_file_close(worker->cb_ctx);
    candog_worker_run_callback(worker, event);
}

// Verify
static bool
candog_worker_verify(CanDogWorker* worker, size_t total_size, CanDogCustomEventWorker* event) {
    uint8_t data_buffer_chip[SPI_MEM_FILE_BUFFER_SIZE];
    uint8_t data_buffer_file[SPI_MEM_FILE_BUFFER_SIZE];
    size_t offset = 0;
    bool success = true;
    while (true) {
        furi_delay_tick(10); // to give some time to OS
        size_t block_size = SPI_MEM_FILE_BUFFER_SIZE;
        if (candog_worker_check_for_stop(worker)) break;
        if (offset >= total_size) break;
        if ((offset + block_size) > total_size) block_size = total_size - offset;
        if (!candog_tools_read_block(worker->chip_info, offset, data_buffer_chip, block_size)) {
            *event = CanDogCustomEventWorkerChipFail;
            success = false;
            break;
        }
        if (!candog_file_read_block(worker->cb_ctx, data_buffer_file, block_size)) {
            success = false;
            break;
        }
        if (memcmp(data_buffer_chip, data_buffer_file, block_size) != 0) {
            *event = CanDogCustomEventWorkerVerifyFail;
            success = false;
            break;
        }
        offset += block_size;
        candog_worker_run_callback(worker, CanDogCustomEventWorkerBlockReaded);
    }
    if (success) *event = CanDogCustomEventWorkerDone;
    return success;
}

static void candog_worker_verify_process(CanDogWorker* worker) {
    CanDogCustomEventWorker event = CanDogCustomEventWorkerFileFail;
    size_t total_size = candog_worker_modes_get_total_size(worker);
    do {
        if (!candog_worker_await_chip_busy(worker)) break;
        if (!candog_file_open(worker->cb_ctx)) break;
        if (!candog_worker_verify(worker, total_size, &event)) break;
    } while (0);
    candog_file_close(worker->cb_ctx);
    candog_worker_run_callback(worker, event);
}

// Erase
static void candog_worker_erase_process(CanDogWorker* worker) {
    CanDogCustomEventWorker event = CanDogCustomEventWorkerChipFail;
    do {
        if (!candog_worker_await_chip_busy(worker)) break;
        if (!candog_tools_erase_chip(worker->chip_info)) break;
        if (!candog_worker_await_chip_busy(worker)) break;
        event = CanDogCustomEventWorkerDone;
    } while (0);
    candog_worker_run_callback(worker, event);
}

// Write
static bool candog_worker_write_block_by_page(
    CanDogWorker* worker,
    size_t offset,
    uint8_t* data,
    size_t block_size,
    size_t page_size) {
    for (size_t i = 0; i < block_size; i += page_size) {
        if (!candog_worker_await_chip_busy(worker)) return false;
        if (!candog_tools_write_bytes(worker->chip_info, offset, data, page_size)) return false;
        offset += page_size;
        data += page_size;
    }
    return true;
}

static bool
candog_worker_write(CanDogWorker* worker, size_t total_size, CanDogCustomEventWorker* event) {
    bool success = true;
    uint8_t data_buffer[SPI_MEM_FILE_BUFFER_SIZE];
    size_t page_size = candog_chip_get_page_size(worker->chip_info);
    size_t offset = 0;
    while (true) {
        furi_delay_tick(10); // to give some time to OS
        size_t block_size = SPI_MEM_FILE_BUFFER_SIZE;
        if (candog_worker_check_for_stop(worker)) break;
        if (offset >= total_size) break;
        if ((offset + block_size) > total_size) block_size = total_size - offset;
        if (!candog_file_read_block(worker->cb_ctx, data_buffer, block_size)) {
            *event = CanDogCustomEventWorkerFileFail;
            success = false;
            break;
        }
        if (!candog_worker_write_block_by_page(
            worker, offset, data_buffer, block_size, page_size)) {
            success = false;
            break;
        }
        offset += block_size;
        candog_worker_run_callback(worker, CanDogCustomEventWorkerBlockReaded);
    }
    return success;
}

static void candog_worker_write_process(CanDogWorker* worker) {
    CanDogCustomEventWorker event = CanDogCustomEventWorkerChipFail;
    size_t total_size =
        candog_worker_modes_get_total_size(worker); // need to be executed before opening file
    do {
        if (!candog_file_open(worker->cb_ctx)) break;
        if (!candog_worker_await_chip_busy(worker)) break;
        if (!candog_worker_write(worker, total_size, &event)) break;
        if (!candog_worker_await_chip_busy(worker)) break;
        event = CanDogCustomEventWorkerDone;
    } while (0);
    candog_file_close(worker->cb_ctx);
    candog_worker_run_callback(worker, event);
}
