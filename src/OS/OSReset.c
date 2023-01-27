#include <DVD.h>
#include <OS.h>
#include <PAD/Pad.h>
#include <SC.h>
#include <VI/vi3in1.h>

#include <string.h>

static OSShutdownFunctionQueue ShutdownFunctionQueue;

static void KillThreads(void);

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info) {
    OSShutdownFunctionInfo* it;
    OSShutdownFunctionInfo* prev;
    OSShutdownFunctionInfo* tail;

    for (it = ShutdownFunctionQueue.head; it != NULL && it->prio <= info->prio;
         it = it->next) {
        ;
    }

    if (it == NULL) {

        tail = ShutdownFunctionQueue.tail;
        if (tail == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            ShutdownFunctionQueue.tail->next = info;
        }

        info->prev = tail;
        info->next = NULL;

        it = ShutdownFunctionQueue.head;
        ShutdownFunctionQueue.tail = info;
    } else {
        info->next = it;

        prev = it->prev;
        it->prev = info;
        info->prev = prev;

        if (prev == NULL) {
            ShutdownFunctionQueue.head = info;
        } else {
            prev->next = info;
        }
    }
}

BOOL __OSCallShutdownFunctions(u32 arg0, u32 arg1) {
    OSShutdownFunctionInfo* iter;
    BOOL failure;
    u32 prio;

    prio = 0;
    failure = FALSE;

    for (iter = ShutdownFunctionQueue.head; iter != NULL; iter = iter->next) {
        if (failure && prio != iter->prio) {
            break;
        }

        failure |= !iter->func(arg0, arg1);
        prio = iter->prio;
    }

    failure |= !__OSSyncSram();

    return !failure;
}

void __OSShutdownDevices(u32 arg0) {
    BOOL padIntr;
    BOOL osIntr;
    BOOL keepEnable;

    switch (arg0) {
    case 0:
    case 4:
    case 5:
    case 6:
        keepEnable = FALSE;
        break;
    case 1:
    case 2:
    case 3:
    default:
        keepEnable = TRUE;
        break;
    }

    __OSStopAudioSystem();

    if (!keepEnable) {
        padIntr = __PADDisableRecalibration(TRUE);
    }

    while (!__OSCallShutdownFunctions(0, arg0)) {
        ;
    }

    while (!__OSSyncSram()) {
        ;
    }

    osIntr = OSDisableInterrupts();
    __OSCallShutdownFunctions(1, arg0);
    LCDisable();

    if (!keepEnable) {
        __PADDisableRecalibration(padIntr);
    }

    KillThreads();
}

// TODO: There must be a better way....
void __OSGetDiscState(u8* out) {
    u32 flags;

    if (__DVDGetCoverStatus() != 2) {
        *out = 3;
    } else if (*out == 1) {
        if (!__OSGetRTCFlags(&flags) || flags == 0) {
            goto status_1;
        }

    status_2:
        *out = 2;
    } else {
        goto status_2;

    status_1:
        *out = 1;
    }
}

static void KillThreads(void) {
    OSThread* iter;
    OSThread* next;

    for (iter = OS_THREAD_QUEUE.head; iter != NULL; iter = next) {
        next = iter->next2;

        switch (iter->state) {
        case OS_THREAD_STATE_SLEEPING:
        case OS_THREAD_STATE_READY:
            OSCancelThread(iter);
            break;
        }
    }
}

void OSShutdownSystem(void) {
    SCIdleMode idleMode;
    OSStateFlags stateFlags;
    OSIOSRev iosRev;

    memset(&idleMode, 0, sizeof(SCIdleMode));
    SCInit();
    while (SCCheckStatus() == SC_STATUS_1) {
        ;
    }
    SCGetIdleMode(&idleMode);

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();
    __OSReadStateFlags(&stateFlags);

    __OSGetDiscState(&stateFlags.discState);
    if (idleMode.wc24 == TRUE) {
        stateFlags.BYTE_0x5 = 5;
    } else {
        stateFlags.BYTE_0x5 = 1;
    }

    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);
    __OSGetIOSRev(&iosRev);

    if (idleMode.wc24 == TRUE) {
        OSDisableScheduler();
        __OSShutdownDevices(5);
        OSEnableScheduler();
        __OSLaunchMenu();
    } else {
        OSDisableScheduler();
        __OSShutdownDevices(2);
        __OSShutdownToSBY();
    }
}

void OSReturnToMenu(void) {
    OSStateFlags stateFlags;

    __OSStopPlayRecord();
    __OSUnRegisterStateEvent();
    __DVDPrepareReset();

    __OSReadStateFlags(&stateFlags);
    __OSGetDiscState(&stateFlags.discState);
    stateFlags.BYTE_0x5 = 3;
    __OSClearRTCFlags();
    __OSWriteStateFlags(&stateFlags);

    OSDisableScheduler();
    __OSShutdownDevices(5);
    OSEnableScheduler();

    __OSLaunchMenu();
    OSDisableScheduler();
    __VISetRGBModeImm();
    __OSHotReset();

#line 843
    OSError("OSReturnToMenu(): Falied to boot system menu.\n");
}

u32 OSGetResetCode(void) {
    if (__OSRebootParams.WORD_0x0 != 0) {
        return __OSRebootParams.WORD_0x4 | 0x80000000;
    }

    return OS_PI_RESET >> 3;
}

void OSResetSystem(u32 arg0, u32 arg1, u32 arg2) {
#pragma unused(arg0)
#pragma unused(arg1)
#pragma unused(arg2)

#line 1020
    OSError("OSResetSystem() is obsoleted. It doesn't work any longer.\n");
}
