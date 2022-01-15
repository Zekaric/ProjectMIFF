/******************************************************************************
file:       util
author:     Robbert de Groot
company:
copyright:  2021, Robbert de Groot

description:
General utils
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
func: _TypeGetName
******************************************************************************/
MiffC2 *_TypeGetNameC2(MiffType const type)
{
   switch (type)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return U_LITERAL(miffTypeKEY_VALUE_BLOCK_START_STR);
   case miffTypeKEY_VALUE_BLOCK_STOP:   return U_LITERAL(miffTypeKEY_VALUE_BLOCK_STOP_STR);
   case miffTypeTYPE:                   return U_LITERAL(miffTypeTYPE_STR);
   case miffTypeSTRING:                 return U_LITERAL(miffTypeSTRING_STR);
   case miffTypeVARIABLE:               return U_LITERAL(miffTypeVARIABLE_STR);
   case miffTypeBOOLEAN:                return U_LITERAL(miffTypeBOOLEAN_STR);
   case miffTypeI1:                     return U_LITERAL(miffTypeI1_STR);
   case miffTypeI2:                     return U_LITERAL(miffTypeI2_STR);
   case miffTypeI4:                     return U_LITERAL(miffTypeI4_STR);
   case miffTypeI8:                     return U_LITERAL(miffTypeI8_STR);
   case miffTypeI16:                    return U_LITERAL(miffTypeI16_STR);
   case miffTypeI32:                    return U_LITERAL(miffTypeI32_STR);
   case miffTypeI64:                    return U_LITERAL(miffTypeI64_STR);
   case miffTypeI128:                   return U_LITERAL(miffTypeI128_STR);
   case miffTypeI256:                   return U_LITERAL(miffTypeI256_STR);
   case miffTypeN1:                     return U_LITERAL(miffTypeN1_STR);
   case miffTypeN2:                     return U_LITERAL(miffTypeN2_STR);
   case miffTypeN4:                     return U_LITERAL(miffTypeN4_STR);
   case miffTypeN8:                     return U_LITERAL(miffTypeN8_STR);
   case miffTypeN16:                    return U_LITERAL(miffTypeN16_STR);
   case miffTypeN32:                    return U_LITERAL(miffTypeN32_STR);
   case miffTypeN64:                    return U_LITERAL(miffTypeN64_STR);
   case miffTypeN128:                   return U_LITERAL(miffTypeN128_STR);
   case miffTypeN256:                   return U_LITERAL(miffTypeN256_STR);
   case miffTypeR4:                     return U_LITERAL(miffTypeR4_STR);
   case miffTypeR8:                     return U_LITERAL(miffTypeR8_STR);
   case miffTypeABI1:                   return U_LITERAL(miffTypeABI1_STR);
   case miffTypeABI2:                   return U_LITERAL(miffTypeABI2_STR);
   case miffTypeABI4:                   return U_LITERAL(miffTypeABI4_STR);
   case miffTypeABI8:                   return U_LITERAL(miffTypeABI8_STR);
   case miffTypeABN1:                   return U_LITERAL(miffTypeABN1_STR);
   case miffTypeABN2:                   return U_LITERAL(miffTypeABN2_STR);
   case miffTypeABN4:                   return U_LITERAL(miffTypeABN4_STR);
   case miffTypeABN8:                   return U_LITERAL(miffTypeABN8_STR);
   case miffTypeABR4:                   return U_LITERAL(miffTypeABR4_STR);
   case miffTypeABR8:                   return U_LITERAL(miffTypeABR8_STR);
   case miffTypeABCI1:                  return U_LITERAL(miffTypeABCI1_STR);
   case miffTypeABCI2:                  return U_LITERAL(miffTypeABCI2_STR);
   case miffTypeABCI4:                  return U_LITERAL(miffTypeABCI4_STR);
   case miffTypeABCI8:                  return U_LITERAL(miffTypeABCI8_STR);
   case miffTypeABCN1:                  return U_LITERAL(miffTypeABCN1_STR);
   case miffTypeABCN2:                  return U_LITERAL(miffTypeABCN2_STR);
   case miffTypeABCN4:                  return U_LITERAL(miffTypeABCN4_STR);
   case miffTypeABCN8:                  return U_LITERAL(miffTypeABCN8_STR);
   case miffTypeABCR4:                  return U_LITERAL(miffTypeABCR4_STR);
   case miffTypeABCR8:                  return U_LITERAL(miffTypeABCR8_STR);
   case miffTypeABCDI1:                 return U_LITERAL(miffTypeABCDI1_STR);
   case miffTypeABCDI2:                 return U_LITERAL(miffTypeABCDI2_STR);
   case miffTypeABCDI4:                 return U_LITERAL(miffTypeABCDI4_STR);
   case miffTypeABCDI8:                 return U_LITERAL(miffTypeABCDI8_STR);
   case miffTypeABCDN1:                 return U_LITERAL(miffTypeABCDN1_STR);
   case miffTypeABCDN2:                 return U_LITERAL(miffTypeABCDN2_STR);
   case miffTypeABCDN4:                 return U_LITERAL(miffTypeABCDN4_STR);
   case miffTypeABCDN8:                 return U_LITERAL(miffTypeABCDN8_STR);
   case miffTypeABCDR4:                 return U_LITERAL(miffTypeABCDR4_STR);
   case miffTypeABCDR8:                 return U_LITERAL(miffTypeABCDR8_STR);
   case miffTypeMATRIX2X2R4:            return U_LITERAL(miffTypeMATRIX2X2R4_STR);
   case miffTypeMATRIX2X2R8:            return U_LITERAL(miffTypeMATRIX2X2R8_STR);
   case miffTypeMATRIX3X3R4:            return U_LITERAL(miffTypeMATRIX3X3R4_STR);
   case miffTypeMATRIX3X3R8:            return U_LITERAL(miffTypeMATRIX3X3R8_STR);
   case miffTypeMATRIX4X4R4:            return U_LITERAL(miffTypeMATRIX4X4R4_STR);
   case miffTypeMATRIX4X4R8:            return U_LITERAL(miffTypeMATRIX4X4R8_STR);
   }

   return L"";
}

