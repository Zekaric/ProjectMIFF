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
function
******************************************************************************/
MiffBool _WriteCompressByte(Miff * const miff, MiffN1 const byte)
{
   miff->compressMemByteData[miff->compressMemByteIndex++] = byte;

   if (miff->compressMemByteIndex == miff->compressMemByteCount)
   {
      returnFalseIf(!_CompressWrite(          miff, miff->compressMemByteCount, miff->compressMemByteData));
      returnFalseIf(!_WriteTxtRecordSeparator(miff));

      // Reset the buffer.
      miff->compressMemByteIndex = 0;
      _MemClearTypeArray(miff->compressMemByteCount, MiffN1, miff->compressMemByteData);
   }

   returnTrue;
}

/******************************************************************************
func: _WriteTxtC1
******************************************************************************/
MiffBool _WriteTxtC1(Miff const * const miff, MiffC1 const * const value)
{
   return miff->setBuffer(miff->dataRepo, _C1GetCount(value), value);
}

/******************************************************************************
func: _WriteTxtC2
******************************************************************************/
MiffBool _WriteTxtC2(Miff const * const miff, MiffC2 const * const c2)
{
   MiffN4  c1Count;
   MiffC1 *c1;

   if (!_C2ToC1(_C2GetCount(c2) + 1, c2, &c1Count, &c1))
   {
      returnFalse;
   }

   _WriteTxtC1(miff, c1);

   _MemDestroy(c1);

   returnTrue;
}

/******************************************************************************
func: _WriteTxtRecordArrayCount
******************************************************************************/
MiffBool _WriteTxtRecordArrayCount(Miff const * const miff, MiffN4 const count)
{
   return _WriteTxtValue4(miff, miffTypeN4, *((Miff4 *) &count));
}

/******************************************************************************
func: _WriteTxtRecordCompressChunkSize
******************************************************************************/
MiffBool _WriteTxtRecordChunkSize(Miff const * const miff, MiffN4 const chunkSize)
{
   return _WriteTxtValue4(miff, miffTypeN4, *((Miff4 *) &chunkSize));
}

/******************************************************************************
func: _WriteTxtRecordCompressFlag
******************************************************************************/
MiffBool _WriteTxtRecordCompressFlag(Miff const * const miff, MiffBool const isCompressed)
{
   if (!isCompressed)
   {
      return _WriteTxtC1(miff, (MiffC1 *) "-");
   }
    
   return _WriteTxtC1(miff, (MiffC1 *) ":");
}

/******************************************************************************
func: _WriteTxtRecordEnder
******************************************************************************/
MiffBool _WriteTxtRecordEnder(Miff const * const miff)
{
   return _WriteTxtC1(miff, (MiffC1 *) "\n");
}

/******************************************************************************
func: _WriteTxtRecordKeyC2
******************************************************************************/
MiffBool _WriteTxtRecordKeyC2(Miff const * const miff, MiffC2 const * const key)
{
   MiffC1 c1Key[256];
   MiffN1 c1Count;

   _MemClearTypeArray(256, MiffC1, c1Key);

   returnFalseIf(!_C2ToC1Key(_C2GetCount(key), key, &c1Count, c1Key));
   return _WriteTxtC1(miff, c1Key);
}

/******************************************************************************
func: _WriteTxtRecordSeparator
******************************************************************************/
MiffBool _WriteTxtRecordSeparator(Miff const * const miff)
{
   return _WriteTxtC1(miff, (MiffC1 *) "\t");
}

/******************************************************************************
func: _WriteTxtRecordType
******************************************************************************/
MiffBool _WriteTxtRecordType(Miff const * const miff, MiffType const type)
{
   return _WriteTxtType(miff, type);
}

