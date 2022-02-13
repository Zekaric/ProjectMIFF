/******************************************************************************
file:       MiffLib
author:     Robbert de Groot
company:
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw MIFF files.
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) 2021, Robbert de Groot

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
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "mifflib.h"
#include "local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static MiffBool    _isStarted = miffBoolFALSE;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc,
   MiffC2 * const subFormatName, MiffN8 * const subFormatVersion, void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = _MiffMemCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the miff structure.
   if (!miffCreateReaderContent(
         miff,
         isByteSwaping,
         getBufferFunc,
         subFormatName,
         subFormatVersion,
         dataRepo))
   {
      _MiffMemDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateReaderContent
******************************************************************************/
MiffBool miffCreateReaderContent(Miff * const miff, MiffBool const isByteSwaping,
   MiffGetBuffer getBufferFunc, MiffC2 * const subFormatName, MiffN8 * const subFormatVersion,
   void * const dataRepo)
{
   MiffN1 ntemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   _MiffMemClearType(Miff, miff);
   miff->method               = miffMethodREADING;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->getBuffer            = getBufferFunc;
   miff->readByteCountActual  = 1024;
   miff->readStrCountActual   = 1024;
   miff->readByteCount        = 0;
   miff->readStrCount         = 0;
   miff->readByteData         = _MiffMemCreateTypeArray(1024, MiffN1);
   miff->readStrData          = _MiffMemCreateTypeArray(1024, MiffC2);
   returnFalseIf(!miff->readByteData);
   returnFalseIf(!miff->readStrData);

   // Read the header.
   returnFalseIf(!_MiffReadLine(miff));
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      4, 
      (MiffN1 *) MIFF_HEADER_FILETYPE_STR));

   returnFalseIf(!_MiffReadLine(miff));
   miff->version = _MiffC1ToN(miff->readByteCount, (MiffC1 *) miff->readByteData);
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      1, 
      (MiffN1 *) MIFF_HEADER_VERSION_STR));

   returnFalseIf(!_MiffReadLine(miff));
   _MiffC1ToC2Key(  miff->readByteCount, (MiffC1 *) miff->readByteData, &ntemp, miff->subFormatNameC2);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffC2, subFormatName, miff->subFormatNameC2);
   returnFalseIf(!_MiffReadLine(miff));
   miff->subFormatVersion =
      *subFormatVersion   = _MiffC1ToN(miff->readByteCount, miff->readByteData);

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc,
   MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure
   miff = _MiffMemCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the structure
   if (!miffCreateWriterContent(
         miff,
         isByteSwaping,
         setBufferFunc,
         subFormatName,
         subFormatVersion,
         dataRepo))
   {
      _MiffMemDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateWriterContent
******************************************************************************/
MiffBool miffCreateWriterContent(Miff * const miff, MiffBool const isByteSwaping,
   MiffSetBuffer setBufferFunc, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion,
   void * const dataRepo)
{
   MiffN4 count;

   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBufferFunc  ||
      !subFormatName  ||
      !subFormatVersion);

   _MiffMemClearType(Miff, miff);
   miff->version              = 1;
   miff->method               = miffMethodWRITING;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->setBuffer            = setBufferFunc;
   miff->subFormatVersion     = subFormatVersion;

   count = min(255, _MiffC2GetCount(subFormatName));
   _MiffMemCopyTypeArray(count, MiffC2, miff->subFormatNameC2, subFormatName);

   // Write the miff header.
   _MiffWriteC1(        miff, (MiffC1 *) MIFF_HEADER_FILETYPE_STR);
   miffRecordSetEnd(miff);
   _MiffWriteC1(        miff, (MiffC1 *) MIFF_HEADER_VERSION_STR);
   miffRecordSetEnd(miff);

   _MiffWriteC2(        miff, miff->subFormatNameC2);
   miffRecordSetEnd(miff);
   _MiffWriteN(         miff, miff->subFormatVersion);
   miffRecordSetEnd(miff);

   returnTrue;
}

/******************************************************************************
func: miffDestroy
******************************************************************************/
void miffDestroy(Miff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   miffDestroyContent(miff);
}

/******************************************************************************
func: miffDestroyContent
******************************************************************************/
void miffDestroyContent(Miff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   _MiffMemDestroy(miff->readByteData);
   _MiffMemDestroy(miff->readStrData);
   _MiffMemDestroy(miff);
}

/******************************************************************************
func: miffGetValueBoolean
******************************************************************************/
MiffBool miffGetValueBoolean(Miff * const miff, MiffBool * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeBOOLEAN  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   *value = miffBoolFALSE;
   if (miff->readByteData[0] == 'T')
   {
      *value = miffBoolTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: miffGetValueABI
******************************************************************************/
MiffBool miffGetValueABI(Miff * const miff, MiffABI8   * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABI1     ||
        miff->currentRecord.type == miffTypeABI2     ||
        miff->currentRecord.type == miffTypeABI4     ||
        miff->currentRecord.type == miffTypeABI8     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABN
******************************************************************************/
MiffBool miffGetValueABN(Miff * const miff, MiffABN8   * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABN1     ||
        miff->currentRecord.type == miffTypeABN2     ||
        miff->currentRecord.type == miffTypeABN4     ||
        miff->currentRecord.type == miffTypeABN8     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABR4
******************************************************************************/
MiffBool miffGetValueABR4(Miff * const miff, MiffABR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABR4     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->a));
   returnFalseIf(!_MiffReadR4(miff, &value->b));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABR4S
******************************************************************************/
MiffBool miffGetValueABR4S(Miff * const miff, MiffABR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABR4S    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->a));
   returnFalseIf(!_MiffReadR4S(miff, &value->b));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABR8
******************************************************************************/
MiffBool miffGetValueABR8(Miff * const miff, MiffABR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABR8     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->a));
   returnFalseIf(!_MiffReadR8(miff, &value->b));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABR8S
******************************************************************************/
MiffBool miffGetValueABR8S(Miff * const miff, MiffABR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABR8S    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->a));
   returnFalseIf(!_MiffReadR8S(miff, &value->b));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCI
