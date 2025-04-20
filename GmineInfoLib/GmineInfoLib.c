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
static Gstr const *_itemTypeStr[] =
{
   "-",
   "boolean",
   "integer",
   "natural",
   "real",
   "string",
   "date",
   "datetime",
   "time",
   "percent",
   "formula"
};

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/
static Gb _isStarted = gbFALSE;

/**************************************************************************************************
prototype:
**************************************************************************************************/
static Gstr const *_StrFromItemType(GmineInfoItemType const type);

static Gb          _WriteProperty(  GmineInfo       * const gmineInfo, GmineInfoProperty const * const prop);

/**************************************************************************************************
macro:
**************************************************************************************************/
#define WRITE_B(        MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteB(    (MI), (KEY),  (VAR)->VALUE)); }
#define WRITE_I(        MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteI(    (MI), (KEY),  (VAR)->VALUE)); }
#define WRITE_R(        MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteR(    (MI), (KEY),  (VAR)->VALUE)); }
#define WRITE_N(        MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteN(    (MI), (KEY),  (VAR)->VALUE)); }
#define WRITE_STR(      MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteStr(  (MI), (KEY),  (VAR)->VALUE)); }
#define WRITE_COLOR(    MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteColor((MI), (KEY), &(VAR)->VALUE)); }
#define WRITE_POINT(    MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWritePoint((MI), (KEY), &(VAR)->VALUE)); }
#define WRITE_ITEM_TYPE(MI, VAR, KEY, ISSET, VALUE) if (!(VAR)->ISSET) { returnFalseIf(!_MiIoWriteStr(  (MI), (KEY), _itemTypeStr[(VAR)->VALUE])); }

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoAddImage
**************************************************************************************************/
Gb gmineInfoAddImage(GmineInfo * const gmineInfo, GmineInfoImage * const image)
{
   return gmineInfoArrayAddLast(&gmineInfo->imageArray, image);
}

/**************************************************************************************************
func: gmineInfoBlockTypeNext

Block order is important for reading in the MineInfo file in one go.  Some data needs to be defined
before it is being referred to.  This is why writing out blocks in a specific order is important.
This function is only necessary for writing.  Reading will a MineInfo file works differently.
**************************************************************************************************/
GmineInfoBlockType gmineInfoBlockTypeNext(GmineInfo * const gmineInfo)
{
   // Bad input
   returnIf(
         !_isStarted ||
         !gmineInfo,
      gmineInfoBlockType_END);

   // No more blocks.
   returnIf(
         gmineInfo->currentBlockType == gmineInfoBlockType_END,
      gmineInfoBlockType_END);

   // If we started a block but not stopped it then the user is doing something wrong.
   returnIf(
         gmineInfo->isWriting      &&
         gmineInfo->isBlockStarted &&
         !gmineInfo->isBlockStopped,
      gmineInfoBlockType_END);

   // Move to the next block
   gmineInfo->currentBlockType++;

   // Reset.
   gmineInfo->isBlockStarted    =
      gmineInfo->isBlockStopped = gbFALSE;

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

   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->itemArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->imageArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->propertyArray));

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

   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->itemArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->imageArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->propertyArray));

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

   gmineInfoDataDloc(gmineInfo->data);

   gmineInfoArrayForEach(    &gmineInfo->itemArray, gmineInfoItemDloc);
   gmineInfoArrayDlocContent(&gmineInfo->itemArray);

   gmineInfoArrayForEach(    &gmineInfo->imageArray, gmineInfoImageDloc);
   gmineInfoArrayDlocContent(&gmineInfo->imageArray);

   gmineInfoArrayForEach(    &gmineInfo->propertyArray, gmineInfoPropertyDloc);
   gmineInfoArrayDlocContent(&gmineInfo->propertyArray);

   _MiMemClearType(gmineInfo, GmineInfo);
}

/**************************************************************************************************
func: gmineInfoIsStarted
**************************************************************************************************/
Gb gmineInfoIsStarted(void)
{
   return _isStarted;
}

/**************************************************************************************************
func: gmineInfoSetData
**************************************************************************************************/
Gb gmineInfoSetData(GmineInfo * const gmineInfo, GmineInfoData * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value);

   gmineInfo->data = value;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetItemListAppend
