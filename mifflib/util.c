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
func: _CompressAndWrite
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
func: _CurrentIndexInc
******************************************************************************/
MiffBool _CurrentIndexInc(Miff * const miff)
{
   // Special case.
   if (miff->typeCurrent.type == miffValueTypeKEY_VALUE_BLOCK_START ||
       miff->typeCurrent.type == miffValueTypeKEY_VALUE_BLOCK_STOP)
   {
      // Reset the record.
      miff->typeCurrent.type = miffValueTypeNONE;
      returnFalseIf(!_WriteTxtRecordEnder(miff));
      returnTrue;
   }

   // Move to the next index in the var array.
   miff->typeVarArrayIndex++;

   // Are we still writing out elements in the array?
   if (miff->typeVarArrayIndex < miff->typeUnrolledArray[miff->typeVarIndex].type.arrayCount)
   {
      if (miff->typeCurrent.compressFlag == miffCompressFlagNONE &&
          miff->typeUnrolledArray[miff->typeVarIndex].type.compressFlag == miffCompressFlagNONE)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }

      // If we don't know the size of the array then reset the index.
      if (miff->typeUnrolledArray[miff->typeVarIndex].type.arrayCount == miffArrayCountUNKNOWN)
      {
         miff->typeVarArrayIndex = 0;
      }

      returnTrue;
   }

   // Restart the type var array index;
   miff->typeVarArrayIndex = 0;

   // Move to the next type in the unroll.
   miff->typeVarIndex++;
   if (miff->typeVarIndex < miff->typeUnrolledArrayCount)
   {
      if (miff->typeCurrent.compressFlag == miffCompressFlagNONE)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }

      returnTrue;
   }

   // Restart the type var index.
   miff->typeVarIndex = 0;

   // Move to the next array item in the type
   miff->typeCurrentIndex++;

   // Are we still writing out elements?
   if (miff->typeCurrentIndex < miff->typeCurrent.arrayCount)
   {
      if (miff->typeCurrent.compressFlag == miffCompressFlagNONE)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }

      // if we don't know the size of the array then reset the index.
      if (miff->typeCurrent.arrayCount == miffArrayCountUNKNOWN)
      {
         miff->typeCurrentIndex = 0;
      }

      returnTrue;
   }
   
   // Finish the last chunk.
   if (miff->typeCurrent.compressFlag == miffCompressFlagCHUNK_COMPRESS)
   {
      returnFalseIf(!_CompressAndWrite(miff, miff->compressMemByteIndex, miff->compressMemByteData));
   }

   // Reset the record.
   miff->typeCurrent.type = miffValueTypeNONE;
   returnFalseIf(!_WriteTxtRecordEnder(miff));

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

/******************************************************************************
func: _UserTypeUnroll
******************************************************************************/
MiffBool _UserTypeUnroll(Miff * const miff, MiffC2 const * const name, MiffValueType const type)
{
   MiffN4  index;
   MiffC2 *nameTemp;
   
   // Different unrolled value present.
   // For all unrolled type strings.
   forCount(index, miff->typeUnrolledCount)
   {
      // Clean up the string memory.
      _MemDestroy(miff->typeUnrolledArray[index].nameC2);
   }

   // Clean out the memory.
   _MemClearTypeArray(miff->typeUnrolledCount, MiffUnrollRecord, miff->typeUnrolledArray);

   miff->typeUnrolledCount = 0;

   forCount(index, miff->typeList[type].varCount)
   {
      // Need to inflate the unrolled list.
      if (miff->typeVarIndex == miff->typeUnrolledArrayCount)
      {
         MiffUnrollRecord *rollTemp;

         // Create new buffers.
         rollTemp = _MemCreateTypeArray(miff->typeUnrolledArrayCount + 80, MiffUnrollRecord);
         returnFalseIf(!rollTemp);

         // Copy over the old buffer.
         _MemCopyTypeArray(miff->typeUnrolledArrayCount, MiffUnrollRecord, rollTemp, miff->typeUnrolledArray);

         // Destroy the old buffers.
         _MemDestroy(miff->typeUnrolledArray);

         // Use the new buffers.
         miff->typeUnrolledArray = rollTemp;

         // Set the new buffer size.
         miff->typeUnrolledArrayCount += 80;
      }

      // Simple type.
      if (miff->typeList[type].varList[index].type < miffValueTypeFIRST_USER_TYPE)
      {
         miff->typeUnrolledArray[miff->typeVarIndex].nameC2 = _C2Append(name, L".", miff->typeList[type].varList[index].nameC2);
         returnFalseIf(miff->typeUnrolledArray[miff->typeVarIndex].nameC2);

         miff->typeUnrolledArray[miff->typeVarIndex].type.arrayCount      = miff->typeList[type].varList[index].arrayCount;
         miff->typeUnrolledArray[miff->typeVarIndex].type.chunkByteCount  = miff->typeList[type].varList[index].chunkByteCount;
         miff->typeUnrolledArray[miff->typeVarIndex].type.compressFlag    = miff->typeList[type].varList[index].compressFlag;
         miff->typeUnrolledArray[miff->typeVarIndex].type.type            = miff->typeList[type].varList[index].type;
         miff->typeVarIndex++;
      }
      // User type.
      else 
      {
         nameTemp = _C2Append(name, L".", miff->typeList[type].varList[index].nameC2);
         returnFalseIf(!nameTemp);

         // Recurse into the type to fill in the fields.
         returnFalseIf(_UserTypeUnroll(miff, nameTemp, miff->typeList[type].varList[index].type));

         _MemDestroy(nameTemp);
      }
   }

   // Set the unrolled count.
   miff->typeUnrolledCount = miff->typeVarIndex;

   returnTrue;
}
