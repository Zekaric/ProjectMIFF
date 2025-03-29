/**************************************************************************************************
file:       GmineInfo_array
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
A simple dynamic array structure.
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "GmineInfo_local.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoArrayCloc
**************************************************************************************************/
GmineInfoArray *gmineInfoArrayCloc(void)
{
   GmineInfoArray *gmineInfoArray;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoArray = _MiMemClocType(GmineInfoArray);
   returnNullIf(!gmineInfoArray);

   if (!gmineInfoArrayClocContent(gmineInfoArray))
   {
      _MiMemDloc(gmineInfoArray);
      returnNull;
   }

   return gmineInfoArray;
}

/**************************************************************************************************
func: gmineInfoArrayClocContent
**************************************************************************************************/
Gb gmineInfoArrayClocContent(GmineInfoArray * const gmineInfoArray)
{
   returnFalseIf(!gmineInfoIsStarted());

   _MiMemClearType(gmineInfoArray, GmineInfoArray);

   gmineInfoArray->countBuffer = 8;
   gmineInfoArray->buffer      = _MiMemClocTypeArray(gmineInfoArray->countBuffer, void *);
   returnFalseIF(!gmineInfoArray->buffer);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoArrayDloc
**************************************************************************************************/
void gmineInfoArrayDloc(GmineInfoArray * const gmineInfoArray)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   gmineInfoArrayDlocContent(gmineInfoArray);

   _MiMemDloc(gmineInfoArray);
}

/**************************************************************************************************
func: gmineInfoArrayDlocContent
**************************************************************************************************/
void gmineInfoArrayDlocContent(GmineInfoArray * const gmineInfoArray)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   _MiMemDloc(gmineInfoArray->buffer);

   _MiMemClearType(gmineInfoArray, GmineInfoArray);
}

/**************************************************************************************************
func: gmineInfoArrayGetCount
**************************************************************************************************/
Gcount gmineInfoArrayGetCount(GmineInfoArray const * const gmineInfoArray)
{
   greturn0If(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   return gmineInfoArray->count;
}

/**************************************************************************************************
func: gmineInfoArrayAddLast
**************************************************************************************************/
Gb gmineInfoArrayAddLast(GmineInfoArray * const gmineInfoArray, void * const value)
{
   void **buffer;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   // Need to increase the buffer size.
   if (gmineInfoArray->count >= gmineInfoArray->countBuffer)
   {
      // Allocate a new buffer.
      gmineInfoArray->countBuffer *= 2;
      buffer = _MiMemClocTypeArray(gmineInfoArray->countBuffer, void *);
      returnFalseIf(!buffer);

      // Copy the over buffer over.
      _MiMemCopyTypeArray(buffer, gmineInfoArray->count, void *, gmineInforArray->buffer);

      // Clean up the old buffer.
      _MiMemDloc(gmineInfoArray->buffer);

      // Set the new buffer as the array.
      gmineInfoArray->buffer = buffer;
   }

   // Add the value to the end of the array.
   gmineInfoArray->buffer[gmineInfoArray->count++] = value;

   returnTrue;
}
