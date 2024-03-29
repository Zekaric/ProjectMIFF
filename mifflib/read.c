/******************************************************************************
file:       write
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
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
#include "local.h"

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffReadLine
******************************************************************************/
MiffBool _MiffReadLine(Miff * const miff)
{
   MiffN4  index;
   MiffN1  byte;
   MiffN1 *bufTemp;

   returnFalseIf(!miff);

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (miff->readByteCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _MiffMemCreateTypeArray(miff->readByteCountActual * 2, MiffN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _MiffMemCopyTypeArray(miff->readByteCountActual, MiffN1, bufTemp, miff->readByteData);
         // Destroy the old buffer.
         _MiffMemDestroy(miff->readByteData);

         // Reset the internal buffer to the new larger buffer.
         miff->readByteCountActual = miff->readByteCountActual * 2;
         miff->readByteData        = bufTemp;
      }

      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, &byte));
      // End of line, we done reading.
      breakIf(byte == '\n');

      // Add the letter to the byte array.
      miff->readByteData[index++] = byte;
   }

   // Need NULL terminator.
   miff->readByteCount       = index;
   miff->readByteData[index] = 0;

   returnTrue;
}

/******************************************************************************
func: _MiffReadLineSkip
******************************************************************************/
MiffBool _MiffReadLineSkip(Miff * const miff)
{
   MiffN1 byte;

   returnFalseIf(!miff);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadPart
******************************************************************************/
MiffBool _MiffReadPart(Miff * const miff)
{
   MiffN4  index;
   MiffN1  byte;
   MiffN1 *bufTemp;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->readRecordIsDone);

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (miff->readByteCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _MiffMemCreateTypeArray(miff->readByteCountActual * 2, MiffN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _MiffMemCopyTypeArray(miff->readByteCountActual, MiffN1, bufTemp, miff->readByteData);
         // Destroy the old buffer.
         _MiffMemDestroy(miff->readByteData);

         // Reset the internal buffer to the new larger buffer.
         miff->readByteCountActual = miff->readByteCountActual * 2;
         miff->readByteData        = bufTemp;
      }

      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, &byte));
      // End of line or part, we done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      miff->readByteData[index++] = byte;
   }

   // Need NULL terminator.
   miff->readByteCount       = index;
   miff->readByteData[index] = 0;

   if (byte == '\n')
   {
      miff->readRecordIsDone = miffBoolTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadArrayCount
******************************************************************************/
MiffBool _MiffReadArrayCount(Miff * const miff, MiffN4 * const count)
{
   returnFalseIf(!count);

   *count = 0;

   returnFalseIf(!miff);

   returnFalseIf(!_MiffReadPart(miff));

   if (miff->readByteData[0] == L'*')
   {
      miff->currentRecord.arrayCount = miffArrayCountUNKNOWN;
      returnTrue;
   }

   miff->currentRecord.arrayCount = (MiffN4) _MiffC1ToN(miff->readByteCount, (MiffC1 *) miff->readByteData);

   *count = miff->currentRecord.arrayCount;

   returnTrue;
}

/******************************************************************************
func: _MiffReadC2Key
******************************************************************************/
MiffBool _MiffReadC2Key(Miff * const miff, MiffC2 * const key)
{
   MiffN1 keySize;

   returnFalseIf(!key);

   _MiffMemClearTypeArray(miffKeySIZE, MiffC2, key);

   returnFalseIf(!miff);

   returnFalseIf(!_MiffReadPart(miff));

   _MiffMemClearTypeArray(miffKeySIZE, MiffC2, miff->currentRecord.nameC2);
   _MiffC1ToC2Key(
      miff->readByteCount,
      (MiffC1 *) miff->readByteData,
      &keySize,
      miff->currentRecord.nameC2);

   _MiffMemCopyTypeArray(miffKeySIZE, MiffC2, key, miff->currentRecord.nameC2);

   returnTrue;
}

/******************************************************************************
func: _MiffReadType
******************************************************************************/
MiffBool _MiffReadType(Miff * const miff, MiffType * const type, MiffC2 * const typeName)
{
   MiffN4 index;
   MiffN1 count;

   returnFalseIf(!type);

   *type = miffTypeNONE;

   returnFalseIf(!miff);

   returnFalseIf(!_MiffReadPart(miff));

   returnFalseIf(miff->readByteCount == 0);

   // Get the name of the type just in case it is a user type.
   _MiffC1ToC2Key(miff->readByteCount, miff->readByteData, &count, typeName);

   forCount (index, miffTypeCOUNT)
   {
      continueIf(_MiffTypeGetNameSize(index) == 0);

      if (_MiffMemIsEqual(
            _MiffTypeGetNameSize(index), (MiffN1 *) _MiffTypeGetNameC1(index),
            miff->readByteCount,     miff->readByteData))
      {
         *type = index;

         returnTrue;
      }
   }

   *type = miffTypeUSER_TYPE;

   returnTrue;
}

/******************************************************************************
func: _MiffReadR4
******************************************************************************/
MiffBool _MiffReadR4(Miff * const miff, MiffR4 * const value)
{
   Miff4      vtemp;
   MiffBase64Data data;

   returnFalseIf(!_MiffReadPart(miff));

   data = _MiffBase64Restart(miff->readByteData);

   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[3]));

   _MiffByteSwap4(miff, &vtemp);

   *value = vtemp.r;

   returnTrue;
}

/******************************************************************************
func: _MiffReadR4S
******************************************************************************/
MiffBool _MiffReadR4S(Miff * const miff, MiffR4 * const value)
{
   MiffC1 *ctemp;

   returnFalseIf(!_MiffReadPart(miff));

   *value = (MiffR4) strtod((char *) miff->readByteData, (char **) &ctemp);

   returnTrue;
}

/******************************************************************************
func: _MiffReadR8
******************************************************************************/
MiffBool _MiffReadR8(Miff * const miff, MiffR8 * const value)
{
   Miff8      vtemp;
   MiffBase64Data data;

   returnFalseIf(!_MiffReadPart(miff));

   data = _MiffBase64Restart(miff->readByteData);

   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[3]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[4]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[5]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[6]));
   returnFalseIf(!_MiffBase64Get(&data, &vtemp.byte[7]));

   _MiffByteSwap8(miff, &vtemp);

   *value = vtemp.r;

   returnTrue;
}

/******************************************************************************
func: _MiffReadR8S
******************************************************************************/
MiffBool _MiffReadR8S(Miff * const miff, MiffR8 * const value)
{
   MiffC1 *ctemp;

   returnFalseIf(!_MiffReadPart(miff));

   *value = strtod((char *) miff->readByteData, (char **) &ctemp);

   returnTrue;
}
