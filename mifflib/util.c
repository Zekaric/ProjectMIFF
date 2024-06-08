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
   case miffTypeBLOCK_START: return miffTypeKEY_VALUE_BLOCK_START_STR;
   case miffTypeBLOCK_STOP:  return miffTypeKEY_VALUE_BLOCK_STOP_STR;
   case miffTypeTYPE:        return miffTypeTYPE_STR;
   case miffTypeSTR:         return miffTypeSTR_STR;
   case miffTypeVAR:         return miffTypeVARIABLE_STR;
   case miffTypeUSER:        return miffTypeUSER_TYPE_STR;
   case miffTypeBOOL:        return miffTypeBOOLEAN_STR;
   case miffTypeI:           return miffTypeI_STR;
   case miffTypeN:           return miffTypeN_STR;
   case miffTypeR4:          return miffTypeR4_STR;
   case miffTypeR8:          return miffTypeR8_STR;
   }

   return "";
}

/******************************************************************************
func: _MiffTypeGetNameSize
******************************************************************************/
MiffN4 _MiffTypeGetNameSize(MiffType const type)
{
   switch (type)
   {
   case miffTypeBLOCK_START: return miffTypeKEY_VALUE_BLOCK_START_STR_SIZE;
   case miffTypeBLOCK_STOP:  return miffTypeKEY_VALUE_BLOCK_STOP_STR_SIZE;
   case miffTypeTYPE:        return miffTypeTYPE_STR_SIZE;
   case miffTypeSTR:         return miffTypeSTR_STR_SIZE;
   case miffTypeVAR:         return miffTypeVARIABLE_STR_SIZE;
   case miffTypeUSER:        return miffTypeUSER_TYPE_STR_SIZE;
   case miffTypeBOOL:        return miffTypeBOOLEAN_STR_SIZE;
   case miffTypeI:           return miffTypeI_STR_SIZE;
   case miffTypeN:           return miffTypeN_STR_SIZE;
   case miffTypeR4:          return miffTypeR4_STR_SIZE;
   case miffTypeR8:          return miffTypeR8_STR_SIZE;
   }

   return 0;
}