**************************************************************************************************/
Gb gmineInfoSetItemListAppend(GmineInfo * const gmineInfo, GmineInfoItem * const gmineInfoItem)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoItem);

   return gmineInfoArrayAddLast(&gmineInfo->itemArray, gmineInfoItem);
}

/**************************************************************************************************
func: gmineInfoSetImageListAppend
**************************************************************************************************/
Gb gmineInfoSetImageListAppend(GmineInfo * const gmineInfo, GmineInfoImage * const gmineInfoImage)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoImage);

   return gmineInfoArrayAddLast(&gmineInfo->imageArray, gmineInfoImage);
}

/**************************************************************************************************
func: gmineInfoSetPropertyListAppend
**************************************************************************************************/
Gb gmineInfoSetPropertyListAppend(GmineInfo * const gmineInfo, GmineInfoProperty * const gmineInfoProperty)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoProperty);

   return gmineInfoArrayAddLast(&gmineInfo->propertyArray, gmineInfoProperty);
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
func: gmineInfoWriteBlockStop
**************************************************************************************************/
Gb gmineInfoWriteBlockStop(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo->isBlockStarted);

   gmineInfo->isBlockStopped = gbTRUE;

   return _MiIoWriteBlockStop(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentData
**************************************************************************************************/
Gb gmineInfoWriteBlockContentData(GmineInfo * const gmineInfo)
{
   Gcount             count;
   Gindex             index;
   GmineInfoKeyValue *kv;

   // Ensure writing in order.
   returnFalseIf(
      !_isStarted ||
      !gmineInfo->isBlockStarted);

   WRITE_STR(  gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_COMPANY_NAME,   isSetAuthorName,    companyName);
   WRITE_STR(  gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_COPYRIGHT,      isSetCopyright,     copyright);
   WRITE_STR(  gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_PROJECT_NAME,   isSetProjectName,   projectName);
   WRITE_STR(  gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_PROJECT_SYSTEM, isSetProjectSystem, projectSystem);
   WRITE_POINT(gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_PROJECT_MIN,    isSetProjectMin,    projectMin);
   WRITE_POINT(gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_PROJECT_MAX,    isSetProjectMax,    projectMax);
   WRITE_STR(  gmineInfo, gmineInfo->data, KEY_BLOCK_DATA_AUTHOR_NAME,      isSetAuthorName,    authorName);

   // Write out the user key values.
   count = gmineInfoArrayGetCount(&gmineInfo->data->keyValueArray);
   forCount(index, count)
   {
      kv = (GmineInfoKeyValue *) gmineInfoArrayGetAt(&gmineInfo->data->keyValueArray, index);
      returnFalseIf(!_MiIoWriteStr(gmineInfo, kv->key, kv->value));
   }

   gmineInfo->isBlockDataWritten = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentImageList
**************************************************************************************************/
Gb gmineInfoWriteBlockContentImageList(GmineInfo * const gmineInfo)
{
   Gcount          count;
   Gindex          index;
   GmineInfoImage *image;

   // Ensure writing in order
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeIMAGE_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->imageArray);
   forCount(index, count)
   {
      image = (GmineInfoImage *) gmineInfoArrayGetAt(&gmineInfo->imageArray, index);

      // File path is mandatory.  An image without an image is not an image.
      returnFalseIf(!gmineInfoImageGetFilePath(image));

      returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_IMAGE));

      WRITE_STR(gmineInfo, image, KEY_BLOCK_IMAGE_KEY,       isSetKey,      key);
      WRITE_STR(gmineInfo, image, KEY_BLOCK_IMAGE_NAME,      isSetName,     name);
      WRITE_STR(gmineInfo, image, KEY_BLOCK_IMAGE_FILE_PATH, isSetFilePath, filePath);

      if (image->isInline)
      {
         size_t readCount,
                fileSize;
         Gcount bufferSize;
         Gn1   *buffer;
         Gn1    bufferSmall[1024];
         FILE  *file;
         Gb     isWritten;

         isWritten  = gbFALSE;
         file       = NULL;

         bufferSize = SIZE_1MB;
         buffer     = _MiMemClocTypeArray(count, Gn1);
         if (!buffer)
         {
            bufferSize = 1024;
            buffer     = bufferSmall;
         }

         breakScope
         {
            // Open the file to read.
            returnFalseIf(!fopen_s(&file, gmineInfoImageGetFilePath(image), "rb"));

            // Get the raw file size.
            fileSize = 0;
            _fseeki64_nolock(file, 0, SEEK_END);
            fileSize = _ftelli64(file);

            // Start the binary buffer.
            returnFalseIf(!_MiIoWriteBinStart(gmineInfo, KEY_BLOCK_IMAGE_FILE, fileSize));

            // Return to the start of the file.
            _fseeki64_nolock(file, 0, SEEK_SET);

            // Copy the contents to the MI file.
            loop
            {
               readCount = fread_s(buffer, bufferSize, 1, bufferSize, file);
               breakIf(_MiIoWriteBinBuffer(gmineInfo, (Gcount) readCount, buffer));

               // Read everything there is for the file.
               breakIf(readCount != bufferSize);
            }

            breakIf(_MiIoWriteBinStop(gmineInfo));

            isWritten = gbTRUE;
         }

         // Close the file.
         fclose(file);

         if (count == SIZE_1MB)
         {
            _MiMemDloc(buffer);
         }

         returnFalseIf(!isWritten);
      }

      returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentItemList
**************************************************************************************************/
Gb gmineInfoWriteBlockContentItemList(GmineInfo * const gmineInfo)
{
   Gcount             count,
                      countBin;
   Gindex             index,
                      indexBin;
   GmineInfoItem     *item;
   GmineInfoItemBin  *bin;

   // Ensure writing is in order.
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeITEM_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->itemArray);
   forCount(index, count)
   {
      item = (GmineInfoItem *) gmineInfoArrayGetAt(&gmineInfo->itemArray, index);

      returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM));

      WRITE_STR(      gmineInfo, item, KEY_BLOCK_ITEM_KEY,  isSetKey,  key);
      WRITE_STR(      gmineInfo, item, KEY_BLOCK_ITEM_NAME, isSetName, name);
      WRITE_ITEM_TYPE(gmineInfo, item, KEY_BLOCK_ITEM_TYPE, isSetType, type);

      switch (item->type)
      {
      case gmineInfoItemTypeB:
         WRITE_B(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMin, max.b);
         break;

      case gmineInfoItemTypeI:
         WRITE_I(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMin, max.i);
         break;

      case gmineInfoItemTypeN:
         WRITE_N(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMin, max.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         WRITE_R(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMin, max.r);
         break;
      }

      switch (item->type)
      {
      case gmineInfoItemTypeB:
         WRITE_B(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMax, max.b);
         break;

      case gmineInfoItemTypeI:
         WRITE_I(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMax, max.i);
         break;

      case gmineInfoItemTypeN:
         WRITE_N(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMax, max.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         WRITE_R(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetMax, max.r);
         break;
      }

      if (item->type == gmineInfoItemTypeR ||
            item->type == gmineInfoItemTypePERCENT)
      {
         WRITE_R(gmineInfo, item, KEY_BLOCK_ITEM_PRECISION, isSetPrecision, precision);
      }

      switch (item->type)
      {
      case gmineInfoItemTypeB:
         WRITE_B(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetDefault, defaultValue.b);
         break;

      case gmineInfoItemTypeDATE:
      case gmineInfoItemTypeDATETIME:
      case gmineInfoItemTypeTIME:
      case gmineInfoItemTypeSTR:
         WRITE_STR(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetDefault, defaultValue.str);
         break;

      case gmineInfoItemTypeI:
         WRITE_I(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetDefault, defaultValue.i);
         break;

      case gmineInfoItemTypeN:
         WRITE_N(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetDefault, defaultValue.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         WRITE_R(gmineInfo, item, KEY_BLOCK_ITEM_DEFAULT, isSetDefault, defaultValue.r);
         break;
      }

      WRITE_STR(gmineInfo, item, KEY_BLOCK_ITEM_FORMULA, isSetFormula, formula);

      countBin = gmineInfoArrayGetCount(&item->binArray);
      if (countBin)
      {
         returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM_BIN_LIST));

         forCount(indexBin, countBin)
         {
            bin = (GmineInfoItemBin *) gmineInfoArrayGetAt(&item->binArray, indexBin);

            returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM_BIN));

            switch (item->type)
            {
            case gmineInfoItemTypeB:
               WRITE_B(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_VALUE, isSetValue, value.b);
               break;

            case gmineInfoItemTypeDATE:
            case gmineInfoItemTypeDATETIME:
            case gmineInfoItemTypeTIME:
            case gmineInfoItemTypeSTR:
            case gmineInfoItemTypeFORMULA:
               WRITE_STR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_VALUE, isSetValue, value.str);
               break;

            case gmineInfoItemTypeI:
               WRITE_I(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_VALUE, isSetValue, value.i);
               break;

            case gmineInfoItemTypeN:
               WRITE_N(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_VALUE, isSetValue, value.n);
               break;

            case gmineInfoItemTypeR:
            case gmineInfoItemTypePERCENT:
               WRITE_R(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_VALUE, isSetValue, value.r);
               break;
            }

            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE,      isSetIsVisibleDrillHole,     isVisibleDrillHole);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_FACE, isSetIsVisibleDrillHoleFace, isVisibleDrillHoleFace);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_LINE, isSetIsVisibleDrillHoleLine, isVisibleDrillHoleLine);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_NODE, isSetIsVisibleDrillHoleNode, isVisibleDrillHoleNode);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY,       isSetIsVisibleGeometry,      isVisibleGeometry);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_FACE,  isSetIsVisibleGeometryFace,  isVisibleGeometryFace);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_LINE,  isSetIsVisibleGeometryLine,  isVisibleGeometryLine);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_NODE,  isSetIsVisibleGeometryNode,  isVisibleGeometryNode);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL,          isSetIsVisibleModel,         isVisibleModel);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_FACE,     isSetIsVisibleModelFace,     isVisibleModelFace);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_LINE,     isSetIsVisibleModelLine,     isVisibleModelLine);
            WRITE_B(    gmineInfo, bin, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_NODE,     isSetIsVisibleModelNode,     isVisibleModelNode);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE,           isSetColorDrillHole,         colorDrillHole);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_FACE,      isSetColorDrillHoleFace,     colorDrillHoleFace);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_LINE,      isSetColorDrillHoleLine,     colorDrillHoleLine);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_NODE,      isSetColorDrillHoleNode,     colorDrillHoleNode);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY,            isSetColorGeometry,          colorGeometry);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_FACE,       isSetColorGeometryFace,      colorGeometryFace);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_LINE,       isSetColorGeometryLine,      colorGeometryLine);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_NODE,       isSetColorGeometryNode,      colorGeometryNode);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_MODEL,               isSetColorModel,             colorModel);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_FACE,          isSetColorModelFace,         colorModelFace);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_LINE,          isSetColorModelLine,         colorModelLine);
            WRITE_COLOR(gmineInfo, bin, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_NODE,          isSetColorModelNode,         colorModelNode);

            returnFalseIf(!_WriteProperty(gmineInfo, &bin->property));

            returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
         }

         returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
      }

      returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentPropertyList
