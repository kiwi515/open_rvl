#include "OSMutex.h"
#include "OSInterrupt.h"

void OSInitMutex(OSMutex* mutex) {
    OSInitThreadQueue(&mutex->queue);
    mutex->thread = NULL;
    mutex->lock = 0;
}

void OSLockMutex(OSMutex* mutex) {
    u32 interrupt = OSDisableInterrupts();
    OSThread* currThread = OSGetCurrentThread();

    while (TRUE) {
        if (mutex->thread == NULL) {
            OSMutex* tail;

            mutex->thread = currThread;
            mutex->lock++;

            tail = currThread->mutexQueue.tail;
            if (tail == NULL) {
                currThread->mutexQueue.head = mutex;
            } else {
                tail->next = mutex;
            }

            mutex->prev = tail;
            mutex->next = NULL;
            currThread->mutexQueue.tail = mutex;
            break;
        } else if (mutex->thread == currThread) {
            mutex->lock++;
            break;
        } else {
            currThread->mutex = mutex;
            __OSPromoteThread(mutex->thread, currThread->priority);
            OSSleepThread(&mutex->queue);
            currThread->mutex = NULL;
        }
    }

    OSRestoreInterrupts(interrupt);
}

void OSUnlockMutex(OSMutex* mutex) {
    u32 interrupt = OSDisableInterrupts();
    OSThread* currThread = OSGetCurrentThread();

    if (mutex->thread == currThread) {
        mutex->lock--;

        if (mutex->lock == 0) {
            OSMutex* next = mutex->next;
            OSMutex* prev = mutex->prev;

            if (next == NULL) {
                currThread->mutexQueue.tail = prev;
            } else {
                next->prev = prev;
            }

            if (prev == NULL) {
                currThread->mutexQueue.head = next;
            } else {
                prev->next = next;
            }

            mutex->thread = NULL;
            if (currThread->priority < currThread->WORD_0x2D4) {
                currThread->priority = __OSGetEffectivePriority(currThread);
            }

            OSWakeupThread(&mutex->queue);
        }
    }

    OSRestoreInterrupts(interrupt);
}

void __OSUnlockAllMutex(OSThread* thread) {
    OSMutex* mutex;
    while ((mutex = thread->mutexQueue.head) != NULL) {
        OSMutex* next = mutex->next;
        if (next == NULL) {
            thread->mutexQueue.tail = NULL;
        } else {
            next->prev = NULL;
        }

        thread->mutexQueue.head = next;
        mutex->lock = 0;
        mutex->thread = NULL;
        OSWakeupThread(&mutex->queue);
    }
};

BOOL OSTryLockMutex(OSMutex* mutex) {
    u32 interrupt = OSDisableInterrupts();
    OSThread* currThread = OSGetCurrentThread();
    BOOL lock = FALSE;

    if (mutex->thread == NULL) {
        OSMutex* tail;

        mutex->thread = currThread;
        mutex->lock++;

        tail = currThread->mutexQueue.tail;
        if (tail == NULL) {
            currThread->mutexQueue.head = mutex;
        } else {
            tail->next = mutex;
        }

        mutex->prev = tail;
        mutex->next = NULL;

        currThread->mutexQueue.tail = mutex;
        lock = TRUE;
    } else if (mutex->thread == currThread) {
        mutex->lock++;
        lock = TRUE;
    } else {
        lock = FALSE;
    }

    OSRestoreInterrupts(interrupt);
    return lock;
}
