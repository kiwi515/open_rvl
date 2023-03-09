#ifndef RVL_FACE_LIBRARY_CONTROLLER_H
#define RVL_FACE_LIBRARY_CONTROLLER_H
#include <RVLFaceLibrary/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// 0x34919d
typedef struct RFLCtrlCharInfo {

} RFLCtrlCharInfo;

typedef struct RFLCtrlData {
    u32 id;                                   // at 0x0
    u16 secretFlag;                           // at 0x4
    u16 padding;                              // at 0x6
    RFLCtrlCharInfo* data[RFL_CTRL_CHAR_MAX]; // at 0x8
    u16 deleted;                              // at 0x2EC
    u16 checksum;                             // at 0x2EE
} RFLCtrlData;

typedef struct RFLCtrlWriteDeleteList {
    u8 isDelete[100];
} RFLCtrlWriteDeleteList;

typedef struct RFLCtrlBufManager {
    RFLCtrlData* buffer[RFL_CTRL_MAX];             // at 0x0
    BOOL isLoaded[RFL_CTRL_MAX];                   // at 0x10
    RFLCtrlData* replaceBuf[2];                    // at 0x20
    BOOL isRead;                                   // at 0x28
    CARDFileInfo file;                             // at 0x2C
    void* tempBuffer;                              // at 0x40
    void* readBuffer;                              // at 0x44
    void* writeBuffer;                             // at 0x48
    u16 writeSize;                                 // at 0x4C
    u16 writeOffset;                               // at 0x4E
    u16 deleted;                                   // at 0x50
    s32 writeChan;                                 // at 0x54
    const RFLCtrlWriteDeleteList* writeDeleteList; // at 0x58
    u16 readSize;                                  // at 0x5C
    u16 currPos;                                   // at 0x5E
    BOOL readIsChMode;                             // at 0x60
    u8 writeCount;                                 // at 0x64
    u16 rwLen;                                     // at 0x66
    u8 retry;                                      // at 0x68
    u8* verifyBuffer;                              // at 0x6C
    u8 hiddenMDB[24];                              // at 0x0
} RFLCtrlBufManager;

void RFLiInitCtrlBuf(MEMiHeapHead*);

#ifdef __cplusplus
}
#endif
#endif
