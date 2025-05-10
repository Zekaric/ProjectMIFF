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
static Gstr const *_StrFromItemType(   GmineInfoItemType const type);

static Gb          _WriteB(            GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, Gb const value);
static Gb          _WriteColor(        GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, GmineInfoColor const * const value);
static Gb          _WriteI(            GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, Gi8 const value);
static Gb          _WriteItemType(     GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, GmineInfoItemType const value);
static Gb          _WriteKeyValueList( GmineInfo       * const gmineInfo, GmineInfoArray * const keyValueArray);
static Gb          _WriteN(            GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, Gn8 const value);

static Gb          _WriteProperty(     GmineInfo       * const gmineInfo, GmineInfoProperty const * const prop);

static Gb          _WritePoint(        GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, GmineInfoPoint const * const value);
static Gb          _WriteR(            GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, Gr8 const value);
static Gb          _WriteStr(          GmineInfo       * const gmineInfo, char const * const key, Gb const isSet, Gstr const * const value);

/**************************************************************************************************
macro:
**************************************************************************************************/

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

   // Closing the file.
   _MiIoDloc(gmineInfo);

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
func: gmineInfoWriteBlockStopList
**************************************************************************************************/
Gb gmineInfoWriteBlockStopList(GmineInfo * const gmineInfo)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo->isBlockStarted);

   gmineInfo->isBlockStopped = gbTRUE;

   return _MiIoWriteBlockStopList(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentData
**************************************************************************************************/
Gb gmineInfoWriteBlockContentData(GmineInfo * const gmineInfo)
{
   GmineInfoData  *data;

   // Ensure writing in order.
   returnFalseIf(
      !_isStarted ||
      !gmineInfo->isBlockStarted);

   data = gmineInfo->data;
   returnFalseIf(!_WriteStr(  gmineInfo, KEY_BLOCK_DATA_COMPANY_NAME,  data->isSetCompanyName,   data->companyName));
   returnFalseIf(!_WriteStr(  gmineInfo, KEY_BLOCK_DATA_COPYRIGHT,     data->isSetCopyright,     data->copyright));
   returnFalseIf(!_WriteStr(  gmineInfo, KEY_BLOCK_DATA_PROJECT_NAME,  data->isSetProjectName,   data->projectName));
   returnFalseIf(!_WriteStr(  gmineInfo, KEY_BLOCK_DATA_PROJECT_SYSTEM,data->isSetProjectSystem, data->projectSystem));
   returnFalseIf(!_WritePoint(gmineInfo, KEY_BLOCK_DATA_PROJECT_MIN,   data->isSetProjectMin,   &data->projectMin));
   returnFalseIf(!_WritePoint(gmineInfo, KEY_BLOCK_DATA_PROJECT_MAX,   data->isSetProjectMax,   &data->projectMax));
   returnFalseIf(!_WriteStr(  gmineInfo, KEY_BLOCK_DATA_AUTHOR_NAME,   data->isSetAuthorName,    data->authorName));

   // Write out the user key values.
   returnFalseIf(!_WriteKeyValueList(gmineInfo, &data->keyValueArray));

   gmineInfo->isBlockDataWritten = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteBlockContentImageList
**************************************************************************************************/
Gb gmineInfoWriteBlockContentImageList(GmineInfo * const gmineInfo)
{
   Gcount             count;
   Gindex             index;
   GmineInfoImage    *image;
   size_t             readCount;
   size_t             fileSize;
   Gcount             bufferSize;
   Gn1               *buffer;
   Gn1                bufferSmall[1024];
   FILE              *file;
   Gb                 isWritten;
   int                result;

   // Ensure writing in order
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeIMAGE_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->imageArray);
   forCount(index, count)
   {
      image = (GmineInfoImage *) gmineInfoArrayGetAt(&gmineInfo->imageArray, index);

      // File path is mandatory.  An image without an image is not an image.
      returnFalseIf(!gmineInfoImageGetFileName(image));

      returnFalseIf(!_MiIoWriteBlockStartListItem(gmineInfo, KEY_BLOCK_IMAGE));

      _WriteStr(gmineInfo, KEY_BLOCK_IMAGE_KEY,       image->isSetKey,      image->key);
      _WriteStr(gmineInfo, KEY_BLOCK_IMAGE_NAME,      image->isSetName,     image->name);
      _WriteStr(gmineInfo, KEY_BLOCK_IMAGE_FILE_NAME, image->isSetFileName, image->fileName);

      if (image->isInline)
      {
         isWritten  = gbFALSE;
         file       = NULL;

         bufferSize = READ_BLOCK_SIZE;
         buffer     = _MiMemClocTypeArray(bufferSize, Gn1);
         if (!buffer)
         {
            bufferSize = 1024;
            buffer     = bufferSmall;
         }

         breakScope
         {
            // Open the file to read.
            file = fopen(gmineInfoImageGetFilePath(image), "rb");
            returnFalseIf(!file);

            // Get the raw file size.
            fileSize = 0;
            result   = _fseeki64(file, 0, SEEK_END);
            fileSize = _ftelli64(file);
            result   = _fseeki64(file, 0, SEEK_SET);

            // Start the binary buffer.
            returnFalseIf(!_MiIoWriteBinStart(gmineInfo, KEY_BLOCK_IMAGE_FILE, fileSize));

            // Copy the contents to the MI file.
            loop
            {
               readCount = fread(buffer, 1, bufferSize, file);
               breakIf(!readCount);
               breakIf(!_MiIoWriteBinBuffer(gmineInfo, (Gcount) readCount, buffer));

               // Read everything there is for the file.
               breakIf(readCount != bufferSize);
            }

            breakIf(!_MiIoWriteBinStop(gmineInfo));

            isWritten = gbTRUE;
         }

         // Close the file.
         fclose(file);

         if (bufferSize == READ_BLOCK_SIZE)
         {
            _MiMemDloc(buffer);
         }

         returnFalseIf(!isWritten);
      }

      // Write out the user key values.
      returnFalseIf(!_WriteKeyValueList(gmineInfo, &image->keyValueArray));

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

      returnFalseIf(!_MiIoWriteBlockStartListItem(gmineInfo, KEY_BLOCK_ITEM));

      _WriteStr(     gmineInfo, KEY_BLOCK_ITEM_KEY,  item->isSetKey,  item->key);
      _WriteStr(     gmineInfo, KEY_BLOCK_ITEM_NAME, item->isSetName, item->name);
      _WriteItemType(gmineInfo, KEY_BLOCK_ITEM_TYPE, item->isSetType, item->type);

      switch (item->type)
      {
      case gmineInfoItemTypeI:
         _WriteI(gmineInfo, KEY_BLOCK_ITEM_MIN, item->isSetMin, item->min.i);
         break;

      case gmineInfoItemTypeN:
         _WriteN(gmineInfo, KEY_BLOCK_ITEM_MIN, item->isSetMin, item->min.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         _WriteR(gmineInfo, KEY_BLOCK_ITEM_MIN, item->isSetMin, item->min.r);
         break;
      }

      switch (item->type)
      {
      case gmineInfoItemTypeI:
         _WriteI(gmineInfo, KEY_BLOCK_ITEM_MAX, item->isSetMax, item->max.i);
         break;

      case gmineInfoItemTypeN:
         _WriteN(gmineInfo, KEY_BLOCK_ITEM_MAX, item->isSetMax, item->max.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         _WriteR(gmineInfo, KEY_BLOCK_ITEM_MAX, item->isSetMax, item->max.r);
         break;
      }

      if (item->type == gmineInfoItemTypeR ||
          item->type == gmineInfoItemTypePERCENT)
      {
         _WriteR(gmineInfo, KEY_BLOCK_ITEM_PRECISION, item->isSetPrecision, item->precision);
      }

      switch (item->type)
      {
      case gmineInfoItemTypeB:
         _WriteB(gmineInfo, KEY_BLOCK_ITEM_DEFAULT, item->isSetDefault, item->defaultValue.b);
         break;

      case gmineInfoItemTypeDATE:
      case gmineInfoItemTypeDATETIME:
      case gmineInfoItemTypeTIME:
      case gmineInfoItemTypeSTR:
         _WriteStr(gmineInfo, KEY_BLOCK_ITEM_DEFAULT, item->isSetDefault, item->defaultValue.str);
         break;

      case gmineInfoItemTypeI:
         _WriteI(gmineInfo, KEY_BLOCK_ITEM_DEFAULT, item->isSetDefault, item->defaultValue.i);
         break;

      case gmineInfoItemTypeN:
         _WriteN(gmineInfo, KEY_BLOCK_ITEM_DEFAULT, item->isSetDefault, item->defaultValue.n);
         break;

      case gmineInfoItemTypeR:
      case gmineInfoItemTypePERCENT:
         _WriteR(gmineInfo, KEY_BLOCK_ITEM_DEFAULT, item->isSetDefault, item->defaultValue.r);
         break;
      }

      _WriteStr(gmineInfo, KEY_BLOCK_ITEM_FORMULA, item->isSetFormula, item->formula);

      countBin = gmineInfoArrayGetCount(&item->binArray);
      if (countBin)
      {
         returnFalseIf(!_MiIoWriteBlockStartList(gmineInfo, KEY_BLOCK_ITEM_BIN_LIST));

         forCount(indexBin, countBin)
         {
            bin = (GmineInfoItemBin *) gmineInfoArrayGetAt(&item->binArray, indexBin);

            returnFalseIf(!_MiIoWriteBlockStartListItem(gmineInfo, KEY_BLOCK_ITEM_BIN));

            switch (item->type)
            {
            case gmineInfoItemTypeB:
               _WriteB(gmineInfo, KEY_BLOCK_ITEM_BIN_VALUE, bin->isSetValue, bin->value.b);
               break;

            case gmineInfoItemTypeDATE:
            case gmineInfoItemTypeDATETIME:
            case gmineInfoItemTypeTIME:
            case gmineInfoItemTypeSTR:
            case gmineInfoItemTypeFORMULA:
               _WriteStr(gmineInfo, KEY_BLOCK_ITEM_BIN_VALUE, bin->isSetValue, bin->value.str);
               break;

            case gmineInfoItemTypeI:
               _WriteI(gmineInfo, KEY_BLOCK_ITEM_BIN_VALUE, bin->isSetValue, bin->value.i);
               break;

            case gmineInfoItemTypeN:
               _WriteN(gmineInfo, KEY_BLOCK_ITEM_BIN_VALUE, bin->isSetValue, bin->value.n);
               break;

            case gmineInfoItemTypeR:
            case gmineInfoItemTypePERCENT:
               _WriteR(gmineInfo, KEY_BLOCK_ITEM_BIN_VALUE, bin->isSetValue, bin->value.r);
               break;
            }

            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE,      bin->isSetIsVisibleDrillHole,      bin->isVisibleDrillHole);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_FACE, bin->isSetIsVisibleDrillHoleFace,  bin->isVisibleDrillHoleFace);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_LINE, bin->isSetIsVisibleDrillHoleLine,  bin->isVisibleDrillHoleLine);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_NODE, bin->isSetIsVisibleDrillHoleNode,  bin->isVisibleDrillHoleNode);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY,       bin->isSetIsVisibleGeometry,       bin->isVisibleGeometry);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_FACE,  bin->isSetIsVisibleGeometryFace,   bin->isVisibleGeometryFace);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_LINE,  bin->isSetIsVisibleGeometryLine,   bin->isVisibleGeometryLine);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_NODE,  bin->isSetIsVisibleGeometryNode,   bin->isVisibleGeometryNode);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL,          bin->isSetIsVisibleModel,          bin->isVisibleModel);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_FACE,     bin->isSetIsVisibleModelFace,      bin->isVisibleModelFace);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_LINE,     bin->isSetIsVisibleModelLine,      bin->isVisibleModelLine);
            _WriteB(    gmineInfo, KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_NODE,     bin->isSetIsVisibleModelNode,      bin->isVisibleModelNode);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE,           bin->isSetColorDrillHole,         &bin->colorDrillHole);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_FACE,      bin->isSetColorDrillHoleFace,     &bin->colorDrillHoleFace);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_LINE,      bin->isSetColorDrillHoleLine,     &bin->colorDrillHoleLine);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_NODE,      bin->isSetColorDrillHoleNode,     &bin->colorDrillHoleNode);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY,            bin->isSetColorGeometry,          &bin->colorGeometry);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_FACE,       bin->isSetColorGeometryFace,      &bin->colorGeometryFace);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_LINE,       bin->isSetColorGeometryLine,      &bin->colorGeometryLine);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_NODE,       bin->isSetColorGeometryNode,      &bin->colorGeometryNode);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_MODEL,               bin->isSetColorModel,             &bin->colorModel);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_FACE,          bin->isSetColorModelFace,         &bin->colorModelFace);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_LINE,          bin->isSetColorModelLine,         &bin->colorModelLine);
            _WriteColor(gmineInfo, KEY_BLOCK_ITEM_BIN_COLOR_MODEL_NODE,          bin->isSetColorModelNode,         &bin->colorModelNode);

            returnFalseIf(!_WriteProperty(gmineInfo, &bin->property));

            returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
         }

         returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
      }

      // Write the user key values.
      returnFalseIf(!_WriteKeyValueList(gmineInfo, &item->keyValueArray));

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

      returnFalseIf(!_MiIoWriteBlockStartListItem(gmineInfo, KEY_BLOCK_PROPERTY));

      _WriteStr(gmineInfo, KEY_BLOCK_PROPERTY_KEY,  prop->isSetKey,  prop->key);
      _WriteStr(gmineInfo, KEY_BLOCK_PROPERTY_NAME, prop->isSetName, prop->name);

      returnFalseIf(!_WriteProperty(gmineInfo, prop));

      returnFalseIf(!_WriteKeyValueList(gmineInfo, &prop->keyValueArray));

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

   return _MiIoWriteBlockStartList(gmineInfo, KEY_BLOCK_IMAGE_LIST);
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

   return _MiIoWriteBlockStartList(gmineInfo, KEY_BLOCK_ITEM_LIST);
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

   return _MiIoWriteBlockStartList(gmineInfo, KEY_BLOCK_PROPERTY_LIST);
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
func: _WriteB
**************************************************************************************************/
static Gb _WriteB(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, Gb const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteB(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteColor
**************************************************************************************************/
static Gb _WriteColor(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, GmineInfoColor const * const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteColor(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteI
**************************************************************************************************/
static Gb _WriteI(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, Gi8 const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteI(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteItemType
**************************************************************************************************/
static Gb _WriteItemType(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, GmineInfoItemType const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteStr(gmineInfo, key, _itemTypeStr[value]));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteKeyValueList
**************************************************************************************************/
static Gb _WriteKeyValueList(GmineInfo * const gmineInfo, GmineInfoArray * const keyValueArray)
{
   Gindex             index;
   Gcount             count;
   GmineInfoKeyValue *kv;

   // Write out the user key values.
   count = gmineInfoArrayGetCount(keyValueArray);
   forCount(index, count)
   {
      kv = (GmineInfoKeyValue *) gmineInfoArrayGetAt(keyValueArray, index);
      returnFalseIf(!_MiIoWriteStr(gmineInfo, kv->key, kv->value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteN
**************************************************************************************************/
static Gb _WriteN(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, Gn8 const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteN(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteProperty
**************************************************************************************************/
static Gb _WriteProperty(GmineInfo * const gmineInfo, GmineInfoProperty const * const prop)
{
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_ARROW_SIZE_RELATIVE,      prop->isSetIsArrowSizeRelative,       prop->isArrowSizeRelative);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_CLIPPABLE,                prop->isSetIsClippable,               prop->isClippable);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_FONT_BOLD,                prop->isSetIsFontBold,                prop->isFontBold);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_FONT_ITALIC,              prop->isSetIsFontItalic,              prop->isFontItalic);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_FONT_SIZE_RELATIVE,       prop->isSetIsFontSizeRelative,        prop->isFontSizeRelative);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_FONT_STRIKE_OUT,          prop->isSetIsFontStrikeOut,           prop->isFontStrikeOut);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_FONT_UNDERLINE,           prop->isSetIsFontUnderline,           prop->isFontUnderline);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_SELECTABLE,               prop->isSetIsSelectable,              prop->isSelectable);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE,                  prop->isSetIsVisible,                 prop->isVisible);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POINT,            prop->isSetIsVisiblePoint,            prop->isVisiblePoint);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE,         prop->isSetIsVisiblePolyline,         prop->isVisiblePolyline);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_ARROW,   prop->isSetIsVisiblePolylineArrow,    prop->isVisiblePolylineArrow);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_FACE,    prop->isSetIsVisiblePolylineFace,     prop->isVisiblePolylineFace);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_LINE,    prop->isSetIsVisiblePolylineLine,     prop->isVisiblePolylineLine);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_NODE,    prop->isSetIsVisiblePolylineNode,     prop->isVisiblePolylineNode);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE,          prop->isSetIsVisibleSurface,          prop->isVisibleSurface);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_FACE,     prop->isSetIsVisibleSurfaceFace,      prop->isVisibleSurfaceFace);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_LINE,     prop->isSetIsVisibleSurfaceLine,      prop->isVisibleSurfaceLine);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_NODE,     prop->isSetIsVisibleSurfaceNode,      prop->isVisibleSurfaceNode);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_TEXT,             prop->isSetIsVisibleText,             prop->isVisibleText);
   _WriteI(    gmineInfo, KEY_BLOCK_PROPERTY_ARROW_POSITION,              prop->isSetArrowPosition,             prop->arrowPosition);
   _WriteR(    gmineInfo, KEY_BLOCK_PROPERTY_ARROW_SIZE,                  prop->isSetArrowSize,                 prop->arrowSize);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR,                       prop->isSetColor,                    &prop->color);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POINT,                 prop->isSetColorPoint,               &prop->colorPoint);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE,              prop->isSetColorPolyline,            &prop->colorPolyline);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_ARROW,        prop->isSetColorPolylineArrow,       &prop->colorPolylineArrow);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE,         prop->isSetColorPolylineFace,        &prop->colorPolylineFace);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE_PATTERN, prop->isSetColorPolylineFacePattern, &prop->colorPolylineFacePattern);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_LINE,         prop->isSetColorPolylineLine,        &prop->colorPolylineLine);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_POLYLINE_NODE,         prop->isSetColorPolylineNode,        &prop->colorPolylineNode);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE,          prop->isSetColorSurfaceFace,         &prop->colorSurfaceFace);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE_PATTERN,  prop->isSetColorSurfaceFacePattern,  &prop->colorSurfaceFacePattern);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_SURFACE_LINE,          prop->isSetColorSurfaceLine,         &prop->colorSurfaceLine);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_SURFACE_NODE,          prop->isSetColorSurfaceNode,         &prop->colorSurfaceNode);
   _WriteColor(gmineInfo, KEY_BLOCK_PROPERTY_COLOR_TEXT,                  prop->isSetColorText,                &prop->colorText);
   _WriteR(    gmineInfo, KEY_BLOCK_PROPERTY_FACE_TRANSPARENCY,           prop->isSetFaceTransparency,          prop->faceTransparency);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_FONT_NAME,                   prop->isSetFontName,                  prop->fontName);
   _WriteR(    gmineInfo, KEY_BLOCK_PROPERTY_FONT_SIZE,                   prop->isSetFontSize,                  prop->fontSize);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_PATTERN_ARROW,               prop->isSetPatternArrow,              prop->patternArrow);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_PATTERN_FACE,                prop->isSetPatternFace,               prop->patternFace);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_PATTERN_LINE,                prop->isSetPatternLine,               prop->patternLine);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_PATTERN_NODE,                prop->isSetPatternNode,               prop->patternNode);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_01,    prop->isSetIsVisibleLabelData[ 0],    prop->isVisibleLabelData[ 0]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_02,    prop->isSetIsVisibleLabelData[ 1],    prop->isVisibleLabelData[ 1]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_03,    prop->isSetIsVisibleLabelData[ 2],    prop->isVisibleLabelData[ 2]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_04,    prop->isSetIsVisibleLabelData[ 3],    prop->isVisibleLabelData[ 3]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_05,    prop->isSetIsVisibleLabelData[ 4],    prop->isVisibleLabelData[ 4]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_06,    prop->isSetIsVisibleLabelData[ 5],    prop->isVisibleLabelData[ 5]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_07,    prop->isSetIsVisibleLabelData[ 6],    prop->isVisibleLabelData[ 6]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_08,    prop->isSetIsVisibleLabelData[ 7],    prop->isVisibleLabelData[ 7]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_09,    prop->isSetIsVisibleLabelData[ 8],    prop->isVisibleLabelData[ 8]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_10,    prop->isSetIsVisibleLabelData[ 9],    prop->isVisibleLabelData[ 9]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_01,    prop->isSetIsVisibleLabelLine[ 0],    prop->isVisibleLabelLine[ 0]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_02,    prop->isSetIsVisibleLabelLine[ 1],    prop->isVisibleLabelLine[ 1]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_03,    prop->isSetIsVisibleLabelLine[ 2],    prop->isVisibleLabelLine[ 2]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_04,    prop->isSetIsVisibleLabelLine[ 3],    prop->isVisibleLabelLine[ 3]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_05,    prop->isSetIsVisibleLabelLine[ 4],    prop->isVisibleLabelLine[ 4]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_06,    prop->isSetIsVisibleLabelLine[ 5],    prop->isVisibleLabelLine[ 5]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_07,    prop->isSetIsVisibleLabelLine[ 6],    prop->isVisibleLabelLine[ 6]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_08,    prop->isSetIsVisibleLabelLine[ 7],    prop->isVisibleLabelLine[ 7]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_09,    prop->isSetIsVisibleLabelLine[ 8],    prop->isVisibleLabelLine[ 8]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_10,    prop->isSetIsVisibleLabelLine[ 9],    prop->isVisibleLabelLine[ 9]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_01,    prop->isSetIsVisibleLabelNode[ 0],    prop->isVisibleLabelNode[ 0]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_02,    prop->isSetIsVisibleLabelNode[ 1],    prop->isVisibleLabelNode[ 1]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_03,    prop->isSetIsVisibleLabelNode[ 2],    prop->isVisibleLabelNode[ 2]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_04,    prop->isSetIsVisibleLabelNode[ 3],    prop->isVisibleLabelNode[ 3]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_05,    prop->isSetIsVisibleLabelNode[ 4],    prop->isVisibleLabelNode[ 4]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_06,    prop->isSetIsVisibleLabelNode[ 5],    prop->isVisibleLabelNode[ 5]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_07,    prop->isSetIsVisibleLabelNode[ 6],    prop->isVisibleLabelNode[ 6]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_08,    prop->isSetIsVisibleLabelNode[ 7],    prop->isVisibleLabelNode[ 7]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_09,    prop->isSetIsVisibleLabelNode[ 8],    prop->isVisibleLabelNode[ 8]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_10,    prop->isSetIsVisibleLabelNode[ 9],    prop->isVisibleLabelNode[ 9]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_11,    prop->isSetIsVisibleLabelNode[10],    prop->isVisibleLabelNode[10]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_12,    prop->isSetIsVisibleLabelNode[11],    prop->isVisibleLabelNode[11]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_13,    prop->isSetIsVisibleLabelNode[12],    prop->isVisibleLabelNode[12]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_14,    prop->isSetIsVisibleLabelNode[13],    prop->isVisibleLabelNode[13]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_15,    prop->isSetIsVisibleLabelNode[14],    prop->isVisibleLabelNode[14]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_16,    prop->isSetIsVisibleLabelNode[15],    prop->isVisibleLabelNode[15]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_17,    prop->isSetIsVisibleLabelNode[16],    prop->isVisibleLabelNode[16]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_18,    prop->isSetIsVisibleLabelNode[17],    prop->isVisibleLabelNode[17]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_19,    prop->isSetIsVisibleLabelNode[18],    prop->isVisibleLabelNode[18]);
   _WriteB(    gmineInfo, KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_20,    prop->isSetIsVisibleLabelNode[19],    prop->isVisibleLabelNode[19]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_01,               prop->isSetLabelData[ 0],             prop->labelData[ 0]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_02,               prop->isSetLabelData[ 1],             prop->labelData[ 1]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_03,               prop->isSetLabelData[ 2],             prop->labelData[ 2]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_04,               prop->isSetLabelData[ 3],             prop->labelData[ 3]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_05,               prop->isSetLabelData[ 4],             prop->labelData[ 4]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_06,               prop->isSetLabelData[ 5],             prop->labelData[ 5]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_07,               prop->isSetLabelData[ 6],             prop->labelData[ 6]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_08,               prop->isSetLabelData[ 7],             prop->labelData[ 7]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_09,               prop->isSetLabelData[ 8],             prop->labelData[ 8]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_DATA_LABEL_10,               prop->isSetLabelData[ 9],             prop->labelData[ 9]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_01,               prop->isSetLabelLine[ 0],             prop->labelLine[ 0]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_02,               prop->isSetLabelLine[ 1],             prop->labelLine[ 1]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_03,               prop->isSetLabelLine[ 2],             prop->labelLine[ 2]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_04,               prop->isSetLabelLine[ 3],             prop->labelLine[ 3]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_05,               prop->isSetLabelLine[ 4],             prop->labelLine[ 4]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_06,               prop->isSetLabelLine[ 5],             prop->labelLine[ 5]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_07,               prop->isSetLabelLine[ 6],             prop->labelLine[ 6]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_08,               prop->isSetLabelLine[ 7],             prop->labelLine[ 7]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_09,               prop->isSetLabelLine[ 8],             prop->labelLine[ 8]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_LINE_LABEL_10,               prop->isSetLabelLine[ 9],             prop->labelLine[ 9]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_01,               prop->isSetLabelNode[ 0],             prop->labelNode[ 0]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_02,               prop->isSetLabelNode[ 1],             prop->labelNode[ 1]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_03,               prop->isSetLabelNode[ 2],             prop->labelNode[ 2]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_04,               prop->isSetLabelNode[ 3],             prop->labelNode[ 3]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_05,               prop->isSetLabelNode[ 4],             prop->labelNode[ 4]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_06,               prop->isSetLabelNode[ 5],             prop->labelNode[ 5]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_07,               prop->isSetLabelNode[ 6],             prop->labelNode[ 6]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_08,               prop->isSetLabelNode[ 7],             prop->labelNode[ 7]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_09,               prop->isSetLabelNode[ 8],             prop->labelNode[ 8]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_10,               prop->isSetLabelNode[ 9],             prop->labelNode[ 9]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_11,               prop->isSetLabelNode[10],             prop->labelNode[10]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_12,               prop->isSetLabelNode[11],             prop->labelNode[11]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_13,               prop->isSetLabelNode[12],             prop->labelNode[12]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_14,               prop->isSetLabelNode[13],             prop->labelNode[13]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_15,               prop->isSetLabelNode[14],             prop->labelNode[14]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_16,               prop->isSetLabelNode[15],             prop->labelNode[15]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_17,               prop->isSetLabelNode[16],             prop->labelNode[16]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_18,               prop->isSetLabelNode[17],             prop->labelNode[17]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_19,               prop->isSetLabelNode[18],             prop->labelNode[18]);
   _WriteStr(  gmineInfo, KEY_BLOCK_PROPERTY_NODE_LABEL_20,               prop->isSetLabelNode[19],             prop->labelNode[19]);

   returnTrue;
}

/**************************************************************************************************
func: _WritePoint
**************************************************************************************************/
static Gb _WritePoint(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, GmineInfoPoint const * const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWritePoint(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteR
**************************************************************************************************/
static Gb _WriteR(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, Gr8 const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteR(gmineInfo, key, value));
   }

   returnTrue;
}

/**************************************************************************************************
func: _WriteStr
**************************************************************************************************/
static Gb _WriteStr(GmineInfo * const gmineInfo, char const * const key, Gb const isSet, Gstr const * const value)
{
   if (isSet)
   {
      returnFalseIf(!_MiIoWriteStr(gmineInfo, key, value));
   }

   returnTrue;
}
