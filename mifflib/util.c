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
         miff->typeStack[miff->typeStackIndex].type.type = miffValueTypeNONE;
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
func: _CreateDefineList
******************************************************************************/
MiffBool _CreateDefineList(Miff * const miff)
{
   int index;

   forCount(index, miffValueTypeFIRST_USER_TYPE)
   {
      miff->typeList[index].isSet                 = miffBoolTRUE;
      miff->typeList[index].varCount              = 1;
      miff->typeList[index].varList               = _MemCreateTypeArray(1, MiffTypeRecord);
      returnFalseIf(!miff->typeList[index].varList);
      miff->typeList[index].varList[0].arrayCount = 1;
      _MemCopyTypeArray(6, MiffC2, miff->typeList[index].varList[0].nameC2, L"value");
      miff->typeList[index].varList[0].type       = (MiffValueType) index;
   }

   _MemCopyTypeArray(1, MiffC2, miff->typeList[miffValueTypeKEY_VALUE_BLOCK_STOP ].nameC2, L"}");
   _MemCopyTypeArray(1, MiffC2, miff->typeList[miffValueTypeKEY_VALUE_BLOCK_START].nameC2, L"{");
   _MemCopyTypeArray(4, MiffC2, miff->typeList[miffValueTypeTYPE                 ].nameC2, L"type");
   _MemCopyTypeArray(6, MiffC2, miff->typeList[miffValueTypeDEFINE               ].nameC2, L"define");
   _MemCopyTypeArray(1, MiffC2, miff->typeList[miffValueTypeSTRING               ].nameC2, L"\"");
   _MemCopyTypeArray(1, MiffC2, miff->typeList[miffValueTypeBOOLEAN              ].nameC2, L"b");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeI1                   ].nameC2, L"i1");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeI2                   ].nameC2, L"i2");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeI3                   ].nameC2, L"i3");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeI4                   ].nameC2, L"i4");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeI8                   ].nameC2, L"i8");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeI16                  ].nameC2, L"i16");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeI32                  ].nameC2, L"i32");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeI64                  ].nameC2, L"i64");
   _MemCopyTypeArray(4, MiffC2, miff->typeList[miffValueTypeI128                 ].nameC2, L"i128");
   _MemCopyTypeArray(4, MiffC2, miff->typeList[miffValueTypeI256                 ].nameC2, L"i256");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeN1                   ].nameC2, L"n1");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeN2                   ].nameC2, L"n2");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeN3                   ].nameC2, L"n3");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeN4                   ].nameC2, L"n4");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeN8                   ].nameC2, L"n8");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeN16                  ].nameC2, L"n16");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeN32                  ].nameC2, L"n32");
   _MemCopyTypeArray(3, MiffC2, miff->typeList[miffValueTypeN64                  ].nameC2, L"n64");
   _MemCopyTypeArray(4, MiffC2, miff->typeList[miffValueTypeN128                 ].nameC2, L"n128");
   _MemCopyTypeArray(4, MiffC2, miff->typeList[miffValueTypeN256                 ].nameC2, L"n256");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeR4                   ].nameC2, L"r4");
   _MemCopyTypeArray(2, MiffC2, miff->typeList[miffValueTypeR8                   ].nameC2, L"r8");

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
