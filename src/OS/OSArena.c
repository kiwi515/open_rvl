#include "OSArena.h"

static void* __OSArenaLo = (void*)-1;
static void* s_mem2ArenaLo = (void*)-1;
static void* __OSArenaHi = NULL;
static void* s_mem2ArenaHi = NULL;

void* OSGetMEM1ArenaHi(void) { return __OSArenaHi; }

void* OSGetMEM2ArenaHi(void) { return s_mem2ArenaHi; }

void* OSGetArenaHi(void) { return OSGetMEM1ArenaHi(); }

void* OSGetMEM1ArenaLo(void) { return __OSArenaLo; }

void* OSGetMEM2ArenaLo(void) { return s_mem2ArenaLo; }

void* OSGetArenaLo(void) { return OSGetMEM1ArenaLo(); }

void OSSetMEM1ArenaHi(void* hi) { __OSArenaHi = hi; }

void OSSetMEM2ArenaHi(void* hi) { s_mem2ArenaHi = hi; }

void OSSetArenaHi(void* hi) { OSSetMEM1ArenaHi(hi); }

void OSSetMEM1ArenaLo(void* lo) { __OSArenaLo = lo; }

void OSSetMEM2ArenaLo(void* lo) { s_mem2ArenaLo = lo; }

void OSSetArenaLo(void* lo) { OSSetMEM1ArenaLo(lo); }

#ifdef NONMATCHING
// https://decomp.me/scratch/5RY4W
void* OSAllocFromMEM1ArenaLo(size_t sz, int align) {
    u8* mem = ROUND_UP_PTR(OSGetMEM1ArenaLo(), align);
    OSSetMEM1ArenaLo(ROUND_UP_PTR(mem + sz, align));
    return mem;
}
#else
#error
#endif
