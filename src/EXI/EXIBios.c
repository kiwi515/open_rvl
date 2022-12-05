#include "EXIBios.h"

#include <OS/OS.h>
#include <OS/OSInterrupt.h>
#include <OS/OSTime.h>
#include <string.h>

const char* __EXIVersion =
    "<< RVL_SDK - EXI \trelease build: Nov 30 2006 03:26:56 (0x4199_60831) >>";
static EXIData Ecb[EXI_MAX_CHAN];
static u32 IDSerialPort1;

static void SetExiInterruptMask(EXIChannel chan, EXIData* exi) {
    EXIData* exi2 = &Ecb[EXI_CHAN_2];

    switch (chan) {
    case EXI_CHAN_0:
        if (exi->exiCallback == NULL && exi2->exiCallback == NULL ||
            exi->state & EXI_STATE_LOCKED) {
            __OSMaskInterrupts(0x410000);
        } else {
            __OSUnmaskInterrupts(0x410000);
        }
        break;
    case EXI_CHAN_1:
        if (exi->exiCallback == NULL || exi->state & EXI_STATE_LOCKED) {
            __OSMaskInterrupts(0x80000);
        } else {
            __OSUnmaskInterrupts(0x80000);
        }
        break;
    case EXI_CHAN_2:
        if (__OSGetInterruptHandler(OS_INTR_PI_DEBUG) == NULL ||
            exi->state & EXI_STATE_LOCKED) {
            __OSMaskInterrupts(0x40);
        } else {
            __OSUnmaskInterrupts(0x40);
        }
    }
}

static void CompleteTransfer(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    s32 len;

    if (exi->state & EXI_STATE_BUSY) {
        if (exi->state & EXI_STATE_IMM_ACCESS && (len = exi->bytesRead) != 0) {
            u8* buf;
            u32 data;
            int i;

            buf = (u8*)exi->buffer;
            data = EXI_CD006800[chan].WORD_0x10;
            for (i = 0; i < len; i++) {
                *buf++ = data >> (3 - i) * 8;
            }
        }

        exi->state &= ~EXI_STATE_BUSY;
    }
}

