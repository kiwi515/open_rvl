#include "OSTime.h"

#define OS_TIME_MONTH_MAX 12
#define OS_TIME_WEEK_DAY_MAX 7
#define OS_TIME_YEAR_DAY_MAX 365

// End of each month in standard year
static s32 YearDays[OS_TIME_MONTH_MAX] = {0,   31,  59,  90,  120, 151,
                                          181, 212, 243, 273, 304, 334};
// End of each month in leap year
static s32 LeapYearDays[OS_TIME_MONTH_MAX] = {0,   31,  60,  91,  121, 152,
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
    const BOOL intr = OSDisableInterrupts();
    const s64 time = OSGetTime() + OS_SYSTEM_TIME;
    OSRestoreInterrupts(intr);
    return time;
}

s64 __OSTimeToSystemTime(s64 time) {
    const BOOL intr = OSDisableInterrupts();
    const s64 sysTime = OS_SYSTEM_TIME + time;
    OSRestoreInterrupts(intr);
    return sysTime;
}

static BOOL IsLeapYear(s32 year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static s32 GetYearDays(s32 year, s32 mon) {
    return (IsLeapYear(year) ? LeapYearDays : YearDays)[mon];
}

static s32 GetLeapDays(s32 year) {
    if (year < 1) {
        return 0;
    }
    return (year + 3) / 4 - (year - 1) / 100 + (year - 1) / 400;
}

static void GetDates(s32 days, OSCalendarTime* cal) {
    cal->week_day = (days + 6) % OS_TIME_WEEK_DAY_MAX;

    s32 year;
    s32 totalDays;
    // WTF??
    for (year = days / OS_TIME_YEAR_DAY_MAX;
         days < (totalDays = year * OS_TIME_YEAR_DAY_MAX + GetLeapDays(year));
         year--) {
        ;
    }
    days -= totalDays;
    cal->year = year;
    cal->year_day = days;

    s32* p_days = IsLeapYear(year) ? LeapYearDays : YearDays;
    s32 month = OS_TIME_MONTH_MAX;
    while (days < p_days[--month]) {
        ;
    }
    cal->mon = month;
    cal->mon_day = days - p_days[month] + 1;
}

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal) {
    ;
    ;
}

s64 OSCalendarTimeToTicks(const OSCalendarTime* cal) {
    ;
    ;
}
