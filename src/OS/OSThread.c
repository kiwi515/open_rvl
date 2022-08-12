#include "OSThread.h"
#include "OSAlarm.h"
#include "OSError.h"
#include "OSInterrupt.h"
#include "OSMutex.h"
#include "__ppc_eabi_init.h"

static void DefaultSwitchThreadCallback(OSThread* currThread,
                                        OSThread* newThread) {}
static OSSwitchThreadCallback SwitchThreadCallback =
    DefaultSwitchThreadCallback;

static OSThread DefaultThread;
static OSThreadQueue RunQueue[32];
static OSContext IdleContext;
static OSThread IdleThread;

volatile static s32 Reschedule = 0;
volatile static BOOL RunQueueHint = FALSE;
volatile static u32 RunQueueBits = 0;

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback newCb) {
    OSSwitchThreadCallback oldCb;
    u32 interrupt = OSDisableInterrupts();

    oldCb = SwitchThreadCallback;
    SwitchThreadCallback =
        (newCb != NULL) ? newCb : DefaultSwitchThreadCallback;

    OSRestoreInterrupts(interrupt);
    return (oldCb == DefaultSwitchThreadCallback) ? NULL : oldCb;
}

void __OSThreadInit(void) {
    int i;
    OSThread* thread = &DefaultThread;
    OSThreadQueue* queue;
    OSThread* tail;

    thread->state = OS_THREAD_STATE_RUNNING;
    thread->flags = OS_THREAD_DETACHED;
    thread->WORD_0x2D4 = 16;
    thread->priority = 16;
    thread->suspend = 0;
    thread->WORD_0x2D8 = 0xFFFFFFFF;
    thread->mutex = NULL;

    OSInitThreadQueue(&thread->threadQueue);
    OSInitMutexQueue(&thread->mutexQueue);

    OS_CURRENT_FPU_CONTEXT = &thread->context;
    OSClearContext(&thread->context);
    OSSetCurrentContext(&thread->context);

    thread->stackBegin = (u32*)_stack_addr;
    thread->stackEnd = (u32*)_stack_end;
    *thread->stackEnd = OS_THREAD_STACK_MAGIC;

    OSSetCurrentThread(thread);
    OSClearStack(0x00);

    RunQueueBits = 0;
    RunQueueHint = FALSE;
    for (i = 0; i < 32; i++) {
        OSInitThreadQueue(&RunQueue[i]);
    }

    OSInitThreadQueue(&OS_THREAD_QUEUE);

    tail = OS_THREAD_QUEUE.tail;
    if (tail == NULL) {
        OS_THREAD_QUEUE.head = thread;
    } else {
        tail->next2 = thread;
    }

    thread->prev2 = tail;
    thread->next2 = NULL;
    OS_THREAD_QUEUE.tail = thread;

    OSClearContext(&IdleContext);
    Reschedule = 0;
}

void OSSetCurrentThread(OSThread* thread) {
    SwitchThreadCallback(OSGetCurrentThread(), thread);
    OS_CURRENT_THREAD = thread;
}

void OSInitMutexQueue(OSMutexQueue* queue) {
    queue->tail = NULL;
    queue->head = NULL;
}

void OSInitThreadQueue(OSThreadQueue* queue) {
    queue->tail = NULL;
    queue->head = NULL;
}

OSThread* OSGetCurrentThread(void) { return OS_CURRENT_THREAD; }

static void __OSSwitchThread(OSThread* thread) {
    OSSetCurrentThread(thread);
    OSSetCurrentContext(&thread->context);
    OSLoadContext(&thread->context);
}

BOOL OSIsThreadTerminated(OSThread* thread) {
    return thread->state == OS_THREAD_STATE_MORIBUND ||
                   thread->state == OS_THREAD_STATE_EXITED
               ? TRUE
               : FALSE;
}

static BOOL __OSIsThreadActive(OSThread* thread) {
    OSThread* iter;

    if (thread->state == OS_THREAD_STATE_EXITED) {
        return FALSE;
    }

    for (iter = OS_THREAD_QUEUE.head; iter != NULL; iter = iter->next2) {
        if (thread == iter) {
            return TRUE;
        }
    }

    return FALSE;
}

