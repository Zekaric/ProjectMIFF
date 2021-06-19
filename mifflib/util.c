/******************************************************************************
file:       util
author:     Robbert de Groot
company:    
copyright:  2021, Robbert de Groot

description:
General utils
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "local.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
MiffBool _CompressAndWrite(Miff * const miff, MiffN4 const byteCount, void const * const byteData)
{
   MiffN4    index;
   MiffN4    cbyteCount;
   MiffN1   *cbyteData;

   // Special but valide case.
   if (byteCount == 0)
   {
      returnFalseIf(!_WriteTxtValueN(miff, (MiffN8) 0));
   
      returnFalseIf(!_WriteTxtRecordSeparator(miff));

      returnTrue;
   }

   // Compress the buffer.  It may not be the same size as miff->compressChunkByteCount.
   cbyteCount = _MemCompressBound(byteCount);
   cbyteData  = _MemCreateTypeArray(cbyteCount, MiffN1);
   returnFalseIf(!cbyteData);
         
   returnFalseIf(!_MemCompress(byteCount, byteData, &cbyteCount, cbyteData));

   returnFalseIf(!_WriteTxtValueN(miff, (MiffN8) cbyteCount));
   
   returnFalseIf(!_WriteTxtRecordSeparator(miff));

   _Base64Restart();

   forCount(index, cbyteCount)
   {
      returnFalseIf(!_Base64Set(miff, cbyteData[index]));
   }
   returnFalseIf(!_Base64SetEnd(miff));

   // Cleanup
   _MemDestroy(cbyteData);

   returnTrue;
}

/******************************************************************************
func: _CurrentIndexInc
******************************************************************************/
MiffBool _CurrentIndexInc(Miff * const miff)
{
   miff->currentIndex++;

   if (miff->currentRecord.type == miffValueTypeKEY_VALUE_BLOCK_START ||
       miff->currentRecord.type == miffValueTypeKEY_VALUE_BLOCK_STOP  ||
       miff->currentIndex       == miff->currentRecord.arrayCount)
   {
      // Finish the last chunk.
      if (miff->currentRecord.compressFlag == miffCompressFlagCHUNK_COMPRESS)
      {
         returnFalseIf(!_CompressAndWrite(miff, miff->compressMemByteIndex, miff->compressMemByteData));
      }

      // Reset the record.
      miff->currentRecord.type = miffValueTypeNONE;
      returnFalseIf(!_WriteTxtRecordEnder(miff));
   }
   else
   {
      if (miff->currentRecord.compressFlag == miffCompressFlagNONE)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }
   }

   returnTrue;
}

/******************************************************************************
func: _MemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
MiffBool _MemIsEqual(MiffN4 const countA, MiffN1 const * const memA, MiffN4 const countB, 
   MiffN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}
