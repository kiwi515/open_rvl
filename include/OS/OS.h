#ifndef RVL_SDK_OS_H
#define RVL_SDK_OS_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

volatile u32 OS_PI_REG : 0xcc003000;

typedef struct {
    u8 id;         // at 0x0
    u8 ver_major;  // at 0x1
    u8 ver_minor;  // at 0x2
    u8 build_mon;  // at 0x3
    u8 build_day;  // at 0x4
    u8 build_year; // at 0x5
} OSIOSRev;

void __OSGetIOSRev(OSIOSRev*);

#ifdef __cplusplus
}
#endif
#endif