**************************************************************************************************/
Gb gmineInfoWriteBlockContentPropertyList(GmineInfo * const gmineInfo)
{
   Gcount             count;
   Gindex             index;
   GmineInfoProperty *prop;

   // Ensure writing is in order.
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypePROPERTY_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->propertyArray);
   forCount(index, count)
   {
      prop = (GmineInfoProperty *) gmineInfoArrayGetAt(&gmineInfo->propertyArray, index);

      returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_PROPERTY));

      WRITE_STR(gmineInfo, prop, KEY_BLOCK_PROPERTY_KEY,  isSetKey,  key);
      WRITE_STR(gmineInfo, prop, KEY_BLOCK_PROPERTY_NAME, isSetName, name);

      returnFalseIf(!_WriteProperty(gmineInfo, prop));

      returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteBlockStartData
**************************************************************************************************/
Gb gmineInfoWriteBlockStartData(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      gmineInfo->currentBlockType != gmineInfoBlockTypeDATA);

   gmineInfo->isBlockStarted = gbTRUE;

   return _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DATA);
}

/**************************************************************************************************
func: gmineInfoWriteBlockStartImageList
**************************************************************************************************/
Gb gmineInfoWriteBlockStartImageList(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      gmineInfo->currentBlockType != gmineInfoBlockTypeIMAGE_LIST);

   gmineInfo->isBlockStarted = gbTRUE;

   return _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_IMAGE_LIST);
}

