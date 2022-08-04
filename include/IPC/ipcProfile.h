#ifndef RVL_SDK_IPC_PROFILE_H
#define RVL_SDK_IPC_PROFILE_H
#include "ipcclt.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void IPCiProfInit(void);
void IPCiProfQueueReq(IPCRequestEx*, s32);
void IPCiProfAck(void);
void IPCiProfReply(IPCRequestEx*, s32);

#ifdef __cplusplus
}
#endif
#endif