/******************************************************************************
func: _WriteTxtType
******************************************************************************/
MiffBool _WriteTxtType(Miff const * const miff, MiffType const value)
{
   switch (value)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return _WriteTxtC1(miff, (MiffC1 *) "{"       );
   case miffTypeKEY_VALUE_BLOCK_STOP:   return _WriteTxtC1(miff, (MiffC1 *) "}"       );
   case miffTypeTYPE:                   return _WriteTxtC1(miff, (MiffC1 *) "type"    );
   case miffTypeSTRING:                 return _WriteTxtC1(miff, (MiffC1 *) "\""      );
   case miffTypeVARIABLE:               return _WriteTxtC1(miff, (MiffC1 *) "v"       );
   case miffTypeBOOLEAN:                return _WriteTxtC1(miff, (MiffC1 *) "b"       );
   case miffTypeI1:                     return _WriteTxtC1(miff, (MiffC1 *) "i1"      );
   case miffTypeI2:                     return _WriteTxtC1(miff, (MiffC1 *) "i2"      );
   case miffTypeI4:                     return _WriteTxtC1(miff, (MiffC1 *) "i4"      );
   case miffTypeI8:                     return _WriteTxtC1(miff, (MiffC1 *) "i8"      );
   case miffTypeI16:                    return _WriteTxtC1(miff, (MiffC1 *) "i16"     );
   case miffTypeI32:                    return _WriteTxtC1(miff, (MiffC1 *) "i32"     );
   case miffTypeI64:                    return _WriteTxtC1(miff, (MiffC1 *) "i64"     );
   case miffTypeI128:                   return _WriteTxtC1(miff, (MiffC1 *) "i128"    );
   case miffTypeI256:                   return _WriteTxtC1(miff, (MiffC1 *) "i256"    );
   case miffTypeN1:                     return _WriteTxtC1(miff, (MiffC1 *) "n1"      );
   case miffTypeN2:                     return _WriteTxtC1(miff, (MiffC1 *) "n2"      );
   case miffTypeN4:                     return _WriteTxtC1(miff, (MiffC1 *) "n4"      );
   case miffTypeN8:                     return _WriteTxtC1(miff, (MiffC1 *) "n8"      );
   case miffTypeN16:                    return _WriteTxtC1(miff, (MiffC1 *) "n16"     );
   case miffTypeN32:                    return _WriteTxtC1(miff, (MiffC1 *) "n32"     );
   case miffTypeN64:                    return _WriteTxtC1(miff, (MiffC1 *) "n64"     );
   case miffTypeN128:                   return _WriteTxtC1(miff, (MiffC1 *) "n128"    );
   case miffTypeN256:                   return _WriteTxtC1(miff, (MiffC1 *) "n256"    );
   case miffTypeR4:                     return _WriteTxtC1(miff, (MiffC1 *) "r4"      );
   case miffTypeR8:                     return _WriteTxtC1(miff, (MiffC1 *) "r8"      );
   case miffTypeABI1:                   return _WriteTxtC1(miff, (MiffC1 *) "abi1"    );
   case miffTypeABI2:                   return _WriteTxtC1(miff, (MiffC1 *) "abi2"    );
   case miffTypeABI4:                   return _WriteTxtC1(miff, (MiffC1 *) "abi4"    );
   case miffTypeABI8:                   return _WriteTxtC1(miff, (MiffC1 *) "abi8"    );
   case miffTypeABN1:                   return _WriteTxtC1(miff, (MiffC1 *) "abn1"    );
   case miffTypeABN2:                   return _WriteTxtC1(miff, (MiffC1 *) "abn2"    );
   case miffTypeABN4:                   return _WriteTxtC1(miff, (MiffC1 *) "abn4"    );
   case miffTypeABN8:                   return _WriteTxtC1(miff, (MiffC1 *) "abn8"    );
   case miffTypeABR4:                   return _WriteTxtC1(miff, (MiffC1 *) "abr4"    );
   case miffTypeABR8:                   return _WriteTxtC1(miff, (MiffC1 *) "abr8"    );
   case miffTypeABCI1:                  return _WriteTxtC1(miff, (MiffC1 *) "abci1"   );
   case miffTypeABCI2:                  return _WriteTxtC1(miff, (MiffC1 *) "abci2"   );
   case miffTypeABCI4:                  return _WriteTxtC1(miff, (MiffC1 *) "abci4"   );
   case miffTypeABCI8:                  return _WriteTxtC1(miff, (MiffC1 *) "abci8"   );
   case miffTypeABCN1:                  return _WriteTxtC1(miff, (MiffC1 *) "abcn1"   );
   case miffTypeABCN2:                  return _WriteTxtC1(miff, (MiffC1 *) "abcn2"   );
   case miffTypeABCN4:                  return _WriteTxtC1(miff, (MiffC1 *) "abcn4"   );
   case miffTypeABCN8:                  return _WriteTxtC1(miff, (MiffC1 *) "abcn8"   );
   case miffTypeABCR4:                  return _WriteTxtC1(miff, (MiffC1 *) "abcr4"   );
   case miffTypeABCR8:                  return _WriteTxtC1(miff, (MiffC1 *) "abcr8"   );
   case miffTypeABCDI1:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdi1"  );
   case miffTypeABCDI2:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdi2"  );
   case miffTypeABCDI4:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdi4"  );
   case miffTypeABCDI8:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdi8"  );
   case miffTypeABCDN1:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdn1"  );
   case miffTypeABCDN2:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdn2"  );
   case miffTypeABCDN4:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdn4"  );
   case miffTypeABCDN8:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdn8"  );
   case miffTypeABCDR4:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdr4"  );
   case miffTypeABCDR8:                 return _WriteTxtC1(miff, (MiffC1 *) "abcdr8"  );
   case miffTypeMATRIX2X2R4:            return _WriteTxtC1(miff, (MiffC1 *) "mat2x2r4");
   case miffTypeMATRIX2X2R8:            return _WriteTxtC1(miff, (MiffC1 *) "mat2x2r8");
   case miffTypeMATRIX3X3R4:            return _WriteTxtC1(miff, (MiffC1 *) "mat3x3r4");
   case miffTypeMATRIX3X3R8:            return _WriteTxtC1(miff, (MiffC1 *) "mat3x3r8");
   case miffTypeMATRIX4X4R4:            return _WriteTxtC1(miff, (MiffC1 *) "mat4x4r4");
   case miffTypeMATRIX4X4R8:            return _WriteTxtC1(miff, (MiffC1 *) "mat4x4r8");
      
   }

   returnFalse;
}

