#ifndef RVL_SDK_VF_PF_STRING_H
#define RVL_SDK_VF_PF_STRING_H
#include <revolution/VF/pf.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PFSTR_CODEMODE_NONE,
    PFSTR_CODEMODE_OEM,
    PFSTR_CODEMODE_UNICODE,
    PFSTR_CODEMODE_LOCAL,
} PFSTR_CodeMode;

typedef struct PFSTR {
    const void* begin; // at 0x0
    const void* end;   // at 0x4
    const void* local; // at 0x8
    u32 mode;          // at 0xC
} PFSTR;

void VFiPFSTR_SetCodeMode(PFSTR* str, u32 mode);
u32 VFiPFSTR_GetCodeMode(const PFSTR* str);
void VFiPFSTR_SetLocalStr(PFSTR* str, const wchar_t* data);
const void* VFiPFSTR_GetStrPos(const PFSTR* str, u32 mode);
void VFiPFSTR_MoveStrPos(PFSTR* str, s16 n);
PFResult VFiPFSTR_InitStr(PFSTR* str, const void* data, u32 mode);
u16 VFiPFSTR_StrLen(const PFSTR* str);
u16 VFiPFSTR_StrNumChar(const PFSTR* str, u32 mode);
int VFiPFSTR_StrCmp(const PFSTR* str, const char* cmp);
int VFiPFSTR_StrNCmp(PFSTR* str, const char* cmp, u32 mode, s16 pos, u16 n);
void VFiPFSTR_ToUpperNStr(PFSTR* str, u16 n, char* out);

#ifdef __cplusplus
}
#endif
#endif
