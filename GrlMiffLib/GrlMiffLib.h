/**************************************************************************************************
file:       GrlMiffLib
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Header only file common to all projects.
**************************************************************************************************/

#if !defined (GRL_MIFF_LIB)
#define       GRL_MIFF_LIB

/**************************************************************************************************
include:
**************************************************************************************************/
#include <stdint.h>
#include <float.h>
#include <math.h>

#include "GrlMiffLib.h"

/**************************************************************************************************
constant:
**************************************************************************************************/
#if !defined(GRLH)

#define Gi1MAX                INT8_MAX
#define Gi1MIN                INT8_MIN
#define Gi2MAX                INT16_MAX
#define Gi2MIN                INT16_MIN
#define Gi4MAX                INT32_MAX
#define Gi4MIN                INT32_MIN
#define Gi8MAX                INT64_MAX
#define Gi8MIN                INT64_MIN

#define Gn1MAX                UINT8_MAX
#define Gn2MAX                UINT16_MAX
#define Gn4MAX                UINT32_MAX
#define Gn8MAX                UINT64_MAX

#define Gr4MAX                FLT_MAX
#define Gr4MIN                FLT_MIN
#define Gr8MAX                DBL_MAX
#define Gr8MIN                DBL_MIN

#define GrNAN                 NAN
#define Gr4EPSILON            FLT_EPSILON
#define Gr8EPSILON            DBL_EPSILON

#define GcountMAX             Gi4MAX

#define GindexERROR           Gi4MIN
#define GindexMAX             Gi4MAX

#define GoffsetMAX            Gi4MAX
#define GoffsetMIN            Gi4MIN

#define GsizeMAX              Gi4MAX

#endif

#define GkeySIZE              257
#define GkeyBYTE_COUNT        256

/**************************************************************************************************
type:
**************************************************************************************************/
#if !defined(GRLH)

typedef enum
{
   gbFALSE              =  0,
   gbTRUE               =  1,
   gbUNDEFINED          = -1
} GbValues;

typedef int8_t             Gi1;
typedef int16_t            Gi2;
typedef int32_t            Gi4;
typedef int64_t            Gi8;
typedef uint8_t            Gn1;
typedef uint16_t           Gn2;
typedef uint32_t           Gn4;
typedef uint64_t           Gn8;
typedef float              Gr4;
typedef double             Gr8;

typedef Gi4                Gb;

// Count Index Offset Size
// A lot of functions have parameters that indicate a count, index, offset, and
// size.  So I made them their own types to make it more obvious the intended
// used of the variable/parameter.  These will use the most efficient integer
// representation for the architecture.
typedef Gi4                Gcount;
typedef Gi4                Gindex;
typedef Gi4                Goffset;
typedef Gi4                Gsize;

#endif

// UTF8
typedef char               Gstr;

typedef enum
{
   gformatNONE,
   gformatJSON,
   gformatMIFF,
} Gformat;

typedef enum
{
   gmethodREAD,
   gmethodWRITE,
   gmethodWRITE_MINIMIZED
} Gmethod;

typedef void *(*GmemCloc)(    Gcount const memByteCount);
typedef void  (*GmemDloc)(    void * const mem);

typedef Gb    (*GgetBuffer)(  void * const dataSource,      Gcount const byteCount, Gn1       * const data);

typedef Gb    (*GsetBuffer)(  void * const dataDestination, Gcount const byteCount, Gn1 const * const data);

/**************************************************************************************************
macro:
**************************************************************************************************/
#if !defined(GRLH)

#endif
#define gABS(V)                     (((V) <    0) ? -(V) : (V))
#define gMAX(A, B)                  (((A) >= (B)) ?  (A) : (B))
#define gMIN(A, B)                  (((A) >= (B)) ?  (B) : (A))

// Loop macroes.
// I know some will hate these but I find they help prevent typo mistakes and to some degree they
// help in readability.
// for* - finite loops.
#define forCount(INDEX, COUNT)      for ((INDEX) = 0;           (INDEX) <  (COUNT); (INDEX) += 1)
#define forCountDown(INDEX, COUNT)  for ((INDEX) = (COUNT) - 1; (INDEX) >= (COUNT); (INDEX) -= 1)
// loop* - infinite loop.
#define loop                        for (;;)
#define loopCount(INDEX)            for ((INDEX) = 0;            ; (INDEX) += 1)
// loopOnce - single iteration loop.
#define loopOnce                    for (int __index__ = 0; __index__ < 1; __index__++)
// breakable scope
#define breakScope                  loopOnce

// return - macroes.
#define returnIf(C,V)      if ((C)) { return (V); }
#define return0If(C)       if ((C)) { return 0; }
#define return1If(C)       if ((C)) { return 1; }
#define returnVoidIf(C)    if ((C)) { return; }
#define returnNullIf(C)    if ((C)) { return NULL; }
#define returnTrueIf(C)    if ((C)) { return gbTRUE; }
#define returnFalseIf(C)   if ((C)) { return gbFALSE; }
#define returnFalse        return gbFALSE
#define returnNull         return NULL
#define returnTrue         return gbTRUE
#define continueIf(C)      if ((C)) { continue; }
#define breakIf(C)         if ((C)) { break; }

// Goto macroes
// I am not a huge goto user but there are cases where they are useful.  Similar
// macroes to the returns above.
// stopIf jumps to a specific label in the code.
/*lint -save -e9022 */
#define gotoIf(C,G)        if ((C)) { goto G; }
/*lint -restore */
#define stop()             goto STOP
#define stopIf(C)          if ((C)) { goto STOP; }

// debug macroes
// Essentially wrapping assert but in a style that I find easier to read.
#if defined(_DEBUG)

#define debugHaltIf(VALUE, STRING)  if (VALUE) { debugHalt(STRING); }
#define debugHalt(STRING)           /*lint -save -e944 -e917 -e920 -e960 -e9008 -e9007 */ assert(gbFALSE && (STRING)) /*lint -restore */
#define debugPrintU2(WSTR)          OutputDebugStringW(WSTR)
#define debugPrintA( WSTR)          OutputDebugStringA(WSTR)
#define debugPrintFormatU2(FMT, ...)     \
{\
   wchar_t __debug_str[1024];\
   swprintf_s(__debug_str, 1024, FMT, __VA_ARGS__);\
   debugPrintU2(__debug_str);\
}
#define debugPrintFormatA(FMT, ...)     \
{\
   char __debug_str[1024];\
   sprintf_s(__debug_str, 1024, FMT, __VA_ARGS__);\
   debugPrintA(__debug_str);\
}

#if grlWINDOWS == 1
#define debugCheckMemory()             _CrtCheckMemory()
#define debugSetCheckMemoryAgresive()  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF)
#else
#define debugCheckMemory()
#define debugSetCheckMemoryAgresive()
#endif

#else

#define debugHaltIf(VALUE, STRING)
#define debugHalt(STRING)
#define debugPrint(WSTR)
#define debugCheckMemory()
#define debugPrintMsg(FMT, ...)

#endif

#define gcountIsGood(C)    (0 <= (C))

#define gindexIsGood(I,C)  (0 <= (I) && (I) < (C))

#define gsizeIsGood(S)     (0 <= (S))

#endif
