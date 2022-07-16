#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
#include <GX.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

void __GXSetDirtyState(void);

void __GXSendFlushPrim(void);

#ifdef __cplusplus
}
#endif
#endif
