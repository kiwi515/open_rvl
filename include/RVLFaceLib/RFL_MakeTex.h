#ifndef RVL_FACE_LIBRARY_MAKE_TEX_H
#define RVL_FACE_LIBRARY_MAKE_TEX_H
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLCharInfo;

void RFLiMakeTexture(const struct RFLCharInfo* info, u8** texBuffer,
                     RFLResolution res);

RFLiMaskResolution RFLiGetMaxMaskRsl(RFLResolution);

u32 RFLiGetMaskBufSize(RFLResolution);

#ifdef __cplusplus
}
#endif
#endif
