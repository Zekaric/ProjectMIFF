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

/******************************************************************************
global:
function:
******************************************************************************/
BsfB         bsfGet(    BsfDataGet * const buffer, BsfN1 * const byte);

BsfDataGet   bsfPrepGet(BsfN1 const * const buffer);
BsfDataSet   bsfPrepSet(BsfN1       * const buffer);

BsfB         bsfSet(    BsfDataSet * const buffer, BsfN1   const byte);
BsfB         bsfSetEnd( BsfDataSet * const buffer);
void         bsfStart(  void);
void         bsfStop(   void);

#endif
