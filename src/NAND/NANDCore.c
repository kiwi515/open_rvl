#include "NANDCore.h"

#include <TRK/__mem.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include <OS/OS.h>
#include <OS/OSError.h>
#include <OS/OSInterrupt.h>
#include <OS/OSReset.h>
#include <OS/OSTime.h>

#include <ESP/esp.h>

#define NAND_BANNER_HEADER_MAGIC 'WIBN'

static void nandShutdownCallback(IPCResult, void*);
static void nandGetTypeCallback(IPCResult, void*);
static BOOL nandOnShutdown(u32, u32);
static IPCResult _ES_InitLib(s32*);
static IPCResult _ES_GetDataDir(s32*, u64, char*) __attribute__((never_inline));
static IPCResult _ES_GetTitleId(s32*, u64*);
static IPCResult _ES_CloseLib(s32*);

static const char* __NANDVersion =
    "<< RVL_SDK - NAND \trelease build: Nov 30 2006 03:32:57 (0x4199_60831) >>";

static NANDLibState s_libState = NAND_LIB_UNINITIALIZED;
static char s_currentDir[64] __attribute__((aligned(32))) = "/";
static OSShutdownFunctionInfo s_shutdownFuncInfo = {nandOnShutdown, 255, NULL,
                                                    NULL};

static char s_homeDir[64] __attribute__((aligned(32)));

void nandRemoveTailToken(char* newp, const char* oldp) {
    int i;

    if (oldp[0] == '/' && oldp[1] == '\0') {
        newp[0] = '/';
        newp[1] = '\0';
    } else {
        for (i = strlen(oldp) - 1; i >= 0; i--) {
            if (oldp[i] == '/') {
                if (i != 0) {
                    strncpy(newp, oldp, i);
                    newp[i] = '\0';
                    break;
                } else {
                    newp[0] = '/';
                    newp[1] = '\0';
                    break;
                }
            }
        }
    }
}

void nandGetHeadToken(char* head, char* rest, const char* path) {
    int i;

    for (i = 0; i <= strlen(path); i++) {
        if (path[i] == '/') {
            strncpy(head, path, i);
            head[i] = '\0';

            if (path[i + 1] == '\0') {
                *rest = '\0';
            } else {
                strcpy(rest, path + i + 1);
            }
            break;
        } else if (path[i] == '\0') {
            strncpy(head, path, i);
            head[i] = '\0';
            *rest = '\0';
            break;
        }
    }
}

void nandGetRelativeName(char* name, const char* path) {
    int i;

    if (strcmp("/", path) == 0) {
        strcpy(name, "");
    } else {
        for (i = strlen(path) - 1; i >= 0; i--) {
            if (path[i] == '/') {
                break;
            }
        }

        strcpy(name, path + i + 1);
    }
}

void nandConvertPath(char* abs, const char* dir, const char* rel) {
    char head[128];
    char rest[128];
    char notail[128];
    char newdir[128];

    if (strlen(rel) == 0) {
        strcpy(abs, dir);
        return;
    }

    nandGetHeadToken(head, rest, rel);

    if (strcmp(head, ".") == 0) {
        nandConvertPath(abs, dir, rest);
    } else if (strcmp(head, "..") == 0) {
        nandRemoveTailToken(notail, dir);
        nandConvertPath(abs, notail, rest);
    } else if (head[0] != '\0') {
        if (strcmp(dir, "/") == 0) {
            sprintf(newdir, "/%s", head);
        } else {
            sprintf(newdir, "%s/%s", dir, head);
        }
        nandConvertPath(abs, newdir, rest);
    } else {
        strcpy(abs, dir);
    }
}

// IPA won't inline this fsr
inline BOOL nandIsRelativePath(const char* path) {
    return *path == '/' ? FALSE : TRUE;
}

BOOL nandIsPrivatePath(const char* path) {
    const size_t len = sizeof("/shared2") - 1;
    return strncmp(path, "/shared2", len) == 0;
}

BOOL nandIsUnderPrivatePath(const char* path) {
    const size_t len = sizeof("/shared2/") - 1;
    if (strncmp(path, "/shared2", len) == 0 && path[len] != '\0') {
        return TRUE;
    }

    return FALSE;
}