******************************************************************************/
MiffBool miffGetValueABCI(Miff * const miff, MiffABCI8  * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCI1    ||
        miff->currentRecord.type == miffTypeABCI2    ||
        miff->currentRecord.type == miffTypeABCI4    ||
        miff->currentRecord.type == miffTypeABCI8    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->c = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABCN
******************************************************************************/
MiffBool miffGetValueABCN(Miff * const miff, MiffABCN8  * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCN1    ||
        miff->currentRecord.type == miffTypeABCN2    ||
        miff->currentRecord.type == miffTypeABCN4    ||
        miff->currentRecord.type == miffTypeABCN8    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->c = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABCR4
******************************************************************************/
MiffBool miffGetValueABCR4(Miff * const miff, MiffABCR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCR4    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->a));
   returnFalseIf(!_MiffReadR4(miff, &value->b));
   returnFalseIf(!_MiffReadR4(miff, &value->c));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCR4S
******************************************************************************/
MiffBool miffGetValueABCR4S(Miff * const miff, MiffABCR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCR4S   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->a));
   returnFalseIf(!_MiffReadR4S(miff, &value->b));
   returnFalseIf(!_MiffReadR4S(miff, &value->c));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCR8
******************************************************************************/
MiffBool miffGetValueABCR8(Miff * const miff, MiffABCR8  * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCR8    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->a));
   returnFalseIf(!_MiffReadR8(miff, &value->b));
   returnFalseIf(!_MiffReadR8(miff, &value->c));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCR8S
******************************************************************************/
MiffBool miffGetValueABCR8S(Miff * const miff, MiffABCR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCR8S   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->a));
   returnFalseIf(!_MiffReadR8S(miff, &value->b));
   returnFalseIf(!_MiffReadR8S(miff, &value->c));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDI
******************************************************************************/
MiffBool miffGetValueABCDI(Miff * const miff, MiffABCDI8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDI1   ||
        miff->currentRecord.type == miffTypeABCDI2   ||
        miff->currentRecord.type == miffTypeABCDI4   ||
        miff->currentRecord.type == miffTypeABCDI8   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->c = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->d = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDN
******************************************************************************/
MiffBool miffGetValueABCDN(Miff * const miff, MiffABCDN8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDN1   ||
        miff->currentRecord.type == miffTypeABCDN2   ||
        miff->currentRecord.type == miffTypeABCDN4   ||
        miff->currentRecord.type == miffTypeABCDN8   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   value->a = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->b = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->c = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnFalseIf(!_MiffReadPart(miff));

   value->d = (MiffI8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDR4
******************************************************************************/
MiffBool miffGetValueABCDR4(Miff * const miff, MiffABCDR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDR4   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->a));
   returnFalseIf(!_MiffReadR4(miff, &value->b));
   returnFalseIf(!_MiffReadR4(miff, &value->c));
   returnFalseIf(!_MiffReadR4(miff, &value->d));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDR4S
******************************************************************************/
MiffBool miffGetValueABCDR4S(Miff * const miff, MiffABCDR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDR4S  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->a));
   returnFalseIf(!_MiffReadR4S(miff, &value->b));
   returnFalseIf(!_MiffReadR4S(miff, &value->c));
   returnFalseIf(!_MiffReadR4S(miff, &value->d));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDR8
******************************************************************************/
MiffBool miffGetValueABCDR8(Miff * const miff, MiffABCDR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDR8   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->a));
   returnFalseIf(!_MiffReadR8(miff, &value->b));
   returnFalseIf(!_MiffReadR8(miff, &value->c));
   returnFalseIf(!_MiffReadR8(miff, &value->d));

   returnTrue;
}

/******************************************************************************
func: miffGetValueABCDR8S
******************************************************************************/
MiffBool miffGetValueABCDR8S(Miff * const miff, MiffABCDR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeABCDR8S  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->a));
   returnFalseIf(!_MiffReadR8S(miff, &value->b));
   returnFalseIf(!_MiffReadR8S(miff, &value->c));
   returnFalseIf(!_MiffReadR8S(miff, &value->d));

   returnTrue;
}

