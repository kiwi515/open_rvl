#ifndef RVL_SDK_ESP_H
#define RVL_SDK_ESP_H
#include <ARC/arc.h>
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ESP_SEEK_BEGIN,
} ESP_SeekOrigin;

s32 ESP_ReadContentFile(u32, void*, u32);
s32 ESP_SeekContentFile(u32, s32, ESP_SeekOrigin);

#ifdef __cplusplus
}
#endif
#endif
