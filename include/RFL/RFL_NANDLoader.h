#ifndef RVL_SDK_RFL_NAND_LOADER_H
#define RVL_SDK_RFL_NAND_LOADER_H
#include "RFL_System.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitLoader(void);

RFLError RFLiLoadResourceHeaderAsync(void);

BOOL RFLFreeCachedResource(void);
BOOL RFLIsResourceCached(void);

#ifdef __cplusplus
}
#endif
#endif
