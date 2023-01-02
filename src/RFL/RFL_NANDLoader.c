#include "RFL_NANDLoader.h"
#include "RFL_NANDAccess.h"

#include <NAND/nand.h>
#include <TRK/__mem.h>

static const u32 scTmpSize = 0x400;
static const char* scResFileFullPathName = "/shared2/FaceLib/RFL_Res.dat";

void RFLiInitLoader(void) {
    RFLLoader* loader;
    int i;

    loader = RFLiGetLoader();
    for (i = 0; i < RFL_ARC_MAX; i++) {
        loader->resources[i].numFiles = 0;
        loader->resources[i].biggestSize = 0;
        loader->resources[i].offset = 0;
    }

    loader->cacheSize = 0;
    loader->cache = NULL;
    loader->cached = FALSE;
    loader->version = 0;
    loader->headerBuf1 = NULL;
    loader->headerBuf2 = NULL;
}

/**
 * RFL_Res.dat documentation from:
 * https://wiki.tockdom.com/wiki/RFL_Res.dat_(File_Format)
 */
static void parseOnmemoryRes_(void) __attribute__((never_inline)) {
    RFLLoader* loader;
    int i;

    loader = RFLiGetLoader();
    loader->version = *(u16*)((u8*)loader->cache + 2);
    for (i = 0; i < RFL_ARC_MAX; i++) {
        // Pointer to section offset in header
        u32* p_offset = (u32*)((u8*)loader->cache + ((i + 1) * 4));
        // Pointer to section
        u32* p_section = (u32*)((u8*)loader->cache + *p_offset);
        // Load resource
        loader->resources[i].numFiles = ((u16*)p_section)[0];
        loader->resources[i].biggestSize = ((u16*)p_section)[1];
        loader->resources[i].offset = *p_offset + 4;
    }
}

static void loadResRead2ndcallback_(void) {
    RFLLoader* loader;
    BOOL free;
    RFLAccessCallback cb;
    u32* headerBuf1;
    u32* headerBuf2;

    loader = RFLiGetLoader();
    free = FALSE;
    cb = NULL;
    if (RFLGetAsyncStatus() == RFL_RESULT_OK) {
        const u16 res = loader->numResources;
        headerBuf1 = (u32*)loader->headerBuf1;
        if (res < RFL_ARC_MAX) {
            headerBuf2 = (u32*)loader->headerBuf2;
            headerBuf1 = &headerBuf1[res + 1];
            loader->resources[res].numFiles = ((u16*)headerBuf2)[0];
            loader->resources[res].biggestSize = ((u16*)headerBuf2)[1];
            loader->resources[res].offset = *headerBuf1 + 4;
            loader->numResources++;
            switch (RFLiReadAsync(RFL_ACCESS_RES, loader->headerBuf2, 32,
                                  loadResRead2ndcallback_, *(headerBuf1 + 1))) {
            case RFL_RESULT_BUSY:
                break;
            case RFL_RESULT_OK:
                break;
            default:
                free = TRUE;
            }
        } else {
            free = TRUE;
            cb = NULL;
        }
    } else {
        free = TRUE;
        RFLiSetFileBroken(RFL_BROKEN_IO_ERROR);
    }

    if (free) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;
        RFLiFree(loader->headerBuf2);
        loader->headerBuf2 = NULL;
        RFLiCloseAsync(RFL_ACCESS_RES, cb);
    }
}

static void errclosecallback_(void) { RFLExit(); }

static void loadResRead1stcallback_(void) {
    RFLLoader* loader;
    BOOL free;
    u32* headerBuf1;
    void* headerBuf2;

    loader = RFLiGetLoader();
    free = FALSE;
    if (RFLGetAsyncStatus() == RFL_RESULT_OK) {
        headerBuf1 = (u32*)loader->headerBuf1;
        loader->version = *(u16*)((u8*)headerBuf1 + 2);

        headerBuf2 = RFLiAlloc32(32);
        loader->headerBuf2 = headerBuf2;
        loader->numResources = 0;

        switch (RFLiReadAsync(RFL_ACCESS_RES, headerBuf2, 32,
                              loadResRead2ndcallback_, *(headerBuf1 + 1))) {
        case RFL_RESULT_BUSY:
            break;
        case RFL_RESULT_OK:
            break;
        default:
            free = TRUE;
        }
    } else {
        free = TRUE;
        RFLiSetFileBroken(RFL_BROKEN_IO_ERROR);
    }

    if (free) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;

        if (loader->headerBuf2 != NULL) {
            RFLiFree(loader->headerBuf2);
            loader->headerBuf2 = NULL;
        }

        RFLiCloseAsync(RFL_ACCESS_RES, errclosecallback_);
    }
}

