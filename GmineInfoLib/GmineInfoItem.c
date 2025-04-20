/**************************************************************************************************
file:       GmineInfoItem
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Item routines for the Mine Information
**************************************************************************************************/

/**************************************************************************************************
MIT License

Copyright (c) 2025, Robbert de Groot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
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
static Gn4   _itemId    = 1;

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoItemCloc
**************************************************************************************************/
GmineInfoItem *gmineInfoItemCloc(void)
{
   GmineInfoItem *gmineInfoItem;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoItem = _MiMemClocType(GmineInfoItem);
   returnNullIf(!gmineInfoItem);

   if (!gmineInfoItemClocContent(gmineInfoItem))
   {
      _MiMemDloc(gmineInfoItem);
      return NULL;
   }

   return gmineInfoItem;
}

/**************************************************************************************************
func: gmineInfoItemClocContent
**************************************************************************************************/
Gb gmineInfoItemClocContent(GmineInfoItem * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   _MiMemClearType(gmineInfoItem, GmineInfoItem);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemDloc
**************************************************************************************************/
void gmineInfoItemDloc(GmineInfoItem * const gmineInfoItem)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   gmineInfoItemDlocContent(gmineInfoItem);

   _MiMemDloc(gmineInfoItem);
}

/**************************************************************************************************
func: gmineInfoItemDlocContent
**************************************************************************************************/
void gmineInfoItemDlocContent(GmineInfoItem * const gmineInfoItem)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   // Clean up the strings.
   if (gmineInfoItem->type == gmineInfoItemTypeDATE     ||
       gmineInfoItem->type == gmineInfoItemTypeDATETIME ||
       gmineInfoItem->type == gmineInfoItemTypeFORMULA  ||
       gmineInfoItem->type == gmineInfoItemTypeSTR      ||
       gmineInfoItem->type == gmineInfoItemTypeTIME)
   {
      _MiMemDloc(gmineInfoItem->defaultValue.str);
   }
   _MiMemDloc(gmineInfoItem->key);
   _MiMemDloc(gmineInfoItem->name);

   // Clean up the key value array.
   gmineInfoArrayForEach(    &gmineInfoItem->keyValueArray, gmineInfoKeyValueDloc);
   gmineInfoArrayDlocContent(&gmineInfoItem->keyValueArray);

   // Clear out the memory.
   _MiMemClearType(gmineInfoItem, GmineInfoItem);

   return;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueB
**************************************************************************************************/
Gb gmineInfoItemGetDefaultValueB(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeB);

   return (gmineInfoItem->defaultValue.i) ? gbTRUE : gbFALSE;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueDate
**************************************************************************************************/
Gstr const * gmineInfoItemGetDefaultValueDate(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeDATE);

   return gmineInfoItem->defaultValue.str;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueDateTime
**************************************************************************************************/
Gstr const * gmineInfoItemGetDefaultValueDateTime(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeDATETIME);

   return gmineInfoItem->defaultValue.str;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueI
**************************************************************************************************/
Gi8 gmineInfoItemGetDefaultValueI(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   return gmineInfoItem->defaultValue.i;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueN
**************************************************************************************************/
Gn8 gmineInfoItemGetDefaultValueN(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   return gmineInfoItem->defaultValue.n;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValuePercent
**************************************************************************************************/
Gr8 gmineInfoItemGetDefaultValuePercent(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypePERCENT);

   return gmineInfoItem->defaultValue.r;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueR
**************************************************************************************************/
Gr8 gmineInfoItemGetDefaultValueR(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   return gmineInfoItem->defaultValue.r;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueStr
**************************************************************************************************/
Gstr const * gmineInfoItemGetDefaultValueStr(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeSTR);

   return gmineInfoItem->defaultValue.str;
}

/**************************************************************************************************
func: gmineInfoItemGetDefaultValueTime
**************************************************************************************************/
Gstr const * gmineInfoItemGetDefaultValueTime(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeTIME);

   return gmineInfoItem->defaultValue.str;
}

/**************************************************************************************************
func: gmineInfoItemGetFormula
**************************************************************************************************/
Gstr const *gmineInfoItemGetFormula(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeFORMULA);

   return gmineInfoItem->formula;
}

