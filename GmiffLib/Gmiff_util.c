/**************************************************************************************************
file:       Gmiff_util.c
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Utility functions for the Gmiff library.
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "Gmiff_local.h"

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
func: gmiffSetRecordBlockStart
**************************************************************************************************/
Gb gmiffSetRecordBlockStart(Gmiff * const miff, Gstr const * const name)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeBLOCK_START, 0, name));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordBlockStop
**************************************************************************************************/
Gb gmiffSetRecordBlockStop(Gmiff * const miff)
{
   returnFalseIf(!miff);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeBLOCK_STOP,  0, NULL));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordNull
**************************************************************************************************/
Gb gmiffSetRecordNull(Gmiff * const miff, Gstr const * const name)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueNull(miff));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordBool
**************************************************************************************************/
Gb gmiffSetRecordBool(Gmiff * const miff, Gstr const * const name, Gb const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueB(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordI
**************************************************************************************************/
Gb gmiffSetRecordI(Gmiff * const miff, Gstr const * const name, Gi8 const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueI(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordN
**************************************************************************************************/
Gb gmiffSetRecordN(Gmiff * const miff, Gstr const * const name, Gn8 const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(gmiffSetRecordValueN(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordR
**************************************************************************************************/
Gb gmiffSetRecordR(Gmiff * const miff, Gstr const * const name, Gr8 const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueR(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordR4
**************************************************************************************************/
Gb gmiffSetRecordR4(Gmiff * const miff, Gstr const * const name, Gr4 const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueR4(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordStr
**************************************************************************************************/
Gb gmiffSetRecordStr(Gmiff * const miff, Gstr const * const name, Gstr * const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueStr(miff, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordBinBuffer
**************************************************************************************************/
Gb gmiffSetRecordBinBuffer(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gn1 * const value)
{
   returnFalseIf(
      !miff ||
      !name);

   returnFalseIf(!gmiffSetRecordStart(miff, gmiffRecTypeVALUE, 1, name));

   returnFalseIf(!gmiffSetRecordValueBinBuffer(miff, count, value));

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordBoolArray
**************************************************************************************************/
Gb gmiffSetRecordBoolArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gb const * const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartBoolArray(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetRecordValueB(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordIArray
**************************************************************************************************/
Gb gmiffSetRecordIArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gi8 const * const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartIArray(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetRecordValueI(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordNArray
**************************************************************************************************/
Gb gmiffSetRecordNArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gn8 const * const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartNArray(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetRecordValueN(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordRArray
**************************************************************************************************/
Gb gmiffSetRecordRArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gr8 const * const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartRArray(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(gmiffSetRecordValueR(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordR4Array
**************************************************************************************************/
Gb gmiffSetRecordR4Array(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gr4 const * const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartR4Array(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetRecordValueR4(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetRecordStrArray
**************************************************************************************************/
Gb gmiffSetRecordStrArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gstr ** const array)
{
   Gindex index;

   returnFalseIf(
      !miff ||
      !name ||
      (count != 0 && !array));

   returnFalseIf(!gmiffSetRecordStartStrArray(miff, name, count));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetRecordValueStr(miff, array[index]));
   }

   return gmiffSetRecordStop(miff);
}