static void loadResGetlengthcallback_(void) {
    RFLLoader* loader;
    void* headerBuf1;

    loader = RFLiGetLoader();
    if (RFLGetAsyncStatus() == RFL_RESULT_OK) {
        headerBuf1 = RFLiAlloc32(0x100);
        loader->headerBuf1 = headerBuf1;
        switch (RFLiReadAsync(RFL_ACCESS_RES, headerBuf1, 0x100,
                              loadResRead1stcallback_, 0)) {
        case RFL_RESULT_BUSY:
            break;
        case RFL_RESULT_OK:
            break;
        default:
            RFLiFree(loader->headerBuf1);
            loader->headerBuf1 = NULL;
            RFLiCloseAsync(RFL_ACCESS_RES, NULL);
        }
    } else {
        RFLiCloseAsync(RFL_ACCESS_RES, errclosecallback_);
    }
}

static void loadResOpencallback_(void) {
    RFLLoader* loader;

    if (RFLGetAsyncStatus() == RFL_RESULT_OK) {
        loader = RFLiGetLoader();
        loader->cacheSize = 0;
        switch (RFLiGetLengthAsync(RFL_ACCESS_RES, &loader->cacheSize,
                                   loadResGetlengthcallback_)) {
        case RFL_RESULT_BUSY:
            break;
        case RFL_RESULT_OK:
            break;
        default:
            RFLiCloseAsync(RFL_ACCESS_RES, NULL);
        }
    } else {
        RFLExit();
    }
}

RFLResult RFLiLoadResourceHeaderAsync(void) {
    RFLLoader* loader;

    loader = RFLiGetLoader();
    if (loader == NULL) {
        RFLiEndWorking(RFL_RESULT_CRITICAL);
        return RFL_RESULT_CRITICAL;
    }

    if (RFLIsResourceCached()) {
        parseOnmemoryRes_();
        RFLiEndWorking(RFL_RESULT_OK);
        return RFL_RESULT_BUSY;
    }

    return RFLiOpenAsync(RFL_ACCESS_RES, NAND_ACCESS_READ,
                         loadResOpencallback_);
}

static u32 getCachedLength_(RFLLoader* loader, u32 resIdx, u16 fileIdx) {
    RFLResource* res = &loader->resources[resIdx];
    const void* resBuf = (u8*)loader->cache + res->offset;
    const u32 next = ((u32*)resBuf)[fileIdx + 1];
    const u32 self = ((u32*)resBuf)[fileIdx];
    return next - self;
}

static u32 getNANDLength_(RFLLoader* loader, u32 resIdx, u16 fileIdx) {
    NANDFileInfo file;
    RFLResource* res;
    void* tmpBuf;
    u32 length = 0;

    tmpBuf = RFLiAlloc32(scTmpSize);
    res = &loader->resources[resIdx];

    if (NANDPrivateOpen(scResFileFullPathName, &file, NAND_ACCESS_READ) ==
        NAND_RESULT_OK) {
        const u32 readSize = ROUND_UP(res->numFiles * 4 + 4, 32);
        NANDSeek(&file, res->offset, NAND_SEEK_BEG);

        if (NANDRead(&file, tmpBuf, readSize) == readSize) {
            const u32 self = ((u32*)tmpBuf)[fileIdx];
            const u32 next = ((u32*)tmpBuf)[fileIdx + 1];
            length = next - self;
        }

        NANDClose(&file);
    }

    RFLiFree(tmpBuf);
    return length;
}

static u32 getLength_(u32 resIdx, u16 fileIdx) {
    RFLLoader* loader;

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return 0;
    }

    if (fileIdx >= loader->resources[resIdx].numFiles) {
        return 0;
    }

    if (RFLIsResourceCached()) {
        return getCachedLength_(loader, resIdx, fileIdx);
    } else {
        return getNANDLength_(loader, resIdx, fileIdx);
    }
}

static void* getCachedFile_(void* dst, RFLLoader* loader, u32 resIdx,
                            u16 fileIdx) {
    const u8* cache = (u8*)loader->cache;
    RFLResource* res = &loader->resources[resIdx];
    const void* resBuf = cache + res->offset;
    const u32 self = ((u32*)resBuf)[fileIdx];
    const u32 next = ((u32*)resBuf)[fileIdx + 1];
    const u32 size = next - self;
    const u32 src = res->offset + self + (res->numFiles * 4);

    memcpy(dst, src + 4 + cache, size);
    return dst;
}

