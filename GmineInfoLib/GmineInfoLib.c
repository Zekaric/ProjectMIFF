/**************************************************************************************************
file:       GmineInfo
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
The top level routines for the Mine Information
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
static Gb    _isStarted = gbFALSE;
static Gn4   _itemId    = 1;

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoBlockTypeNext

Block order is important for reading in the MineInfo file in one go.  Some data needs to be defined
before it is being referred to.  This is why writing out blocks in a specific order is important.
This function is only necessary for writing.  Reading will a MineInfo file works differently.
**************************************************************************************************/
GmineInfoBlockType gmineInfoBlockTypeNext(GmineInfo * const gmineInfo)
{
   returnIf(
         !_isStarted ||
         !gmineInfo  ||
         gmineInfo->currentBlockType == gmineInfoBlockType_END,
      gmineInfoBlockType_END);

   // If writing...
   if (gmineInfo->isWriting)
   {
      // Close up the previous block
      switch (gmineInfo->currentBlockType)
      {
      case gmineInfoBlockTypeDATA:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeITEM:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeGEOMETRY:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeDRILL_HOLE:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeMODEL:
         _MiIoWriteBlockStop(    gmineInfo);
         break;
      }
   }

   // Move to the next block
   gmineInfo->currentBlockType++;

   // Clear the memory for the next block.
   switch (gmineInfo->currentBlockType)
   {
   case gmineInfoBlockTypeDATA:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DATA);
      break;

   case gmineInfoBlockTypeITEM:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM);
      break;

   case gmineInfoBlockTypeGEOMETRY:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_GEOMETRY);
      break;

   case gmineInfoBlockTypeDRILL_HOLE:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DRILL_HOLE);
      break;

   case gmineInfoBlockTypeMODEL:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_MODEL);
      break;
   }

   return gmineInfo->currentBlockType;
}

/**************************************************************************************************
func: gmineInfoClocReader
**************************************************************************************************/
GmineInfo *gmineInfoClocReader(GmineInfoFileType const fileType, GgetBuffer getBufferFunc,
   void * const dataRepo)
{
   GmineInfo *gmineInfo;

   returnNullIf(!_isStarted);

   gmineInfo = _MiMemClocType(GmineInfo);
   returnNullIf(!gmineInfo);

   if (!gmineInfoClocReaderContent(gmineInfo, fileType, getBufferFunc, dataRepo))
   {
      _MiMemDloc(gmineInfo);
      return NULL;
   }

   return gmineInfo;
}

