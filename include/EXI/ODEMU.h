#ifndef RVL_SDK_EXI_ODEMU_H
#define RVL_SDK_EXI_ODEMU_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline u32 ODEMUGetSize(u32 mail) { return mail & 0x1FFF; }

static inline BOOL ODEMUIsValidMail(u32 mail) {
    return (mail & 0x1F000000) == 0x1F000000;
}

static inline u32 ODEMUGetPage(u32 mail) { return (mail & 0xFF0000) >> 16; }

static inline u32 ODEMUGetPc2NngcOffset(u32 mail) {
    if (!(ODEMUGetPage(mail) & 0x1)) {
        return 0;
    }

    return 0x800;
}

static inline u32 ODEMUGenMailData(u32 ofs, u32 size) {
    return (ofs & 0xff) << 0x10 | 0x1f000000 | size & 0x1fff;
}

#ifdef __cplusplus
}
#endif
#endif