/**************************************************************************************************
func: gmineInfoWriteBlockStartItemList
**************************************************************************************************/
Gb gmineInfoWriteBlockStartItemList(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      gmineInfo->currentBlockType != gmineInfoBlockTypeITEM_LIST);

   gmineInfo->isBlockStarted = gbTRUE;

   return _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM_LIST);
}

/**************************************************************************************************
func: gmineInfoWriteBlockStartPropertyList
**************************************************************************************************/
Gb gmineInfoWriteBlockStartPropertyList(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      gmineInfo->currentBlockType != gmineInfoBlockTypePROPERTY_LIST);

   gmineInfo->isBlockStarted = gbTRUE;

   return _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_PROPERTY_LIST);
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _StrFromItemType
**************************************************************************************************/
static Gstr const *_StrFromItemType(GmineInfoItemType const type)
{
   switch (type)
   {
   case gmineInfoItemTypeB:
      return "boolean";

   case gmineInfoItemTypeI:
      return "integer";

   case gmineInfoItemTypeN:
      return "natural";

   case gmineInfoItemTypeR:
      return "real";

   case gmineInfoItemTypeSTR:
      return "string";

   case gmineInfoItemTypeDATE:
      return "date";

   case gmineInfoItemTypeTIME:
      return "time";

   case gmineInfoItemTypeDATETIME:
      return "datetime";

   case gmineInfoItemTypePERCENT:
      return "percent";

   case gmineInfoItemTypeFORMULA:
      return "formula";
   }

   return "-";
}

