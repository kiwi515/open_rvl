#include "OSTime.h"

#define OS_TIME_USEC_MAX 1000
#define OS_TIME_MSEC_MAX 1000
#define OS_TIME_MONTH_MAX 12
#define OS_TIME_WEEK_DAY_MAX 7
#define OS_TIME_YEAR_DAY_MAX 365

#define OS_TIME_SECS_IN_HOUR (60 * 60)
#define OS_TIME_SECS_IN_DAY (OS_TIME_SECS_IN_HOUR * 24)

#define BIAS 0xB2575

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
    const BOOL enabled = OSDisableInterrupts();
    const s64 time = OSGetTime() + OS_SYSTEM_TIME;
    OSRestoreInterrupts(enabled);
    return time;
}

s64 __OSTimeToSystemTime(s64 time) {
    const BOOL enabled = OSDisableInterrupts();
    const s64 sysTime = OS_SYSTEM_TIME + time;
    OSRestoreInterrupts(enabled);
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

static void GetDates(s32 days, OSCalendarTime* cal)
    __attribute__((never_inline)) {
    s32 year;
    s32 totalDays;
    s32* p_days;
    s32 month;

    cal->week_day = (days + 6) % OS_TIME_WEEK_DAY_MAX;

    // WTF??
    for (year = days / OS_TIME_YEAR_DAY_MAX;
         days < (totalDays = year * OS_TIME_YEAR_DAY_MAX + GetLeapDays(year));
         year--) {
        ;
    }
    days -= totalDays;
    cal->year = year;
    cal->year_day = days;

    p_days = IsLeapYear(year) ? LeapYearDays : YearDays;
    for (month = OS_TIME_MONTH_MAX; days < p_days[--month];) {
        ;
    }
    cal->month = month;
    cal->month_day = days - p_days[month] + 1;
}

void OSTicksToCalendarTime(s64 ticks, OSCalendarTime* cal) {
    s32 days, secs;
    s64 d;

    d = ticks % OS_SEC_TO_TICKS(1);
    if (d < 0) {
        d += OS_SEC_TO_TICKS(1);
    }

    cal->usec = OS_TICKS_TO_USEC(d) % OS_TIME_USEC_MAX;
    cal->msec = OS_TICKS_TO_MSEC(d) % OS_TIME_MSEC_MAX;
    ticks -= d;

    days = (OS_TICKS_TO_SEC(ticks) / OS_TIME_SECS_IN_DAY) + BIAS;
    secs = OS_TICKS_TO_SEC(ticks) % OS_TIME_SECS_IN_DAY;
    if (secs < 0) {
        days -= 1;
        secs += OS_TIME_SECS_IN_DAY;
    }

    GetDates(days, cal);
    cal->hour = secs / 60 / 60;
    cal->min = (secs / 60) % 60;
    cal->sec = secs % 60;
}

#ifdef NON_MATCHING
#error OSCalendarTimeToTicks has not yet been matched. (// https://decomp.me/scratch/fAlfM)
#endif
s64 OSCalendarTimeToTicks(const OSCalendarTime* cal) {
    ;
    ;
}
