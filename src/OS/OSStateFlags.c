#include "OSStateFlags.h"

#include <NAND/nand.h>
#include <TRK/__mem.h>

static OSStateFlags StateFlags __attribute__((aligned(32)));

static u32 CheckSum(OSStateFlags* state) {
    int i;

    u32 checksum = 0;
    const u32* ptr = &state->WORD_0x4;
    for (i = 0; i < (sizeof(OSStateFlags) / 0x4) - 1; i++, ptr++) {
        checksum += *ptr;
    }

    return checksum;
}

BOOL __OSWriteStateFlags(const OSStateFlags* newState) {
    NANDFileInfo file;

    memcpy(&StateFlags, newState, sizeof(OSStateFlags));
    StateFlags.checksum = CheckSum(&StateFlags);

    if (NANDOpen("/title/00000001/00000002/data/state.dat", &file,
                 NAND_ACCESS_WRITE) == 0) {
        if (NANDWrite(&file, &StateFlags, sizeof(OSStateFlags)) !=
            sizeof(OSStateFlags)) {
            NANDClose(&file);
            return FALSE;
        }
        if (NANDClose(&file) != 0) {
            return FALSE;
        }
    } else {
        return FALSE;
    }

    return TRUE;
}

BOOL __OSReadStateFlags(OSStateFlags* stateOut) {
    NANDFileInfo file;

    if (NANDOpen("/title/00000001/00000002/data/state.dat", &file,
                 NAND_ACCESS_READ) == 0) {

        const int bytesRead =
            NANDRead(&file, &StateFlags, sizeof(OSStateFlags));
        NANDClose(&file);

        if (bytesRead != sizeof(OSStateFlags)) {
            NANDDelete("/title/00000001/00000002/data/state.dat");
            memset(stateOut, 0, sizeof(OSStateFlags));
            return FALSE;
        }
    } else {
        memset(stateOut, 0, sizeof(OSStateFlags));
        return FALSE;
    }

    if (CheckSum(&StateFlags) != StateFlags.checksum) {
        memset(stateOut, 0, sizeof(OSStateFlags));
        return FALSE;
    }

    memcpy(stateOut, &StateFlags, sizeof(OSStateFlags));
    return TRUE;
}
