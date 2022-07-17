#ifndef RVL_SDK_GX_H
#define RVL_SDK_GX_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Pack value into bitfield
#define GX_BITFIELD_SET(field, pos, size, value)                               \
    (field) =                                                                  \
        __rlwimi((field), (value), (pos), 31 - (pos) - (size) + 1, 31 - (pos))

extern volatile union {
    char c;
    short s;
    int i;
    void* p;
    float f;
} WGPIPE : 0xcc008000;

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

#ifdef __cplusplus
}
#endif
#endif
