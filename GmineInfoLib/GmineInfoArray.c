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
func: gmineInfoArrayAddLast
**************************************************************************************************/
Gb gmineInfoArrayAddLast(GmineInfoArray * const gmineInfoArray, void * const value)
{
   void **buffer;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   // Need to increase the buffer size.
   if (gmineInfoArray->count >= gmineInfoArray->countMax)
   {
      // Allocate a new buffer.
      gmineInfoArray->countMax *= 2;
      buffer = _MiMemClocTypeArray(gmineInfoArray->countMax, void *);
      returnFalseIf(!buffer);

      // Copy the over buffer over.
      _MiMemCopyTypeArray(buffer, gmineInfoArray->count, void *, gmineInfoArray->buffer);

      // Clean up the old buffer.
      _MiMemDloc(gmineInfoArray->buffer);

      // Set the new buffer as the array.
      gmineInfoArray->buffer = buffer;
   }

   // Add the value to the end of the array.
   gmineInfoArray->buffer[gmineInfoArray->count++] = value;

   returnTrue;
}

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
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   _MiMemClearType(gmineInfoArray, GmineInfoArray);

   gmineInfoArray->countMax = 8;
   gmineInfoArray->buffer   = _MiMemClocTypeArray(gmineInfoArray->countMax, void *);

   return (gmineInfoArray->buffer) ? gbTRUE : gbFALSE;
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
func: gmineInfoArrayForEach
**************************************************************************************************/
void gmineInfoArrayForEach(GmineInfoArray * const gmineInfoArray, void (*func)(void *))
{
   Gcount count;
   Gindex index;

   count = gmineInfoArray->count;
   forCount(index, count)
   {
      func(gmineInfoArray->buffer[index]);
   }
}

/**************************************************************************************************
func: gmineInfoArrayGetAt
**************************************************************************************************/
void *gmineInfoArrayGetAt(GmineInfoArray const * const gmineInfoArray, Gindex const index)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoArray       ||
      index < 0 || gmineInfoArray->count <= index);

   return gmineInfoArray->buffer[index];
}

/**************************************************************************************************
func: gmineInfoArrayGetCount
**************************************************************************************************/
Gcount gmineInfoArrayGetCount(GmineInfoArray const * const gmineInfoArray)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoArray);

   return gmineInfoArray->count;
}
