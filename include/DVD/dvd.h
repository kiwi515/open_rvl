#ifndef RVL_SDK_DVD_H
#define RVL_SDK_DVD_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSAlarm;

typedef struct DVDDriveBlock {
    char UNK_0x0[0xC];
    UNKWORD WORD_0xC;
    char UNK_0x10[0x30 - 0x10];
} DVDDriveBlock;

typedef struct DVDDriveInfo {
    char UNK_0x0[0x2];
    u16 deviceCodeAddr; // at 0x2
    char UNK_0x4[0x20 - 0x4];
} DVDDriveInfo;

typedef void (*DVDInquiryCallback)(s32, DVDDriveBlock*);

void DVDInit(void);

BOOL DVDInquiryAsync(DVDDriveBlock*, DVDDriveInfo*, DVDInquiryCallback);

u32 __DVDGetCoverStatus(void);

void __DVDPrepareReset(void);
BOOL __DVDTestAlarm(struct OSAlarm*);

#ifdef __cplusplus
}
#endif
#endif
