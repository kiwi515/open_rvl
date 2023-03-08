#ifndef RVL_SDK_ESP_H
#define RVL_SDK_ESP_H
#include <revolution/ARC.h>
#include <revolution/IPC.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ESPTicket {
    u8 dummy[0x2A4];
    char padding[0x2C0 - 0x2A4];
} ESPTicket;

typedef struct ESPTmd {
    u8 dummy[0x49E4];
    char padding[0x4A00 - 0x49E4];
} ESPTmd;

s32 ESP_ReadContentFile(UNKWORD, void*, u32);
s32 ESP_SeekContentFile(UNKWORD, s32, UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
