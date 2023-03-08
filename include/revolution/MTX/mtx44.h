#ifndef RVL_SDK_MTX_MTX44_H
#define RVL_SDK_MTX_MTX44_H
#include <revolution/MTX/mtx.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void C_MTXFrustum(Mtx44, f32, f32, f32, f32, f32, f32);
void C_MTXPerspective(Mtx44, f32, f32, f32, f32);
void C_MTXOrtho(Mtx44, f32, f32, f32, f32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
