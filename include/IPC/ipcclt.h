#ifndef RVL_SDK_IPC_CLT_H
#define RVL_SDK_IPC_CLT_H
#include <OS/OSThread.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Documentation from:
 * https://wiibrew.org/wiki/IOS
 * https://wiibrew.org/wiki/IOS/Resource_request
 * http://wiibrew.org/wiki/IPC_(SDK)
 */

typedef enum {
    IPC_RESULT_NO_PRIVATE_AUTH = -102,

    IPC_RESULT_ALLOC_FAILED = -22,
    IPC_RESULT_BUSY = -8,
    IPC_RESULT_INVALID = -4,

    IPC_RESULT_OK = 0
} IPCResult;

typedef enum {
    IPC_REQ_NONE,
    IPC_REQ_OPEN,
    IPC_REQ_CLOSE,
    IPC_REQ_READ,
    IPC_REQ_WRITE,
    IPC_REQ_SEEK,
    IPC_REQ_IOCTL,
    IPC_REQ_IOCTLV
} IPCRequestType;

typedef enum {
    // TO-DO
    IPC_IOCTL_TYPE_0,
} IPCIoctlType;

typedef enum {
    IPC_IOCTLV_GET_DATA_DIR = 29,
    IPC_IOCTLV_GET_TITLE_ID = 32,
} IPCIoctlvType;

typedef enum {
    IPC_OPEN_NONE,
    IPC_OPEN_READ,
    IPC_OPEN_WRITE,
    IPC_OPEN_3
} IPCOpenMode;

typedef enum {
    IPC_SEEK_BEG,
    IPC_SEEK_CUR,
    IPC_SEEK_END,
} IPCSeekMode;

typedef void (*IPCAsyncCallback)(IPCResult, void*);

typedef struct IPCIOVector {
    void* base; // at 0x0
    u32 length; // at 0x4
} IPCIOVector;

typedef struct IPCOpenArgs {
    const char* path; // at 0x0
    IPCOpenMode mode; // at 0x4
} IPCOpenArgs;

typedef struct IPCReadWriteArgs {
    void* data; // at 0x0
    u32 length; // at 0x4
} IPCReadWriteArgs;

typedef struct IPCSeekArgs {
    s32 offset;       // at 0x0
    IPCSeekMode mode; // at 0x4
} IPCSeekArgs;

typedef struct IPCIoctlArgs {
    IPCIoctlType type; // at 0x0
    void* in;          // at 0x4
    s32 inSize;        // at 0x8
    void* out;         // at 0xC
    s32 outSize;       // at 0x10
} IPCIoctlArgs;

typedef struct IPCIoctlvArgs {
    IPCIoctlType type;    // at 0x0
    u32 inCount;          // at 0x4
    u32 ioCount;          // at 0x8
    IPCIOVector* vectors; // at 0xC
} IPCIoctlvArgs;

typedef struct IPCRequest {
    IPCRequestType type; // at 0x0
    s32 ret;             // at 0x4
    s32 fd;              // at 0x8
    union {
        IPCOpenArgs open;
        IPCReadWriteArgs rw;
        IPCSeekArgs seek;
        IPCIoctlArgs ioctl;
        IPCIoctlvArgs ioctlv;
    }; // at 0xC
} IPCRequest;

typedef struct IPCRequestEx {
    IPCRequest base;           // at 0x0
    IPCAsyncCallback callback; // at 0x20
    void* callbackArg;         // at 0x24
    BOOL reboot;               // at 0x28
    OSThreadQueue queue;       // at 0x2C
    char padding[64 - 0x34];
} IPCRequestEx;

IPCResult IPCCltInit(void);
IPCResult IOS_OpenAsync(const char*, IPCOpenMode, IPCAsyncCallback, void*);
IPCResult IOS_Open(const char*, IPCOpenMode);
IPCResult IOS_CloseAsync(s32, IPCAsyncCallback, void*);
IPCResult IOS_Close(s32);
IPCResult IOS_ReadAsync(s32, void*, s32, IPCAsyncCallback, void*);
IPCResult IOS_Read(s32, void*, s32);
IPCResult IOS_WriteAsync(s32, const void*, s32, IPCAsyncCallback, void*);
IPCResult IOS_Write(s32, const void*, s32);
IPCResult IOS_SeekAsync(s32, s32, IPCSeekMode, IPCAsyncCallback, void*);
IPCResult IOS_Seek(s32, s32, IPCSeekMode);
IPCResult IOS_IoctlAsync(s32, IPCIoctlType, void*, s32, void*, s32,
                         IPCAsyncCallback, void*);
IPCResult IOS_Ioctl(s32, IPCIoctlType, void*, s32, void*, s32);
IPCResult IOS_IoctlvAsync(s32, IPCIoctlType, s32, s32, IPCIOVector*,
                          IPCAsyncCallback, void*);
IPCResult IOS_Ioctlv(s32, IPCIoctlType, s32, s32, IPCIOVector*);
IPCResult IOS_IoctlvReboot(s32, IPCIoctlType, s32, s32, IPCIOVector*);

#ifdef __cplusplus
}
#endif
#endif
