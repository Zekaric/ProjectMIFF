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
/******************************************************************************
func: _CompressStart
******************************************************************************/
MiffBool _CompressStart(Miff * const miff, MiffN4 const compressedChunkByteCount)
{
   miff->isCompressed             = miffBoolTRUE;
   miff->compressedChunkByteCount = compressedChunkByteCount;

   // We need a larger buffer than what we currently have.
   if (miff->compressMemByteCountActual < miff->compressedChunkByteCount)
   {
      miff->compressMemByteCountActual = miff->compressedChunkByteCount;

      _MemDestroy(miff->compressMemByteData);

      // Allocate the buffer.
      miff->compressMemByteData = _MemCreateTypeArray(miff->compressedChunkByteCount, MiffN1);
      if (!miff->compressMemByteData)
      {
         miff->currentRecord.type = miffTypeNONE;
         returnFalse;
      }
   }
   // Clear out the buffer just for good measure.
   else
   {
      _MemClearTypeArray(miff->compressMemByteCountActual, MiffN1, miff->compressMemByteData);
   }

   // Set the byte count of the internal memory.
   miff->compressMemByteCount = miff->compressedChunkByteCount;
   miff->compressMemByteIndex = 0;

   returnTrue;
}

/******************************************************************************
func: _CompressStop
******************************************************************************/
void _CompressStop(Miff * const miff)
{
   miff->isCompressed = miffBoolFALSE;
}

/******************************************************************************
func: _CompressWrite
******************************************************************************/
MiffBool _CompressWrite(Miff * const miff, MiffN4 const byteCount, void const * const byteData)
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