s32 OSDisableScheduler(void) {
    s32 old;
    u32 interrupt = OSDisableInterrupts();

    old = Reschedule++;

    OSRestoreInterrupts(interrupt);

    return old;
}

s32 OSEnableScheduler(void) {
    s32 old;
    u32 interrupt = OSDisableInterrupts();

    old = Reschedule--;

    OSRestoreInterrupts(interrupt);

    return old;
}

void OSClearStack(u8 val) {
    u32* end;
    u32* begin;
    u32 longVal;

    longVal = val << 24 | val << 16 | val << 8 | val;

    begin = (u32*)OSGetStackPointer();
    end = OSGetCurrentThread()->stackEnd + 1;

    for (; end < (u32*)begin; end++) {
        *end = longVal;
    }
}

static void SetRun(OSThread* thread) {
    OSThreadQueue* queue;
    OSThread* tail;

    queue = &RunQueue[thread->priority];
    thread->queue = queue;
    tail = thread->queue->tail;

    if (tail == NULL) {
        thread->queue->head = thread;
    } else {
        tail->next = thread;
    }

    thread->prev = tail;
    thread->next = NULL;
    thread->queue->tail = thread;

    RunQueueBits |= (1 << 31 - thread->priority);
    RunQueueHint = TRUE;
}

static void UnsetRun(OSThread* thread) {
    OSThreadQueue* queue;
    OSThread* next;
    OSThread* prev;

    next = thread->next;
    queue = thread->queue;
    prev = thread->prev;

    if (next == NULL) {
        queue->tail = prev;
    } else {
        next->prev = prev;
    }

    if (prev == NULL) {
        queue->head = next;
    } else {
        prev->next = next;
    }

    if (queue->head == NULL) {
        RunQueueBits &= ~(1 << OS_PRIORITY_MAX - thread->priority);
    }

    thread->queue = NULL;
}

s32 __OSGetEffectivePriority(OSThread* thread) {
    s32 prio = thread->WORD_0x2D4;

    OSMutex* mutex;
    for (mutex = thread->mutexQueue.head; mutex != NULL; mutex = mutex->next) {
        OSThread* mutexThread = mutex->queue.head;
        if (mutexThread != NULL && mutexThread->priority < prio) {
            prio = mutexThread->priority;
        }
    }

    return prio;
}

static OSThread* SetEffectivePriority(OSThread* thread, s32 prio) {
    OSThread* iter;
    OSThread* iterPrev;
    OSThread* next;
    OSThread* prev;

    switch (thread->state) {
    case OS_THREAD_STATE_READY:
        UnsetRun(thread);
        thread->priority = prio;
        thread->queue = &RunQueue[prio];
        SetRun(thread);
        break;
    case OS_THREAD_STATE_SLEEPING:
        next = thread->next;
        prev = thread->prev;
        if (next == NULL) {
            thread->queue->tail = prev;
        } else {
            next->prev = prev;
        }

        if (prev == NULL) {
            thread->queue->head = next;
        } else {
            prev->next = next;
        }

        thread->priority = prio;

        for (iter = thread->queue->head;
             iter != NULL && iter->priority <= thread->priority;
             iter = iter->next) {
        }

        if (iter == NULL) {
            iter = thread->queue->tail;
            if (iter == NULL) {
                thread->queue->head = thread;
            } else {
                iter->next = thread;
            }
            thread->prev = iter;
            thread->next = NULL;
            thread->queue->tail = thread;
        } else {
            thread->next = iter;
            iterPrev = iter->prev;
            iter->prev = thread;
            thread->prev = iterPrev;

            if (iterPrev == NULL) {
                thread->queue->head = thread;
            } else {
                iterPrev->next = thread;
            }
        }

        if (thread->mutex != NULL) {
            return thread->mutex->thread;
        }
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = TRUE;
        thread->priority = prio;
        break;
    default:
        break;
    }

    return NULL;
}

