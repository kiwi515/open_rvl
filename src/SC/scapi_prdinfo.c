#include "scapi_prdinfo.h"

#include <string.h>

#include <OS/OSAddress.h>

#define XOR_KEY 0x73B5DBFA
#define SETTING_TXT_SIZE 0x100

static SCRegion ProductAreaAndStringTbl[] = {{SC_AREA_JPN, "JPN"},
                                             {SC_AREA_USA, "USA"},
                                             {SC_AREA_EUR, "EUR"},
                                             {SC_AREA_AUS, "AUS"},
                                             {SC_AREA_BRA, "BRA"},
                                             {SC_AREA_TWN, "TWN"},
                                             {SC_AREA_TWN, "ROC"},
                                             {SC_AREA_KOR, "KOR"},
                                             {SC_AREA_HKG, "HKG"},
                                             {SC_AREA_ASI, "ASI"},
                                             {SC_AREA_LTN, "LTN"},
                                             {SC_AREA_SAF, "SAF"},
                                             {-1, ""}};

BOOL __SCF1(const char* type, char* buf, u32 sz) {
    u8 ptext;
    BOOL found = FALSE;
    u32 i;
    const u8* settings = (const u8*)OSPhysicalToCached(OS_PHYS_SC_SETTING_TXT);
    u32 key = XOR_KEY;
    u32 type_ofs = 0;
    u32 buf_ofs = 0;

    for (i = 0; i < SETTING_TXT_SIZE; i++, key = key >> 31 | key << 1) {
        ptext = settings[i];

        if (ptext != 0x00) {
            ptext ^= key;
            if (type[type_ofs] == 0x00 && ptext == '=') {
                found = TRUE;
                break;
            }

            if (((ptext ^ type[type_ofs]) & 0xDF) == 0) {
                type_ofs++;
            } else {
                type_ofs = 0;
            }
        }
    }

    if (found) {
        for (i++; i < SETTING_TXT_SIZE && buf_ofs < sz; i++) {
            key = key >> 31 | key << 1;
            ptext = settings[i];

            if (ptext != 0x00) {
                ptext ^= key;
                if (ptext == '\r' || ptext == '\n') {
                    ptext = 0x00;
                }
            }

            buf[buf_ofs++] = ptext;

            if (ptext == 0x00) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL SCGetProductAreaString(char* buf, u32 sz) {
    return __SCF1("AREA", buf, sz);
}

s8 SCGetProductArea(void) {
    s8 area;
    char name[4];
    SCRegion* iter = ProductAreaAndStringTbl;

    if (SCGetProductAreaString(name, sizeof(name))) {
        for (; (area = iter->area) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return area;
            }
        }
    }

    return -1;
}
