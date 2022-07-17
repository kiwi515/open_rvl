#ifndef MSL_MATH_DOUBLE_H
#define MSL_MATH_DOUBLE_H
#include "s_cos.h"
#include "w_sqrt.h"
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline f32 sqrtf(f32 x) { return sqrt(x); }
static inline f32 cosf(f32 x) { return cos(x); }

#ifdef __cplusplus
}
#endif
#endif