/******************************************************************************
func: _WriteTxtValue1
******************************************************************************/
MiffBool _WriteTxtValue1(Miff const * const miff, MiffType const type, Miff1 const value)
{
   if (type == miffTypeBOOLEAN)
   {
      if (value.n)
      {
         return _WriteTxtC1(miff, (MiffC1 *) "T");
      }

      return _WriteTxtC1(miff, (MiffC1 *) "F");
   }

   if (type == miffTypeI1)
   {
      return _WriteTxtValueI(miff, (MiffI8) value.i);
   }

   return _WriteTxtValueN(miff, (MiffN8) value.n);
}

/******************************************************************************
func: _WriteTxtValue2
******************************************************************************/
MiffBool _WriteTxtValue2(Miff const * const miff, MiffType const type, Miff2 const value)
{
   if (type == miffTypeI2)
   {
      return _WriteTxtValueI(miff, (MiffI8) value.i);
   }

   return _WriteTxtValueN(miff, (MiffN8) value.n);
}

/******************************************************************************
func: _WriteTxtValue4
******************************************************************************/
MiffBool _WriteTxtValue4(Miff const * const miff, MiffType const type, Miff4 const value)
{
   if (type == miffTypeR4)
   {
      Miff4 vtemp;

      vtemp = value;
      _ByteSwap4(miff, &vtemp);

      returnFalseIf(!_Base64Set(miff, value.byte[0]));
      returnFalseIf(!_Base64Set(miff, value.byte[1]));
      returnFalseIf(!_Base64Set(miff, value.byte[2]));
      returnFalseIf(!_Base64Set(miff, value.byte[3]));

      return _Base64SetEnd(miff);
   }

   if (type == miffTypeI4)
   {
      return _WriteTxtValueI(miff, (MiffI8) value.i);
   }

   return _WriteTxtValueN(miff, (MiffN8) value.n);
}

/******************************************************************************
func: _WriteTxtValue8
******************************************************************************/
MiffBool _WriteTxtValue8(Miff const * const miff, MiffType const type, Miff8 const value)
{
   if (type == miffTypeR8)
   {
      Miff8 vtemp;

      vtemp = value;
      _ByteSwap8(miff, &vtemp);

      returnFalseIf(!_Base64Set(miff, value.byte[0]));
      returnFalseIf(!_Base64Set(miff, value.byte[1]));
      returnFalseIf(!_Base64Set(miff, value.byte[2]));
      returnFalseIf(!_Base64Set(miff, value.byte[3]));
      returnFalseIf(!_Base64Set(miff, value.byte[4]));
      returnFalseIf(!_Base64Set(miff, value.byte[5]));
      returnFalseIf(!_Base64Set(miff, value.byte[6]));
      returnFalseIf(!_Base64Set(miff, value.byte[7]));

      return _Base64SetEnd(miff);
   }

   if (type == miffTypeI8)
   {
      return _WriteTxtValueI(miff, value.i);
   }

   return _WriteTxtValueN(miff, value.n);
}

