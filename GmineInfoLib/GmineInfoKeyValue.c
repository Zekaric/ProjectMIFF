/**************************************************************************************************
file:       GmineInfoKeyValue
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Key value structure functions.
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
func: gmineInfoKeyValueCloc
**************************************************************************************************/
GmineInfoKeyValue *gmineInfoKeyValueCloc(Gstr const * key, Gstr const * value)
{
   GmineInfoKeyValue *gmineInfoKeyValue;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoKeyValue = _MiMemClocType(GmineInfoKeyValue);
   returnNullIf(!gmineInfoKeyValue);

   if (!gmineInfoKeyValueClocContent(gmineInfoKeyValue, key, value))
   {
      _MiMemDloc(   gmineInfoKeyValue);
      return NULL;
   }

   return gmineInfoKeyValue;
}

/**************************************************************************************************
func: gmineInfoKeyValueClocContent
**************************************************************************************************/
Gb gmineInfoKeyValueClocContent(GmineInfoKeyValue * const gmineInfoKeyValue, Gstr const * key, Gstr const * value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue    ||
      !key                  ||
      !value);

   _MiMemClearType(gmineInfoKeyValue, GmineInfoKeyValue);

   gmineInfoKeyValue->key   = _MiStrClone(key);
   gmineInfoKeyValue->value = _MiStrClone(value);

   returnFalseIf(
      !gmineInfoKeyValue->key ||
      !gmineInfoKeyValue->value);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoKeyValueDloc
**************************************************************************************************/
void gmineInfoKeyValueDloc(GmineInfoKeyValue * const gmineInfoKeyValue)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   gmineInfoKeyValueDlocContent(gmineInfoKeyValue);

   _MiMemDloc(gmineInfoKeyValue);
}

/**************************************************************************************************
func: gmineInfoKeyValueDlocContent
**************************************************************************************************/
void gmineInfoKeyValueDlocContent(GmineInfoKeyValue * const gmineInfoKeyValue)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   _MiMemDloc(gmineInfoKeyValue->key);
   _MiMemDloc(gmineInfoKeyValue->value);

   _MiMemClearType(gmineInfoKeyValue, GmineInfoKeyValue);
}

/**************************************************************************************************
func: gmineInfoKeyValueGetKey
**************************************************************************************************/
Gstr const * gmineInfoKeyValueGetKey(GmineInfoKeyValue const * const gmineInfoKeyValue)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   return gmineInfoKeyValue->key;
}

/**************************************************************************************************
func: gmineInfoKeyValueGetValue
**************************************************************************************************/
Gstr const * gmineInfoKeyValueGetValue(GmineInfoKeyValue const * const gmineInfoKeyValue)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   return gmineInfoKeyValue->value;
}

/**************************************************************************************************
func: gmineInfoKeyValueSetKey
**************************************************************************************************/
Gb gmineInfoKeyValueSetKey(GmineInfoKeyValue * const gmineInfoKeyValue, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   _MiMemDloc(gmineInfoKeyValue->key);
   gmineInfoKeyValue->key = _MiStrClone(value);

   return (gmineInfoKeyValue->key) ? gbTRUE : gbFALSE;
}

/**************************************************************************************************
func: gmineInfoKeyValueSetValue
**************************************************************************************************/
Gb gmineInfoKeyValueSetValue(GmineInfoKeyValue * const gmineInfoKeyValue, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoKeyValue);

   _MiMemDloc(gmineInfoKeyValue->key);
   gmineInfoKeyValue->value = _MiStrClone(value);

   return (gmineInfoKeyValue->value) ? gbTRUE : gbFALSE;
}

/**************************************************************************************************
KeyValueArray
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoKeyValueArrayAdd
**************************************************************************************************/
Gb gmineInfoKeyValueArrayAdd(GmineInfoArray * const array, Gstr const * const key,
   Gstr const * const value)
{
   GmineInfoKeyValue *kv;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !array);

   kv = gmineInfoKeyValueCloc(key, value);
   returnFalseIf(!kv);

   return gmineInfoArrayAddLast(array, kv);
}

/**************************************************************************************************
func: gmineInfoKeyValueArrayGetAt
**************************************************************************************************/
Gb gmineInfoKeyValueArrayGetAt(GmineInfoArray const * const array, Gindex const index,
   Gstr const ** const key, Gstr const ** const value)
{
   GmineInfoKeyValue *kv;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !array);

   kv = (GmineInfoKeyValue *) gmineInfoArrayGetAt(array, index);
   returnFalseIf(!kv);

   *key   = gmineInfoKeyValueGetKey(  kv);
   *value = gmineInfoKeyValueGetValue(kv);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoKeyValueArrayGetCount
**************************************************************************************************/
Gb gmineInfoKeyValueArrayGetCount(GmineInfoArray const * const array)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !array);

   return gmineInfoArrayGetCount(array);
}

