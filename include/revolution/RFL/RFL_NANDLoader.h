#ifndef RVL_SDK_RFL_NAND_LOADER_H
#define RVL_SDK_RFL_NAND_LOADER_H
#include <revolution/RFL/RFL_Model.h>
#include <revolution/RFL/RFL_System.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLiInitLoader(void);
RFLResult RFLiLoadResourceHeaderAsync(void);
u32 RFLiGetTexSize(RFLPartTex, u16);
RFLTexHeader* RFLiLoadTexture(RFLPartTex, u16, void*);
u32 RFLiGetShpTexSize(RFLPartShpTex, u16);
RFLTexHeader* RFLiLoadShpTexture(RFLPartShpTex, u16, void*);
u32 RFLiGetShapeSize(RFLPartShp, u16);
void* RFLiLoadShape(RFLPartShp, u16, void*);
BOOL RFLFreeCachedResource(void);
BOOL RFLIsResourceCached(void);

#ifdef __cplusplus
}
#endif
#endif
