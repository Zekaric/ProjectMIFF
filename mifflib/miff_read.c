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
#include "miff_local.h"

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffGetBin
******************************************************************************/
MiffB _MiffGetBin(Miff * const miff, MiffN const binCount, MiffN1 * const binData)
{
   MiffN        index;

#if 1
   MiffData     data;

   forCount(index, binCount)
   {
      data = miffGetValueBinData(miff, &binData[index]);
      returnFalseIf(data == miffDataERROR);
   }
   data = miffGetValueBinDataEnd(miff);
   returnFalseIf(data == miffDataERROR);

   returnTrue;
#else
   MiffN        bufferIndex;
   BsfDataGet   data;
   MiffN1       byte,
                buffer[4];

   bufferIndex = 0;
   loop
   {
      // Get next base64 chunk.
      forCount(index, 4)
      {
         returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));
         breakIf(byte == '\t' ||
                 byte == '\n');
         buffer[index] = byte;
      }

      // Type cast safe.  We are not writing to binData.
      data = bsfPrepGet((BsfN1 *) buffer);

      returnFalseIf(!bsfGet(&data, &binData[bufferIndex++]));
      breakIf(bufferIndex == binCount);

      returnFalseIf(!bsfGet(&data, &binData[bufferIndex++]));
      breakIf(bufferIndex == binCount);

      returnFalseIf(!bsfGet(&data, &binData[bufferIndex++]));
      breakIf(bufferIndex == binCount);

      // No more data in the record part.  If we having read the whole buffer
      // yet then there is a problem.
      breakIf (byte == '\t' ||
               byte == '\n');
   }

   if (byte == '\t')
   {
      miff->isPartDone = miffTRUE;
   }
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }
   else
   {
      _MiffGetPartEnd(miff);
   }

   // Something when wrong when the file was written.
   returnFalseIf(bufferIndex != binCount);

   returnTrue;
#endif
}

/******************************************************************************
func: _MiffGetBinByte
******************************************************************************/
MiffData _MiffGetBinByte(Miff * const miff, MiffN1 * const binByte)
{
   MiffN1 byte;

   // Set the last letter.
   if (miff->valueIndex == 0)
   {
      miff->bsfLastLetter = 0;
      returnIf(!bsfPrep(&miff->bsfData), miffDataERROR);
   }

   // We have already reached the end of the bin buffer.
   returnIf(
         miff->bsfLastLetter == '\t' ||
         miff->bsfLastLetter == '\n',
      miffDataERROR);

   returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte), miffDataERROR);
   returnIf(
         byte == '\t' ||
         byte == '\n',
      miffDataERROR);
   miff->bsfLastLetter = byte;

   // Convert the base64 to a byte.
   if (bsfToByte(&miff->bsfData, byte, binByte) == 0)
   {
      // Need another base64 byte to convert to a byte.
      returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte), miffDataERROR);
      returnIf(
            byte == '\t' ||
            byte == '\n',
         miffDataERROR);
      miff->bsfLastLetter = byte;

      // Definitely something else wrong here.
      returnIf(bsfToByte(&miff->bsfData, byte, binByte) != 1, miffDataERROR);
   }

   miff->valueIndex++;

   return miffDataIS_DATA;
}

/******************************************************************************
func: _MiffGetBinByteEnd
******************************************************************************/
MiffData _MiffGetBinByteEnd(Miff * const miff)
{
   // Done reading.  What's next?
   if      (miff->bsfLastLetter == '\t')
   {
      miff->isPartDone = miffTRUE;
   }
   else if (miff->bsfLastLetter == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }
   else 
   {
      _MiffGetPartEnd(miff);
   }

   returnIf(miff->isRecordDone, miffDataIS_RECORD_DONE);
   returnIf(miff->isPartDone,   miffDataIS_PART_DONE);
   return                       miffDataERROR;
}

/******************************************************************************
func: _MiffGetLineSkip
******************************************************************************/
MiffB _MiffGetLineSkip(Miff * const miff)
{
   MiffN1 byte;

   returnFalseIf(!miff);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetPart
******************************************************************************/
MiffB _MiffGetPart(Miff * const miff, MiffB const trimLeadingTabs)
{
   MiffN4   index;
   MiffN1   byte;
   MiffB    trimTabs;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   trimTabs = trimLeadingTabs;
   index    = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

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
      // Only read up to a key size.  Ignore everything else.
      if (index < miffKeyBYTE_COUNT)
      {
         miff->readData[index++] = byte;
      }
   }

   // Need NULL terminator.
   miff->readData[index] = 0;
   miff->readCount       = index;

   miff->isPartDone = miffTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetPartEnd

Skip to the next tab or newline.
******************************************************************************/
MiffB _MiffGetPartEnd(Miff * const miff)
{
   MiffN4   index;
   MiffN1   byte;

   // Nothing left to read for this record.
   returnFalseIf(!miff);

   returnTrueIf(
      miff->isPartDone ||
      miff->isRecordDone);

   index = 0;
   byte  = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');
   }

   miff->isPartDone = miffTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetStrLetter
******************************************************************************/
MiffData _MiffGetStrLetter(Miff * const miff, MiffStr * const letter)
{
   returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) letter), miffDataERROR); 

   // Escape single character slash, tab, and newline characters.
   switch (*letter)
   {
   case '\t':
      miff->isPartDone = miffTRUE;
      return miffDataIS_PART_DONE;

   case '\n':
      // An actual tab or cursor return character should never be inside a string.
      miff->isRecordDone = miffTRUE;
      return miffDataIS_RECORD_DONE;

   case '\\':
      returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) letter), miffDataERROR);
      switch (*letter)
      {
      case '\\':
         *letter = '\\';
         break;

      case 't':
         *letter = '\t';
         break;

      case 'n':
         *letter = '\n';
         break;
      }
      break;

   default:
      break;
   }

   return miffDataIS_DATA;
}

/******************************************************************************
func: _MiffGetValueHeader
******************************************************************************/
MiffStr _MiffGetValueHeader(Miff * const miff)
{
   MiffN1 byte;

   return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

   return (MiffStr) byte;
}

/******************************************************************************
func: _MiffGetValueBufferCount
******************************************************************************/
MiffN _MiffGetValueBufferCount(Miff * const miff)
{
   MiffN   index;
   MiffStr buffer[32];

   forCount(index, 32)
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &buffer[index]));
      if (buffer[index] == ' ')
      {
         buffer[index] = 0;
         break;
      }
   }

   return _MiffStrToN(index, (MiffN1 *) buffer);
}
