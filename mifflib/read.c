/******************************************************************************
file:       write
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) !!!!YEAR!!!!, Robbert de Groot

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
func: _ReadTxtLine
******************************************************************************/
MiffBool _ReadTxtLine(Miff * const miff)
{
   MiffN4  index;
   MiffN1  byte;
   MiffN1 *bufTemp;

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (miff->readByteCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _MemCreateTypeArray(miff->readByteCountActual * 2, MiffN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _MemCopyTypeArray(miff->readByteCountActual, MiffN1, bufTemp, miff->readByteData);
         // Destroy the old buffer.
         _MemDestroy(miff->readByteData);

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
func: _ReadTxtLineSkip
******************************************************************************/
MiffBool _ReadTxtLineSkip(Miff * const miff)
{
   MiffN1 byte;

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _ReadTxtPart
******************************************************************************/
MiffBool _ReadTxtPart(Miff * const miff)
{
   MiffN4  index;
   MiffN1  byte;
   MiffN1 *bufTemp;

   // Nothing left to read for this record.
   returnFalseIf(miff->readRecordIsDone);

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (miff->readByteCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _MemCreateTypeArray(miff->readByteCountActual * 2, MiffN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _MemCopyTypeArray(miff->readByteCountActual, MiffN1, bufTemp, miff->readByteData);
         // Destroy the old buffer.
         _MemDestroy(miff->readByteData);

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
func: _ReadTxtRecordType
******************************************************************************/
MiffBool _ReadTxtRecordType(Miff * const miff, MiffType *type)
{
   MiffN4 index;
   MiffC2 c2Type[miffTypeSTR_SIZE_MAX];

   returnFalseIf(!_ReadTxtPart(miff));

   forCount(index, miffTypeSTR_SIZE_MAX)
   {
      c2Type[index] = (MiffC2) miff->readByteData[index];
   }

   forCount (index, miffTypeCOUNT)
   {
      continueIf(_TypeGetNameSize(index) == 0);

      if (_MemIsEqual(
            _TypeGetNameSize(index), (MiffN1 *) _TypeGetNameC1(index),
            _TypeGetNameSize(index), miff->readByteData))
      {
         *type = index;

         return miffBoolTRUE;
      }
   }

   return miffBoolFALSE;
}
