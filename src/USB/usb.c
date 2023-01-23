#include <FS.h>
#include <OS.h>
#include <USB.h>

#include <stdio.h>

#define USB_HEAP_SIZE 0x1000
#define USB_MAX_NCLEAN 8

typedef struct USBCommandBlock {
    USBCallback callback; // at 0x0
    void* callbackArg;    // at 0x4
    char UNK_0x8[0x4];
    void* clean[USB_MAX_NCLEAN]; // at 0xC
    u32 nclean;                  // at 0x2C
    char UNK_0x30[0x40 - 0x30];
    char path[FS_MAX_PATH]; // at 0x40
} USBCommandBlock;

static s32 hId = -1;

static void* hi;
static void* lo;

static u8 s_usb_log = FALSE;

void USB_LOG(const char* fmt, ...) {
    va_list list;

    if (s_usb_log) {
        OSReport("USB: ");
        va_start(list, fmt);
        vprintf(fmt, list);
        va_end(list);
    }
}

void USB_ERR(const char* fmt, ...) {
    va_list list;

    if (s_usb_log) {
        OSReport("USB ERR: ");
        va_start(list, fmt);
        vprintf(fmt, list);
        va_end(list);
    }
}

static void* IOSAlloc(size_t size) {
    void* mem = iosAllocAligned(hId, size, 32);

    if (mem == NULL) {
        USB_ERR("iosAllocAligned(%d, %u) failed: %d\n", hId, size, mem);
    }

    return mem;
}

static void IOSFree(void* mem) {
    s32 result;

    if (mem != NULL) {
        result = iosFree(hId, mem);

        if (result < 0) {
            USB_ERR("iosFree(%d, 0x%x) failed: %d\n", hId, mem, result);
        }
    }
}

IPCResult IUSB_OpenLib(void) {
    IPCResult result;
    BOOL enabled;

    result = IPC_RESULT_OK;
    enabled = OSDisableInterrupts();

    if (hId != -1) {
        USB_LOG("Library is already initialized. Heap Id = %d\n", hId);
        goto end;
    }

    if (lo == NULL) {
        lo = IPCGetBufferLo();
        hi = IPCGetBufferHi();

        USB_LOG("iusb size: %d lo: %x hi: %x\n", sizeof(USBCommandBlock), lo,
                hi);

        if ((u8*)lo + USB_HEAP_SIZE > hi) {
            USB_ERR("Not enough IPC arena\n");
            result = IPC_RESULT_ALLOC_FAILED;
            goto end;
        }

        IPCSetBufferLo((u8*)lo + USB_HEAP_SIZE);
    }

    hId = iosCreateHeap(lo, USB_HEAP_SIZE);
    if (hId < 0) {
        USB_ERR("Not enough heaps\n");
        result = IPC_RESULT_ALLOC_FAILED;
    }

end:
    OSRestoreInterrupts(enabled);
    return result;
}

IPCResult IUSB_CloseLib(void) { return IPC_RESULT_OK; }

static IPCResult _intrBlkCtrlCb(IPCResult result, void* arg) {
    int i;
    USBCommandBlock* block = (USBCommandBlock*)arg;

    USB_LOG("_intrBlkCtrlCb returned: %d\n", result);
    USB_LOG("_intrBlkCtrlCb: nclean = %d\n", block->nclean);

    if (block->nclean != 7 && block->nclean != 3 && block->nclean != 0) {
        USB_ERR("__intBlkCtrlCb: got invalid nclean\n");
    } else {
        for (i = 0; i < block->nclean; i++) {
            USB_LOG("Freeing clean[%d] = %x\n", i, block->clean[i]);
            IOSFree(block->clean[i]);
        }
    }

    USB_LOG("cb = %x cbArg = %x\n", block->callback, block->callbackArg);

    if (block->callback != NULL) {
        block->callback(result, block->callbackArg);
    }

    IOSFree(block);
    return result;
}

IPCResult IUSB_OpenDeviceIds(const char* module, u16 vid, u16 pid,
                             IPCResult* resultOut) {
    IPCResult result;
    USBCommandBlock* block;

    block = NULL;

    if (resultOut == NULL) {
        result = IPC_RESULT_INVALID_INTERNAL;
        goto end;
    }

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("OpenDeviceIds: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    snprintf(block->path, sizeof(block->path), "/dev/usb/%s/%x/%x", module, vid,
             pid);
    USB_LOG("OpenDevice - %s\n", block->path);

    result = IOS_Open(block->path, IPC_OPEN_NONE);
    USB_LOG("OpenDevice returned: %d\n", result);

    *resultOut = result;

end:
    IOSFree(block);
    return result;
}

IPCResult IUSB_CloseDeviceAsync(s32 fd, USBCallback callback,
                                void* callbackArg) {
    IPCResult result;
    USBCommandBlock* block;

    USB_LOG("CloseDevice\n");

    block = IOSAlloc(sizeof(USBCommandBlock));
    if (block == NULL) {
        USB_ERR("CloseDeviceAsync: Not enough memory\n");
        result = IPC_RESULT_ALLOC_FAILED;
        goto end;
    }

    block->callback = callback;
    block->callbackArg = callbackArg;
    block->nclean = 0;

    result = IOS_CloseAsync(fd, _intrBlkCtrlCb, block);
    USB_LOG("CloseDevice returned: %d\n", result);

    if (result < 0) {
        IOSFree(block);
    }

end:
    return result;
}

static IPCResult __IntrBlkMsgInt(s32 fd, UNKWORD arg2, u32 size, void* buffer,
                                 USBCallback callback, void* callbackArg);

// IUSB_ReadIntrMsgAsync
// IUSB_ReadBlkMsgAsync
// IUSB_WriteBlkMsgAsync
// __CtrlMsgInt
// IUSB_WriteCtrlMsgAsync