/******************************************************************************
func: _TypeGetNameC1
******************************************************************************/
MiffC1 *_TypeGetNameC1(MiffType const type)
{
   switch (type)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return (MiffC1 *) miffTypeKEY_VALUE_BLOCK_START_STR;
   case miffTypeKEY_VALUE_BLOCK_STOP:   return (MiffC1 *) miffTypeKEY_VALUE_BLOCK_STOP_STR;
   case miffTypeTYPE:                   return (MiffC1 *) miffTypeTYPE_STR;
   case miffTypeSTRING:                 return (MiffC1 *) miffTypeSTRING_STR;
   case miffTypeVARIABLE:               return (MiffC1 *) miffTypeVARIABLE_STR;
   case miffTypeBOOLEAN:                return (MiffC1 *) miffTypeBOOLEAN_STR;
   case miffTypeI1:                     return (MiffC1 *) miffTypeI1_STR;
   case miffTypeI2:                     return (MiffC1 *) miffTypeI2_STR;
   case miffTypeI4:                     return (MiffC1 *) miffTypeI4_STR;
   case miffTypeI8:                     return (MiffC1 *) miffTypeI8_STR;
   case miffTypeI16:                    return (MiffC1 *) miffTypeI16_STR;
   case miffTypeI32:                    return (MiffC1 *) miffTypeI32_STR;
   case miffTypeI64:                    return (MiffC1 *) miffTypeI64_STR;
   case miffTypeI128:                   return (MiffC1 *) miffTypeI128_STR;
   case miffTypeI256:                   return (MiffC1 *) miffTypeI256_STR;
   case miffTypeN1:                     return (MiffC1 *) miffTypeN1_STR;
   case miffTypeN2:                     return (MiffC1 *) miffTypeN2_STR;
   case miffTypeN4:                     return (MiffC1 *) miffTypeN4_STR;
   case miffTypeN8:                     return (MiffC1 *) miffTypeN8_STR;
   case miffTypeN16:                    return (MiffC1 *) miffTypeN16_STR;
   case miffTypeN32:                    return (MiffC1 *) miffTypeN32_STR;
   case miffTypeN64:                    return (MiffC1 *) miffTypeN64_STR;
   case miffTypeN128:                   return (MiffC1 *) miffTypeN128_STR;
   case miffTypeN256:                   return (MiffC1 *) miffTypeN256_STR;
   case miffTypeR4:                     return (MiffC1 *) miffTypeR4_STR;
   case miffTypeR8:                     return (MiffC1 *) miffTypeR8_STR;
   case miffTypeABI1:                   return (MiffC1 *) miffTypeABI1_STR;
   case miffTypeABI2:                   return (MiffC1 *) miffTypeABI2_STR;
   case miffTypeABI4:                   return (MiffC1 *) miffTypeABI4_STR;
   case miffTypeABI8:                   return (MiffC1 *) miffTypeABI8_STR;
   case miffTypeABN1:                   return (MiffC1 *) miffTypeABN1_STR;
   case miffTypeABN2:                   return (MiffC1 *) miffTypeABN2_STR;
   case miffTypeABN4:                   return (MiffC1 *) miffTypeABN4_STR;
   case miffTypeABN8:                   return (MiffC1 *) miffTypeABN8_STR;
   case miffTypeABR4:                   return (MiffC1 *) miffTypeABR4_STR;
   case miffTypeABR8:                   return (MiffC1 *) miffTypeABR8_STR;
   case miffTypeABCI1:                  return (MiffC1 *) miffTypeABCI1_STR;
   case miffTypeABCI2:                  return (MiffC1 *) miffTypeABCI2_STR;
   case miffTypeABCI4:                  return (MiffC1 *) miffTypeABCI4_STR;
   case miffTypeABCI8:                  return (MiffC1 *) miffTypeABCI8_STR;
   case miffTypeABCN1:                  return (MiffC1 *) miffTypeABCN1_STR;
   case miffTypeABCN2:                  return (MiffC1 *) miffTypeABCN2_STR;
   case miffTypeABCN4:                  return (MiffC1 *) miffTypeABCN4_STR;
   case miffTypeABCN8:                  return (MiffC1 *) miffTypeABCN8_STR;
   case miffTypeABCR4:                  return (MiffC1 *) miffTypeABCR4_STR;
   case miffTypeABCR8:                  return (MiffC1 *) miffTypeABCR8_STR;
   case miffTypeABCDI1:                 return (MiffC1 *) miffTypeABCDI1_STR;
   case miffTypeABCDI2:                 return (MiffC1 *) miffTypeABCDI2_STR;
   case miffTypeABCDI4:                 return (MiffC1 *) miffTypeABCDI4_STR;
   case miffTypeABCDI8:                 return (MiffC1 *) miffTypeABCDI8_STR;
   case miffTypeABCDN1:                 return (MiffC1 *) miffTypeABCDN1_STR;
   case miffTypeABCDN2:                 return (MiffC1 *) miffTypeABCDN2_STR;
   case miffTypeABCDN4:                 return (MiffC1 *) miffTypeABCDN4_STR;
   case miffTypeABCDN8:                 return (MiffC1 *) miffTypeABCDN8_STR;
   case miffTypeABCDR4:                 return (MiffC1 *) miffTypeABCDR4_STR;
   case miffTypeABCDR8:                 return (MiffC1 *) miffTypeABCDR8_STR;
   case miffTypeMATRIX2X2R4:            return (MiffC1 *) miffTypeMATRIX2X2R4_STR;
   case miffTypeMATRIX2X2R8:            return (MiffC1 *) miffTypeMATRIX2X2R8_STR;
   case miffTypeMATRIX3X3R4:            return (MiffC1 *) miffTypeMATRIX3X3R4_STR;
   case miffTypeMATRIX3X3R8:            return (MiffC1 *) miffTypeMATRIX3X3R8_STR;
   case miffTypeMATRIX4X4R4:            return (MiffC1 *) miffTypeMATRIX4X4R4_STR;
   case miffTypeMATRIX4X4R8:            return (MiffC1 *) miffTypeMATRIX4X4R8_STR;
   }

   return (MiffC1 *) "";
}