/******************************************************************************
func: miffGetValueI
******************************************************************************/
MiffBool miffGetValueI(Miff * const miff, MiffI8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeI1       ||
        miff->currentRecord.type == miffTypeI2       ||
        miff->currentRecord.type == miffTypeI4       ||
        miff->currentRecord.type == miffTypeI8       ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   *value = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix2x2R4
******************************************************************************/
MiffBool miffGetValueMatrix2x2R4(Miff * const miff, MiffMatrix2x2R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix2x2R4S
******************************************************************************/
MiffBool miffGetValueMatrix2x2R4S(Miff * const miff, MiffMatrix2x2R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix2x2R8
******************************************************************************/
MiffBool miffGetValueMatrix2x2R8(Miff * const miff, MiffMatrix2x2R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix2x2R8S
******************************************************************************/
MiffBool miffGetValueMatrix2x2R8S(Miff * const miff, MiffMatrix2x2R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix3x3R4
******************************************************************************/
MiffBool miffGetValueMatrix3x3R4(Miff * const miff, MiffMatrix3x3R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix3x3R4S
******************************************************************************/
MiffBool miffGetValueMatrix3x3R4S(Miff * const miff, MiffMatrix3x3R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix3x3R8
******************************************************************************/
MiffBool miffGetValueMatrix3x3R8(Miff * const miff, MiffMatrix3x3R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix3x3R8S
******************************************************************************/
MiffBool miffGetValueMatrix3x3R8S(Miff * const miff, MiffMatrix3x3R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix4x4R4
******************************************************************************/
MiffBool miffGetValueMatrix4x4R4(Miff * const miff, MiffMatrix4x4R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[0][3]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[1][3]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[2][3]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[3][0]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[3][1]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[3][2]));
   returnFalseIf(!_MiffReadR4(miff, &value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix4x4R4S
******************************************************************************/
MiffBool miffGetValueMatrix4x4R4S(Miff * const miff, MiffMatrix4x4R4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[0][3]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[1][3]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[2][3]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[3][0]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[3][1]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[3][2]));
   returnFalseIf(!_MiffReadR4S(miff, &value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix4x4R8
******************************************************************************/
MiffBool miffGetValueMatrix4x4R8(Miff * const miff, MiffMatrix4x4R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[0][3]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[1][3]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[2][3]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[3][0]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[3][1]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[3][2]));
   returnFalseIf(!_MiffReadR8(miff, &value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueMatrix4x4R8S
******************************************************************************/
MiffBool miffGetValueMatrix4x4R8S(Miff * const miff, MiffMatrix4x4R8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[0][3]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[1][3]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[2][3]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[3][0]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[3][1]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[3][2]));
   returnFalseIf(!_MiffReadR8S(miff, &value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffGetValueN
******************************************************************************/
MiffBool miffGetValueN(Miff * const miff, MiffN8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeN1       ||
        miff->currentRecord.type == miffTypeN2       ||
        miff->currentRecord.type == miffTypeN4       ||
        miff->currentRecord.type == miffTypeN8       ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   *value = (MiffN8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueR4
******************************************************************************/
MiffBool miffGetValueR4(Miff * const miff, MiffR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR4       ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4(miff, value))

   returnTrue;
}

/******************************************************************************
func: miffGetValueR4S
******************************************************************************/
MiffBool miffGetValueR4S(Miff * const miff, MiffR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR4S      ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR4S(miff, value))

   returnTrue;
}

/******************************************************************************
func: miffGetValueR8
******************************************************************************/
MiffBool miffGetValueR8(Miff * const miff, MiffR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR8       ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8(miff, value));

   returnTrue;
}

/******************************************************************************
func: miffGetValueR8S
******************************************************************************/
MiffBool miffGetValueR8S(Miff * const miff, MiffR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR8S      ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadR8S(miff, value));

   returnTrue;
}

/******************************************************************************
func: miffGetValueStringC2

value is not dynamic.  Caller must clone the string for persistence.
******************************************************************************/
MiffBool miffGetValueStringC2(Miff *const miff, MiffC2 **const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeSTRING   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffReadPart(miff));

   _MiffC1EncodedToC1(&miff->readByteCount, miff->readByteData);

   // Adjust the str buffer.
   if (miff->readByteCount >= miff->readStrCountActual)
   {
      _MiffMemDestroy(miff->readStrData);

      // Get the new buffer size.
      miff->readStrCountActual = ((miff->readByteCount / 1024) + 2) * 1024;
      miff->readStrData        = _MiffMemCreateTypeArray(miff->readStrCountActual, MiffC2);

      // Ran out of memory.
      returnFalseIf(!miff->readStrData);
   }

   // Clear the string.
   _MiffMemClearTypeArray(miff->readStrCountActual, MiffC2, miff->readStrData);

   // Convert the stirng to Unicode.
   _MiffC1ToC2(miff->readByteCount, miff->readByteData, &miff->readStrCount, miff->readStrData);

   // Return string.
   *value = miff->readStrData;

   returnTrue;
}

/******************************************************************************
func: miffRecordGetBegin

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffBool miffRecordGetBegin(Miff * const miff, MiffType * const type, MiffC2 * const typeName,
   MiffC2 * const key, MiffN4 * const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->readRecordIsDone = miffBoolFALSE;

   *type                  = miffTypeNONE;
   *count                 = 0;

   // Clear the key.
   _MiffMemClearTypeArray(miffKeySIZE, MiffC2, key);

   // Read in the type.
   returnFalseIf(!_MiffReadType(miff, &miff->currentRecord.type, typeName));
   *type = miff->currentRecord.type;

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      return miff->readRecordIsDone;
   }

   // Read in the name of the record
   returnFalseIf(!_MiffReadC2Key(miff, miff->currentRecord.nameC2));
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffC2, key, miff->currentRecord.nameC2);

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_START)
   {
      return miff->readRecordIsDone;
   }

   // Read in the array count
   returnFalseIf(!_MiffReadArrayCount(miff, &miff->currentRecord.arrayCount));
   *count = miff->currentRecord.arrayCount;

   returnTrue;
}

/******************************************************************************
func: miffRecordGetEnd
******************************************************************************/
MiffBool miffRecordGetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->readRecordIsDone);

   returnFalseIf(!_MiffReadLineSkip(miff));

   miff->readRecordIsDone = miffBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: miffRecordGetCount
******************************************************************************/
MiffN8 miffRecordGetCount(Miff * const miff)
{
   return0If(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type == miffTypeNONE);

   return miff->currentRecord.arrayCount;
}

/******************************************************************************
func: miffRecordGetName
******************************************************************************/
MiffC2 *miffRecordGetName(Miff * const miff)
{
   returnNullIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type == miffTypeNONE);

   return miff->currentRecord.nameC2;
}

/******************************************************************************
func: miffRecordGetType
******************************************************************************/
MiffType miffRecordGetType(Miff * const miff)
{
   returnIf(
         !_isStarted ||
         !miff,
      miffTypeNONE);

   return miff->currentRecord.type;
}

/******************************************************************************
func: miffRecordSetBegin
******************************************************************************/
MiffBool miffRecordSetBegin(Miff * const miff, MiffType const type, MiffC2 const * const typeName, 
   MiffC2 const * const key, MiffN4 const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Write the type for the record.  Common for all cases.
   if (type == miffTypeUSER_TYPE)
   {
      returnFalseIf(!_MiffWriteC2Key(miff, typeName));
   }
   else
   {
      returnFalseIf(!_MiffWriteC1(   miff, _MiffTypeGetNameC1(type)));
   }

   // We are ending a key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnFalseIf(!miffRecordSetEnd(miff));
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentArrayIndex        = 0;
   miff->currentRecord.type       = type;
   miff->currentRecord.arrayCount = count;
   _MiffMemClearTypeArray(miffKeySIZE,      MiffC2, miff->currentRecord.nameC2);
   _MiffMemCopyTypeArray( _MiffC2GetCount(key), MiffC2, miff->currentRecord.nameC2, key);

   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteC2Key(           miff, miff->currentRecord.nameC2));
   // We are starting a new key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_START)
   {
      // TODO key value block stack push.
      return miffRecordSetEnd(miff);
   }

   // Write out the record.
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, count));
   returnFalseIf(!miffRecordSetSeparator(miff));

   returnTrue;
}

/******************************************************************************
func: miffRecordSetEnd
******************************************************************************/
MiffBool miffRecordSetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->currentRecord.type = miffTypeNONE;

   return _MiffWriteC1(miff, (MiffC1 *) "\n");
}

/******************************************************************************
func: miffRecordSetSeparator
******************************************************************************/
MiffBool miffRecordSetSeparator(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteC1(miff, (MiffC1 *) "\t");
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _MiffMemStart(memCreateFunc, memDestroyFunc);

   _isStarted = miffBoolTRUE;

   _MiffBase64Start();

   returnTrue;
}

/******************************************************************************
func: miffStop
******************************************************************************/
void miffStop(void)
{
   returnVoidIf(!_isStarted);

   _MiffMemStop();

   _isStarted = miffBoolFALSE;
}

/******************************************************************************
func: miffSetBlockStart
******************************************************************************/
MiffBool miffSetBlockStart(Miff * const miff, MiffC2 const * const key)
{
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_START, NULL, key, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffSetBlockStop
******************************************************************************/
MiffBool miffSetBlockStop(Miff * const miff)
{
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_STOP, NULL, NULL, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffSet1ABI1
******************************************************************************/
MiffBool miffSet1ABI1(Miff * const miff, MiffC2 const * const key, MiffABI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI1, NULL, key, 1));
   returnFalseIf(!miffSetValueABI1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI2
******************************************************************************/
MiffBool miffSet1ABI2(Miff * const miff, MiffC2 const * const key, MiffABI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI2, NULL, key, 1));
   returnFalseIf(!miffSetValueABI2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI4
******************************************************************************/
MiffBool miffSet1ABI4(Miff * const miff, MiffC2 const * const key, MiffABI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI4, NULL, key, 1));
   returnFalseIf(!miffSetValueABI4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN1
******************************************************************************/
MiffBool miffSet1ABN1(Miff * const miff, MiffC2 const * const key, MiffABN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN1, NULL, key, 1));
   returnFalseIf(!miffSetValueABN1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN2
******************************************************************************/
MiffBool miffSet1ABN2(Miff * const miff, MiffC2 const * const key, MiffABN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN2, NULL, key, 1));
   returnFalseIf(!miffSetValueABN2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN4
******************************************************************************/
MiffBool miffSet1ABN4(Miff * const miff, MiffC2 const * const key, MiffABN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN4, NULL, key, 1));
   returnFalseIf(!miffSetValueABN4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR4
******************************************************************************/
MiffBool miffSet1ABR4(Miff * const miff, MiffC2 const * const key, MiffABR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4, NULL, key, 1));
   returnFalseIf(!miffSetValueABR4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR4S
******************************************************************************/
MiffBool miffSet1ABR4S(Miff * const miff, MiffC2 const * const key, MiffABR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4S, NULL, key, 1));
   returnFalseIf(!miffSetValueABR4S( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR8
******************************************************************************/
MiffBool miffSet1ABR8(Miff * const miff, MiffC2 const * const key, MiffABR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8, NULL, key, 1));
   returnFalseIf(!miffSetValueABR8(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR8S
******************************************************************************/
MiffBool miffSet1ABR8S(Miff * const miff, MiffC2 const * const key, MiffABR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8S, NULL, key, 1));
   returnFalseIf(!miffSetValueABR8S( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI1
******************************************************************************/
MiffBool miffSet1ABCI1(Miff * const miff, MiffC2 const * const key, MiffABCI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI1, NULL, key, 1));
   returnFalseIf(!miffSetValueABCI1( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI2
******************************************************************************/
MiffBool miffSet1ABCI2(Miff * const miff, MiffC2 const * const key, MiffABCI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI2, NULL, key, 1));
   returnFalseIf(!miffSetValueABCI2( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI4
******************************************************************************/
MiffBool miffSet1ABCI4(Miff * const miff, MiffC2 const * const key, MiffABCI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCI4( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN1
******************************************************************************/
MiffBool miffSet1ABCN1(Miff * const miff, MiffC2 const * const key, MiffABCN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN1, NULL, key, 1));
   returnFalseIf(!miffSetValueABCN1( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN2
******************************************************************************/
MiffBool miffSet1ABCN2(Miff * const miff, MiffC2 const * const key, MiffABCN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN2, NULL, key, 1));
   returnFalseIf(!miffSetValueABCN2( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN4
******************************************************************************/
MiffBool miffSet1ABCN4(Miff * const miff, MiffC2 const * const key, MiffABCN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCN4( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR4
******************************************************************************/
MiffBool miffSet1ABCR4(Miff * const miff, MiffC2 const * const key, MiffABCR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCR4( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR4S
******************************************************************************/
MiffBool miffSet1ABCR4S(Miff * const miff, MiffC2 const * const key, MiffABCR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4S, NULL, key, 1));
   returnFalseIf(!miffSetValueABCR4S(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR8
******************************************************************************/
MiffBool miffSet1ABCR8(Miff * const miff, MiffC2 const * const key, MiffABCR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8, NULL, key, 1));
   returnFalseIf(!miffSetValueABCR8( miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR8S
******************************************************************************/
MiffBool miffSet1ABCR8S(Miff * const miff, MiffC2 const * const key, MiffABCR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8S, NULL, key, 1));
   returnFalseIf(!miffSetValueABCR8S(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI1
******************************************************************************/
MiffBool miffSet1ABCDI1(Miff * const miff, MiffC2 const * const key, MiffABCDI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI1, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDI1(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI2
******************************************************************************/
MiffBool miffSet1ABCDI2(Miff * const miff, MiffC2 const * const key, MiffABCDI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI2, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDI2(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI4
******************************************************************************/
MiffBool miffSet1ABCDI4(Miff * const miff, MiffC2 const * const key, MiffABCDI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDI4(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN1
******************************************************************************/
MiffBool miffSet1ABCDN1(Miff * const miff, MiffC2 const * const key, MiffABCDN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN1, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDN1(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN2
******************************************************************************/
MiffBool miffSet1ABCDN2(Miff * const miff, MiffC2 const * const key, MiffABCDN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN2, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDN2(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN4
******************************************************************************/
MiffBool miffSet1ABCDN4(Miff * const miff, MiffC2 const * const key, MiffABCDN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDN4(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR4
******************************************************************************/
MiffBool miffSet1ABCDR4(Miff * const miff, MiffC2 const * const key, MiffABCDR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR4, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDR4(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR4S
******************************************************************************/
MiffBool miffSet1ABCDR4S(Miff * const miff, MiffC2 const * const key, MiffABCDR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR4S, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDR4S(miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR8
******************************************************************************/
MiffBool miffSet1ABCDR8(Miff * const miff, MiffC2 const * const key, MiffABCDR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR8, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDR8(miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR8S
******************************************************************************/
MiffBool miffSet1ABCDR8S(Miff * const miff, MiffC2 const * const key, MiffABCDR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR8S, NULL, key, 1));
   returnFalseIf(!miffSetValueABCDR8S(miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Boolean
******************************************************************************/
MiffBool miffSet1Boolean(Miff * const miff, MiffC2 const * const key, MiffBool const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeBOOLEAN, NULL, key, 1));
   returnFalseIf(!miffSetValueBoolean(miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I1
******************************************************************************/
MiffBool miffSet1I1(Miff * const miff, MiffC2 const * const key, MiffI1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, NULL, key, 1));
   returnFalseIf(!miffSetValueI(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I2
******************************************************************************/
MiffBool miffSet1I2(Miff * const miff, MiffC2 const * const key, MiffI2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, NULL, key, 1));
   returnFalseIf(!miffSetValueI(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I4
******************************************************************************/
MiffBool miffSet1I4(Miff * const miff, MiffC2 const * const key, MiffI4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, NULL, key, 1));
   returnFalseIf(!miffSetValueI(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I8
******************************************************************************/
MiffBool miffSet1I8(Miff * const miff, MiffC2 const * const key, MiffI8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, NULL, key, 1));
   returnFalseIf(!miffSetValueI(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R4
******************************************************************************/
MiffBool miffSet1Matrix2x2R4(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R4, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R4S
******************************************************************************/
MiffBool miffSet1Matrix2x2R4S(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX2X2R4S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R4S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R8
******************************************************************************/
MiffBool miffSet1Matrix2x2R8(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R8, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R8S
******************************************************************************/
MiffBool miffSet1Matrix2x2R8S(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX2X2R8S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R8S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R4
******************************************************************************/
MiffBool miffSet1Matrix3x3R4(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R4, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R4S
******************************************************************************/
MiffBool miffSet1Matrix3x3R4S(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX3X3R4S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R4S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R8
******************************************************************************/
MiffBool miffSet1Matrix3x3R8(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R8, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R8S
******************************************************************************/
MiffBool miffSet1Matrix3x3R8S(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX3X3R8S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R8S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R4
******************************************************************************/
MiffBool miffSet1Matrix4x4R4(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R4, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R4S
******************************************************************************/
MiffBool miffSet1Matrix4x4R4S(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX4X4R4S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R4S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R8
******************************************************************************/
MiffBool miffSet1Matrix4x4R8(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R8, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R8S
******************************************************************************/
MiffBool miffSet1Matrix4x4R8S(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(      miff, miffTypeMATRIX4X4R8S, NULL, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R8S(miff, value));
   returnFalseIf(!miffRecordSetEnd(        miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N1
******************************************************************************/
MiffBool miffSet1N1(Miff * const miff, MiffC2 const * const key, MiffN1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, NULL, key, 1));
   returnFalseIf(!miffSetValueN(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N2
******************************************************************************/
MiffBool miffSet1N2(Miff * const miff, MiffC2 const * const key, MiffN2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, NULL, key, 1));
   returnFalseIf(!miffSetValueN(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N4
******************************************************************************/
MiffBool miffSet1N4(Miff * const miff, MiffC2 const * const key, MiffN4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, NULL, key, 1));
   returnFalseIf(!miffSetValueN(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N8
******************************************************************************/
MiffBool miffSet1N8(Miff * const miff, MiffC2 const * const key, MiffN8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, NULL, key, 1));
   returnFalseIf(!miffSetValueN(     miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R4
******************************************************************************/
MiffBool miffSet1R4(Miff * const miff, MiffC2 const * const key, MiffR4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, NULL, key, 1));
   returnFalseIf(!miffSetValueR4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R4S
******************************************************************************/
MiffBool miffSet1R4S(Miff * const miff, MiffC2 const * const key, MiffR4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4S, NULL, key, 1));
   returnFalseIf(!miffSetValueR4S(   miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R8
******************************************************************************/
MiffBool miffSet1R8(Miff * const miff, MiffC2 const * const key, MiffR8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, NULL, key, 1));
   returnFalseIf(!miffSetValueR8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R8S
******************************************************************************/
MiffBool miffSet1R8S(Miff * const miff, MiffC2 const * const key, MiffR8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8S, NULL, key, 1));
   returnFalseIf(!miffSetValueR8S(   miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Type
******************************************************************************/
MiffBool miffSet1Type(Miff * const miff, MiffC2 const * const key, MiffType const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, NULL, key, 1));
   returnFalseIf(!miffSetValueType(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1StringC2
******************************************************************************/
MiffBool miffSet1StringC2(Miff * const miff, MiffC2 const * const key, MiffC2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(  miff, miffTypeSTRING, NULL, key, 1));
   returnFalseIf(!miffSetValueStringC2(miff, value));
   returnFalseIf(!miffRecordSetEnd(    miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI1
******************************************************************************/
MiffBool miffSetNABI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABI1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI2
******************************************************************************/
MiffBool miffSetNABI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABI2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI4
******************************************************************************/
MiffBool miffSetNABI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABI4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN1
******************************************************************************/
MiffBool miffSetNABN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABN1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN2
******************************************************************************/
MiffBool miffSetNABN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABN2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN4
******************************************************************************/
MiffBool miffSetNABN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABN4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR4
******************************************************************************/
MiffBool miffSetNABR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABR4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR4S
******************************************************************************/
MiffBool miffSetNABR4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABR4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR8
******************************************************************************/
MiffBool miffSetNABR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABR8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR8S
******************************************************************************/
MiffBool miffSetNABR8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABR8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI1
******************************************************************************/
MiffBool miffSetNABCI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCI1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI2
******************************************************************************/
MiffBool miffSetNABCI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCI2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI4
******************************************************************************/
MiffBool miffSetNABCI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCI4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN1
******************************************************************************/
MiffBool miffSetNABCN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCN1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN2
******************************************************************************/
MiffBool miffSetNABCN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCN2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN4
******************************************************************************/
MiffBool miffSetNABCN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCN4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR4
******************************************************************************/
MiffBool miffSetNABCR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCR4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR4S
******************************************************************************/
MiffBool miffSetNABCR4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCR4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR8
******************************************************************************/
MiffBool miffSetNABCR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCR8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR8S
******************************************************************************/
MiffBool miffSetNABCR8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCR8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI1
******************************************************************************/
MiffBool miffSetNABCDI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDI1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI2
******************************************************************************/
MiffBool miffSetNABCDI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDI2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI4
******************************************************************************/
MiffBool miffSetNABCDI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDI4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN1
******************************************************************************/
MiffBool miffSetNABCDN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDN1(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN2
******************************************************************************/
MiffBool miffSetNABCDN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDN2(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN4
******************************************************************************/
MiffBool miffSetNABCDN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDN4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR4
******************************************************************************/
MiffBool miffSetNABCDR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDR4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR4S
******************************************************************************/
MiffBool miffSetNABCDR4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDR4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR8
******************************************************************************/
MiffBool miffSetNABCDR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDR8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR8S
******************************************************************************/
MiffBool miffSetNABCDR8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueABCDR8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNBoolean
******************************************************************************/
MiffBool miffSetNBoolean(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffBool const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeBOOLEAN, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueBoolean(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI1
******************************************************************************/
MiffBool miffSetNI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueI(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI2
******************************************************************************/
MiffBool miffSetNI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueI(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI4
******************************************************************************/
MiffBool miffSetNI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueI(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI8
******************************************************************************/
MiffBool miffSetNI8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueI(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R4
******************************************************************************/
MiffBool miffSetNMatrix2x2R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix2x2R4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R4S
******************************************************************************/
MiffBool miffSetNMatrix2x2R4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix2x2R4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R8
******************************************************************************/
MiffBool miffSetNMatrix2x2R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix2x2R8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R8S
******************************************************************************/
MiffBool miffSetNMatrix2x2R8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix2x2R8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R4
******************************************************************************/
MiffBool miffSetNMatrix3x3R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix3x3R4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R4S
******************************************************************************/
MiffBool miffSetNMatrix3x3R4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix3x3R4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R8
******************************************************************************/
MiffBool miffSetNMatrix3x3R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix3x3R8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R8S
******************************************************************************/
MiffBool miffSetNMatrix3x3R8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix3x3R8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R4
******************************************************************************/
MiffBool miffSetNMatrix4x4R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix4x4R4(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R4S
******************************************************************************/
MiffBool miffSetNMatrix4x4R4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix4x4R4S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R8
******************************************************************************/
MiffBool miffSetNMatrix4x4R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix4x4R8(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R8S
******************************************************************************/
MiffBool miffSetNMatrix4x4R8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueMatrix4x4R8S(miff, &value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN1
******************************************************************************/
MiffBool miffSetNN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueN(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN2
******************************************************************************/
MiffBool miffSetNN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueN(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN4
******************************************************************************/
MiffBool miffSetNN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueN(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN8
******************************************************************************/
MiffBool miffSetNN8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueN(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR4
******************************************************************************/
MiffBool miffSetNR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueR4(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR4S
******************************************************************************/
MiffBool miffSetNR4S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueR4S(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR8
******************************************************************************/
MiffBool miffSetNR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueR8(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR8S
******************************************************************************/
MiffBool miffSetNR8S(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8S, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueR8S(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNType
******************************************************************************/
MiffBool miffSetNType(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffType const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueType(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNStringC2
******************************************************************************/
MiffBool miffSetNStringC2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffC2 const * const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeSTRING, NULL, key, count));
   forCount(index, count)
   {
      if (index)
      {
         returnFalseIf(!miffRecordSetSeparator(miff));
      }
      returnFalseIf(!miffSetValueStringC2(miff, value[index]));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetValueABI1
******************************************************************************/
MiffBool miffSetValueABI1(Miff * const miff, MiffABI1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI1     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABI2
******************************************************************************/
MiffBool miffSetValueABI2(Miff * const miff, MiffABI2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI2     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABI4
******************************************************************************/
MiffBool miffSetValueABI4(Miff * const miff, MiffABI4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI4     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN1
******************************************************************************/
MiffBool miffSetValueABN1(Miff * const miff, MiffABN1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN1     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN2
******************************************************************************/
MiffBool miffSetValueABN2(Miff * const miff, MiffABN2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN2     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN4
******************************************************************************/
MiffBool miffSetValueABN4(Miff * const miff, MiffABN4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN4     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABR4
******************************************************************************/
MiffBool miffSetValueABR4(Miff * const miff, MiffABR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR4     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABR4S
******************************************************************************/
MiffBool miffSetValueABR4S(Miff * const miff, MiffABR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR4S    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABR8
******************************************************************************/
MiffBool miffSetValueABR8(Miff * const miff, MiffABR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR8     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABR8S
******************************************************************************/
MiffBool miffSetValueABR8S(Miff * const miff, MiffABR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR8S    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->b));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI1
******************************************************************************/
MiffBool miffSetValueABCI1(Miff * const miff, MiffABCI1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI1    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI2
******************************************************************************/
MiffBool miffSetValueABCI2(Miff * const miff, MiffABCI2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI2    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI4
******************************************************************************/
MiffBool miffSetValueABCI4(Miff * const miff, MiffABCI4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI4    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN1
******************************************************************************/
MiffBool miffSetValueABCN1(Miff * const miff, MiffABCN1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN1    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN2
******************************************************************************/
MiffBool miffSetValueABCN2(Miff * const miff, MiffABCN2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN2    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN4
******************************************************************************/
MiffBool miffSetValueABCN4(Miff * const miff, MiffABCN4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN4    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR4
******************************************************************************/
MiffBool miffSetValueABCR4(Miff * const miff, MiffABCR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR4    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR4S
******************************************************************************/
MiffBool miffSetValueABCR4S(Miff * const miff, MiffABCR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR4S   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR8
******************************************************************************/
MiffBool miffSetValueABCR8(Miff * const miff, MiffABCR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR8    ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR8S
******************************************************************************/
MiffBool miffSetValueABCR8S(Miff * const miff, MiffABCR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR8S   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->c));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI1
******************************************************************************/
MiffBool miffSetValueABCDI1(Miff * const miff, MiffABCDI1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI1   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI2
******************************************************************************/
MiffBool miffSetValueABCDI2(Miff * const miff, MiffABCDI2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI2   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI4
******************************************************************************/
MiffBool miffSetValueABCDI4(Miff * const miff, MiffABCDI4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI4   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteI(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteI(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN1
******************************************************************************/
MiffBool miffSetValueABCDN1(Miff * const miff, MiffABCDN1 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN1   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN2
******************************************************************************/
MiffBool miffSetValueABCDN2(Miff * const miff, MiffABCDN2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN2   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN4
******************************************************************************/
MiffBool miffSetValueABCDN4(Miff * const miff, MiffABCDN4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN4   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteN(               miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(               miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR4
******************************************************************************/
MiffBool miffSetValueABCDR4(Miff * const miff, MiffABCDR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR4   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR4S
******************************************************************************/
MiffBool miffSetValueABCDR4S(Miff * const miff, MiffABCDR4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR4S  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR8
******************************************************************************/
MiffBool miffSetValueABCDR8(Miff * const miff, MiffABCDR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR8   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR8S
******************************************************************************/
MiffBool miffSetValueABCDR8S(Miff * const miff, MiffABCDR8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR8S  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8S(             miff, value->a));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->b));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->c));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->d));

   returnTrue;
}

/******************************************************************************
func: miffSetValueBoolean
******************************************************************************/
MiffBool miffSetValueBoolean(Miff * const miff, MiffBool const value)
{
   MiffC1 *c1;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeBOOLEAN  ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   c1 = (MiffC1 *) (value ? "T" : "F");

   returnFalseIf(!_MiffWriteC1(miff, c1));

   returnTrue;
}

/******************************************************************************
func: miffSetValueI
******************************************************************************/
MiffBool miffSetValueI(Miff * const miff, MiffI8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff)

   return _MiffWriteI(miff, value);
}

/******************************************************************************
func: miffSetValueMatrix2x2R4
******************************************************************************/
MiffBool miffSetValueMatrix2x2R4(Miff * const miff, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix2x2R4S
******************************************************************************/
MiffBool miffSetValueMatrix2x2R4S(Miff * const miff, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix2x2R8
******************************************************************************/
MiffBool miffSetValueMatrix2x2R8(Miff * const miff, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix2x2R8S
******************************************************************************/
MiffBool miffSetValueMatrix2x2R8S(Miff * const miff, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));
   
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][1]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R4
******************************************************************************/
MiffBool miffSetValueMatrix3x3R4(Miff * const miff, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R4S
******************************************************************************/
MiffBool miffSetValueMatrix3x3R4S(Miff * const miff, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R8
******************************************************************************/
MiffBool miffSetValueMatrix3x3R8(Miff * const miff, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R8S
******************************************************************************/
MiffBool miffSetValueMatrix3x3R8S(Miff * const miff, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][2]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R4
******************************************************************************/
MiffBool miffSetValueMatrix4x4R4(Miff * const miff, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R4 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[0][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[1][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[2][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[3][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[3][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[3][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4(              miff, value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R4S
******************************************************************************/
MiffBool miffSetValueMatrix4x4R4S(Miff * const miff, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R4S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[0][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[1][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[2][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[3][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[3][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[3][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR4S(             miff, value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R8
******************************************************************************/
MiffBool miffSetValueMatrix4x4R8(Miff * const miff, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R8 ||
        miff->currentRecord.type == miffTypeVARIABLE    ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[0][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[1][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[2][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[3][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[3][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[3][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8(              miff, value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R8S
******************************************************************************/
MiffBool miffSetValueMatrix4x4R8S(Miff * const miff, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R8S ||
        miff->currentRecord.type == miffTypeVARIABLE     ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[0][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[1][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[2][3]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[3][0]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[3][1]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[3][2]));
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteR8S(             miff, value->cell[3][3]));

   returnTrue;
}

/******************************************************************************
func: miffSetValueN
******************************************************************************/
MiffBool miffSetValueN(Miff * const miff, MiffN8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteN(miff, value);
}

/******************************************************************************
func: miffSetValueR4
******************************************************************************/
MiffBool miffSetValueR4(Miff * const miff, MiffR4 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteR4(miff, value);
}

/******************************************************************************
func: miffSetValueR4S
******************************************************************************/
MiffBool miffSetValueR4S(Miff * const miff, MiffR4 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteR4S(miff, value);
}

/******************************************************************************
func: miffSetValueR8
******************************************************************************/
MiffBool miffSetValueR8(Miff * const miff, MiffR8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteR8(miff, value);
}

/******************************************************************************
func: miffSetValueR8S
******************************************************************************/
MiffBool miffSetValueR8S(Miff * const miff, MiffR8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteR8S(miff, value);
}

/******************************************************************************
func: miffSetValueStringC2
******************************************************************************/
MiffBool miffSetValueStringC2(Miff * const miff, MiffC2 const * const value)
{
   MiffC1   *c1,
            *c1e;
   MiffN4    c1Count,
             c1eCount;
   MiffBool  result;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeSTRING   ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   c1     =
      c1e = NULL;
   result = miffBoolFALSE;
   once
   {
      breakIf(!_MiffC2ToC1(_MiffC2GetCount(value), value, &c1Count, &c1));

      breakIf(!_MiffC1ToC1Encoded(_MiffC1GetCount(c1), c1, &c1eCount, &c1e));

      breakIf(!_MiffWriteC1(miff, c1e));

      result = miffBoolTRUE;
   }

   _MiffMemDestroy(c1e);
   _MiffMemDestroy(c1);

   returnTrue;
}

/******************************************************************************
func: miffSetValueType
******************************************************************************/
MiffBool miffSetValueType(Miff * const miff, MiffType const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeTYPE     ||
        miff->currentRecord.type == miffTypeVARIABLE ||
        miff->currentRecord.type == miffTypeUSER_TYPE));

   return _MiffWriteC1(miff, _MiffTypeGetNameC1(value));
}

/******************************************************************************
func: miffTypeGetC1
******************************************************************************/
MiffC1 const *miffTypeGetC1(MiffType const type)
{
   return _MiffTypeGetNameC1(type);
}

/******************************************************************************
func: miffTypeGetC2
******************************************************************************/
MiffC2 const *miffTypeGetC2(MiffType const type)
{
   return _MiffTypeGetNameC2(type);
}