BOOL nandIsInitialized(void) {
    return s_libState == NAND_LIB_INITIALIZED ? TRUE : FALSE;
}

// Stubbed for release
void nandReportErrorCode(IPCResult result) {}

// Padding for the string table in order to match NANDInit
const char* NANDCore_c_PADDING() { return "ABCDEFGH"; }

NANDResult nandConvertErrorCode(IPCResult result) {
    int i;

    // clang-format off
    const s32 errorMap[] = {IPC_RESULT_OK,           NAND_RESULT_OK,
                            -0x0066,                 NAND_RESULT_ACCESS,
                            -0x0067,                 NAND_RESULT_CORRUPT,
                            -0x0072,                 NAND_RESULT_ECC_CRIT,
                            -0x0069,                 NAND_RESULT_EXISTS,
                            -0x0074,                 NAND_RESULT_AUTHENTICATION,
                            -0x0065,                 NAND_RESULT_INVALID,
                            -0x006C,                 NAND_RESULT_MAXBLOCKS,
                            -0x006D,                 NAND_RESULT_MAXFD,
                            -0x006B,                 NAND_RESULT_MAXFILES,
                            -0x006A,                 NAND_RESULT_NOEXISTS,
                            -0x0073,                 NAND_RESULT_NOTEMPTY,
                            -0x0068,                 NAND_RESULT_UNKNOWN,
                            -0x006F,                 NAND_RESULT_OPENFD,
                            -0x0075,                 NAND_RESULT_UNKNOWN,
                            -0x0076,                 NAND_RESULT_BUSY,
                            -0x0077,                 NAND_RESULT_FATAL_ERROR,
                            -0x0001,                 NAND_RESULT_ACCESS,
                            -0x0002,                 NAND_RESULT_EXISTS,
                            -0x0003,                 NAND_RESULT_UNKNOWN,
                            IPC_RESULT_INVALID,      NAND_RESULT_INVALID,
                            -0x0005,                 NAND_RESULT_UNKNOWN,
                            -0x0006,                 NAND_RESULT_NOEXISTS,
                            -0x0007,                 NAND_RESULT_UNKNOWN,
                            IPC_RESULT_BUSY,         NAND_RESULT_BUSY,
                            -0x0009,                 NAND_RESULT_UNKNOWN,
                            -0x000A,                 NAND_RESULT_UNKNOWN,
                            -0x000B,                 NAND_RESULT_UNKNOWN,
                            -0x000C,                 NAND_RESULT_ECC_CRIT,
                            -0x000D,                 NAND_RESULT_UNKNOWN,
                            -0x000E,                 NAND_RESULT_UNKNOWN,
                            -0x000F,                 NAND_RESULT_UNKNOWN,
                            -0x0010,                 NAND_RESULT_UNKNOWN,
                            -0x0011,                 NAND_RESULT_UNKNOWN,
                            -0x0012,                 NAND_RESULT_UNKNOWN,
                            -0x0013,                 NAND_RESULT_UNKNOWN,
                            -0x0014,                 NAND_RESULT_UNKNOWN,
                            -0x0015,                 NAND_RESULT_UNKNOWN,
                            IPC_RESULT_ALLOC_FAILED, NAND_RESULT_ALLOC_FAILED,
                            -0x0017,                 NAND_RESULT_UNKNOWN};
    // clang-format on

    i = 0;

    if (result >= 0) {
        return result;
    }

    for (; i < ARRAY_LENGTH(errorMap); i += 2) {
        if (result == errorMap[i]) {
            nandReportErrorCode(result);
            return errorMap[i + 1];
        }
    }

    OSReport("CAUTION!  Unexpected error code [%d] was found.\n", result);
    nandReportErrorCode(result);
    return NAND_RESULT_UNKNOWN;
}

void nandGenerateAbsPath(char* abs, const char* rel) {
    s32 len;

    if (strlen(rel) == 0) {
        strcpy(abs, "");
    } else if (nandIsRelativePath(rel)) {
        nandConvertPath(abs, s_currentDir, rel);
    } else {
        strcpy(abs, rel);
        len = strlen(abs);
        if (len != 0 && abs[len - 1] == '/' && len - 1 != 0) {
            abs[len - 1] = '\0';
        }
    }
}