static void UpdatePriority(OSThread* thread) {
    s32 prio = 0;

    while (TRUE) {
        if (thread->suspend > 0) {
            return;
        }

        prio = __OSGetEffectivePriority(thread);
        if (thread->priority == prio) {
            return;
        }

        thread = SetEffectivePriority(thread, prio);
        if (thread == NULL) {
            return;
        }
    }
}

void __OSPromoteThread(OSThread* thread, s32 prio) {
    while (TRUE) {
        if (thread->suspend > 0) {
            return;
        }

        if (thread->priority <= prio) {
            return;
        }

        thread = SetEffectivePriority(thread, prio);
        if (thread == NULL) {
            return;
        }
    }
}

static OSThread* SelectThread(BOOL b) {
    OSThreadQueue* queue;
    OSThread* currThread;
    OSThread* head;
    OSThread* next;
    OSContext* currCtx;
    s32 prio;

    if (Reschedule > 0) {
        return NULL;
    }

    currCtx = OSGetCurrentContext();
    currThread = OSGetCurrentThread();

    if (currCtx != &currThread->context) {
        return NULL;
    }

    if (currThread != NULL) {
        if (currThread->state == OS_THREAD_STATE_RUNNING) {
            if (!b) {
                prio = __cntlzw(RunQueueBits);
                if (currThread->priority <= prio) {
                    return NULL;
                }
            }

            currThread->state = OS_THREAD_STATE_READY;
            SetRun(currThread);
        }

        if (!(currThread->context.SHORT_0x1A2 & 0x2)) {
            if (OSSaveContext(&currThread->context)) {
                return NULL;
            }
        }
    }

    if (RunQueueBits == 0) {
        OSSetCurrentThread(NULL);
        OSSetCurrentContext(&IdleContext);

        do {
            OSEnableInterrupts();

            while (RunQueueBits == 0) {
            }
            OSDisableInterrupts();

        } while (RunQueueBits == 0);

        OSClearContext(&IdleContext);
    }

    RunQueueHint = FALSE;

    prio = __cntlzw(RunQueueBits);
    queue = &RunQueue[prio];
    head = queue->head;
    next = head->next;

    if (next == NULL) {
        queue->tail = NULL;
    } else {
        next->prev = NULL;
    }

    queue->head = next;
    if (next == NULL) {
        RunQueueBits &= ~(1 << OS_PRIORITY_MAX - prio);
    }

    head->queue = NULL;
    head->state = OS_THREAD_STATE_RUNNING;
    __OSSwitchThread(head);
    return head;
}

void __OSReschedule(void) {
    if (RunQueueHint) {
        SelectThread(FALSE);
    }
}

void OSYieldThread(void) {
    u32 interrupt = OSDisableInterrupts();
    SelectThread(TRUE);
    OSRestoreInterrupts(interrupt);
}

BOOL OSCreateThread(OSThread* thread, OSThreadFunc func, void* funcArg,
                    void* stackBegin, u32 stackSize, s32 prio, u16 flags) {
    u32 interrupt;
    int i;
    OSThread* tail;
    void* sp;

    if (prio < OS_PRIORITY_MIN || prio > OS_PRIORITY_MAX) {
        return FALSE;
    }

    thread->state = OS_THREAD_STATE_READY;
    thread->flags = flags & OS_THREAD_DETACHED;
    thread->WORD_0x2D4 = prio;
    thread->priority = prio;
    thread->suspend = 1;
    thread->WORD_0x2D8 = 0xFFFFFFFF;
    thread->mutex = NULL;
    OSInitThreadQueue(&thread->threadQueue);
    OSInitMutexQueue(&thread->mutexQueue);

    sp = ROUND_DOWN_PTR(stackBegin, 8);
    *((u32*)sp - 2) = 0x00000000;
    *((u32*)sp - 1) = 0x00000000;

    OSInitContext(&thread->context, func, (char*)sp - 8);
    thread->context.lr = (u32)OSExitThread;
    thread->context.gprs[3] = (u32)funcArg;

    thread->stackBegin = (u32*)stackBegin;
    thread->stackEnd = (u32*)((char*)stackBegin - stackSize);
    *thread->stackEnd = OS_THREAD_STACK_MAGIC;

    thread->WORD_0x30C = 0;
    for (i = 0; i < 2; i++) {
        thread->ARR_0x310[i] = 0;
    }

    interrupt = OSDisableInterrupts();
    if (__OSErrorTable[OS_ERR_FP_EXCEPTION] != NULL) {
        thread->context.srr1 |= 0x900;
        thread->context.SHORT_0x1A2 |= 0x1;
        thread->context.fpscr = __OSFpscrEnableBits & 0xF8;
        thread->context.fpscr |= 0x4;

        for (i = 0; i < 32; i++) {
            *(u64*)&thread->context.fprs[i] = 0xFFFFFFFFFFFFFFFF;
            *(u64*)&thread->context.psfs[i] = 0xFFFFFFFFFFFFFFFF;
        }
    }

    tail = OS_THREAD_QUEUE.tail;
    if (tail == NULL) {
        OS_THREAD_QUEUE.head = thread;
    } else {
        tail->next2 = thread;
    }

    thread->prev2 = tail;
    thread->next2 = NULL;
    OS_THREAD_QUEUE.tail = thread;
    tail = thread;

    OSRestoreInterrupts(interrupt);
    return TRUE;
}

