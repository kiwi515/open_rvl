#ifndef RVL_SDK_RFL_NAND_ACCESS_H
#define RVL_SDK_RFL_NAND_ACCESS_H
#include <NAND/NANDOpenClose.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitAccessInfo(MEMiHeapHead*);
void RFLiExitAccessInfo(void);
BOOL RFLiIsWorking(void);

void RFLiEndWorking(RFLError);

RFLError RFLiOpenAsync(RFLAccessType, NANDAccessType, RFLAccessCallback);
RFLError RFLiReadAsync(RFLAccessType, void*, u32, RFLAccessCallback, u32);

void RFLiCloseAsync(RFLAccessType, RFLAccessCallback);

RFLError RFLiGetLengthAsync(RFLAccessType, u32*, RFLAccessCallback);

#ifdef __cplusplus
}
#endif
#endif