void nandGetParentDirectory(char* dir, const char* path) {
    int i;

    for (i = strlen(path); i >= 0; i--) {
        if (path[i] == '/') {
            break;
        }
    }

    if (i == 0) {
        strcpy(dir, "/");
    } else {
        strncpy(dir, path, i);
        dir[i] = '\0';
    }
}

NANDResult NANDInit(void) {
    IPCResult result;
    u64 titleid;
    s32 fd;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (s_libState == NAND_LIB_INITIALIZING) {
        OSRestoreInterrupts(enabled);
        return NAND_RESULT_BUSY;
    } else if (s_libState == NAND_LIB_INITIALIZED) {
        OSRestoreInterrupts(enabled);
        return NAND_RESULT_OK;
    }

    s_libState = NAND_LIB_INITIALIZING;
    OSRestoreInterrupts(enabled);

    result = ISFS_OpenLib();
    if (result == IPC_RESULT_OK) {

        result = _ES_InitLib(&fd);

        if (result == IPC_RESULT_OK) {
            result = _ES_GetTitleId(&fd, &titleid);
        }

        if (result == IPC_RESULT_OK) {
            result = _ES_GetDataDir(&fd, titleid, s_homeDir);
        }

        if (result == IPC_RESULT_OK) {
            strcpy(s_currentDir, s_homeDir);
        }

        _ES_CloseLib(&fd);

        if (result != IPC_RESULT_OK) {
            OSReport("Failed to set home directory.\n");
        }

        OSRegisterShutdownFunction(&s_shutdownFuncInfo);
        enabled = OSDisableInterrupts();
        s_libState = NAND_LIB_INITIALIZED;
        OSRestoreInterrupts(enabled);
        OSRegisterVersion(__NANDVersion);
        return NAND_RESULT_OK;
    } else {
        enabled = OSDisableInterrupts();
        s_libState = NAND_LIB_UNINITIALIZED;
        OSRestoreInterrupts(enabled);
        return nandConvertErrorCode(result);
    }
}

static BOOL nandOnShutdown(u32 r3, u32 r4) {
    if (r3 == 0) {
        if (r4 == 2) {
            volatile BOOL shutdown = FALSE;
            s64 start = OSGetTime();
            ISFS_ShutdownAsync(nandShutdownCallback, (void*)&shutdown);

            while (OS_TICKS_TO_MSEC(OSGetTime() - start) < 500) {
                if (shutdown) {
                    break;
                }
            }
        }

        return TRUE;
    }

    return TRUE;
}

static void nandShutdownCallback(IPCResult result, void* arg) {
    *(BOOL*)arg = TRUE;
}

NANDResult NANDGetCurrentDir(char* out) {
    BOOL enabled;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    strcpy(out, s_currentDir);
    OSRestoreInterrupts(enabled);
    return NAND_RESULT_OK;
}

NANDResult NANDGetHomeDir(char* out) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    strcpy(out, s_homeDir);
    return NAND_RESULT_OK;
}

void nandCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;
    block->callback(nandConvertErrorCode(result), block);
}

static IPCResult nandGetType(const char* path, u8* type,
                             NANDCommandBlock* block, BOOL async, BOOL priv) {
    char absPath[64];
    u32 sp08;
    IPCResult result;

    if (strlen(path) == 0) {
        return -0x65;
    }

    if (async) {
        nandGenerateAbsPath(block->path, path);

        if (!priv && nandIsUnderPrivatePath(block->path)) {
            return -0x66;
        }

        block->pType = type;
        return ISFS_ReadDirAsync(block->path, NULL, &block->WORD_0x30,
                                 nandGetTypeCallback, block);
    } else {
        __memclr(absPath, sizeof(absPath));
        nandGenerateAbsPath(absPath, path);

        if (!priv && nandIsUnderPrivatePath(absPath)) {
            return -0x66;
        }

        sp08 = 0;
        result = ISFS_ReadDir(absPath, NULL, &sp08);
        if (result == IPC_RESULT_OK || result == -0x66) {
            *type = 2;
            return IPC_RESULT_OK;
        } else if (result == -0x65) {
            *type = 1;
            return IPC_RESULT_OK;
        }
    }
}

