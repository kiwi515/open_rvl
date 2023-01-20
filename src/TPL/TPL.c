#include "TPL.h"

#include <OS.h>

#define TPL_VERSION 0x0020AF30

void TPLBind(TPLPalette* pal) {
    u16 i;

#line 24
    OSAssert(pal->version == TPL_VERSION,
             "invalid version number for texture palette");

    pal->imageTable = (TPLDescriptor*)(pal->imageTableOfs + (u32)pal);

    for (i = 0; i < pal->numImages; i++) {
        if (pal->imageTable[i].imageHeader != NULL) {
            pal->imageTable[i].imageHeader =
                (TPLImageHeader*)((char*)pal + pal->imageTable[i].imageOfs);

            if (!pal->imageTable[i].imageHeader->unpacked) {
                pal->imageTable[i].imageHeader->data =
                    (char*)pal + pal->imageTable[i].imageHeader->offset;
                pal->imageTable[i].imageHeader->unpacked = TRUE;
            }
        }

        if (pal->imageTable[i].palHeader != NULL) {
            pal->imageTable[i].palHeader =
                (TPLPaletteHeader*)((char*)pal + pal->imageTable[i].palOfs);

            if (!pal->imageTable[i].palHeader->unpacked) {
                pal->imageTable[i].palHeader->data =
                    (char*)pal + pal->imageTable[i].palHeader->offset;
                pal->imageTable[i].palHeader->unpacked = TRUE;
            }
        }
    }
}

TPLDescriptor* TPLGet(TPLPalette* pal, u32 id) {
    return &pal->imageTable[id % pal->numImages];
}
