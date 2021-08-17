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
      miff->defineList[index].isSet                 = miffBoolTRUE;
      miff->defineList[index].varCount              = 1;
      miff->defineList[index].varList               = _MemCreateTypeArray(1, MiffTypeRecord);
      returnFalseIf(!miff->defineList[index].varList);
      miff->defineList[index].varList[0].arrayCount = 1;
      _MemCopyTypeArray(6, MiffC2, miff->defineList[index].varList[0].nameC2, L"value");
      miff->defineList[index].varList[0].type       = (MiffValueType) index;
   }

   _MemCopyTypeArray(1, MiffC2, miff->defineList[miffValueTypeKEY_VALUE_BLOCK_STOP ].nameC2, L"}");
   _MemCopyTypeArray(1, MiffC2, miff->defineList[miffValueTypeKEY_VALUE_BLOCK_START].nameC2, L"{");
   _MemCopyTypeArray(4, MiffC2, miff->defineList[miffValueTypeTYPE                 ].nameC2, L"type");
   _MemCopyTypeArray(6, MiffC2, miff->defineList[miffValueTypeDEFINE               ].nameC2, L"define");
   _MemCopyTypeArray(1, MiffC2, miff->defineList[miffValueTypeSTRING               ].nameC2, L"\"");
   _MemCopyTypeArray(1, MiffC2, miff->defineList[miffValueTypeBOOLEAN              ].nameC2, L"b");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeI1                   ].nameC2, L"i1");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeI2                   ].nameC2, L"i2");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeI3                   ].nameC2, L"i3");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeI4                   ].nameC2, L"i4");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeI8                   ].nameC2, L"i8");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeI16                  ].nameC2, L"i16");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeI32                  ].nameC2, L"i32");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeI64                  ].nameC2, L"i64");
   _MemCopyTypeArray(4, MiffC2, miff->defineList[miffValueTypeI128                 ].nameC2, L"i128");
   _MemCopyTypeArray(4, MiffC2, miff->defineList[miffValueTypeI256                 ].nameC2, L"i256");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeN1                   ].nameC2, L"n1");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeN2                   ].nameC2, L"n2");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeN3                   ].nameC2, L"n3");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeN4                   ].nameC2, L"n4");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeN8                   ].nameC2, L"n8");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeN16                  ].nameC2, L"n16");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeN32                  ].nameC2, L"n32");
   _MemCopyTypeArray(3, MiffC2, miff->defineList[miffValueTypeN64                  ].nameC2, L"n64");
   _MemCopyTypeArray(4, MiffC2, miff->defineList[miffValueTypeN128                 ].nameC2, L"n128");
   _MemCopyTypeArray(4, MiffC2, miff->defineList[miffValueTypeN256                 ].nameC2, L"n256");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeR4                   ].nameC2, L"r4");
   _MemCopyTypeArray(2, MiffC2, miff->defineList[miffValueTypeR8                   ].nameC2, L"r8");

   returnTrue;
}

/******************************************************************************
func: _CurrentIndexInc
******************************************************************************/
MiffBool _CurrentIndexInc(Miff * const miff)
{
   if (miff->currentRecord.type == miffValueTypeDEFINE)
   {
      miff->defineIndex++;

      if (miff->defineIndex == miff->defineList[miff->defineCurrentType].varCount)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }
      else
      {
         returnFalseIf(!_WriteTxtRecordEnder(miff));
      }

      // Reset the record.
      miff->currentRecord.type = miffValueTypeNONE;

      returnTrue;
   }

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

/******************************************************************************
func: _UserTypeUnroll
******************************************************************************/
MiffBool _UserTypeUnroll(Miff * const miff, MiffC2 const * const name, MiffValueType const type)
{
   MiffN4  index;
   MiffC2 *nameTemp;
   
   // Different unrolled value present.
   // For all unrolled type strings.
   forCount(index, miff->defineUnrolledCount)
   {
      // Clean up the string memory.
      _MemDestroy(miff->defineUnrolledArray[index].nameC2);
   }

   // Clean out the memory.
   _MemClearTypeArray(miff->defineUnrolledCount, MiffUnrollRecord, miff->defineUnrolledArray);

   miff->defineUnrolledCount = 0;

   forCount(index, miff->defineList[type].varCount)
   {
      // Need to inflate the unrolled list.
      if (miff->defineIndex == miff->defineUnrolledArrayCount)
      {
         MiffUnrollRecord *rollTemp;

         // Create new buffers.
         rollTemp = _MemCreateTypeArray(miff->defineUnrolledArrayCount + 80, MiffUnrollRecord);
         returnFalseIf(!rollTemp);

         // Copy over the old buffer.
         _MemCopyTypeArray(miff->defineUnrolledArrayCount, MiffUnrollRecord, rollTemp, miff->defineUnrolledArray);

         // Destroy the old buffers.
         _MemDestroy(miff->defineUnrolledArray);

         // Use the new buffers.
         miff->defineUnrolledArray = rollTemp;

         // Set the new buffer size.
         miff->defineUnrolledArrayCount += 80;
      }

      // Simple type.
      if (miff->defineList[type].varList[index].type < miffValueTypeFIRST_USER_TYPE)
      {
         miff->defineUnrolledArray[miff->defineIndex].nameC2 = _C2Append(name, L".", miff->defineList[type].varList[index].nameC2);
         returnFalseIf(miff->defineUnrolledArray[miff->defineIndex].nameC2);

         miff->defineUnrolledArray[miff->defineIndex].type.arrayCount      = miff->defineList[type].varList[index].arrayCount;
         miff->defineUnrolledArray[miff->defineIndex].type.chunkByteCount  = miff->defineList[type].varList[index].chunkByteCount;
         miff->defineUnrolledArray[miff->defineIndex].type.compressFlag    = miff->defineList[type].varList[index].compressFlag;
         miff->defineUnrolledArray[miff->defineIndex].type.type            = miff->defineList[type].varList[index].type;
         miff->defineIndex++;
      }
      // User type.
      else 
      {
         nameTemp = _C2Append(name, L".", miff->defineList[type].varList[index].nameC2);
         returnFalseIf(!nameTemp);

         // Recurse into the type to fill in the fields.
         returnFalseIf(_UserTypeUnroll(miff, nameTemp, miff->defineList[type].varList[index].type));

         _MemDestroy(nameTemp);
      }
   }

   // Set the unrolled count.
   miff->defineUnrolledCount = miff->defineIndex;

   returnTrue;
}
