#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x, y, z;
} Vec;

void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXInverse(const Mtx, Mtx);
void PSMTXInvXpose(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXRotAxisRad(Mtx, const struct Vec*, f32);
void PSMTXTransApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);
void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);

void C_MTXLookAt(Mtx, Vec*, Vec*, Vec*);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
