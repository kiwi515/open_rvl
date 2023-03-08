#ifndef RVL_SDK_OS_CACHE_H
#define RVL_SDK_OS_CACHE_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

void DCEnable(void);
void DCInvalidateRange(const void*, u32);
void DCFlushRange(const void*, u32);
void DCStoreRange(const void*, u32);
void DCFlushRangeNoSync(const void*, u32);
void DCStoreRangeNoSync(const void*, u32);
void DCZeroRange(const void*, u32);

void ICInvalidateRange(const void*, u32);
void ICFlashInvalidate(void);
void ICEnable(void);

void LCEnable(void);
void LCDisable(void);
void LCLoadBlocks(void*, const void*, u32);
void LCStoreBlocks(void*, const void*, u32);
u32 LCStoreData(void*, const void*, u32);
u32 LCQueueLength(void);
void LCQueueWait(u32);

void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);

void DMAErrorHandler(u8, struct OSContext*, u32, u32, ...);

void __OSCacheInit(void);

#ifdef __cplusplus
}
#endif
#endif
