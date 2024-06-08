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
local:
variable:
******************************************************************************/
static _locale_t  _locale;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffUtilStart
******************************************************************************/
void _MiffUtilStart(void)
{
   _locale = _create_locale(LC_ALL, "C");
}

/******************************************************************************
func: _MiffUtilStop
******************************************************************************/
void _MiffUtilStop(void)
{
   // Nothing to do.
}

/******************************************************************************
func: _MiffLocaleGet
******************************************************************************/
_locale_t _MiffLocaleGet(void)
{
   return _locale;
}

/******************************************************************************
func: _MiffMemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
MiffBool _MiffMemIsEqual(MiffN const countA, MiffN1 const * const memA, MiffN const countB,
   MiffN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}

/******************************************************************************
func: _MiffTypeGetName
******************************************************************************/
MiffStr *_MiffTypeGetName(MiffType const type)
{
   switch (type)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return (MiffStr *) miffTypeKEY_VALUE_BLOCK_START_STR;
   case miffTypeKEY_VALUE_BLOCK_STOP:   return (MiffStr *) miffTypeKEY_VALUE_BLOCK_STOP_STR;
   case miffTypeTYPE:                   return (MiffStr *) miffTypeTYPE_STR;
   case miffTypeSTR:                    return (MiffStr *) miffTypeSTR_STR;
   case miffTypeVARIABLE:               return (MiffStr *) miffTypeVARIABLE_STR;
   case miffTypeUSER_TYPE:              return (MiffStr *) miffTypeUSER_TYPE_STR;
   case miffTypeBOOLEAN:                return (MiffStr *) miffTypeBOOLEAN_STR;
   case miffTypeI1:                     return (MiffStr *) miffTypeI1_STR;
   case miffTypeI2:                     return (MiffStr *) miffTypeI2_STR;
   case miffTypeI4:                     return (MiffStr *) miffTypeI4_STR;
   case miffTypeI8:                     return (MiffStr *) miffTypeI8_STR;
   case miffTypeI:                      return (MiffStr *) miffTypeI_STR;
   case miffTypeN1:                     return (MiffStr *) miffTypeN1_STR;
   case miffTypeN2:                     return (MiffStr *) miffTypeN2_STR;
   case miffTypeN4:                     return (MiffStr *) miffTypeN4_STR;
   case miffTypeN8:                     return (MiffStr *) miffTypeN8_STR;
   case miffTypeN:                      return (MiffStr *) miffTypeN_STR;
   case miffTypeR4:                     return (MiffStr *) miffTypeR4_STR;
   case miffTypeR4S:                    return (MiffStr *) miffTypeR4S_STR;
   case miffTypeR8:                     return (MiffStr *) miffTypeR8_STR;
   case miffTypeR8S:                    return (MiffStr *) miffTypeR8S_STR;
#if 0
   case miffTypeABI1:                   return miffTypeABI1_STR);
   case miffTypeABI2:                   return miffTypeABI2_STR);
   case miffTypeABI4:                   return miffTypeABI4_STR);
   case miffTypeABI8:                   return miffTypeABI8_STR);
   case miffTypeABN1:                   return miffTypeABN1_STR);
   case miffTypeABN2:                   return miffTypeABN2_STR);
   case miffTypeABN4:                   return miffTypeABN4_STR);
   case miffTypeABN8:                   return miffTypeABN8_STR);
   case miffTypeABR4:                   return miffTypeABR4_STR);
   case miffTypeABR4S:                  return miffTypeABR4S_STR);
   case miffTypeABR8:                   return miffTypeABR8_STR);
   case miffTypeABR8S:                  return miffTypeABR8S_STR);
   case miffTypeABCI1:                  return miffTypeABCI1_STR);
   case miffTypeABCI2:                  return miffTypeABCI2_STR);
   case miffTypeABCI4:                  return miffTypeABCI4_STR);
   case miffTypeABCI8:                  return miffTypeABCI8_STR);
   case miffTypeABCN1:                  return miffTypeABCN1_STR);
   case miffTypeABCN2:                  return miffTypeABCN2_STR);
   case miffTypeABCN4:                  return miffTypeABCN4_STR);
   case miffTypeABCN8:                  return miffTypeABCN8_STR);
   case miffTypeABCR4:                  return miffTypeABCR4_STR);
   case miffTypeABCR4S:                 return miffTypeABCR4S_STR);
   case miffTypeABCR8:                  return miffTypeABCR8_STR);
   case miffTypeABCR8S:                 return miffTypeABCR8S_STR);
   case miffTypeABCDI1:                 return miffTypeABCDI1_STR);
   case miffTypeABCDI2:                 return miffTypeABCDI2_STR);
   case miffTypeABCDI4:                 return miffTypeABCDI4_STR);
   case miffTypeABCDI8:                 return miffTypeABCDI8_STR);
   case miffTypeABCDN1:                 return miffTypeABCDN1_STR);
   case miffTypeABCDN2:                 return miffTypeABCDN2_STR);
   case miffTypeABCDN4:                 return miffTypeABCDN4_STR);
   case miffTypeABCDN8:                 return miffTypeABCDN8_STR);
   case miffTypeABCDR4:                 return miffTypeABCDR4_STR);
   case miffTypeABCDR4S:                return miffTypeABCDR4S_STR);
   case miffTypeABCDR8:                 return miffTypeABCDR8_STR);
   case miffTypeABCDR8S:                return miffTypeABCDR8S_STR);
   case miffTypeMATRIX2X2R4:            return miffTypeMATRIX2X2R4_STR);
   case miffTypeMATRIX2X2R4S:           return miffTypeMATRIX2X2R4S_STR);
   case miffTypeMATRIX2X2R8:            return miffTypeMATRIX2X2R8_STR);
   case miffTypeMATRIX2X2R8S:           return miffTypeMATRIX2X2R8S_STR);
   case miffTypeMATRIX3X3R4:            return miffTypeMATRIX3X3R4_STR);
   case miffTypeMATRIX3X3R4S:           return miffTypeMATRIX3X3R4S_STR);
   case miffTypeMATRIX3X3R8:            return miffTypeMATRIX3X3R8_STR);
   case miffTypeMATRIX3X3R8S:           return miffTypeMATRIX3X3R8S_STR);
   case miffTypeMATRIX4X4R4:            return miffTypeMATRIX4X4R4_STR);
   case miffTypeMATRIX4X4R4S:           return miffTypeMATRIX4X4R4S_STR);
   case miffTypeMATRIX4X4R8:            return miffTypeMATRIX4X4R8_STR);
   case miffTypeMATRIX4X4R8S:           return miffTypeMATRIX4X4R8S_STR);