/**************************************************************************************************
func: _WriteProperty
**************************************************************************************************/
static Gb _WriteProperty(GmineInfo *gmineInfo, GmineInfoProperty const * const prop)
{
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_ARROW_SIZE_RELATIVE,      isSetIsArrowSizeRelative,      isArrowSizeRelative);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_CLIPPABLE,                isSetIsClippable,              isClippable);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_FONT_BOLD,                isSetIsFontBold,               isFontBold);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_FONT_ITALIC,              isSetIsFontItalic,             isFontItalic);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_FONT_SIZE_RELATIVE,       isSetIsFontSizeRelative,       isFontSizeRelative);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_FONT_STRIKE_OUT,          isSetIsFontStrikeOut,          isFontStrikeOut);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_FONT_UNDERLINE,           isSetIsFontUnderline,          isFontUnderline);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_SELECTABLE,               isSetIsSelectable,             isSelectable);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE,                  isSetIsVisible,                isVisible);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POINT,            isSetIsVisiblePoint,           isVisiblePoint);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE,         isSetIsVisiblePolyline,        isVisiblePolyline);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_ARROW,   isSetIsVisiblePolylineArrow,   isVisiblePolylineArrow);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_FACE,    isSetIsVisiblePolylineFace,    isVisiblePolylineFace);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_LINE,    isSetIsVisiblePolylineLine,    isVisiblePolylineLine);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_NODE,    isSetIsVisiblePolylineNode,    isVisiblePolylineNode);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE,          isSetIsVisibleSurface,         isVisibleSurface);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_FACE,     isSetIsVisibleSurfaceFace,     isVisibleSurfaceFace);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_LINE,     isSetIsVisibleSurfaceLine,     isVisibleSurfaceLine);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_NODE,     isSetIsVisibleSurfaceNode,     isVisibleSurfaceNode);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_TEXT,             isSetIsVisibleText,            isVisibleText);
   WRITE_I(    gmineInfo, prop, KEY_BLOCK_PROPERTY_ARROW_POSITION,              isSetArrowPosition,            arrowPosition);
   WRITE_R(    gmineInfo, prop, KEY_BLOCK_PROPERTY_ARROW_SIZE,                  isSetArrowSize,                arrowSize);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR,                       isSetColor,                    color);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POINT,                 isSetColorPoint,               colorPoint);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE,              isSetColorPolyline,            colorPolyline);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_ARROW,        isSetColorPolylineArrow,       colorPolylineArrow);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE,         isSetColorPolylineFace,        colorPolylineFace);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE_PATTERN, isSetColorPolylineFacePattern, colorPolylineFacePattern);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_LINE,         isSetColorPolylineLine,        colorPolylineLine);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_NODE,         isSetColorPolylineNode,        colorPolylineNode);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE,          isSetColorSurfaceFace,         colorSurfaceFace);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE_PATTERN,  isSetColorSurfaceFacePattern,  colorSurfaceFacePattern);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_SURFACE_LINE,          isSetColorSurfaceLine,         colorSurfaceLine);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_SURFACE_NODE,          isSetColorSurfaceNode,         colorSurfaceNode);
   WRITE_COLOR(gmineInfo, prop, KEY_BLOCK_PROPERTY_COLOR_TEXT,                  isSetColorText,                colorText);
   WRITE_R(    gmineInfo, prop, KEY_BLOCK_PROPERTY_FACE_TRANSPARENCY,           isSetFaceTransparency,         faceTransparency);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_FONT_NAME,                   isSetFontName,                 fontName);
   WRITE_R(    gmineInfo, prop, KEY_BLOCK_PROPERTY_FONT_SIZE,                   isSetFontSize,                 fontSize);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_PATTERN_ARROW,               isSetPatternArrow,             patternArrow);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_PATTERN_FACE,                isSetPatternFace,              patternFace);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_PATTERN_LINE,                isSetPatternLine,              patternLine);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_PATTERN_NODE,                isSetPatternNode,              patternNode);

   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_01,    isSetIsVisibleLabelData[ 0],   isVisibleLabelData[ 0]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_02,    isSetIsVisibleLabelData[ 1],   isVisibleLabelData[ 1]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_03,    isSetIsVisibleLabelData[ 2],   isVisibleLabelData[ 2]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_04,    isSetIsVisibleLabelData[ 3],   isVisibleLabelData[ 3]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_05,    isSetIsVisibleLabelData[ 4],   isVisibleLabelData[ 4]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_06,    isSetIsVisibleLabelData[ 5],   isVisibleLabelData[ 5]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_07,    isSetIsVisibleLabelData[ 6],   isVisibleLabelData[ 6]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_08,    isSetIsVisibleLabelData[ 7],   isVisibleLabelData[ 7]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_09,    isSetIsVisibleLabelData[ 8],   isVisibleLabelData[ 8]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_10,    isSetIsVisibleLabelData[ 9],   isVisibleLabelData[ 9]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_01,    isSetIsVisibleLabelLine[ 0],   isVisibleLabelLine[ 0]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_02,    isSetIsVisibleLabelLine[ 1],   isVisibleLabelLine[ 1]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_03,    isSetIsVisibleLabelLine[ 2],   isVisibleLabelLine[ 2]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_04,    isSetIsVisibleLabelLine[ 3],   isVisibleLabelLine[ 3]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_05,    isSetIsVisibleLabelLine[ 4],   isVisibleLabelLine[ 4]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_06,    isSetIsVisibleLabelLine[ 5],   isVisibleLabelLine[ 5]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_07,    isSetIsVisibleLabelLine[ 6],   isVisibleLabelLine[ 6]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_08,    isSetIsVisibleLabelLine[ 7],   isVisibleLabelLine[ 7]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_09,    isSetIsVisibleLabelLine[ 8],   isVisibleLabelLine[ 8]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_10,    isSetIsVisibleLabelLine[ 9],   isVisibleLabelLine[ 9]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_01,    isSetIsVisibleLabelNode[ 0],   isVisibleLabelNode[ 0]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_02,    isSetIsVisibleLabelNode[ 1],   isVisibleLabelNode[ 1]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_03,    isSetIsVisibleLabelNode[ 2],   isVisibleLabelNode[ 2]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_04,    isSetIsVisibleLabelNode[ 3],   isVisibleLabelNode[ 3]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_05,    isSetIsVisibleLabelNode[ 4],   isVisibleLabelNode[ 4]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_06,    isSetIsVisibleLabelNode[ 5],   isVisibleLabelNode[ 5]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_07,    isSetIsVisibleLabelNode[ 6],   isVisibleLabelNode[ 6]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_08,    isSetIsVisibleLabelNode[ 7],   isVisibleLabelNode[ 7]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_09,    isSetIsVisibleLabelNode[ 8],   isVisibleLabelNode[ 8]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_10,    isSetIsVisibleLabelNode[ 9],   isVisibleLabelNode[ 9]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_11,    isSetIsVisibleLabelNode[10],   isVisibleLabelNode[10]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_12,    isSetIsVisibleLabelNode[11],   isVisibleLabelNode[11]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_13,    isSetIsVisibleLabelNode[12],   isVisibleLabelNode[12]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_14,    isSetIsVisibleLabelNode[13],   isVisibleLabelNode[13]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_15,    isSetIsVisibleLabelNode[14],   isVisibleLabelNode[14]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_16,    isSetIsVisibleLabelNode[15],   isVisibleLabelNode[15]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_17,    isSetIsVisibleLabelNode[16],   isVisibleLabelNode[16]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_18,    isSetIsVisibleLabelNode[17],   isVisibleLabelNode[17]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_19,    isSetIsVisibleLabelNode[18],   isVisibleLabelNode[18]);
   WRITE_B(    gmineInfo, prop, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_20,    isSetIsVisibleLabelNode[19],   isVisibleLabelNode[19]);

   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_01,               isSetLabelData[ 0],            labelData[ 0]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_02,               isSetLabelData[ 1],            labelData[ 1]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_03,               isSetLabelData[ 2],            labelData[ 2]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_04,               isSetLabelData[ 3],            labelData[ 3]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_05,               isSetLabelData[ 4],            labelData[ 4]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_06,               isSetLabelData[ 5],            labelData[ 5]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_07,               isSetLabelData[ 6],            labelData[ 6]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_08,               isSetLabelData[ 7],            labelData[ 7]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_09,               isSetLabelData[ 8],            labelData[ 8]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_DATA_LABEL_10,               isSetLabelData[ 9],            labelData[ 9]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_01,               isSetLabelLine[ 0],            labelLine[ 0]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_02,               isSetLabelLine[ 1],            labelLine[ 1]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_03,               isSetLabelLine[ 2],            labelLine[ 2]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_04,               isSetLabelLine[ 3],            labelLine[ 3]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_05,               isSetLabelLine[ 4],            labelLine[ 4]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_06,               isSetLabelLine[ 5],            labelLine[ 5]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_07,               isSetLabelLine[ 6],            labelLine[ 6]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_08,               isSetLabelLine[ 7],            labelLine[ 7]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_09,               isSetLabelLine[ 8],            labelLine[ 8]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_LINE_LABEL_10,               isSetLabelLine[ 9],            labelLine[ 9]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_01,               isSetLabelNode[ 0],            labelNode[ 0]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_02,               isSetLabelNode[ 1],            labelNode[ 1]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_03,               isSetLabelNode[ 2],            labelNode[ 2]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_04,               isSetLabelNode[ 3],            labelNode[ 3]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_05,               isSetLabelNode[ 4],            labelNode[ 4]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_06,               isSetLabelNode[ 5],            labelNode[ 5]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_07,               isSetLabelNode[ 6],            labelNode[ 6]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_08,               isSetLabelNode[ 7],            labelNode[ 7]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_09,               isSetLabelNode[ 8],            labelNode[ 8]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_10,               isSetLabelNode[ 9],            labelNode[ 9]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_11,               isSetLabelNode[10],            labelNode[10]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_12,               isSetLabelNode[11],            labelNode[11]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_13,               isSetLabelNode[12],            labelNode[12]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_14,               isSetLabelNode[13],            labelNode[13]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_15,               isSetLabelNode[14],            labelNode[14]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_16,               isSetLabelNode[15],            labelNode[15]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_17,               isSetLabelNode[16],            labelNode[16]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_18,               isSetLabelNode[17],            labelNode[17]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_19,               isSetLabelNode[18],            labelNode[18]);
   WRITE_STR(  gmineInfo, prop, KEY_BLOCK_PROPERTY_NODE_LABEL_20,               isSetLabelNode[19],            labelNode[19]);

   returnTrue;
}