#ifndef RVL_SDK_VF_PF_VOLUME_H
#define RVL_SDK_VF_PF_VOLUME_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef size_t (*PF_ConvertCharFunc)(const void* src, void* dst);
typedef size_t (*PF_UnicodeCharWidthFunc)(const void* data);
typedef size_t (*PF_OemCharWidthFunc)(int ch);
typedef BOOL (*PF_IsMBCharFunc)(char ch, u32 mode);

typedef struct PF_VOL_SET {
    char UNK_0x0[0x24];
    PF_ConvertCharFunc convOemToUnicode;      // at 0x24
    PF_ConvertCharFunc convUnicodeToOem;      // at 0x28
    PF_OemCharWidthFunc oemCharWidth;         // at 0x2C
    PF_IsMBCharFunc oemIsMBChar;              // at 0x30
    PF_UnicodeCharWidthFunc unicodeCharWidth; // at 0x34
    PF_IsMBCharFunc unicodeIsMBChar;          // at 0x38
} PF_VOL_SET;

extern PF_VOL_SET VFipf_vol_set;

#ifdef __cplusplus
}
#endif
#endif
