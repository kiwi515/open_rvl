#include "NANDCheck.h"
#include "NANDCore.h"

static const char* USER_DIR_LIST[] = {"/meta",
                                      "/ticket",
                                      "/title/00010000",
                                      "/title/00010001",
                                      "/title/00010003",
                                      "/title/00010004",
                                      "/title/00010005",
                                      "/title/00010006",
                                      "/title/00010007",
                                      "/shared2/title",
                                      NULL,
                                      NULL};

static IPCResult nandCalcUsage(s32* r3, s32* r4, const char** dirList) {
    IPCResult result = -0x75;
    *r3 = 0;
    *r4 = 0;

    for (; *dirList != NULL; dirList++) {
        s32 sp0C = 0;
        s32 sp08 = 0;

        result = ISFS_GetUsage(*dirList, &sp0C, &sp08);
        if (result == IPC_RESULT_OK) {
            *r3 += sp0C;
            *r4 += sp08;
        } else if (result == -0x6A) {
            result = IPC_RESULT_OK;
        } else {
            break;
        }
    }

    return result;
}

static IPCResult nandCalcUserUsage(s32* r3, s32* r4) {
    return nandCalcUsage(r3, r4, USER_DIR_LIST);
}

static u32 nandCheck(u32 fsBlock, u32 iNode, u32 r5, u32 r6, u32 r7, u32 r8) {
    u32 answer = 0;

    if (r5 + fsBlock > 0x400) {
        answer |= 0x1;
    }

    if (r6 + iNode > 0x21) {
        answer |= 0x2;
    }

    if (r7 + fsBlock > 0x4400) {
        answer |= 0x4;
    }

    if (r8 + iNode > 0xFA0) {
        answer |= 0x8;
    }

    return answer;
}

NANDResult NANDCheck(u32 fsBlock, u32 iNode, u32* answer) {
    IPCResult result;
    s32 sp14 = -1;
    s32 sp10 = -1;
    s32 sp0C = -1;
    s32 sp08 = -1;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    result = ISFS_GetUsage(nandGetHomeDir(), &sp14, &sp10);
    if (result != IPC_RESULT_OK) {
        return nandConvertErrorCode(result);
    }

    result = nandCalcUserUsage(&sp0C, &sp08);
    if (result != IPC_RESULT_OK) {
        return nandConvertErrorCode(result);
    }

    *answer = nandCheck(fsBlock, iNode, sp14, sp10, sp0C, sp08);
    return NAND_RESULT_OK;
}
