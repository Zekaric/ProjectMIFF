/**************************************************************************************************
file:       gb64lib
author:     Robbert de Groot
company:    Zekaric
copyright:  2024, Zekaric

description:
Simple base 64 routines.
**************************************************************************************************/

#if !defined(GbsfLIB_H)
#define      GbsfLIB_H

/**************************************************************************************************
include:
**************************************************************************************************/
#include <stdint.h>

/**************************************************************************************************
type:
**************************************************************************************************/
typedef enum
{
   gbsfFALSE,
   gbsfTRUE
} GbsfB;

typedef int32_t      GbsfI;
typedef uint8_t      GbsfN1;

#if 0
typedef struct
{
   GbsfI              state;
   GbsfN1             byte;
   GbsfI              index;
   GbsfN1 const      *buffer;
} GbsfDataGet;

typedef struct
{
   GbsfI              state;
   GbsfN1             byte;
   GbsfI              index;
   GbsfN1            *buffer;
} GbsfDataSet;
#endif

typedef struct
{
   GbsfI              state;
   GbsfN1             byte;
} GbsfData;

/**************************************************************************************************
global:
function:
**************************************************************************************************/
#if 0
GbsfB        gbsfGet(    GbsfDataGet * const buffer, GbsfN1 * const byte);

GbsfDataGet  gbsfPrepGet(GbsfN1 const * const buffer);
GbsfDataSet  gbsfPrepSet(GbsfN1       * const buffer);

GbsfB        gbsfSet(    GbsfDataSet * const buffer, GbsfN1   const byte);
GbsfB        gbsfSetEnd( GbsfDataSet * const buffer);
#endif

void         gbsfStart(  void);
void         gbsfStop(   void);

GbsfB        gbsfPrep(    GbsfData * const bsfData);
GbsfI        gbsfToBsf(   GbsfData * const bsfData, GbsfN1 const byte,    GbsfN1 * const bsfByte1, GbsfN1 * const bsfByte2);
GbsfN1       gbsfToBsfEnd(GbsfData * const bsfData,                       GbsfN1 * const bsfByte);
GbsfI        gbsfToByte(  GbsfData * const bsfData, GbsfN1 const bsfByte, GbsfN1 * const byte);

#endif