/******************************************************************************
func: _TypeGetNameSize
******************************************************************************/
MiffN4 _TypeGetNameSize(MiffType const type)
{
   switch (type)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return miffTypeKEY_VALUE_BLOCK_START_STR_SIZE;
   case miffTypeKEY_VALUE_BLOCK_STOP:   return miffTypeKEY_VALUE_BLOCK_STOP_STR_SIZE;
   case miffTypeTYPE:                   return miffTypeTYPE_STR_SIZE;
   case miffTypeSTRING:                 return miffTypeSTRING_STR_SIZE;
   case miffTypeVARIABLE:               return miffTypeVARIABLE_STR_SIZE;
   case miffTypeBOOLEAN:                return miffTypeBOOLEAN_STR_SIZE;
   case miffTypeI1:                     return miffTypeI1_STR_SIZE;
   case miffTypeI2:                     return miffTypeI2_STR_SIZE;
   case miffTypeI4:                     return miffTypeI4_STR_SIZE;
   case miffTypeI8:                     return miffTypeI8_STR_SIZE;
   case miffTypeI16:                    return miffTypeI16_STR_SIZE;
   case miffTypeI32:                    return miffTypeI32_STR_SIZE;
   case miffTypeI64:                    return miffTypeI64_STR_SIZE;
   case miffTypeI128:                   return miffTypeI128_STR_SIZE;
   case miffTypeI256:                   return miffTypeI256_STR_SIZE;
   case miffTypeN1:                     return miffTypeN1_STR_SIZE;
   case miffTypeN2:                     return miffTypeN2_STR_SIZE;
   case miffTypeN4:                     return miffTypeN4_STR_SIZE;
   case miffTypeN8:                     return miffTypeN8_STR_SIZE;
   case miffTypeN16:                    return miffTypeN16_STR_SIZE;
   case miffTypeN32:                    return miffTypeN32_STR_SIZE;
   case miffTypeN64:                    return miffTypeN64_STR_SIZE;
   case miffTypeN128:                   return miffTypeN128_STR_SIZE;
   case miffTypeN256:                   return miffTypeN256_STR_SIZE;
   case miffTypeR4:                     return miffTypeR4_STR_SIZE;
   case miffTypeR8:                     return miffTypeR8_STR_SIZE;
   case miffTypeABI1:                   return miffTypeABI1_STR_SIZE;
   case miffTypeABI2:                   return miffTypeABI2_STR_SIZE;
   case miffTypeABI4:                   return miffTypeABI4_STR_SIZE;
   case miffTypeABI8:                   return miffTypeABI8_STR_SIZE;
   case miffTypeABN1:                   return miffTypeABN1_STR_SIZE;
   case miffTypeABN2:                   return miffTypeABN2_STR_SIZE;
   case miffTypeABN4:                   return miffTypeABN4_STR_SIZE;
   case miffTypeABN8:                   return miffTypeABN8_STR_SIZE;
   case miffTypeABR4:                   return miffTypeABR4_STR_SIZE;
   case miffTypeABR8:                   return miffTypeABR8_STR_SIZE;
   case miffTypeABCI1:                  return miffTypeABCI1_STR_SIZE;
   case miffTypeABCI2:                  return miffTypeABCI2_STR_SIZE;
   case miffTypeABCI4:                  return miffTypeABCI4_STR_SIZE;
   case miffTypeABCI8:                  return miffTypeABCI8_STR_SIZE;
   case miffTypeABCN1:                  return miffTypeABCN1_STR_SIZE;
   case miffTypeABCN2:                  return miffTypeABCN2_STR_SIZE;
   case miffTypeABCN4:                  return miffTypeABCN4_STR_SIZE;
   case miffTypeABCN8:                  return miffTypeABCN8_STR_SIZE;
   case miffTypeABCR4:                  return miffTypeABCR4_STR_SIZE;
   case miffTypeABCR8:                  return miffTypeABCR8_STR_SIZE;
   case miffTypeABCDI1:                 return miffTypeABCDI1_STR_SIZE;
   case miffTypeABCDI2:                 return miffTypeABCDI2_STR_SIZE;
   case miffTypeABCDI4:                 return miffTypeABCDI4_STR_SIZE;
   case miffTypeABCDI8:                 return miffTypeABCDI8_STR_SIZE;
   case miffTypeABCDN1:                 return miffTypeABCDN1_STR_SIZE;
   case miffTypeABCDN2:                 return miffTypeABCDN2_STR_SIZE;
   case miffTypeABCDN4:                 return miffTypeABCDN4_STR_SIZE;
   case miffTypeABCDN8:                 return miffTypeABCDN8_STR_SIZE;
   case miffTypeABCDR4:                 return miffTypeABCDR4_STR_SIZE;
   case miffTypeABCDR8:                 return miffTypeABCDR8_STR_SIZE;
   case miffTypeMATRIX2X2R4:            return miffTypeMATRIX2X2R4_STR_SIZE;
   case miffTypeMATRIX2X2R8:            return miffTypeMATRIX2X2R8_STR_SIZE;
   case miffTypeMATRIX3X3R4:            return miffTypeMATRIX3X3R4_STR_SIZE;
   case miffTypeMATRIX3X3R8:            return miffTypeMATRIX3X3R8_STR_SIZE;
   case miffTypeMATRIX4X4R4:            return miffTypeMATRIX4X4R4_STR_SIZE;
   case miffTypeMATRIX4X4R8:            return miffTypeMATRIX4X4R8_STR_SIZE;
   }

   return 0;
}
