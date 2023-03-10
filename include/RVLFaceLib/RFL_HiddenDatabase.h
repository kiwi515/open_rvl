#ifndef RVL_FACE_LIBRARY_HIDDEN_DATABASE_H
#define RVL_FACE_LIBRARY_HIDDEN_DATABASE_H
#include <RVLFaceLib/RFL_Database.h>
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// 34b4e4:
typedef struct RFLHDBList {
    RFLDBCharInfo data[RFL_DB_CHAR_MAX]; // at 0x0
    u16 num;                             // at 0x1900
    u8 current;                          // at 0x1901
} RFLHDBList;

// 0x34b561
typedef struct RFLHDBManager {
    BOOL loaded;                  // at 0x0
    BOOL readError;               // at 0x4
    void* writeTmp;               // at 0x8
    s16 writeIndex;               // at 0xC
    RFLDatabaseCallback writeCb;  // at 0x10
    void* formatTmp;              // at 0x14
    RFLDatabaseCallback formatCb; // at 0x18
    s16 formatIndex;              // at 0x1C
    RFLDBCharInfo* loadDst;       // at 0x20
    void* loadTmp;                // at 0x24
    u32 loadArg;                  // at 0x28
    RFLAlarmCallback loadCb;      // at 0x2C
    u16 loadIndex;                // at 0x30
    RFLDBCharInfo* cachedDB;      // at 0x34
    BOOL cached;                  // at 0x38
    RFLHDBList list;              // at 0x3C
} RFLHDBManager;

void RFLiInitHiddenDatabase(void);

#ifdef __cplusplus
}
#endif
#endif