BOOL EXIImm(EXIChannel chan, void* buf, s32 len, u32 type,
            EXICallback callback) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled = OSDisableInterrupts();

    if (exi->state & EXI_STATE_BUSY || !(exi->state & EXI_STATE_SELECTED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->tcCallback = callback;
    if (callback != NULL) {
        u32 mask = 0x200000;
        EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
        __OSUnmaskInterrupts(mask >> (chan * 3));
    }

    exi->state |= EXI_STATE_IMM_ACCESS;

    if (type != EXI_READ) {
        int i;
        u32 word = 0;
        for (i = 0; i < len; i++) {
            word |= ((u8*)buf)[i] << (3 - i) * 8;
        }

        EXI_CD006800[chan].WORD_0x10 = word;
    }

    exi->buffer = buf;
    exi->bytesRead = (type != EXI_WRITE) ? len : 0;
    EXI_CD006800[chan].WORD_0xC = type << 2 | 1 | (len - 1) * 16;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIImmEx(EXIChannel chan, void* buf, s32 len, u32 type) {
    while (len != 0) {
        s32 immLen = (len < 4) ? len : 4;

        if (!EXIImm(chan, buf, immLen, type, FALSE)) {
            return FALSE;
        }

        if (!EXISync(chan)) {
            return FALSE;
        }

        buf = (u8*)buf + immLen;
        len -= immLen;
    }

    return TRUE;
}

BOOL EXIDma(EXIChannel chan, void* buf, s32 len, u32 type,
            EXICallback callback) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled = OSDisableInterrupts();

    if (exi->state & EXI_STATE_BUSY || !(exi->state & EXI_STATE_SELECTED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->tcCallback = callback;
    if (callback != NULL) {
        u32 mask = 0x200000;
        EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
        __OSUnmaskInterrupts(mask >> (chan * 3));
    }

    exi->state |= EXI_STATE_DMA_ACCESS;

    EXI_CD006800[chan].WORD_0x4 = (u32)buf & 0xFFFFFFE0;
    EXI_CD006800[chan].WORD_0x8 = len;
    EXI_CD006800[chan].WORD_0xC = type << 2 | 3;

    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXISync(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    BOOL ret = FALSE;

    while (exi->state & EXI_STATE_SELECTED) {
        if (!(EXI_CD006800[chan].WORD_0xC & 1)) {
            BOOL enabled = OSDisableInterrupts();

            if (exi->state & EXI_STATE_SELECTED) {
                CompleteTransfer(chan);

                if (__OSGetDIConfig() != 0xFF ||
                    (OSGetConsoleType() & OS_CONSOLE_MASK) ==
                        OS_CONSOLE_MASK_TDEV ||
                    exi->bytesRead != 4 || EXI_CD006800[chan].WORD_0x0 & 0x70 ||
                    (EXI_CD006800[chan].WORD_0x10 != 0x01010000 &&
                     EXI_CD006800[chan].WORD_0x10 != 0x05070000 &&
                     EXI_CD006800[chan].WORD_0x10 != 0x04220001) ||
                    OS_DVD_DEVICE_CODE_ADDR == 0x8200) {
                    ret = TRUE;
                }
            }

            OSRestoreInterrupts(enabled);
            break;
        }
    }

    return ret;
}

void EXIClearInterrupts(EXIChannel chan, BOOL r4, BOOL r5, BOOL r6) {
    u32 val = EXI_CD006800[chan].WORD_0x0 & 0x7F5;

    if (r4) {
        val |= 0x2;
    }

    if (r5) {
        val |= 0x8;
    }

    if (r6) {
        val |= 0x800;
    }

    EXI_CD006800[chan].WORD_0x0 = val;
}

EXICallback EXISetExiCallback(EXIChannel chan, EXICallback callback) {
    EXIData* exi;
    EXICallback old;
    BOOL enabled;

    exi = &Ecb[chan];
    enabled = OSDisableInterrupts();

    old = exi->exiCallback;
    exi->exiCallback = callback;

    if (chan != EXI_CHAN_2) {
        SetExiInterruptMask(chan, exi);
    } else {
        SetExiInterruptMask(EXI_CHAN_0, &Ecb[EXI_CHAN_0]);
    }

    OSRestoreInterrupts(enabled);
    return old;
}

void EXIProbeReset(void) {
    OS_GLOBAL_800030C0[0] = OS_GLOBAL_800030C0[1] = 0;
    Ecb[EXI_CHAN_0].WORD_0x20 = Ecb[EXI_CHAN_1].WORD_0x20 = 0;
    __EXIProbe(EXI_CHAN_0);
    __EXIProbe(EXI_CHAN_1);
}

static BOOL __EXIProbe(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    BOOL ret;
    u32 flag;

    if (chan == EXI_CHAN_2) {
        return TRUE;
    }

    ret = TRUE;
    enabled = OSDisableInterrupts();
    flag = EXI_CD006800[chan].WORD_0x0;

    if (!(exi->state & EXI_STATE_ATTACHED)) {
        if (flag & 0x800) {
            EXIClearInterrupts(chan, FALSE, FALSE, TRUE);
            exi->WORD_0x20 = 0;
            OS_GLOBAL_800030C0[chan] = 0;
        }

        if (flag & 0x1000) {
            s32 time = (s32)(OS_TICKS_TO_MSEC(OSGetTime()) / 100) + 1;
            if (OS_GLOBAL_800030C0[chan] == 0) {
                OS_GLOBAL_800030C0[chan] = time;
            }

            if (time - OS_GLOBAL_800030C0[chan] < 3) {
                ret = FALSE;
            }
        } else {
            exi->WORD_0x20 = 0;
            OS_GLOBAL_800030C0[chan] = 0;
            ret = FALSE;
        }
    } else if (!(flag & 0x1000) || flag & 0x800) {
        exi->WORD_0x20 = 0;
        OS_GLOBAL_800030C0[chan] = 0;
        ret = FALSE;
    }

    OSRestoreInterrupts(enabled);
    return ret;
}

BOOL EXIProbe(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];

    BOOL ret = __EXIProbe(chan);
    if (ret && exi->WORD_0x20 == 0) {
        u32 id;
        return EXIGetID(chan, 0, &id);
    }
}

static BOOL __EXIAttach(EXIChannel chan, EXICallback callback) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    u32 mask;

    enabled = OSDisableInterrupts();
    if (exi->state & EXI_STATE_ATTACHED || !__EXIProbe(chan)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    EXIClearInterrupts(chan, TRUE, FALSE, FALSE);
    exi->extCallback = callback;
    mask = 0x100000;
    __OSUnmaskInterrupts(mask >> (chan * 3));
    exi->state |= EXI_STATE_ATTACHED;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIAttach(EXIChannel chan, EXICallback callback) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    BOOL ret;

    EXIProbe(chan);
    enabled = OSDisableInterrupts();

    if (exi->WORD_0x20 == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    ret = __EXIAttach(chan, callback);
    OSRestoreInterrupts(enabled);
    return ret;
}

BOOL EXIDetach(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    u32 mask;

    enabled = OSDisableInterrupts();
    if (!(exi->state & EXI_STATE_ATTACHED)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    if (exi->state & EXI_STATE_LOCKED && exi->dev == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~EXI_STATE_ATTACHED;
    mask = 0x500000;
    __OSMaskInterrupts(mask >> (chan * 3));
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXISelect(EXIChannel chan, u32 dev, u32 freq) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    u32 flag;

    enabled = OSDisableInterrupts();

    if (exi->state & EXI_STATE_SELECTED ||
        chan != EXI_CHAN_2 &&
            (dev == 0 && !(exi->state & EXI_STATE_ATTACHED) &&
                 !__EXIProbe(chan) ||
             !(exi->state & EXI_STATE_LOCKED) || exi->dev != dev)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state |= EXI_STATE_SELECTED;

    flag = EXI_CD006800[chan].WORD_0x0;
    flag &= 0x405;
    flag |= (1 << dev) << 7 | freq << 4;
    EXI_CD006800[chan].WORD_0x0 = flag;

    if (exi->state & EXI_STATE_ATTACHED) {
        switch (chan) {
        case EXI_CHAN_0:
            __OSMaskInterrupts(0x100000);
            break;
        case EXI_CHAN_1:
            __OSMaskInterrupts(0x20000);
            break;
        }
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIDeselect(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    u32 flag;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (!(exi->state & EXI_STATE_SELECTED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~EXI_STATE_SELECTED;
    flag = EXI_CD006800[chan].WORD_0x0;
    EXI_CD006800[chan].WORD_0x0 = flag & 0x405;

    if (exi->state & EXI_STATE_ATTACHED) {
        switch (chan) {
        case EXI_CHAN_0:
            __OSUnmaskInterrupts(0x100000);
            break;
        case EXI_CHAN_1:
            __OSUnmaskInterrupts(0x20000);
            break;
        }
    }

    OSRestoreInterrupts(enabled);

    if (chan != EXI_CHAN_2 && flag & 0x80) {
        return __EXIProbe(chan) != FALSE;
    }

    return TRUE;
}

static void EXIIntrruptHandler(s16 intr, OSContext* ctx) {
    EXIData* exi;
    EXIChannel chan;
    EXICallback callback;
    OSContext temp;

    chan = (EXIChannel)((intr - OS_INTR_EXI_0_EXI) / 3);
    exi = &Ecb[chan];

    EXIClearInterrupts(chan, TRUE, FALSE, FALSE);

    callback = exi->exiCallback;
    if (callback != NULL) {
        OSClearContext(&temp);
        OSSetCurrentContext(&temp);
        callback(chan, ctx);
        OSClearContext(&temp);
        OSSetCurrentContext(ctx);
    }
}

static void TCIntrruptHandler(s16 intr, OSContext* ctx) {
    EXIData* exi;
    EXIChannel chan;
    EXICallback callback;
    OSContext temp;

    chan = (EXIChannel)((intr - OS_INTR_EXI_0_TC) / 3);
    exi = &Ecb[chan];

    __OSMaskInterrupts(0x80000000 >> intr);
    EXIClearInterrupts(chan, FALSE, TRUE, FALSE);

    callback = exi->tcCallback;
    if (callback != NULL) {
        exi->tcCallback = NULL;
        CompleteTransfer(chan);
        OSClearContext(&temp);
        OSSetCurrentContext(&temp);
        callback(chan, ctx);
        OSClearContext(&temp);
        OSSetCurrentContext(ctx);
    }
}

static void EXTIntrruptHandler(s16 intr, OSContext* ctx) {
    EXIChannel chan;
    EXIData* exi;
    EXICallback callback;
    OSContext temp;
    u32 mask;

    chan = (EXIChannel)((intr - OS_INTR_EXI_0_EXT) / 3);
    mask = 0x500000;
    __OSMaskInterrupts(mask >> (chan * 3));

    exi = &Ecb[chan];
    callback = exi->extCallback;
    exi->state &= ~EXI_STATE_ATTACHED;

    if (callback != NULL) {
        OSClearContext(&temp);
        OSSetCurrentContext(&temp);
        exi->extCallback = NULL;
        callback(chan, ctx);
        OSClearContext(&temp);
        OSSetCurrentContext(ctx);
    }
}

void EXIInit(void) {
    u32 id;

    do {
        do {
        } while ((EXI_CD006800[EXI_CHAN_0].WORD_0xC & 1) == 1);
    } while ((EXI_CD006800[EXI_CHAN_1].WORD_0xC & 1) == 1 ||
             (EXI_CD006800[EXI_CHAN_2].WORD_0xC & 1) == 1);

    __OSMaskInterrupts(0x7F8000);

    EXI_CD006800[EXI_CHAN_0].WORD_0x0 = 0;
    EXI_CD006800[EXI_CHAN_1].WORD_0x0 = 0;
    EXI_CD006800[EXI_CHAN_2].WORD_0x0 = 0;

    EXI_CD006800[EXI_CHAN_0].WORD_0x0 = 0x2000;

    __OSSetInterruptHandler(OS_INTR_EXI_0_EXI, EXIIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_0_TC, TCIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_0_EXT, EXTIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_1_EXI, EXIIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_1_TC, TCIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_1_EXT, EXTIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_2_EXI, EXIIntrruptHandler);
    __OSSetInterruptHandler(OS_INTR_EXI_2_TC, TCIntrruptHandler);

    EXIGetID(EXI_CHAN_0, 2, &IDSerialPort1);

    if (__OSInIPL) {
        EXIProbeReset();
    } else {
        if ((EXIGetID(EXI_CHAN_0, 0, &id) && id == 0x07010000)) {
            __OSEnableBarnacle(EXI_CHAN_1, 0);
        } else if (EXIGetID(EXI_CHAN_1, 0, &id) && id == 0x07010000) {
            __OSEnableBarnacle(EXI_CHAN_0, 2);
        }
    }

    OSRegisterVersion(__EXIVersion);
}

BOOL EXILock(EXIChannel chan, u32 dev, EXICallback callback) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    if (exi->state & EXI_STATE_LOCKED) {
        if (callback != NULL) {
            for (i = 0; i < exi->numItems; i++) {
                if (exi->items[i].dev == dev) {
                    OSRestoreInterrupts(enabled);
                    return FALSE;
                }
            }

            exi->items[exi->numItems].callback = callback;
            exi->items[exi->numItems].dev = dev;
            exi->numItems++;
        }

        OSRestoreInterrupts(enabled);
        return FALSE;
    } else {
        exi->state |= EXI_STATE_LOCKED;
        exi->dev = dev;
        SetExiInterruptMask(chan, exi);
        OSRestoreInterrupts(enabled);
        return TRUE;
    }
}

BOOL EXIUnlock(EXIChannel chan) {
    EXIData* exi = &Ecb[chan];
    BOOL enabled;
    EXICallback callback;
    s32 numItems;

    enabled = OSDisableInterrupts();

    if (!(exi->state & EXI_STATE_LOCKED)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= ~EXI_STATE_LOCKED;
    SetExiInterruptMask(chan, exi);

    numItems = exi->numItems;
    if (exi->numItems > 0) {
        callback = exi->items[0].callback;
        numItems = --exi->numItems;

        if (numItems > 0) {
            memmove(&exi->items[0], &exi->items[1],
                    exi->numItems * sizeof(EXIItem));
        }

        callback(chan, NULL);
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

// Does nothing???
static void UnlockedHandler(EXIChannel chan, OSContext* ctx) {
    u32 id;
    EXIGetID(chan, 0, &id);
}

#ifdef NON_MATCHING
// EXIUnlock isn't inlining when it needs to be.
s32 EXIGetID(EXIChannel chan, u32 dev, u32* out) {
    EXIData* exi = &Ecb[chan];
    u32 imm;
    s32 ret;
    s32 val;
    BOOL enabled;

    if (chan == EXI_CHAN_0 && dev == 2 && IDSerialPort1 != 0) {
        *out = IDSerialPort1;
        return 1;
    }

    if (chan < EXI_CHAN_2 && dev == 0) {
        if (!__EXIProbe(chan)) {
            return 0;
        }

        if (exi->WORD_0x20 == OS_GLOBAL_800030C0[chan]) {
            *out = exi->id;
            return exi->WORD_0x20;
        }

        if (!__EXIAttach(chan, NULL)) {
            return 0;
        }

        val = OS_GLOBAL_800030C0[chan];
    }

    enabled = OSDisableInterrupts();
    ret = !EXILock(chan, dev, (chan < 2 && dev == 0) ? UnlockedHandler : NULL);
    if (ret == 0) {
        ret = !EXISelect(chan, dev, 0);
        if (ret == 0) {
            imm = 0;
            ret |= !EXIImm(chan, &imm, sizeof(u16), EXI_WRITE, NULL);
            ret |= !EXISync(chan);
            ret |= !EXIImm(chan, out, sizeof(u32), EXI_READ, NULL);
            ret |= !EXISync(chan);
            ret |= !EXIDeselect(chan);
        }
        EXIUnlock(chan);
    }

    OSRestoreInterrupts(enabled);

    if (chan < EXI_CHAN_2 && dev == 0) {
        EXIDetach(chan);

        enabled = OSDisableInterrupts();
        ret |= val != OS_GLOBAL_800030C0[chan];
        if (ret == 0) {
            exi->id = *out;
            exi->WORD_0x20 = val;
        }

        OSRestoreInterrupts(enabled);
        return ret != 0 ? 0 : exi->WORD_0x20;
    }

    return ret == 0;
}
#endif
