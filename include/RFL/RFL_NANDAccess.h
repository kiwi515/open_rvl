#ifndef RVL_SDK_RFL_NAND_ACCESS_H
#define RVL_SDK_RFL_NAND_ACCESS_H
#include "RFL_System.h"
#include <NAND/NANDOpenClose.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitAccessInfo(MEMiHeapHead*);
void RFLiExitAccessInfo(void);
BOOL RFLiIsWorking(void);
void RFLiStartWorking(void);
void RFLiEndWorkingReason(RFLResult, RFLReason);
void RFLiEndWorking(RFLResult);
void RFLiSetCommandBlock(RFLAccessType, UNKWORD);
NANDFileInfo* RFLiGetWorkingFile(RFLAccessType);

RFLResult RFLiOpenAsync(RFLAccessType, NANDAccessType, RFLAccessCallback);

RFLResult RFLiReadAsync(RFLAccessType, void*, u32, RFLAccessCallback, u32);

void RFLiCloseAsync(RFLAccessType, RFLAccessCallback);

RFLResult RFLiGetLengthAsync(RFLAccessType, u32*, RFLAccessCallback);

#ifdef __cplusplus
}
#endif
#endif