/**************************************************************************************************
func: gmineInfoItemGetKey
**************************************************************************************************/
Gstr const * gmineInfoItemGetKey(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->key;
}

/**************************************************************************************************
func: gmineInfoItemGetMaxI
**************************************************************************************************/
Gi8 gmineInfoItemGetMaxI(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   return gmineInfoItem->max.i;
}

/**************************************************************************************************
func: gmineInfoItemGetMaxN
**************************************************************************************************/
Gn8 gmineInfoItemGetMaxN(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   return gmineInfoItem->max.n;
}

/**************************************************************************************************
func: gmineInfoItemGetMaxR
**************************************************************************************************/
Gr8 gmineInfoItemGetMaxR(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   return gmineInfoItem->max.r;
}

/**************************************************************************************************
func: gmineInfoItemGetMinI
**************************************************************************************************/
Gi8 gmineInfoItemGetMinI(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   return gmineInfoItem->min.i;
}

/**************************************************************************************************
func: gmineInfoItemGetMinN
**************************************************************************************************/
Gn8 gmineInfoItemGetMinN(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   return gmineInfoItem->min.n;
}

/**************************************************************************************************
func: gmineInfoItemGetMinR
**************************************************************************************************/
Gr8 gmineInfoItemGetMinR(GmineInfoItem const * const gmineInfoItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   return gmineInfoItem->min.r;
}

/**************************************************************************************************
func: gmineInfoItemGetName
**************************************************************************************************/
Gstr const * gmineInfoItemGetName(GmineInfoItem const * const gmineInfoItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->name;
}

