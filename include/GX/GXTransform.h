#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H
#include "GX.h"
#include "GXTev.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void __GXSetProjection(void);

void __GXSetViewport(void);

void __GXSetMatrixIndex(UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
