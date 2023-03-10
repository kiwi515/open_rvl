#ifndef RVL_FACE_LIBRARY_NAND_LOADER_H
#define RVL_FACE_LIBRARY_NAND_LOADER_H
#include <RVLFaceLib/RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLTexture;

typedef struct RFLArchive {
    u16 numFiles;    // at 0x0
    u16 biggestSize; // at 0x2
    u32 offset;      // at 0x4
} RFLArchive;

typedef struct RFLLoader {
    RFLArchive archives[RFLiArcID_Max]; // at 0x0
    u32 cacheSize;                      // at 0x90
    void* cache;                        // at 0x94
    BOOL cached;                        // at 0x98
    u16 version;                        // at 0x9C
    void* headerBuf1;                   // at 0xA0
    void* headerBuf2;                   // at 0xA4
    u8 numResources;                    // at 0xA8
} RFLLoader;

void RFLiInitLoader(void);
RFLErrcode RFLiLoadResourceHeaderAsync(void);
u32 RFLiGetTexSize(RFLiPartsTex part, u16 file);
struct RFLTexture* RFLiLoadTexture(RFLiPartsTex part, u16 file, void* dst);
u32 RFLiGetShpTexSize(RFLiPartsShpTex part, u16 file);
struct RFLTexture* RFLiLoadShpTexture(RFLiPartsShpTex part, u16 file,
                                      void* dst);
u32 RFLiGetShapeSize(RFLiPartsShp part, u16 file);
void* RFLiLoadShape(RFLiPartsShp part, u16 file, void* dst);
BOOL RFLFreeCachedResource(void);
BOOL RFLIsResourceCached(void);

#ifdef __cplusplus
}
#endif
#endif