/******************************************************************************
func: _WriteTxtValueI
******************************************************************************/
MiffBool _WriteTxtValueI(Miff const * const miff, MiffI8 const value)
{
   MiffN8 ntemp;

   ntemp = 0;
   if (value < 0)
   {
      miff->setBuffer(miff->dataRepo, 1, (MiffN1 *) "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _WriteTxtValueN(miff, ntemp);
}

/******************************************************************************
func: _WriteTxtValueN
******************************************************************************/
MiffBool _WriteTxtValueN(Miff const * const miff, MiffN8 const value)
{
   int    index,
          count,
          digit;
   MiffN8 temp;
   MiffN1 string[32];

   temp = value;
   for (index = 0; ; index++)
   {
      digit = temp % 10;
      temp  = temp / 10;

      string[index] = (MiffN1) ('0' + digit);

      breakIf(temp == 0);
   }

   count = index + 1;
   forCountDown (index, count)
   {
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &string[index]));
   }

   returnTrue;
}

/******************************************************************************
func: _WriteTxtValueType
******************************************************************************/
MiffBool _WriteTxtValueType(Miff const * const miff, MiffType const value)
{
   return _WriteTxtType(miff, value);
}

/******************************************************************************
func: _WriteTxtValueC2
******************************************************************************/
MiffBool _WriteTxtValueC2(Miff const * const miff, MiffC2 const * const value)
{
   MiffC1   *c1,
            *c1e;
   MiffN4    c1Count,
             c1eCount;
   MiffBool  result;

   c1     =
      c1e = NULL;
   result = miffBoolFALSE;
   once
   {
      breakIf(!_C2ToC1(_C2GetCount(value), value, &c1Count, &c1));

      breakIf(!_C1ToC1Encoded(_C1GetCount(c1), c1, &c1eCount, &c1e));

      breakIf(!_WriteTxtC1(miff, c1e));

      result = miffBoolTRUE;
   }

   _MemDestroy(c1e);
   _MemDestroy(c1);

   returnTrue;
}

/******************************************************************************
func: _WriteValue1
******************************************************************************/
MiffBool _WriteValue1(Miff * const miff, MiffType const type, Miff1 value)
{
   returnFalseIf(
      !(miff->currentRecord.type == type ||
        miff->currentRecord.type == miffTypeVARIABLE));

   if (!miff->isCompressed)
   {
      // Write out the value.
      returnFalseIf(!_WriteTxtValue1(miff, type, value));
      returnTrue;
   }
    
   // Populate the internal buffer before compression.
   returnFalseIf(!_WriteCompressByte(miff, value.byte[0]));
   returnTrue;
}

/******************************************************************************
func: _WriteValue2
******************************************************************************/
MiffBool _WriteValue2(Miff * const miff, MiffType const type, Miff2 value)
{
   returnFalseIf(
      !(miff->currentRecord.type == type ||
        miff->currentRecord.type == miffTypeVARIABLE));

   if (!miff->isCompressed)
   {
      // Write out the value
      returnFalseIf(!_WriteTxtValue2(miff, type, value));
      returnTrue;
   }

   // Ensure proper byte order.
   _ByteSwap2(miff, &value);
   
   // Populate the internal buffer before compression.
   returnFalseIf(!_WriteCompressByte(miff, value.byte[0]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[1]));
   returnTrue;
}

/******************************************************************************
func: _WriteValue4
******************************************************************************/
MiffBool _WriteValue4(Miff * const miff, MiffType const type, Miff4 value)
{
   returnFalseIf(
      !(miff->currentRecord.type == type ||
        miff->currentRecord.type == miffTypeVARIABLE));

   if (!miff->isCompressed)
   {
      // Write out the value
      returnFalseIf(!_WriteTxtValue4(miff, type, value));
      returnTrue;
   }

   // Ensure proper byte order.
   _ByteSwap4(miff, &value);

   // Populate the internal buffer before compression.
   returnFalseIf(!_WriteCompressByte(miff, value.byte[0]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[1]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[2]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[3]));

   returnTrue;
}

/******************************************************************************
func: _WriteValue8
******************************************************************************/
MiffBool _WriteValue8(Miff * const miff, MiffType const type, Miff8 value)
{
   returnFalseIf(
      !(miff->currentRecord.type == type ||
        miff->currentRecord.type == miffTypeVARIABLE));

   if (!miff->isCompressed)
   {
      // Write out the value
      returnFalseIf(!_WriteTxtValue8(miff, type, value));
      returnTrue;
   }

   // Ensure proper byte order.
   _ByteSwap8(miff, &value);

   // Populate the internal buffer before compression.
   returnFalseIf(!_WriteCompressByte(miff, value.byte[0]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[1]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[2]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[3]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[4]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[5]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[6]));
   returnFalseIf(!_WriteCompressByte(miff, value.byte[7]));

   returnTrue;
}
