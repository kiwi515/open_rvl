#ifndef RVL_SDK_OS_MESSAGE_H
#define RVL_SDK_OS_MESSAGE_H
#include <OS/OSThread.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// General-purpose typedef
typedef void* OSMessage;

typedef enum { OS_MSG_PERSISTENT = (1 << 0) } OSMessageFlags;

typedef struct OSMessageQueue {
    OSThreadQueue sendQueue; // at 0x0
    OSThreadQueue recvQueue; // at 0x8
    OSMessage* buffer;       // at 0x10
    s32 capacity;            // at 0x14
    s32 front;               // at 0x18
    s32 size;                // at 0x1C
} OSMessageQueue;

void OSInitMessageQueue(OSMessageQueue*, OSMessage*, s32);
BOOL OSSendMessage(OSMessageQueue*, OSMessage, u32);
BOOL OSReceiveMessage(OSMessageQueue*, OSMessage*, u32);
BOOL OSJamMessage(OSMessageQueue*, OSMessage, u32);

#ifdef __cplusplus
}
#endif
#endif
