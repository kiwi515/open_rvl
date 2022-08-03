#include "ipcMain.h"

#include <OS/OSIpc.h>

static u8 Initialized = FALSE;
static void* IPCCurrentBufferLo = NULL;
static void* IPCCurrentBufferHi = NULL;
static void* IPCBufferLo = NULL;
static void* IPCBufferHi = NULL;

void IPCInit(void) {
    if (!Initialized) {
        IPCBufferHi = __OSGetIPCBufferHi();
        IPCBufferLo = __OSGetIPCBufferLo();
        IPCCurrentBufferHi = IPCBufferHi;
        IPCCurrentBufferLo = IPCBufferLo;
        Initialized = TRUE;
    }
}

u32 IPCReadReg(s32 idx) { return IPC_REGS[idx]; }

void IPCWriteReg(s32 idx, u32 val) { IPC_REGS[idx] = val; }

void* IPCGetBufferHi(void) { return IPCCurrentBufferHi; }

void* IPCGetBufferLo(void) { return IPCCurrentBufferLo; }

void IPCSetBufferLo(void* lo) { IPCCurrentBufferLo = lo; }
