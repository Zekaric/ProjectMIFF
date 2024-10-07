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
#include "json_local.h"

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
func: _JsonUtilStart
******************************************************************************/
void _JsonUtilStart(void)
{
   _locale = _create_locale(LC_ALL, "C");
}

/******************************************************************************
func: _JsonUtilStop
******************************************************************************/
void _JsonUtilStop(void)
{
   // Nothing to do.
}

/******************************************************************************
func: _JsonLocaleGet
******************************************************************************/
_locale_t _JsonLocaleGet(void)
{
   return _locale;
}

/******************************************************************************
func: _JsonMemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
JsonB _JsonMemIsEqual(JsonN const countA, JsonN1 const * const memA, JsonN const countB,
   JsonN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}

#if 0
/******************************************************************************
func: _JsonTypeGetName
******************************************************************************/
jsonStr *_JsonTypeGetName(jsonType const type)
{
   switch (type)
   {
   case jsonTypeKEY_VALUE_BLOCK_START:  return jsonTypeKEY_VALUE_BLOCK_START_STR;
   case jsonTypeKEY_VALUE_BLOCK_STOP:   return jsonTypeKEY_VALUE_BLOCK_STOP_STR;
   case jsonTypeTYPE:                   return jsonTypeTYPE_STR;
   case jsonTypeSTR:                    return jsonTypeSTR_STR;
   case jsonTypeVARIABLE:               return jsonTypeVARIABLE_STR;
   case jsonTypeUSER_TYPE:              return jsonTypeUSER_TYPE_STR;
   case jsonTypeBOOLEAN:                return jsonTypeBOOLEAN_STR;
   case jsonTypeI1:                     return jsonTypeI1_STR;
   case jsonTypeI2:                     return jsonTypeI2_STR;
   case jsonTypeI4:                     return jsonTypeI4_STR;
   case jsonTypeI8:                     return jsonTypeI8_STR;
   case jsonTypeI:                      return jsonTypeI_STR;
   case jsonTypeN1:                     return jsonTypeN1_STR;
   case jsonTypeN2:                     return jsonTypeN2_STR;
   case jsonTypeN4:                     return jsonTypeN4_STR;
   case jsonTypeN8:                     return jsonTypeN8_STR;
   case jsonTypeN:                      return jsonTypeN_STR;
   case jsonTypeR4:                     return jsonTypeR4_STR;
   case jsonTypeR4S:                    return jsonTypeR4S_STR;
   case jsonTypeR8:                     return jsonTypeR8_STR;
   case jsonTypeR8S:                    return jsonTypeR8S_STR;
   }

   return "";
}

/******************************************************************************
func: _JsonTypeGetNameSize
******************************************************************************/
JsonN _JsonTypeGetNameSize(jsonType const type)
{
   switch (type)
   {
   case jsonTypeKEY_VALUE_BLOCK_START:  return jsonTypeKEY_VALUE_BLOCK_START_STR_SIZE;
   case jsonTypeKEY_VALUE_BLOCK_STOP:   return jsonTypeKEY_VALUE_BLOCK_STOP_STR_SIZE;
   case jsonTypeTYPE:                   return jsonTypeTYPE_STR_SIZE;
   case jsonTypeSTR:                    return jsonTypeSTR_STR_SIZE;
   case jsonTypeVARIABLE:               return jsonTypeVARIABLE_STR_SIZE;
   case jsonTypeUSER_TYPE:              return jsonTypeUSER_TYPE_STR_SIZE;
   case jsonTypeBOOLEAN:                return jsonTypeBOOLEAN_STR_SIZE;
   case jsonTypeI1:                     return jsonTypeI1_STR_SIZE;
   case jsonTypeI2:                     return jsonTypeI2_STR_SIZE;
   case jsonTypeI4:                     return jsonTypeI4_STR_SIZE;
   case jsonTypeI8:                     return jsonTypeI8_STR_SIZE;
   case jsonTypeN1:                     return jsonTypeN1_STR_SIZE;
   case jsonTypeN2:                     return jsonTypeN2_STR_SIZE;
   case jsonTypeN4:                     return jsonTypeN4_STR_SIZE;
   case jsonTypeN8:                     return jsonTypeN8_STR_SIZE;
   case jsonTypeR4:                     return jsonTypeR4_STR_SIZE;
   case jsonTypeR4S:                    return jsonTypeR4S_STR_SIZE;
   case jsonTypeR8:                     return jsonTypeR8_STR_SIZE;
   case jsonTypeR8S:                    return jsonTypeR8S_STR_SIZE;
   }

   return 0;
}
#endif
