#ifndef RVL_FACE_LIBRARY_DATABASE_H
#define RVL_FACE_LIBRARY_DATABASE_H
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/OS.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct MEMiHeapHead;

// TODO: 0x34a009
typedef struct RFLDatabase {
    u8 dummy[0x1F1E0];
} RFLDatabase;

// TODO: IDK what this name is
// 0x3491af
typedef struct RFLTrg {
    u8 dummy[0x4A];
} RFLTrg;

// TODO: 0x34a921
typedef struct RFLDBCharInfo {
    u8 dummy[0x40];
} RFLDBCharInfo;

typedef struct RFLCrcInfo {
    void* head;                // at 0x0
    u32 size;                  // at 0x4
    u8* current;               // at 0x8
    u32 count;                 // at 0xC
    u16 crc;                   // at 0x10
    OSAlarm alarm;             // at 0x18
    RFLAlarmCallback callback; // at 0x48
} RFLCrcInfo;

typedef struct RFLDBManager {
    RFLDatabase* database;          // at 0x0
    RFLDatabaseCallback saveCb;     // at 0x4
    RFLDatabaseCallback formatCb;   // at 0x8
    RFLDatabaseCallback bootloadCb; // at 0xC
    RFLiOpenType saveType;          // at 0x10
    RFLTrg trg;                     // at 0x14
    RFLCrcInfo crcInfo;             // at 0x60
} RFLDBManager;

void RFLiInitDatabase(struct MEMiHeapHead*);

RFLErrcode RFLiBootLoadDatabaseAsync(RFLDatabaseCallback);

#ifdef __cplusplus
}
#endif
#endif
