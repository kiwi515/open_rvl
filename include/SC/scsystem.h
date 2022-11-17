#ifndef RVL_SDK_SC_SCSYSTEM_H
#define RVL_SDK_SC_SCSYSTEM_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SC_ITEM_IPL_CB = 0,
    SC_ITEM_IPL_AR = 1,
    SC_ITEM_IPL_DH = 5,
    SC_ITEM_IPL_E60 = 6,
    SC_ITEM_IPL_IDL = 9,
    SC_ITEM_IPL_LNG = 11,
    SC_ITEM_IPL_PGS = 14,
    SC_ITEM_IPL_SSV = 15,
    SC_ITEM_IPL_SND = 17,
    SC_ITEM_BT_DINF = 28,
    SC_ITEM_BT_SENS = 29,
    SC_ITEM_BT_SPKV = 30,
    SC_ITEM_BT_MOT = 31,
    SC_ITEM_BT_BAR = 32,
} SCItemID;

BOOL SCFindByteArrayItem(void*, u32, u32);
BOOL SCReplaceByteArrayItem(const void*, u32, u32);

BOOL SCFindU8Item(u8*, u32);
BOOL SCFindS8Item(s8*, u32);
BOOL SCFindU32Item(u32*, u32);

BOOL SCReplaceU8Item(u8, u32);

#ifdef __cplusplus
}
#endif
#endif
