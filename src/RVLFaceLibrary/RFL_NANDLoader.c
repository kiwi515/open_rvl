#include <RVLFaceLibrary.h>
#include <revolution/NAND.h>
#include <string.h>

static const u32 scTmpSize = 0x400;
static const char* scResFileFullPathName = "/shared2/FaceLib/RFL_Res.dat";

void RFLiInitLoader(void) {
    RFLLoader* loader;
    int i;

    loader = RFLiGetLoader();
    for (i = 0; i < RFLiArcID_Max; i++) {
        loader->archives[i].numFiles = 0;
        loader->archives[i].biggestSize = 0;
        loader->archives[i].offset = 0;
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
    for (i = 0; i < RFLiArcID_Max; i++) {
        // Pointer to section offset in header
        u32* p_offset = (u32*)((u8*)loader->cache + ((i + 1) * 4));
        // Pointer to section
        u32* p_section = (u32*)((u8*)loader->cache + *p_offset);
        // Load archive
        loader->archives[i].numFiles = ((u16*)p_section)[0];
        loader->archives[i].biggestSize = ((u16*)p_section)[1];
        loader->archives[i].offset = *p_offset + 4;
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
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        const u16 arc = loader->numResources;
        headerBuf1 = (u32*)loader->headerBuf1;
        if (arc < RFLiArcID_Max) {
            headerBuf2 = (u32*)loader->headerBuf2;
            headerBuf1 = &headerBuf1[arc + 1];
            loader->archives[arc].numFiles = ((u16*)headerBuf2)[0];
            loader->archives[arc].biggestSize = ((u16*)headerBuf2)[1];
            loader->archives[arc].offset = *headerBuf1 + 4;
            loader->numResources++;
            switch (RFLiReadAsync(RFLiFileType_Resource, loader->headerBuf2, 32,
                                  loadResRead2ndcallback_, *(headerBuf1 + 1))) {
            case RFLErrcode_Busy:
                break;
            case RFLErrcode_Success:
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
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    if (free) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;
        RFLiFree(loader->headerBuf2);
        loader->headerBuf2 = NULL;
        RFLiCloseAsync(RFLiFileType_Resource, cb);
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
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        headerBuf1 = (u32*)loader->headerBuf1;
        loader->version = *(u16*)((u8*)headerBuf1 + 2);

        headerBuf2 = RFLiAlloc32(32);
        loader->headerBuf2 = headerBuf2;
        loader->numResources = 0;

        switch (RFLiReadAsync(RFLiFileType_Resource, headerBuf2, 32,
                              loadResRead2ndcallback_, *(headerBuf1 + 1))) {
        case RFLErrcode_Busy:
            break;
        case RFLErrcode_Success:
            break;
        default:
            free = TRUE;
        }
    } else {
        free = TRUE;
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    if (free) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;

        if (loader->headerBuf2 != NULL) {
            RFLiFree(loader->headerBuf2);
            loader->headerBuf2 = NULL;
        }

        RFLiCloseAsync(RFLiFileType_Resource, errclosecallback_);
    }
}

static void loadResGetlengthcallback_(void) {
    RFLLoader* loader;
    void* headerBuf1;

    loader = RFLiGetLoader();
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        headerBuf1 = RFLiAlloc32(0x100);
        loader->headerBuf1 = headerBuf1;
        switch (RFLiReadAsync(RFLiFileType_Resource, headerBuf1, 0x100,
                              loadResRead1stcallback_, 0)) {
        case RFLErrcode_Busy:
            break;
        case RFLErrcode_Success:
            break;
        default:
            RFLiFree(loader->headerBuf1);
            loader->headerBuf1 = NULL;
            RFLiCloseAsync(RFLiFileType_Resource, NULL);
        }
    } else {
        RFLiCloseAsync(RFLiFileType_Resource, errclosecallback_);
    }
}

static void loadResOpencallback_(void) {
    RFLLoader* loader;

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        loader = RFLiGetLoader();
        loader->cacheSize = 0;
        switch (RFLiGetLengthAsync(RFLiFileType_Resource, &loader->cacheSize,
                                   loadResGetlengthcallback_)) {
        case RFLErrcode_Busy:
            break;
        case RFLErrcode_Success:
            break;
        default:
            RFLiCloseAsync(RFLiFileType_Resource, NULL);
        }
    } else {
        RFLExit();
    }
}

RFLErrcode RFLiLoadResourceHeaderAsync(void) {
    RFLLoader* loader;

    loader = RFLiGetLoader();
    if (loader == NULL) {
        RFLiEndWorking(RFLErrcode_Fatal);
        return RFLErrcode_Fatal;
    }

    if (RFLIsResourceCached()) {
        parseOnmemoryRes_();
        RFLiEndWorking(RFLErrcode_Success);
        return RFLErrcode_Busy;
    }

    return RFLiOpenAsync(RFLiFileType_Resource, NAND_ACCESS_READ,
                         loadResOpencallback_);
}

static u32 getCachedLength_(RFLLoader* loader, u32 arcIdx, u16 fileIdx) {
    RFLArchive* arc = &loader->archives[arcIdx];
    const void* arcBuf = (u8*)loader->cache + arc->offset;
    const u32 next = ((u32*)arcBuf)[fileIdx + 1];
    const u32 self = ((u32*)arcBuf)[fileIdx];
    return next - self;
}

static u32 getNANDLength_(RFLLoader* loader, u32 arcIdx, u16 fileIdx) {
    NANDFileInfo file;
    RFLArchive* arc;
    void* tmpBuf;
    u32 length = 0;

    tmpBuf = RFLiAlloc32(scTmpSize);
    arc = &loader->archives[arcIdx];

    if (NANDPrivateOpen(scResFileFullPathName, &file, NAND_ACCESS_READ) ==
        NAND_RESULT_OK) {
        const u32 readSize = ROUND_UP(arc->numFiles * 4 + 4, 32);
        NANDSeek(&file, arc->offset, NAND_SEEK_BEG);

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

static u32 getLength_(u32 arcIdx, u16 fileIdx) {
    RFLLoader* loader;

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return 0;
    }

    if (fileIdx >= loader->archives[arcIdx].numFiles) {
        return 0;
    }

    if (RFLIsResourceCached()) {
        return getCachedLength_(loader, arcIdx, fileIdx);
    } else {
        return getNANDLength_(loader, arcIdx, fileIdx);
    }
}

static void* getCachedFile_(void* dst, RFLLoader* loader, u32 arcIdx,
                            u16 fileIdx) {
    const u8* cache = (u8*)loader->cache;
    RFLArchive* arc = &loader->archives[arcIdx];
    const void* arcBuf = cache + arc->offset;
    const u32 self = ((u32*)arcBuf)[fileIdx];
    const u32 next = ((u32*)arcBuf)[fileIdx + 1];
    const u32 size = next - self;
    const u32 src = arc->offset + self + (arc->numFiles * 4);

    memcpy(dst, src + 4 + cache, size);
    return dst;
}

static void* getNANDFile_(void* dst, RFLLoader* loader, u32 arcIdx,
                          u16 fileIdx) {
    NANDFileInfo file;
    RFLArchive* arc;
    void* tmpBuf;
    void* arcBuf;
    void* ret;
    u32 readSize;

    ret = NULL;
    arc = &loader->archives[arcIdx];
    tmpBuf = RFLiAlloc32(scTmpSize);

    if (NANDPrivateOpen(scResFileFullPathName, &file, NAND_ACCESS_READ) ==
        NAND_RESULT_OK) {
        u32 arcBufSize;
        s32 seekOffset;
        u32 arcSize;
        u32 arcOffset;

        readSize = ROUND_UP(arc->numFiles * 4 + 4, 32);
        arcSize = 0;
        arcOffset = 0;

        NANDSeek(&file, arc->offset, NAND_SEEK_BEG);

        if (NANDRead(&file, tmpBuf, readSize) == readSize) {
            const u32 next = ((u32*)tmpBuf)[fileIdx + 1];
            const u32 self = ((u32*)tmpBuf)[fileIdx];
            arcSize = next - self;
            arcOffset = self;
        } else {
            RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
            RFLiFree(tmpBuf);
            return NULL;
        }

        seekOffset = arc->offset + arc->numFiles * 4 + 4 + arcOffset;
        arcBufSize = ROUND_UP(arcSize, 32);

        arcBuf = RFLiAlloc32(arcBufSize);
        NANDSeek(&file, seekOffset, NAND_SEEK_BEG);

        if (NANDRead(&file, arcBuf, arcBufSize) == arcBufSize) {
            memcpy(dst, arcBuf, arcSize);
            ret = dst;
        } else {
            RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
            RFLiFree(tmpBuf);
            RFLiFree(arcBuf);
            return NULL;
        }

        RFLiFree(arcBuf);
        NANDClose(&file);
    } else {
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    RFLiFree(tmpBuf);
    return ret;
}

static void* getFile_(void* dst, u32 arcIdx, u16 fileIdx) {
    RFLLoader* loader;

    if (!RFLAvailable()) {
        return 0;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return 0;
    }

    if (fileIdx >= loader->archives[arcIdx].numFiles) {
        return 0;
    }

    if (RFLIsResourceCached()) {
        return getCachedFile_(dst, loader, arcIdx, fileIdx);
    } else {
        return getNANDFile_(dst, loader, arcIdx, fileIdx);
    }
}

u32 RFLiGetTexSize(RFLiPartsTex part, u16 file) {
    static const u32 scParts2Arc[] = {RFLiArcID_Eye, RFLiArcID_Eyebrow,
                                      RFLiArcID_Mouth, RFLiArcID_Mustache,
                                      RFLiArcID_Mole};
    return getLength_(scParts2Arc[part], file);
}

RFLTexHeader* RFLiLoadTexture(RFLiPartsTex part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {RFLiArcID_Eye, RFLiArcID_Eyebrow,
                                      RFLiArcID_Mouth, RFLiArcID_Mustache,
                                      RFLiArcID_Mole};
    return (RFLTexHeader*)getFile_(dst, scParts2Arc[part], file);
}

u32 RFLiGetShpTexSize(RFLiPartsShpTex part, u16 file) {
    static const u32 scParts2Arc[] = {RFLiArcID_FaceTex, RFLiArcID_CapTex,
                                      RFLiArcID_NlineTex, RFLiArcID_GlassTex};
    return getLength_(scParts2Arc[part], file);
}

RFLTexHeader* RFLiLoadShpTexture(RFLiPartsShpTex part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {RFLiArcID_FaceTex, RFLiArcID_CapTex,
                                      RFLiArcID_NlineTex, RFLiArcID_GlassTex};
    return (RFLTexHeader*)getFile_(dst, scParts2Arc[part], file);
}

u32 RFLiGetShapeSize(RFLiPartsShp part, u16 file) {
    static const u32 scParts2Arc[] = {
        RFLiArcID_Nose,  RFLiArcID_ForeHead, RFLiArcID_Faceline,
        RFLiArcID_Hair,  RFLiArcID_Cap,      RFLiArcID_Beard,
        RFLiArcID_Nline, RFLiArcID_Mask,     RFLiArcID_Glass};
    return getLength_(scParts2Arc[part], file);
}

void* RFLiLoadShape(RFLiPartsShp part, u16 file, void* dst) {
    static const u32 scParts2Arc[] = {
        RFLiArcID_Nose,  RFLiArcID_ForeHead, RFLiArcID_Faceline,
        RFLiArcID_Hair,  RFLiArcID_Cap,      RFLiArcID_Beard,
        RFLiArcID_Nline, RFLiArcID_Mask,     RFLiArcID_Glass};
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
