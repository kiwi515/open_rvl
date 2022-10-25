#include "OSTime.h"

// End of each month in standard year
static s32 YearDays[12] = {0,   31,  59,  90,  120, 151,
                           181, 212, 243, 273, 304, 334};
// End of each month in leap year
static s32 LeapYearDays[12] = {0,   31,  60,  91,  121, 152,
                               182, 213, 244, 274, 305, 335};

asm s64 OSGetTime(void) {
    // clang-format off
    nofralloc

    mftbu r3
    mftbl r4

    // Check for possible carry from TBL to TBU
    mftbu r5
    cmpw r3, r5
    bne OSGetTime

    blr
    // clang-format on
}

asm s32 OSGetTick(void){
    // clang-format off
    nofralloc

    mftbl r3
    blr
    // clang-format on
}

s64 __OSGetSystemTime(void) {
    ;
    ;
}

s64 __OSTimeToSystemTime(void) {
    ;
    ;
}

static BOOL IsLeapYear(s32 year) {
    ;
    ;
}

static s32 GetYearDays(s32 year, s32 mon) {
    // TO-DO: Try ternary
    s32* days = NULL;
    if (IsLeapYear(year)) {
        days = LeapYearDays;
    } else {
        days = YearDays;
    }

    return days[mon];
}

static s32 GetLeapDays(s32 year) {
    ;
    ;
}

static s32 GetDates(s32 days, OSCalendarTime* cal) {
    ;
    ;
}

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal) {
    ;
    ;
}

s64 OSCalendarTimeToTicks(const OSCalendarTime* cal) {
    ;
    ;
}
