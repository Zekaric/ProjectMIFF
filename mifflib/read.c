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
func: _MiffReadBufferBase64
******************************************************************************/
MiffB _MiffReadBufferBase64(Miff * const miff, MiffN const bufferCount, 
   MiffN1 * const bufferData)
{
   MiffN             index,
                     bufferIndex;
   MiffBase64DataGet data;
   MiffN1            byte,
                     buffer[4];

   bufferIndex = 0;
   loop
   {
      // Get next base64 chunk.
      forCount(index, 4)
      {
         returnFalseIf(miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &byte));
         breakIf(byte == '\t' || 
                 byte == '\n');
         buffer[index] = byte;
      }

      // Type cast safe.  We are not writing to bufferData.
      data = _MiffBase64PrepGet((MiffN1 *) buffer);

      returnFalseIf(!_MiffBase64Get(&data, &bufferData[bufferIndex++]));
      breakIf(bufferIndex == bufferCount);

      returnFalseIf(!_MiffBase64Get(&data, &bufferData[bufferIndex++]));
      breakIf(bufferIndex == bufferCount);

      returnFalseIf(!_MiffBase64Get(&data, &bufferData[bufferIndex++]));
      breakIf(bufferIndex == bufferCount);

      // No more data in the record part.  If we having read the whole buffer
      // yet then there is a problem.
      breakIf(byte == '\t' || byte == '\n');
   }

   // Something when wrong when the file was written.
   returnFalseIf(bufferIndex != bufferCount);

   returnTrue;
}

/******************************************************************************
func: _MiffReadLineSkip
******************************************************************************/
MiffB _MiffReadLineSkip(Miff * const miff)
{
   MiffN1 byte;

   returnFalseIf(!miff);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadPart
******************************************************************************/
MiffB _MiffReadPart(Miff * const miff, MiffB const trimLeadingTabs)
{
   MiffN4   index;
   MiffN1   byte;
   MiffN1  *bufTemp;
   MiffB    trimTabs;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   trimTabs = trimLeadingTabs;
   index    = 0;
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
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &byte));

      // Eating tabs at the start of the line.
      if (trimTabs && byte == '\t')
      {
         continue;
      }
      else
      {
         trimTabs = miffFALSE;
      }

      // End of line or part, we are done reading.
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
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadPartEnd

Skip to the next tab or newline.
******************************************************************************/
MiffB _MiffReadPartEnd(Miff * const miff)
{
   MiffN4   index;
   MiffN1   byte;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   index = 0;
   byte  = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');
   }

   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadStrEscaped
******************************************************************************/
MiffB _MiffReadStrEscaped(Miff * const miff, MiffN const strLen, MiffStr * const str)
{
   MiffN   index;
   MiffN   bufferIndex;
   MiffStr letter;
   MiffStr bufferData[66];

   // Should be using a different function for reading.
   returnFalseIf(strLen == miffCountUNKNOWN);

   bufferIndex = 0;
   _MiffMemClearTypeArray(66, MiffN1, bufferData);
   forCount(index, strLen)
   {
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &letter)); 

      // Escape single character slash, tab, and newline characters.
      switch (letter)
      {
      case '\t':
      case '\n':
         // An actual tab or cursor return character should never be inside a string.
         returnFalse;

      case '\\':
         returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &letter));
         switch (letter)
         {
         case '\\':
            str[bufferIndex++] = '\\';
            break;

         case 't':
            str[bufferIndex++] = '\t';
            break;

         case 'n':
            str[bufferIndex++] = '\n';
            break;
         }
         break;

      default:
         str[bufferIndex++] = letter;
         break;
      }
   }

   returnTrue;
}

/******************************************************************************
func: _MiffReadValueHeader
******************************************************************************/
MiffStr _MiffReadValueHeader(Miff * const miff)
{
   MiffN1 byte;

   return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &byte));

   return (MiffStr) byte;
}

/******************************************************************************
func: _MiffReadValueBufferCount
******************************************************************************/
MiffN _MiffReadValueBufferCount(Miff * const miff)
{
   MiffN   index;
   MiffStr buffer[32];

   forCount(index, 32)
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffStr *) &buffer[index]));
      if (buffer[index] == ' ')
      {
         buffer[index] = 0;
         break;
      }
   }

   return _MiffAToN(index, (MiffN1 *) buffer);
}
