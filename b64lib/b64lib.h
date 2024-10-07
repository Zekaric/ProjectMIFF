/******************************************************************************
file:       b64lib
author:     Robbert de Groot
company:    Zekaric
copyright:  2024, Zekaric

description:
Simple base 64 routines.
******************************************************************************/

#if !defined(BsfLIB_H)
#define      BsfLIB_H

/******************************************************************************
include:
******************************************************************************/
#include <stdint.h>

/******************************************************************************
type:
******************************************************************************/
typedef enum
{
   bsfFALSE,
   bsfTRUE
} BsfB;

typedef int32_t      BsfI;
typedef uint8_t      BsfN1;

#if 0
typedef struct
{
   BsfI               state;
   BsfN1              byte;
   BsfI               index;
   BsfN1 const       *buffer;
} BsfDataGet;

typedef struct
{
   BsfI               state;
   BsfN1              byte;
   BsfI               index;
   BsfN1             *buffer;
} BsfDataSet;
#endif

typedef struct
{
   BsfI               state;
   BsfN1              byte;
} BsfData;

/******************************************************************************
global:
function:
******************************************************************************/
#if 0
BsfB         bsfGet(    BsfDataGet * const buffer, BsfN1 * const byte);

BsfDataGet   bsfPrepGet(BsfN1 const * const buffer);
BsfDataSet   bsfPrepSet(BsfN1       * const buffer);

BsfB         bsfSet(    BsfDataSet * const buffer, BsfN1   const byte);
BsfB         bsfSetEnd( BsfDataSet * const buffer);
#endif

void         bsfStart(  void);
void         bsfStop(   void);

BsfB         bsfPrep(    BsfData * const bsfData);
BsfI         bsfToBsf(   BsfData * const bsfData, BsfN1 const byte,    BsfN1 * const bsfByte1, BsfN1 * const bsfByte2);
BsfN1        bsfToBsfEnd(BsfData * const bsfData,                      BsfN1 * const bsfByte);
BsfI         bsfToByte(  BsfData * const bsfData, BsfN1 const bsfByte, BsfN1 * const byte);

#endif