void OSExitThread(OSThread* thread) {
    u32 interrupt;
    OSThread* currThread;
    OSThread* next;
    OSThread* prev;

    interrupt = OSDisableInterrupts();
    currThread = OSGetCurrentThread();
    OSClearContext(&currThread->context);

    if ((currThread->flags & OS_THREAD_DETACHED)) {
        next = currThread->next2;
        prev = currThread->prev2;

        if (next == NULL) {
            OS_THREAD_QUEUE.tail = prev;
        } else {
            next->prev2 = prev;
        }

        if (prev == NULL) {
            OS_THREAD_QUEUE.head = next;
        } else {
            prev->next2 = next;
        }

        currThread->state = OS_THREAD_STATE_EXITED;
    } else {
        currThread->state = OS_THREAD_STATE_MORIBUND;
        currThread->WORD_0x2D8 = (u32)thread;
    }

    __OSUnlockAllMutex(currThread);
    OSWakeupThread(&currThread->threadQueue);
    RunQueueHint = TRUE;
    __OSReschedule();
    OSRestoreInterrupts(interrupt);
}

void OSCancelThread(OSThread* thread) {
    u32 interrupt;
    OSThread* next;
    OSThread* prev;

    interrupt = OSDisableInterrupts();

    switch (thread->state) {
    case OS_THREAD_STATE_READY:
        if (thread->suspend <= 0) {
            UnsetRun(thread);
        }
        break;
    case OS_THREAD_STATE_RUNNING:
        RunQueueHint = 1;
        break;
    case OS_THREAD_STATE_SLEEPING:
        next = thread->next;
        prev = thread->prev;
        if (next == NULL) {
            thread->queue->tail = prev;
        } else {
            next->prev = prev;
        }

        if (prev == NULL) {
            thread->queue->head = next;
        } else {
            prev->next = next;
        }

        thread->queue = NULL;
        if (thread->suspend <= 0 && thread->mutex != NULL) {
            UpdatePriority(thread->mutex->thread);
        }
        break;
    default:
        OSRestoreInterrupts(interrupt);
        return;
        break;
    }

    OSClearContext(&thread->context);
    if (thread->flags & OS_THREAD_DETACHED) {
        next = thread->next2;
        prev = thread->prev2;

        if (next == NULL) {
            OS_THREAD_QUEUE.tail = prev;
        } else {
            next->prev2 = prev;
        }

        if (prev == NULL) {
            OS_THREAD_QUEUE.head = next;
        } else {
            prev->next2 = next;
        }

        thread->state = OS_THREAD_STATE_EXITED;
    } else {
        thread->state = OS_THREAD_STATE_MORIBUND;
    }

    __OSUnlockAllMutex(thread);
    OSWakeupThread(&thread->threadQueue);
    __OSReschedule();
    OSRestoreInterrupts(interrupt);
}

