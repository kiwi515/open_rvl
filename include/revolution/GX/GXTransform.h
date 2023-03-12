#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H
#include <revolution/GX/GXTypes.h>
#include <revolution/MTX.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void __GXSetProjection(void);

void GXLoadPosMtxImm(const Mtx, u32);

void GXLoadNrmMtxImm(const Mtx, u32);

void GXSetCurrentMtx(u32);

void __GXSetViewport(void);

void __GXSetMatrixIndex(UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
