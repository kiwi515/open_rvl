#ifndef RVL_SDK_ESP_H
#define RVL_SDK_ESP_H
#include <ARC/arc.h>
#include <IPC/ipcclt.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

s32 ESP_ReadContentFile(UNKWORD, void*, u32);
s32 ESP_SeekContentFile(UNKWORD, s32, UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