#endif
   }

   return (MiffStr *) "";
}

/******************************************************************************
func: _MiffTypeGetNameSize
******************************************************************************/
MiffN4 _MiffTypeGetNameSize(MiffType const type)
{
   switch (type)
   {
   case miffTypeKEY_VALUE_BLOCK_START:  return miffTypeKEY_VALUE_BLOCK_START_STR_SIZE;
   case miffTypeKEY_VALUE_BLOCK_STOP:   return miffTypeKEY_VALUE_BLOCK_STOP_STR_SIZE;
   case miffTypeTYPE:                   return miffTypeTYPE_STR_SIZE;
   case miffTypeSTR:                    return miffTypeSTR_STR_SIZE;
   case miffTypeVARIABLE:               return miffTypeVARIABLE_STR_SIZE;
   case miffTypeUSER_TYPE:              return miffTypeUSER_TYPE_STR_SIZE;
   case miffTypeBOOLEAN:                return miffTypeBOOLEAN_STR_SIZE;
   case miffTypeI1:                     return miffTypeI1_STR_SIZE;
   case miffTypeI2:                     return miffTypeI2_STR_SIZE;
   case miffTypeI4:                     return miffTypeI4_STR_SIZE;
   case miffTypeI8:                     return miffTypeI8_STR_SIZE;
   case miffTypeN1:                     return miffTypeN1_STR_SIZE;
   case miffTypeN2:                     return miffTypeN2_STR_SIZE;
   case miffTypeN4:                     return miffTypeN4_STR_SIZE;
   case miffTypeN8:                     return miffTypeN8_STR_SIZE;
   case miffTypeR4:                     return miffTypeR4_STR_SIZE;
   case miffTypeR4S:                    return miffTypeR4S_STR_SIZE;
   case miffTypeR8:                     return miffTypeR8_STR_SIZE;
   case miffTypeR8S:                    return miffTypeR8S_STR_SIZE;
#if 0
   case miffTypeABI1:                   return miffTypeABI1_STR_SIZE;
   case miffTypeABI2:                   return miffTypeABI2_STR_SIZE;
   case miffTypeABI4:                   return miffTypeABI4_STR_SIZE;
   case miffTypeABI8:                   return miffTypeABI8_STR_SIZE;
   case miffTypeABN1:                   return miffTypeABN1_STR_SIZE;
   case miffTypeABN2:                   return miffTypeABN2_STR_SIZE;
   case miffTypeABN4:                   return miffTypeABN4_STR_SIZE;
   case miffTypeABN8:                   return miffTypeABN8_STR_SIZE;
   case miffTypeABR4:                   return miffTypeABR4_STR_SIZE;
   case miffTypeABR4S:                  return miffTypeABR4S_STR_SIZE;
   case miffTypeABR8:                   return miffTypeABR8_STR_SIZE;
   case miffTypeABR8S:                  return miffTypeABR8S_STR_SIZE;
   case miffTypeABCI1:                  return miffTypeABCI1_STR_SIZE;
   case miffTypeABCI2:                  return miffTypeABCI2_STR_SIZE;
   case miffTypeABCI4:                  return miffTypeABCI4_STR_SIZE;
   case miffTypeABCI8:                  return miffTypeABCI8_STR_SIZE;
   case miffTypeABCN1:                  return miffTypeABCN1_STR_SIZE;
   case miffTypeABCN2:                  return miffTypeABCN2_STR_SIZE;
   case miffTypeABCN4:                  return miffTypeABCN4_STR_SIZE;
   case miffTypeABCN8:                  return miffTypeABCN8_STR_SIZE;
   case miffTypeABCR4:                  return miffTypeABCR4_STR_SIZE;
   case miffTypeABCR4S:                 return miffTypeABCR4S_STR_SIZE;
   case miffTypeABCR8:                  return miffTypeABCR8_STR_SIZE;
   case miffTypeABCR8S:                 return miffTypeABCR8S_STR_SIZE;
   case miffTypeABCDI1:                 return miffTypeABCDI1_STR_SIZE;
   case miffTypeABCDI2:                 return miffTypeABCDI2_STR_SIZE;
   case miffTypeABCDI4:                 return miffTypeABCDI4_STR_SIZE;
   case miffTypeABCDI8:                 return miffTypeABCDI8_STR_SIZE;
   case miffTypeABCDN1:                 return miffTypeABCDN1_STR_SIZE;
   case miffTypeABCDN2:                 return miffTypeABCDN2_STR_SIZE;
   case miffTypeABCDN4:                 return miffTypeABCDN4_STR_SIZE;
   case miffTypeABCDN8:                 return miffTypeABCDN8_STR_SIZE;
   case miffTypeABCDR4:                 return miffTypeABCDR4_STR_SIZE;
   case miffTypeABCDR4S:                return miffTypeABCDR4S_STR_SIZE;
   case miffTypeABCDR8:                 return miffTypeABCDR8_STR_SIZE;
   case miffTypeABCDR8S:                return miffTypeABCDR8S_STR_SIZE;
   case miffTypeMATRIX2X2R4:            return miffTypeMATRIX2X2R4_STR_SIZE;
   case miffTypeMATRIX2X2R4S:           return miffTypeMATRIX2X2R4S_STR_SIZE;
   case miffTypeMATRIX2X2R8:            return miffTypeMATRIX2X2R8_STR_SIZE;
   case miffTypeMATRIX2X2R8S:           return miffTypeMATRIX2X2R8S_STR_SIZE;
   case miffTypeMATRIX3X3R4:            return miffTypeMATRIX3X3R4_STR_SIZE;
   case miffTypeMATRIX3X3R4S:           return miffTypeMATRIX3X3R4S_STR_SIZE;
   case miffTypeMATRIX3X3R8:            return miffTypeMATRIX3X3R8_STR_SIZE;
   case miffTypeMATRIX3X3R8S:           return miffTypeMATRIX3X3R8S_STR_SIZE;
   case miffTypeMATRIX4X4R4:            return miffTypeMATRIX4X4R4_STR_SIZE;
   case miffTypeMATRIX4X4R4S:           return miffTypeMATRIX4X4R4S_STR_SIZE;
   case miffTypeMATRIX4X4R8:            return miffTypeMATRIX4X4R8_STR_SIZE;
   case miffTypeMATRIX4X4R8S:           return miffTypeMATRIX4X4R8S_STR_SIZE;
#endif
   }

   return 0;
}
