#include "OSLink.h"

void __OSModuleInit(void) {
    OS_MODULE_LAST = NULL;
    OS_MODULE_FIRST = NULL;
    OS_MODULE_NAME_TABLE = NULL;
}