BOOL OSJoinThread(OSThread* thread, void* r4) {
    u32 interrupt;
    OSThread* next;
    OSThread* prev;

    interrupt = OSDisableInterrupts();

    if (!(thread->flags & OS_THREAD_DETACHED) &&
        thread->state != OS_THREAD_STATE_MORIBUND &&
        thread->threadQueue.head == NULL) {
        OSSleepThread(&thread->threadQueue);
        if (!__OSIsThreadActive(thread)) {
            OSRestoreInterrupts(interrupt);
            return FALSE;
        }
    }

    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        if (r4 != NULL) {
            *(u32*)r4 = thread->WORD_0x2D8;
        }

        next = thread->next2;
        prev = thread->prev2;

        if (next == NULL) {
            OS_THREAD_QUEUE.tail = prev;
        } else {
            next->prev2 = prev;
        }

        if (prev == NULL) {
            OS_THREAD_QUEUE.head = next;
        } else {
            prev->next2 = next;
        }

        thread->state = OS_THREAD_STATE_EXITED;

        OSRestoreInterrupts(interrupt);
        return TRUE;
    }

    OSRestoreInterrupts(interrupt);
    return FALSE;
}

void OSDetachThread(OSThread* thread) {
    u32 interrupt;
    OSThread* next;
    OSThread* prev;

    interrupt = OSDisableInterrupts();
    thread->flags |= OS_THREAD_DETACHED;

    if (thread->state == OS_THREAD_STATE_MORIBUND) {
        next = thread->next2;
        prev = thread->prev2;

        if (next == NULL) {
            OS_THREAD_QUEUE.tail = prev;
        } else {
            next->prev2 = prev;
        }

        if (prev == NULL) {
            OS_THREAD_QUEUE.head = next;
        } else {
            prev->next2 = next;
        }

        thread->state = OS_THREAD_STATE_EXITED;
    }

    OSWakeupThread(&thread->threadQueue);
    OSRestoreInterrupts(interrupt);
}

s32 OSResumeThread(OSThread* thread) {
    u32 interrupt;
    s32 suspend;
    OSThread* iter;
    OSThread* iterPrev;
    OSThread* next;
    OSThread* prev;

    interrupt = OSDisableInterrupts();
    suspend = thread->suspend--;

    if (thread->suspend < 0) {
        thread->suspend = 0;
    } else if (thread->suspend == 0) {
        switch (thread->state) {
        case OS_THREAD_STATE_READY:
            thread->priority = __OSGetEffectivePriority(thread);
            SetRun(thread);
            break;
        case OS_THREAD_STATE_SLEEPING:
            next = thread->next;
            prev = thread->prev;
            if (next == NULL) {
                thread->queue->tail = prev;
            } else {
                next->prev = prev;
            }

            if (prev == NULL) {
                thread->queue->head = next;
            } else {
                prev->next = next;
            }

            thread->priority = __OSGetEffectivePriority(thread);

            for (iter = thread->queue->head;
                 iter != NULL && iter->priority <= thread->priority;
                 iter = iter->next) {
            }

            if (iter == NULL) {
                iter = thread->queue->tail;
                if (iter == NULL) {
                    thread->queue->head = thread;
                } else {
                    iter->next = thread;
                }
                thread->prev = iter;
                thread->next = NULL;
                thread->queue->tail = thread;
            } else {
                thread->next = iter;
                iterPrev = iter->prev;
                iter->prev = thread;
                thread->prev = iterPrev;

                if (iterPrev == NULL) {
                    thread->queue->head = thread;
                } else {
                    iterPrev->next = thread;
                }
            }

            if (thread->mutex != NULL) {
                UpdatePriority(thread->mutex->thread);
            }
            break;
        default:
            break;
        }

        __OSReschedule();
    }

    OSRestoreInterrupts(interrupt);
    return suspend;
}