NANDResult NANDGetType(const char* path, u8* type) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    return nandConvertErrorCode(nandGetType(path, type, NULL, FALSE, FALSE));
}

NANDResult NANDPrivateGetTypeAsync(const char* path, u8* type,
                                   NANDAsyncCallback callback,
                                   NANDCommandBlock* block) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = callback;
    return nandConvertErrorCode(nandGetType(path, type, block, TRUE, TRUE));
}

static void nandGetTypeCallback(IPCResult result, void* arg) {
    NANDCommandBlock* block = (NANDCommandBlock*)arg;

    if (result == IPC_RESULT_OK || result == -0x66) {
        *block->pType = 2;
        result = IPC_RESULT_OK;
    } else if (result == -0x65) {
        *block->pType = 1;
        result = IPC_RESULT_OK;
    }

    block->callback(nandConvertErrorCode(result), block);
}

const char* nandGetHomeDir(void) { return s_homeDir; }

void NANDInitBanner(NANDBanner* banner, u32 flags, const wchar_t* title,
                    const wchar_t* subtitle) {
    memset(banner, 0, sizeof(NANDBanner));
    banner->flags = flags;
    banner->magic = NAND_BANNER_HEADER_MAGIC;

    if (wcscmp(title, L"") == 0) {
        wcsncpy(banner->title, L" ", NAND_BANNER_TITLE_MAX);
    } else {
        wcsncpy(banner->title, title, NAND_BANNER_TITLE_MAX);
    }

    if (wcscmp(subtitle, L"") == 0) {
        wcsncpy(banner->subtitle, L" ", NAND_BANNER_TITLE_MAX);
    } else {
        wcsncpy(banner->subtitle, subtitle, NAND_BANNER_TITLE_MAX);
    }
}

/**
 * These were actually re(?)implemented in NANDCore/OSExec according to BBA
 */

static IPCResult _ES_InitLib(s32* fd) {
    IPCResult result;

    *fd = -1;
    result = IPC_RESULT_OK;

    *fd = IOS_Open("/dev/es", IPC_OPEN_NONE);
    if (*fd < 0) {
        result = *fd;
    }

    return result;
}

static IPCResult _ES_GetDataDir(s32* fd, u64 titleid, char* out) {
    // TO-DO: Hacky solution
    u8 titleidWork[256] __attribute__((aligned(32)));
    u8 vectorWork[32] __attribute__((aligned(32)));
    IPCIOVector* pVectors = (IPCIOVector*)vectorWork;
    u64* pTitleid = (u64*)titleidWork;

    // Cast is necessary
    if (*fd < 0 || out == ((void*)NULL)) {
        return -0x3F9;
    }

    if ((u32)out % 32 != 0) {
        return -0x3F9;
    }

    pTitleid[0] = titleid;
    pVectors[0].base = pTitleid;
    pVectors[0].length = sizeof(u64);
    pVectors[1].base = out;
    pVectors[1].length = 30;

    return IOS_Ioctlv(*fd, IPC_IOCTLV_GET_DATA_DIR, 1, 1, pVectors);
}

static IPCResult _ES_GetTitleId(s32* fd, u64* out) {
    IPCResult result;
    u64* pTitleid;
    // TO-DO: Hacky solution
    u8 titleidWork[256] __attribute__((aligned(32)));
    u8 vectorWork[32] __attribute__((aligned(32)));
    IPCIOVector* pVectors = (IPCIOVector*)vectorWork;

    // Cast is necessary
    if (*fd < 0 || out == ((void*)NULL)) {
        return -0x3F9;
    }

    pTitleid = (u64*)titleidWork;
    pVectors[0].base = pTitleid;
    pVectors[0].length = sizeof(u64);

    result = IOS_Ioctlv(*fd, IPC_IOCTLV_GET_TITLE_ID, 0, 1, pVectors);
    if (result == IPC_RESULT_OK) {
        *out = *pTitleid;
    }

    return result;
}

static IPCResult _ES_CloseLib(s32* fd) {
    IPCResult result = IPC_RESULT_OK;

    if (*fd >= 0 && (result = IOS_Close(*fd)) == IPC_RESULT_OK) {
        *fd = -1;
    }

    return result;
}
