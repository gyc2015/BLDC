#ifndef TYPES_H
#define TYPES_H


typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef long            int32;
typedef unsigned short  uint16;
typedef unsigned long   uint32;
typedef float           float32;
typedef long double     float64;

typedef unsigned char   BOOL;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef NEGATIVE
#define NEGATIVE 0
#endif

#ifndef POSITIVE
#define POSITIVE 1
#endif

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#define st(x) do { x } while (__LINE__ == -1)

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#endif