s32 OSSuspendThread(OSThread* thread) {
    u32 interrupt;
    s32 suspend;
    OSThread* next;
    OSThread* prev;
    OSThread* tail;

    interrupt = OSDisableInterrupts();
    suspend = thread->suspend++;

    if (suspend == 0) {
        switch (thread->state) {
        case OS_THREAD_STATE_RUNNING:
            RunQueueHint = TRUE;
            thread->state = OS_THREAD_STATE_READY;
            break;
        case OS_THREAD_STATE_READY:
            UnsetRun(thread);
            break;
        case OS_THREAD_STATE_SLEEPING:
            next = thread->next;
            prev = thread->prev;
            if (next == NULL) {
                thread->queue->tail = prev;
            } else {
                next->prev = prev;
            }

            if (prev == NULL) {
                thread->queue->head = next;
            } else {
                prev->next = next;
            }

            thread->priority = OS_PRIORITY_MAX + 1;

            tail = thread->queue->tail;
            if (tail == NULL) {
                thread->queue->head = thread;
            } else {
                tail->next = thread;
            }

            thread->prev = tail;
            thread->next = NULL;
            thread->queue->tail = thread;
            tail = thread;

            if (thread->mutex != NULL) {
                UpdatePriority(thread->mutex->thread);
            }
            break;
        default:
            break;
        }

        __OSReschedule();
    }

    OSRestoreInterrupts(interrupt);
    return suspend;
}

void OSSleepThread(OSThreadQueue* queue) {
    u32 interrupt;
    OSThread* currThread;
    OSThread* iter;
    OSThread* iterPrev;

    interrupt = OSDisableInterrupts();
    currThread = OSGetCurrentThread();

    currThread->state = OS_THREAD_STATE_SLEEPING;
    currThread->queue = queue;

    for (iter = queue->head;
         iter != NULL && iter->priority <= currThread->priority;
         iter = iter->next) {
    }

    if (iter == NULL) {
        iter = queue->tail;
        if (iter == NULL) {
            queue->head = currThread;
        } else {
            iter->next = currThread;
        }
        currThread->prev = iter;
        currThread->next = NULL;
        queue->tail = currThread;
    } else {
        currThread->next = iter;
        iterPrev = iter->prev;
        iter->prev = currThread;
        currThread->prev = iterPrev;

        if (iterPrev == NULL) {
            queue->head = currThread;
        } else {
            iterPrev->next = currThread;
        }
    }

    RunQueueHint = TRUE;
    __OSReschedule();
    OSRestoreInterrupts(interrupt);
}

void OSWakeupThread(OSThreadQueue* queue) {
    u32 interrupt;
    OSThread* head;
    OSThread* next;

    interrupt = OSDisableInterrupts();

    while (queue->head != NULL) {
        head = queue->head;
        next = head->next;

        if (next == NULL) {
            queue->tail = NULL;
        } else {
            next->prev = NULL;
        }

        queue->head = next;

        head->state = OS_THREAD_STATE_READY;
        if (head->suspend <= 0) {
            SetRun(head);
        }
    }

    __OSReschedule();
    OSRestoreInterrupts(interrupt);
}

BOOL OSSetThreadPriority(OSThread* thread, s32 prio) {
    u32 interrupt;

    if (prio < OS_PRIORITY_MIN || prio > OS_PRIORITY_MAX) {
        return FALSE;
    }

    interrupt = OSDisableInterrupts();

    if (thread->WORD_0x2D4 != prio) {
        thread->WORD_0x2D4 = prio;
        UpdatePriority(thread);
        __OSReschedule();
    }

    OSRestoreInterrupts(interrupt);
    return TRUE;
}

static void SleepAlarmHandler(OSAlarm* alarm, OSContext* ctx) {
    OSResumeThread((OSThread*)OSGetAlarmUserData(alarm));
}

void OSSleepTicks(s64 ticks) {
    u32 interrupt;
    OSAlarm alarm;
    OSThread* thread;

    interrupt = OSDisableInterrupts();

    thread = OS_CURRENT_THREAD;
    if (thread == NULL) {
        OSRestoreInterrupts(interrupt);
        return;
    }

    OSCreateAlarm(&alarm);
    OSSetAlarmTag(&alarm, (u32)thread);
    OSSetAlarmUserData(&alarm, thread);
    OSSetAlarm(&alarm, ticks, SleepAlarmHandler);

    OSSuspendThread(thread);
    OSCancelAlarm(&alarm);
    OSRestoreInterrupts(interrupt);
}

// Needed to instantiate IdleThread
OSThread* __OSThread_c_UNUSED(void) { return &IdleThread; }
