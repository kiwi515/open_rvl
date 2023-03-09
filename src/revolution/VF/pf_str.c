#include "pf_str.h"
#include "pf_clib.h"
#include "pf_volume.h"
#include "pf_w_clib.h"

void VFiPFSTR_SetCodeMode(PFSTR* str, u32 mode) { str->mode = mode; }

u32 VFiPFSTR_GetCodeMode(const PFSTR* str) { return str->mode; }

void VFiPFSTR_SetLocalStr(PFSTR* str, const wchar_t* data) {
    if (VFiPFSTR_GetCodeMode(str) == PFSTR_CODEMODE_OEM || data == NULL) {
        str->local = str->begin;
    } else {
        str->local = data;
    }
}

const void* VFiPFSTR_GetStrPos(const PFSTR* str, u32 mode) {
    if (mode == PFSTR_CODEMODE_OEM) {
        return str->begin;
    }

    if (mode == PFSTR_CODEMODE_UNICODE) {
        return str->end;
    }

    return str->local;
}

void VFiPFSTR_MoveStrPos(PFSTR* str, s16 n) {
    s16 i;
    s16 dist = 0;

    if (VFiPFSTR_GetCodeMode(str) == PFSTR_CODEMODE_OEM) {
        const char* data = (const char*)str->begin;

        for (; n != 0; dist++, n--) {
            if (VFipf_vol_set.oemIsMBChar(*data, 1)) {
                dist++;
            }
        }
    } else {
        const char* data = (const char*)str->begin;

        for (i = 0; i < n; i++) {
            const size_t width = VFipf_vol_set.unicodeCharWidth(data);
            data += width * sizeof(wchar_t);
            dist += (s16)width;
        }
    }

    str->begin = (u8*)str->begin + dist;
}

PFResult VFiPFSTR_InitStr(PFSTR* str, const void* data, u32 mode) {
    if (str == NULL || data == NULL) {
        return PF_RESULT_INVALID;
    }

    if (mode == PFSTR_CODEMODE_OEM) {
        str->begin = data;
        str->end = (u8*)data + VFipf_strlen(data);
    } else if (mode == PFSTR_CODEMODE_UNICODE) {
        str->begin = data;
        str->end = (u8*)data + VFipf_w_strlen(data) * sizeof(wchar_t);
    } else {
        return PF_RESULT_INVALID;
    }

    VFiPFSTR_SetCodeMode(str, mode);
    return PF_RESULT_OK;
}

u16 VFiPFSTR_StrLen(const PFSTR* str) {
    return (uintptr_t)str->end - (uintptr_t)str->begin;
}

u16 VFiPFSTR_StrNumChar(const PFSTR* str, u32 mode) {
    const char* data;
    size_t width;
    u16 length;

    if (mode == PFSTR_CODEMODE_OEM) {
        data = (const char*)str->begin;
    } else {
        data = (const char*)str->end;
    }

    if (VFiPFSTR_GetCodeMode(str) == PFSTR_CODEMODE_OEM) {
        length = 0;
        for (; *data != '\0'; data++, length++) {
            if (VFipf_vol_set.oemIsMBChar(*data, PFSTR_CODEMODE_OEM)) {
                data++;
            }
        }
    } else {
        length = 0;
        for (; data[0] != '\0' || data[1] != '\0'; data += width, length++) {
            width = VFipf_vol_set.unicodeCharWidth(data);
        }
    }

    return length;
}

int VFiPFSTR_StrCmp(const PFSTR* str, const char* cmp) {
    const wchar_t* data;
    wchar_t out;

    if (VFiPFSTR_GetCodeMode(str) == PFSTR_CODEMODE_OEM) {
        return VFipf_strcmp(str->begin, cmp);
    }

    data = (const wchar_t*)str->begin;
    do {
        VFipf_vol_set.convOemToUnicode(cmp, &out);
        cmp++;
    } while (*data++ == out && data[-1] != L'\0' && out != L'\0');

    return data[-1] - out;
}

// str must be non-const to match
int VFiPFSTR_StrNCmp(PFSTR* str, const char* cmp, u32 mode, s16 pos, u16 n) {
    const wchar_t* wdata;
    const char* data;
    wchar_t out;

    if (VFiPFSTR_GetCodeMode(str) == PFSTR_CODEMODE_OEM ||
        mode == PFSTR_CODEMODE_LOCAL) {

        if (mode == PFSTR_CODEMODE_OEM) {
            data = (const char*)str->begin + pos;
        } else if (mode == PFSTR_CODEMODE_UNICODE) {
            data = (const char*)str->end + pos;
        } else {
            data = (const char*)str->local + pos;
        }

        return VFipf_strncmp(data, cmp, n);
    }

    if (mode == PFSTR_CODEMODE_OEM) {
        wdata = (wchar_t*)((u8*)str->begin + pos * sizeof(wchar_t));
    } else {
        wdata = (wchar_t*)((u8*)str->end + pos * sizeof(wchar_t));
    }

    do {
        VFipf_vol_set.convOemToUnicode(cmp, &out);
        cmp++;
        n--;
    } while (*wdata++ == out && n > 0 && wdata[-1] != L'\0' && out != L'\0');

    return wdata[-1] - out;
}

// str must be non-const to match
void VFiPFSTR_ToUpperNStr(PFSTR* str, u16 n, char* out) {
    const char* data;
    const wchar_t* wdata;
    u32 ch;
    BOOL letter;

    if (str->mode == PFSTR_CODEMODE_OEM) {
        for (data = (const char*)str->begin; n > 0 && *data != '\0';
             data++, n--) {
            *out++ = VFipf_toupper(*data);
        }
    } else {
        for (wdata = (const wchar_t*)str->begin; n > 0 && *wdata != L'\0';
             n--, wdata++) {
            letter = *wdata >= 'a' && *wdata <= 'z';

            if (letter) {
                ch = *wdata - ('a' - 'A');
            } else {
                ch = *wdata;
            }

            out[0] = ch >> 0 & 0xFF;
            out[1] = ch >> 8 & 0xFF;
            out += sizeof(wchar_t);
        }

        *out = '\0';
    }

    *out = '\0';
}
