#ifndef RVL_SDK_GX_H
#define RVL_SDK_GX_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

extern volatile union {
    char c;
    short s;
    int i;
    void* p;
    float f;
} WGPIPE : 0xcc008000;

#ifdef __cplusplus
}
#endif
#endif
