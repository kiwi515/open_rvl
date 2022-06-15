#ifndef STDDEF_H
#define STDDEF_H

#ifndef NULL
#define NULL 0
#endif // NULL

typedef unsigned long size_t;

#define offsetof(ST, M) ((size_t) & (((ST*)0)->M))

#endif