static void* getNANDFile_(void* dst, RFLLoader* loader, u32 resIdx,
                          u16 fileIdx) {
    NANDFileInfo file;
    RFLResource* res;
    void* tmpBuf;
    void* resBuf;
    void* ret;
    u32 readSize;

    ret = NULL;
    res = &loader->resources[resIdx];
    tmpBuf = RFLiAlloc32(scTmpSize);

    if (NANDPrivateOpen(scResFileFullPathName, &file, NAND_ACCESS_READ) ==
        NAND_RESULT_OK) {
        u32 resBufSize;
        s32 seekOffset;
        u32 resSize;
        u32 resOffset;

        readSize = ROUND_UP(res->numFiles * 4 + 4, 32);
        resSize = 0;
        resOffset = 0;

        NANDSeek(&file, res->offset, NAND_SEEK_BEG);

        if (NANDRead(&file, tmpBuf, readSize) == readSize) {
            const u32 next = ((u32*)tmpBuf)[fileIdx + 1];
            const u32 self = ((u32*)tmpBuf)[fileIdx];
            resSize = next - self;
            resOffset = self;
        } else {
            RFLiSetFileBroken(RFL_BROKEN_IO_ERROR);
            RFLiFree(tmpBuf);
            return NULL;
        }

        seekOffset = res->offset + res->numFiles * 4 + 4 + resOffset;
        resBufSize = ROUND_UP(resSize, 32);

        resBuf = RFLiAlloc32(resBufSize);
        NANDSeek(&file, seekOffset, NAND_SEEK_BEG);

        if (NANDRead(&file, resBuf, resBufSize) == resBufSize) {
            memcpy(dst, resBuf, resSize);
            ret = dst;
        } else {
            RFLiSetFileBroken(RFL_BROKEN_IO_ERROR);
            RFLiFree(tmpBuf);
            RFLiFree(resBuf);
            return NULL;
        }

        RFLiFree(resBuf);
        NANDClose(&file);
    } else {
        RFLiSetFileBroken(RFL_BROKEN_IO_ERROR);
    }

    RFLiFree(tmpBuf);
    return ret;
}

static void* getFile_(void* dst, u32 resIdx, u16 fileIdx) {
    RFLLoader* loader;

    if (!RFLAvailable()) {
        return 0;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return 0;
    }

    if (fileIdx >= loader->resources[resIdx].numFiles) {
        return 0;
    }

    if (RFLIsResourceCached()) {
        return getCachedFile_(dst, loader, resIdx, fileIdx);
    } else {
        return getNANDFile_(dst, loader, resIdx, fileIdx);
    }
}

u32 RFLiGetTexSize(RFLPartTex part, u16 file) {
    static const u32 scParts2Arc[] = {
        RFL_ARC_TEX_EYE, RFL_ARC_TEX_EYEBROW, RFL_ARC_TEX_MOUTH,
        RFL_ARC_TEX_FACIAL_HAIR, RFL_ARC_TEX_MOLE};
    return getLength_(scParts2Arc[part], file);
}

RFLTexHeader* RFLiLoadTexture(RFLPartTex part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {
        RFL_ARC_TEX_EYE, RFL_ARC_TEX_EYEBROW, RFL_ARC_TEX_MOUTH,
        RFL_ARC_TEX_FACIAL_HAIR, RFL_ARC_TEX_MOLE};
    return (RFLTexHeader*)getFile_(dst, scParts2Arc[part], file);
}

u32 RFLiGetShpTexSize(RFLPartShpTex part, u16 file) {
    static const u32 scParts2Arc[] = {RFL_ARC_TEX_MASCARA, RFL_ARC_TEX_CAP,
                                      RFL_ARC_TEX_NOSE, RFL_ARC_TEX_GLASSES};
    return getLength_(scParts2Arc[part], file);
}

RFLTexHeader* RFLiLoadShpTexture(RFLPartShpTex part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {RFL_ARC_TEX_MASCARA, RFL_ARC_TEX_CAP,
                                      RFL_ARC_TEX_NOSE, RFL_ARC_TEX_GLASSES};
    return (RFLTexHeader*)getFile_(dst, scParts2Arc[part], file);
}

u32 RFLiGetShapeSize(RFLPartShp part, u16 file) {
    static const u32 scParts2Arc[] = {
        RFL_ARC_SHP_NOSE,     RFL_ARC_SHP_FOREHEAD, RFL_ARC_SHP_FACE,
        RFL_ARC_SHP_HAIR,     RFL_ARC_SHP_CAP,      RFL_ARC_SHP_BEARD,
        RFL_ARC_SHP_NOSELINE, RFL_ARC_SHP_MASK,     RFL_ARC_SHP_GLASSES};
    return getLength_(scParts2Arc[part], file);
}

void* RFLiLoadShape(RFLPartShp part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {
        RFL_ARC_SHP_NOSE,     RFL_ARC_SHP_FOREHEAD, RFL_ARC_SHP_FACE,
        RFL_ARC_SHP_HAIR,     RFL_ARC_SHP_CAP,      RFL_ARC_SHP_BEARD,
        RFL_ARC_SHP_NOSELINE, RFL_ARC_SHP_MASK,     RFL_ARC_SHP_GLASSES};
    return getFile_(dst, scParts2Arc[part], file);
}

BOOL RFLFreeCachedResource(void) {
    RFLLoader* loader;

    if (!RFLAvailable()) {
        return TRUE;
    }

    if (RFLiIsWorking()) {
        return TRUE;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return TRUE;
    }

    loader->cached = FALSE;
    loader->cache = NULL;
    loader->cacheSize = 0;
    return FALSE;
}

BOOL RFLIsResourceCached(void) {
    RFLLoader* loader;

    if (!RFLAvailable()) {
        return FALSE;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return FALSE;
    }

    return loader->cached;
}
