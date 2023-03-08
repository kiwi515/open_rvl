#ifndef RVL_SDK_RFL_NAND_ACCESS_H
#define RVL_SDK_RFL_NAND_ACCESS_H
#include <revolution/RFL/RFL_System.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitAccessInfo(MEMiHeapHead*);
void RFLiExitAccessInfo(void);
BOOL RFLiIsWorking(void);
void RFLiStartWorking(void);
void RFLiEndWorkingReason(RFLResult, NANDResult);
void RFLiEndWorking(RFLResult);
NANDCommandBlock* RFLiSetCommandBlock(RFLAccessType, UNKWORD);
RFLAccessType RFLiGetType(NANDCommandBlock*);
RFLResult RFLiOpenAsync(RFLAccessType, u8, RFLAccessCallback);
RFLResult RFLiReadAsync(RFLAccessType, void*, u32, RFLAccessCallback, s32);
RFLResult RFLiWriteAsync(RFLAccessType, void*, u32, RFLAccessCallback, s32);
RFLResult RFLiCloseAsync(RFLAccessType, RFLAccessCallback);
RFLResult RFLiGetLengthAsync(RFLAccessType, u32*, RFLAccessCallback);
RFLResult RFLiDeleteAsync(RFLAccessType, RFLAccessCallback);
RFLResult RFLiCreateSaveDirAsync(RFLAccessCallback);

#ifdef __cplusplus
}
#endif
#endif
