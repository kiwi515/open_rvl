#include "OSIpc.h"

static void* IpcBufferLo = (void*)-1;
static void* IpcBufferHi = NULL;

void* __OSGetIPCBufferHi(void) { return IpcBufferHi; }

void* __OSGetIPCBufferLo(void) { return IpcBufferLo; }

void __OSInitIPCBuffer(void) {
    IpcBufferLo = OS_IPC_BUFFER_LO;
    IpcBufferHi = OS_IPC_BUFFER_HI;
}
