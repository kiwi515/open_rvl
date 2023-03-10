#ifndef RVL_FACE_LIBRARY_INTERNAL_MIDDLE_DATABASE_H
#define RVL_FACE_LIBRARY_INTERNAL_MIDDLE_DATABASE_H
#include <RVLFaceLib/RFL_MiddleDatabase.h>
#include <RVLFaceLib/RFLi_Database.h>
#include <RVLFaceLib/RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiMiddleDB {
    RFLMiddleDBType type;          // at 0x0
    RFLiDatabaseCallback callback; // at 0x4
    RFLiDBCharInfo* data;          // at 0x8
    u16 size;                      // at 0xC
    u16 storedSize;                // at 0xE
    u32 userData1;                 // at 0x10
    u32 userData2;                 // at 0x14
} RFLiMiddleDB;

#ifdef __cplusplus
}
#endif
#endif
