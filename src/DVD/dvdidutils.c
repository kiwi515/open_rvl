#include "dvdidutils.h"

#include <string.h>

#define GAME_NAME_LEN 4
#define COMPANY_NAME_LEN 2
#define DISK_NUM_LEN 1
#define VER_NUM_LEN 1

#define GAME_NAME_IDX 0
#define COMPANY_NAME_IDX (GAME_NAME_IDX + GAME_NAME_LEN)
#define DISK_NUM_IDX (COMPANY_NAME_IDX + COMPANY_NAME_LEN)
#define VER_NUM_IDX (DISK_NUM_IDX + DISK_NUM_LEN)

BOOL DVDCompareDiskID(const char* id1, const char* id2) {
    // Compare game name
    if (id1[GAME_NAME_IDX] != '\0' && id2[GAME_NAME_IDX] != '\0' &&
        strncmp(id1, id2, GAME_NAME_LEN) != 0) {
        return FALSE;
    }

    // Compare company name
    if (id1[COMPANY_NAME_IDX] == '\0' || id2[COMPANY_NAME_IDX] == '\0' ||
        strncmp(id1 + COMPANY_NAME_IDX, id2 + COMPANY_NAME_IDX,
                COMPANY_NAME_LEN) != 0) {
        return FALSE;
    }

    // Compare disk number
    if ((u8)id1[DISK_NUM_IDX] != 0xFF && (u8)id2[DISK_NUM_IDX] != 0xFF &&
        (u8)id1[DISK_NUM_IDX] != (u8)id2[DISK_NUM_IDX]) {
        return FALSE;
    }

    // Compare version number
    if ((u8)id1[VER_NUM_IDX] != 0xFF && (u8)id2[VER_NUM_IDX] != 0xFF &&
        (u8)id1[VER_NUM_IDX] != (u8)id2[VER_NUM_IDX]) {
        return FALSE;
    }

    return TRUE;
}
