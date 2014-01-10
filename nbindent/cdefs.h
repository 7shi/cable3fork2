#ifndef __CDEFS_H__
#define __CDEFS_H__

#ifdef WIN32
#define __COPYRIGHT(a) static const char *copyright = a;
#define __RCSID(a) static const char *__rcsid = a;
#else
#include <sys/cdefs.h>
#endif

#endif