/**************************************************************************************************
func: gmineInfoClocReaderContent
**************************************************************************************************/
Gb gmineInfoClocReaderContent(GmineInfo * const gmineInfo, GmineInfoFileType const fileType,
   GgetBuffer getBufferFunc, void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo);

   _MiMemClearType(gmineInfo, GmineInfo);

   gmineInfo->fileType  = fileType;
   gmineInfo->dataRepo  = dataRepo;
   gmineInfo->getBuffer = getBufferFunc;

   // Start reading the file header.
   return _MiIoClocReader(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoClocWriter
**************************************************************************************************/
GmineInfo *gmineInfoClocWriter(GmineInfoFileType const fileType, GsetBuffer setBufferFunc,
   void * const dataRepo)
{
   GmineInfo *gmineInfo;

   returnNullIf(!_isStarted);

   gmineInfo = _MiMemClocType(GmineInfo);
   returnNullIf(!gmineInfo);

   if (!gmineInfoClocWriterContent(gmineInfo, fileType, setBufferFunc, dataRepo))
   {
      _MiMemDloc(gmineInfo);
      return NULL;
   }

   return gmineInfo;
}

/**************************************************************************************************
func: gmineInfoClocWriterContent
**************************************************************************************************/
Gb gmineInfoClocWriterContent(GmineInfo * const gmineInfo, GmineInfoFileType const fileType,
   GsetBuffer setBufferFunc, void * const dataRepo)
{
   returnFalseIf(!_isStarted);

   _MiMemClearType(gmineInfo, GmineInfo);

   gmineInfo->isWriting = gbTRUE;
   gmineInfo->fileType  = fileType;
   gmineInfo->dataRepo  = dataRepo;
   gmineInfo->setBuffer = setBufferFunc;

   // Start by writing out the header for the file.
   return _MiIoClocWriter(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoDloc
**************************************************************************************************/
void gmineInfoDloc(GmineInfo * const gmineInfo)
{
   returnVoidIf(
      !_isStarted ||
      !gmineInfo);

   gmineInfoDlocContent(gmineInfo);

   _MiMemDloc(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoDlocContent
**************************************************************************************************/
void gmineInfoDlocContent(GmineInfo * const gmineInfo)
{
   returnVoidIf(
      !_isStarted ||
      !gmineInfo);

   _MiIoDloc(gmineInfo);

   _MiMemClearType(gmineInfo, GmineInfo);
}

/**************************************************************************************************
func: gmineInfoSetDataAuthorName
**************************************************************************************************/
Gb gmineInfoSetDataAuthorName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value      ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->data.authorName      = _MiStrClone(value);
   gmineInfo->data.isSetAuthorName = (gmineInfo->data.authorName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataCompanyName
**************************************************************************************************/
Gb gmineInfoSetDataCompanyName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value      ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->data.companyName      = _MiStrClone(value);
   gmineInfo->data.isSetCompanyName = (gmineInfo->data.companyName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataCopyright
**************************************************************************************************/
Gb gmineInfoSetDataCopyright(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value      ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->data.copyright      = _MiStrClone(value);
   gmineInfo->data.isSetCopyright = (gmineInfo->data.copyright != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectMax
**************************************************************************************************/
Gb gmineInfoSetDataProjectMax(GmineInfo * const gmineInfo, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value);

   gmineInfo->data.projectMax      = *value;
   gmineInfo->data.isSetProjectMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectMin
**************************************************************************************************/
Gb gmineInfoSetDataProjectMin(GmineInfo * const gmineInfo, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value);

   gmineInfo->data.projectMin      = *value;
   gmineInfo->data.isSetProjectMin = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectName
**************************************************************************************************/
Gb gmineInfoSetDataProjectName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value      ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->data.projectName      = _MiStrClone(value);
   gmineInfo->data.isSetProjectName = (gmineInfo->data.projectName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectSystem
**************************************************************************************************/
Gb gmineInfoSetDataProjectSystem(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value      ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->data.projectSystem      = _MiStrClone(value);
   gmineInfo->data.isSetProjectSystem = (gmineInfo->data.projectSystem != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataOther
**************************************************************************************************/
Gb gmineInfoSetDataOther(GmineInfo * const gmineInfo, Gstr const * const key, Gstr const * const value)
{
   GmineInfoKeyValue *keyValue;

   returnFalseIf(
      !_isStarted                    ||
      !gmineInfo                     ||
      !key                           ||
      !_MiStrGetCount(key,   Gn4MAX) ||
      !value                         ||
      !_MiStrGetCount(value, Gn4MAX));

   keyValue = _MiMemClocType(GmineInfoKeyValue);
   returnFalseIf(!keyValue);

   keyValue->key   = _MiStrClone(key);
   keyValue->value = _MiStrClone(value);

   if (!gmineInfo->data.otherListHead)
   {
      // Start the list.
      gmineInfo->data.otherListHead    =
         gmineInfo->data.otherListTail = keyValue;
   }
   else
   {
      // Append to the list.
      gmineInfo->data.otherListTail->next = keyValue;
      gmineInfo->data.otherListTail       = keyValue;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetItemAppend
**************************************************************************************************/
Gb gmineInfoSetItemAppend(GmineInfo * const gmineInfo, GmineInfoItem * const gmineInfoItem)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoItem);

   if (!gmineInfo->itemHead)
   {
      gmineInfo->itemHead    =
         gmineInfo->itemTail = gmineInfoItem;
   }
   else
   {
      gmineInfo->itemTail->next = gmineInfoItem;
      gmineInfo->itemTail       = gmineInfoItem;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteData
**************************************************************************************************/
Gb gmineInfoWriteData(GmineInfo * const gmineInfo)
{
   GmineInfoKeyValue *keyValue;

   if (gmineInfo->data.isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COMPANY_NAME,     gmineInfo->data.companyName);
   }
   if (gmineInfo->data.isSetCopyright)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COPYRIGHT,        gmineInfo->data.copyright);
   }
   if (gmineInfo->data.isSetProjectName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_NAME,     gmineInfo->data.projectName);
   }
   if (gmineInfo->data.isSetProjectSystem)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_SYSTEM,   gmineInfo->data.projectSystem);
   }
   if (gmineInfo->data.isSetProjectMin)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MIN,     &gmineInfo->data.projectMin);
   }
   if (gmineInfo->data.isSetProjectMax)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MAX,     &gmineInfo->data.projectMax);
   }
   if (gmineInfo->data.isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_AUTHOR_NAME,      gmineInfo->data.authorName);
   }

   keyValue = gmineInfo->data.otherListHead;
   loop
   {
      breakIf(!keyValue);

      _MiIoWriteString(gmineInfo, keyValue->key, keyValue->value);

      keyValue = keyValue->next;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoStart
**************************************************************************************************/
Gb gmineInfoStart(GmemCloc memClocFunc, GmemDloc memDlocFunc)
{
   returnTrueIf(_isStarted);

   returnFalseIf(
      !memClocFunc ||
      !memDlocFunc)

   // Start support libraries.
   returnFalseIf(
      !gjsonStart(memClocFunc, memDlocFunc) ||
      !gmiffStart(memClocFunc, memDlocFunc));

   _isStarted = gbTRUE;

   _MiMemStart(memClocFunc, memDlocFunc);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoStop
**************************************************************************************************/
void gmineInfoStop(void)
{
   returnVoidIf(!_isStarted);

   _isStarted = gbFALSE;
}

/**************************************************************************************************
GmineInfoItem:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoItemCloc
**************************************************************************************************/
GmineInfoItem *gmineInfoItemCloc(void)
{
   GmineInfoItem *gmineInfoItem;

   returnNullIf(!_isStarted);

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
   returnFalseIf(!_isStarted);

   _MiMemClearType(gmineInfoItem, GmineInfoItem);

   gmineInfoItem->id = _itemId++;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemDloc
**************************************************************************************************/
void gmineInfoItemDloc(GmineInfoItem * const gmineInfoItem)
{
   returnVoidIf(
      !_isStarted ||
      !gmineInfoItem);

   gmineInfoItemDlocContent(gmineInfoItem);

   _MiMemDloc(gmineInfoItem);
}

/**************************************************************************************************
func: gmineInfoItemDlocContent
**************************************************************************************************/
void gmineInfoItemDlocContent(GmineInfoItem * const gmineInfoItem)
{
   GmineInfoKeyValue *curr,
                     *next;

   returnVoidIf(
      !_isStarted ||
      !gmineInfoItem);

   // Clean up the strings.
   if (gmineInfoItem->type == gmineInfoItemTypeDATE     ||
       gmineInfoItem->type == gmineInfoItemTypeDATETIME ||
       gmineInfoItem->type == gmineInfoItemTypeFORMULA  ||
       gmineInfoItem->type == gmineInfoItemTypeSTR      ||
       gmineInfoItem->type == gmineInfoItemTypeTIME)
   {
      _MiMemDloc(gmineInfoItem->defaultValue.value.str);
   }
   _MiMemDloc(gmineInfoItem->key);
   _MiMemDloc(gmineInfoItem->name);

   // Clean up the other list.
   next = gmineInfoItem->otherListHead;
   loop
   {
      breakIf(!next);

      curr = next;
      next = curr->next;

      _MiMemDloc(curr->key);
      _MiMemDloc(curr->value);
      _MiMemDloc(curr);
   }

   // Clear out the memory.
   _MiMemClearType(gmineInfoItem, GmineInfoItem);

   return;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultDate
**************************************************************************************************/
Gb gmineInfoItemSetDefaultDate(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value         ||
      gmineInfoItem->type != gmineInfoItemTypeDATE);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultDateTime
**************************************************************************************************/
Gb gmineInfoItemSetDefaultDateTime(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value         ||
      gmineInfoItem->type != gmineInfoItemTypeDATETIME);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultI
**************************************************************************************************/
Gb gmineInfoItemSetDefaultI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.i    = value;
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultN
**************************************************************************************************/
Gb gmineInfoItemSetDefaultN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.n    = value;
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultR
**************************************************************************************************/
Gb gmineInfoItemSetDefaultR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.r    = value;
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultStr
**************************************************************************************************/
Gb gmineInfoItemSetDefaultStr(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value         ||
      gmineInfoItem->type != gmineInfoItemTypeSTR);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultTime
**************************************************************************************************/
Gb gmineInfoItemSetDefaultTime(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value         ||
      gmineInfoItem->type != gmineInfoItemTypeTIME);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultPercent
**************************************************************************************************/
Gb gmineInfoItemSetDefaultPercent(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.r    = value;
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetDefaultFormula
**************************************************************************************************/
Gb gmineInfoItemSetDefaultFormula(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value         ||
      gmineInfoItem->type != gmineInfoItemTypeFORMULA);

   gmineInfoItem->defaultValue.isSet      = gbTRUE;
   gmineInfoItem->defaultValue.value.str  = _MiStrClone(value);
   gmineInfoItem->isSetDefault            = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetKey
**************************************************************************************************/
Gb gmineInfoItemSetKey(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value);

   gmineInfoItem->key      = _MiStrClone(value);
   gmineInfoItem->isSetUid = (gmineInfoItem->key != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxI
**************************************************************************************************/
Gb gmineInfoItemSetMaxI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->max.isSet   = gbTRUE;
   gmineInfoItem->max.value.i = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxN
**************************************************************************************************/
Gb gmineInfoItemSetMaxN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->max.isSet   = gbTRUE;
   gmineInfoItem->max.value.n = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMaxR
**************************************************************************************************/
Gb gmineInfoItemSetMaxR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->max.isSet   = gbTRUE;
   gmineInfoItem->max.value.r = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinI
**************************************************************************************************/
Gb gmineInfoItemSetMinI(GmineInfoItem * const gmineInfoItem, Gi8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeI);

   gmineInfoItem->min.isSet   = gbTRUE;
   gmineInfoItem->min.value.i = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinN
**************************************************************************************************/
Gb gmineInfoItemSetMinN(GmineInfoItem * const gmineInfoItem, Gn8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeN);

   gmineInfoItem->min.isSet   = gbTRUE;
   gmineInfoItem->min.value.n = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetMinR
**************************************************************************************************/
Gb gmineInfoItemSetMinR(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      gmineInfoItem->type != gmineInfoItemTypeR);

   gmineInfoItem->min.isSet   = gbTRUE;
   gmineInfoItem->min.value.r = value;
   gmineInfoItem->isSetMax    = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetName
**************************************************************************************************/
Gb gmineInfoItemSetName(GmineInfoItem * const gmineInfoItem, Gstr const * const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
      !value);

   gmineInfoItem->name      = _MiStrClone(value);
   gmineInfoItem->isSetName = (gmineInfoItem->name != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetOther
**************************************************************************************************/
Gb gmineInfoItemSetOther(GmineInfoItem * const gmineInfoItem, Gstr const * const key, Gstr const * const value)
{
   GmineInfoKeyValue *keyValue;

   returnFalseIf(
      !_isStarted                    ||
      !gmineInfoItem                 ||
      !key                           ||
      !_MiStrGetCount(key,   Gn4MAX) ||
      !value                         ||
      !_MiStrGetCount(value, Gn4MAX));

   keyValue = _MiMemClocType(GmineInfoKeyValue);
   returnFalseIf(!keyValue);

   keyValue->key   = _MiStrClone(key);
   keyValue->value = _MiStrClone(value);

   if (!gmineInfoItem->otherListHead)
   {
      // Start the list.
      gmineInfoItem->otherListHead    =
         gmineInfoItem->otherListTail = keyValue;
   }
   else
   {
      // Append to the list.
      gmineInfoItem->otherListTail->next = keyValue;
      gmineInfoItem->otherListTail       = keyValue;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemSetPrecision
**************************************************************************************************/
Gb gmineInfoItemSetPrecision(GmineInfoItem * const gmineInfoItem, Gr8 const value)
{
   returnFalseIf(
      !_isStarted    ||
      !gmineInfoItem ||
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
      !_isStarted ||
      !gmineInfoItem);

   gmineInfoItem->type      = value;
   gmineInfoItem->isSetType = (value != gmineInfoItemTypeNONE);

   returnTrue;
}