/**************************************************************************************************
func: gmineInfoItemGetPrecision
**************************************************************************************************/
Gr8 gmineInfoItemGetPrecision(GmineInfoItem const * const gmineInfoItem)
{
   return1If(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->precision;
}

/**************************************************************************************************
func: gmineInfoItemGetType
**************************************************************************************************/
GmineInfoItemType gmineInfoItemGetType(GmineInfoItem const * const gmineInfoItem)
{
   returnIf(
         !gmineInfoIsStarted() ||
         !gmineInfoItem,
      gmineInfoItemTypeNONE);

   return gmineInfoItem->type;
}

/**************************************************************************************************
func: gmineInfoItemIsSetDefault
**************************************************************************************************/
Gb gmineInfoItemIsSetDefault(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetDefault;
}

/**************************************************************************************************
func: gmineInfoItemIsSetFormula
**************************************************************************************************/
Gb gmineInfoItemIsSetFormula(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetFormula;
}

/**************************************************************************************************
func: gmineInfoItemIsSetKey
**************************************************************************************************/
Gb gmineInfoItemIsSetKey(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetKey;
}

/**************************************************************************************************
func: gmineInfoItemIsSetMax
**************************************************************************************************/
Gb gmineInfoItemIsSetMax(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetMax;
}

/**************************************************************************************************
func: gmineInfoItemIsSetMin
**************************************************************************************************/
Gb gmineInfoItemIsSetMin(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetMin;
}

/**************************************************************************************************
func: gmineInfoItemIsSetName
**************************************************************************************************/
Gb gmineInfoItemIsSetName(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetName;
}

/**************************************************************************************************
func: gmineInfoItemIsSetPrecision
**************************************************************************************************/
Gb gmineInfoItemIsSetPrecision(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetPrecision;
}

/**************************************************************************************************
func: gmineInfoItemIsSetType
**************************************************************************************************/
Gb gmineInfoItemIsSetType(GmineInfoItem const * const gmineInfoItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   return gmineInfoItem->isSetType;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultB
**************************************************************************************************/
Gb gmineInfoItemSetDefaultB(GmineInfoItem * const gmineInfoItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeB);

   gmineInfoItem->defaultValue.i    = value;
   gmineInfoItem->isSetDefault      = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultDate
**************************************************************************************************/
Gb gmineInfoItemSetDefaultDate(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value                ||
      gmineInfoItem->type != gmineInfoItemTypeDATE);

   gmineInfoItem->defaultValue.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault      = (gmineInfoItem->defaultValue.str) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetDefault;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultDateTime
**************************************************************************************************/
Gb gmineInfoItemSetDefaultDateTime(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value                ||
      gmineInfoItem->type != gmineInfoItemTypeDATETIME);

   gmineInfoItem->defaultValue.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault      = (gmineInfoItem->defaultValue.str) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetDefault;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultI
**************************************************************************************************/
Gb gmineInfoItemSetDefaultI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->defaultValue.i    = value;
   gmineInfoItem->isSetDefault      = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultN
**************************************************************************************************/
Gb gmineInfoItemSetDefaultN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->defaultValue.n    = value;
   gmineInfoItem->isSetDefault      = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultPercent
**************************************************************************************************/
Gb gmineInfoItemSetDefaultPercent(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->defaultValue.r    = value;
   gmineInfoItem->isSetDefault      = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultR
**************************************************************************************************/
Gb gmineInfoItemSetDefaultR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->defaultValue.r    = value;
   gmineInfoItem->isSetDefault      = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultStr
**************************************************************************************************/
Gb gmineInfoItemSetDefaultStr(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value                ||
      gmineInfoItem->type != gmineInfoItemTypeSTR);

   gmineInfoItem->defaultValue.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault      = (gmineInfoItem->defaultValue.str) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetDefault;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultTime
**************************************************************************************************/
Gb gmineInfoItemSetDefaultTime(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value                ||
      gmineInfoItem->type != gmineInfoItemTypeTIME);

   gmineInfoItem->defaultValue.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault      = (gmineInfoItem->defaultValue.str) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetDefault;
}

/**************************************************************************************************
func: gmineInfoItemSetFormula
**************************************************************************************************/
Gb gmineInfoItemSetFormula(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value                ||
      gmineInfoItem->type != gmineInfoItemTypeFORMULA);

   gmineInfoItem->formula      = _MiStrClone(value);
   gmineInfoItem->isSetFormula = (gmineInfoItem->defaultValue.str) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetFormula;
}

/**************************************************************************************************
func: gmineInfoItemSetKey
**************************************************************************************************/
Gb gmineInfoItemSetKey(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value);

   gmineInfoItem->key      = _MiStrClone(value);
   gmineInfoItem->isSetKey = (gmineInfoItem->key) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetKey;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxI
**************************************************************************************************/
Gb gmineInfoItemSetMaxI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->max.i    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxN
**************************************************************************************************/
Gb gmineInfoItemSetMaxN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->max.n    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxR
**************************************************************************************************/
Gb gmineInfoItemSetMaxR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->max.r    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinI
**************************************************************************************************/
Gb gmineInfoItemSetMinI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->min.i    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinN
**************************************************************************************************/
Gb gmineInfoItemSetMinN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->min.n    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinR
**************************************************************************************************/
Gb gmineInfoItemSetMinR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->min.r    = value;
   gmineInfoItem->isSetMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetName
**************************************************************************************************/
Gb gmineInfoItemSetName(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      !value);

   gmineInfoItem->name      = _MiStrClone(value);
   gmineInfoItem->isSetName = (gmineInfoItem->name) ? gbTRUE : gbFALSE;

   return gmineInfoItem->isSetName;
}

/**************************************************************************************************
func: gmineInfoItemSetPrecision
**************************************************************************************************/
Gb gmineInfoItemSetPrecision(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem        ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->precision      = value;
   gmineInfoItem->isSetPrecision = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetType
**************************************************************************************************/
Gb gmineInfoItemSetType(GmineInfoItem * const gmineInfoItem, GmineInfoItemType const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItem);

   gmineInfoItem->type      = value;
   gmineInfoItem->isSetType = (value != gmineInfoItemTypeNONE);

   returnTrue;
}
