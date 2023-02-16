#ifndef RVL_SDK_OS_FAST_CAST_H
#define RVL_SDK_OS_FAST_CAST_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

static f32 __OSu16tof32(register const u16* arg) {
    register f32 ret;

    asm
    {
		psq_l ret, 0(arg), 1, 3
    }

    return ret;
}

static void OSu16tof32(const u16* in, f32* out) { *out = __OSu16tof32(in); }

static u16 __OSf32tou16(register f32 arg) {
    u16 ret[2];
    register u16* ptr = ret;

    asm
    {
		psq_st arg, 0(ptr), 1, 3
    }

    return *ptr;
}

static void OSf32tou16(const f32* in, u16* out) { *out = __OSf32tou16(*in); }

static f32 __OSs16tof32(register const s16* arg) {
    register f32 ret;

    asm
    {
		psq_l ret, 0(arg), 1, 5
    }

    return ret;
}

static void OSs16tof32(const s16* in, f32* out) { *out = __OSs16tof32(in); }

static s16 __OSf32tos16(register f32 arg) {
    s16 ret[2];
    register s16* ptr = ret;

    asm
    {
		psq_st arg, 0(ptr), 1, 5
    }

    return *ptr;
}

static void OSf32tos16(const f32* in, s16* out) { *out = __OSf32tos16(*in); }

#ifdef __cplusplus
}
#endif
#endif
