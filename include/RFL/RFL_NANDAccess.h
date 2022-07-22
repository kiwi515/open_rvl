#ifndef RVL_SDK_RFL_NAND_ACCESS_H
#define RVL_SDK_RFL_NAND_ACCESS_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitAccessInfo(MEMiHeapHead*);
void RFLiExitAccessInfo(void);
BOOL RFLiIsWorking(void);

#ifdef __cplusplus
}
#endif
#endif
