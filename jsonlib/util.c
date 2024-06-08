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
JsonBool _JsonMemIsEqual(JsonN const countA, JsonN1 const * const memA, JsonN const countB,
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
   case jsonTypeKEY_VALUE_BLOCK_START:  return (JsonStr *) jsonTypeKEY_VALUE_BLOCK_START_STR;
   case jsonTypeKEY_VALUE_BLOCK_STOP:   return (JsonStr *) jsonTypeKEY_VALUE_BLOCK_STOP_STR;
   case jsonTypeTYPE:                   return (JsonStr *) jsonTypeTYPE_STR;
   case jsonTypeSTR:                    return (JsonStr *) jsonTypeSTR_STR;
   case jsonTypeVARIABLE:               return (JsonStr *) jsonTypeVARIABLE_STR;
   case jsonTypeUSER_TYPE:              return (JsonStr *) jsonTypeUSER_TYPE_STR;
   case jsonTypeBOOLEAN:                return (JsonStr *) jsonTypeBOOLEAN_STR;
   case jsonTypeI1:                     return (JsonStr *) jsonTypeI1_STR;
   case jsonTypeI2:                     return (JsonStr *) jsonTypeI2_STR;
   case jsonTypeI4:                     return (JsonStr *) jsonTypeI4_STR;
   case jsonTypeI8:                     return (JsonStr *) jsonTypeI8_STR;
   case jsonTypeI:                      return (JsonStr *) jsonTypeI_STR;
   case jsonTypeN1:                     return (JsonStr *) jsonTypeN1_STR;
   case jsonTypeN2:                     return (JsonStr *) jsonTypeN2_STR;
   case jsonTypeN4:                     return (JsonStr *) jsonTypeN4_STR;
   case jsonTypeN8:                     return (JsonStr *) jsonTypeN8_STR;
   case jsonTypeN:                      return (JsonStr *) jsonTypeN_STR;
   case jsonTypeR4:                     return (JsonStr *) jsonTypeR4_STR;
   case jsonTypeR4S:                    return (JsonStr *) jsonTypeR4S_STR;
   case jsonTypeR8:                     return (JsonStr *) jsonTypeR8_STR;
   case jsonTypeR8S:                    return (JsonStr *) jsonTypeR8S_STR;
#if 0
   case jsonTypeABI1:                   return jsonTypeABI1_STR);
   case jsonTypeABI2:                   return jsonTypeABI2_STR);
   case jsonTypeABI4:                   return jsonTypeABI4_STR);
   case jsonTypeABI8:                   return jsonTypeABI8_STR);
   case jsonTypeABN1:                   return jsonTypeABN1_STR);
   case jsonTypeABN2:                   return jsonTypeABN2_STR);
   case jsonTypeABN4:                   return jsonTypeABN4_STR);
   case jsonTypeABN8:                   return jsonTypeABN8_STR);
   case jsonTypeABR4:                   return jsonTypeABR4_STR);
   case jsonTypeABR4S:                  return jsonTypeABR4S_STR);
   case jsonTypeABR8:                   return jsonTypeABR8_STR);
   case jsonTypeABR8S:                  return jsonTypeABR8S_STR);
   case jsonTypeABCI1:                  return jsonTypeABCI1_STR);
   case jsonTypeABCI2:                  return jsonTypeABCI2_STR);
   case jsonTypeABCI4:                  return jsonTypeABCI4_STR);
   case jsonTypeABCI8:                  return jsonTypeABCI8_STR);
   case jsonTypeABCN1:                  return jsonTypeABCN1_STR);
   case jsonTypeABCN2:                  return jsonTypeABCN2_STR);
   case jsonTypeABCN4:                  return jsonTypeABCN4_STR);
   case jsonTypeABCN8:                  return jsonTypeABCN8_STR);
   case jsonTypeABCR4:                  return jsonTypeABCR4_STR);
   case jsonTypeABCR4S:                 return jsonTypeABCR4S_STR);
   case jsonTypeABCR8:                  return jsonTypeABCR8_STR);
   case jsonTypeABCR8S:                 return jsonTypeABCR8S_STR);
   case jsonTypeABCDI1:                 return jsonTypeABCDI1_STR);
   case jsonTypeABCDI2:                 return jsonTypeABCDI2_STR);
   case jsonTypeABCDI4:                 return jsonTypeABCDI4_STR);
   case jsonTypeABCDI8:                 return jsonTypeABCDI8_STR);
   case jsonTypeABCDN1:                 return jsonTypeABCDN1_STR);
   case jsonTypeABCDN2:                 return jsonTypeABCDN2_STR);
   case jsonTypeABCDN4:                 return jsonTypeABCDN4_STR);
   case jsonTypeABCDN8:                 return jsonTypeABCDN8_STR);
   case jsonTypeABCDR4:                 return jsonTypeABCDR4_STR);
   case jsonTypeABCDR4S:                return jsonTypeABCDR4S_STR);
   case jsonTypeABCDR8:                 return jsonTypeABCDR8_STR);
   case jsonTypeABCDR8S:                return jsonTypeABCDR8S_STR);
   case jsonTypeMATRIX2X2R4:            return jsonTypeMATRIX2X2R4_STR);
   case jsonTypeMATRIX2X2R4S:           return jsonTypeMATRIX2X2R4S_STR);
   case jsonTypeMATRIX2X2R8:            return jsonTypeMATRIX2X2R8_STR);
   case jsonTypeMATRIX2X2R8S:           return jsonTypeMATRIX2X2R8S_STR);
   case jsonTypeMATRIX3X3R4:            return jsonTypeMATRIX3X3R4_STR);
   case jsonTypeMATRIX3X3R4S:           return jsonTypeMATRIX3X3R4S_STR);
   case jsonTypeMATRIX3X3R8:            return jsonTypeMATRIX3X3R8_STR);
   case jsonTypeMATRIX3X3R8S:           return jsonTypeMATRIX3X3R8S_STR);
   case jsonTypeMATRIX4X4R4:            return jsonTypeMATRIX4X4R4_STR);
   case jsonTypeMATRIX4X4R4S:           return jsonTypeMATRIX4X4R4S_STR);
   case jsonTypeMATRIX4X4R8:            return jsonTypeMATRIX4X4R8_STR);
   case jsonTypeMATRIX4X4R8S:           return jsonTypeMATRIX4X4R8S_STR);
#endif
   }

   return (JsonStr *) "";
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
#if 0
   case jsonTypeABI1:                   return jsonTypeABI1_STR_SIZE;
   case jsonTypeABI2:                   return jsonTypeABI2_STR_SIZE;
   case jsonTypeABI4:                   return jsonTypeABI4_STR_SIZE;
   case jsonTypeABI8:                   return jsonTypeABI8_STR_SIZE;
   case jsonTypeABN1:                   return jsonTypeABN1_STR_SIZE;
   case jsonTypeABN2:                   return jsonTypeABN2_STR_SIZE;
   case jsonTypeABN4:                   return jsonTypeABN4_STR_SIZE;
   case jsonTypeABN8:                   return jsonTypeABN8_STR_SIZE;
   case jsonTypeABR4:                   return jsonTypeABR4_STR_SIZE;
   case jsonTypeABR4S:                  return jsonTypeABR4S_STR_SIZE;
   case jsonTypeABR8:                   return jsonTypeABR8_STR_SIZE;
   case jsonTypeABR8S:                  return jsonTypeABR8S_STR_SIZE;
   case jsonTypeABCI1:                  return jsonTypeABCI1_STR_SIZE;
   case jsonTypeABCI2:                  return jsonTypeABCI2_STR_SIZE;
   case jsonTypeABCI4:                  return jsonTypeABCI4_STR_SIZE;
   case jsonTypeABCI8:                  return jsonTypeABCI8_STR_SIZE;
   case jsonTypeABCN1:                  return jsonTypeABCN1_STR_SIZE;
   case jsonTypeABCN2:                  return jsonTypeABCN2_STR_SIZE;
   case jsonTypeABCN4:                  return jsonTypeABCN4_STR_SIZE;
   case jsonTypeABCN8:                  return jsonTypeABCN8_STR_SIZE;
   case jsonTypeABCR4:                  return jsonTypeABCR4_STR_SIZE;
   case jsonTypeABCR4S:                 return jsonTypeABCR4S_STR_SIZE;
   case jsonTypeABCR8:                  return jsonTypeABCR8_STR_SIZE;
   case jsonTypeABCR8S:                 return jsonTypeABCR8S_STR_SIZE;
   case jsonTypeABCDI1:                 return jsonTypeABCDI1_STR_SIZE;
   case jsonTypeABCDI2:                 return jsonTypeABCDI2_STR_SIZE;
   case jsonTypeABCDI4:                 return jsonTypeABCDI4_STR_SIZE;
   case jsonTypeABCDI8:                 return jsonTypeABCDI8_STR_SIZE;
   case jsonTypeABCDN1:                 return jsonTypeABCDN1_STR_SIZE;
   case jsonTypeABCDN2:                 return jsonTypeABCDN2_STR_SIZE;
   case jsonTypeABCDN4:                 return jsonTypeABCDN4_STR_SIZE;
   case jsonTypeABCDN8:                 return jsonTypeABCDN8_STR_SIZE;
   case jsonTypeABCDR4:                 return jsonTypeABCDR4_STR_SIZE;
   case jsonTypeABCDR4S:                return jsonTypeABCDR4S_STR_SIZE;
   case jsonTypeABCDR8:                 return jsonTypeABCDR8_STR_SIZE;
   case jsonTypeABCDR8S:                return jsonTypeABCDR8S_STR_SIZE;
   case jsonTypeMATRIX2X2R4:            return jsonTypeMATRIX2X2R4_STR_SIZE;
   case jsonTypeMATRIX2X2R4S:           return jsonTypeMATRIX2X2R4S_STR_SIZE;
   case jsonTypeMATRIX2X2R8:            return jsonTypeMATRIX2X2R8_STR_SIZE;
   case jsonTypeMATRIX2X2R8S:           return jsonTypeMATRIX2X2R8S_STR_SIZE;
   case jsonTypeMATRIX3X3R4:            return jsonTypeMATRIX3X3R4_STR_SIZE;
   case jsonTypeMATRIX3X3R4S:           return jsonTypeMATRIX3X3R4S_STR_SIZE;
   case jsonTypeMATRIX3X3R8:            return jsonTypeMATRIX3X3R8_STR_SIZE;
   case jsonTypeMATRIX3X3R8S:           return jsonTypeMATRIX3X3R8S_STR_SIZE;
   case jsonTypeMATRIX4X4R4:            return jsonTypeMATRIX4X4R4_STR_SIZE;
   case jsonTypeMATRIX4X4R4S:           return jsonTypeMATRIX4X4R4S_STR_SIZE;
   case jsonTypeMATRIX4X4R8:            return jsonTypeMATRIX4X4R8_STR_SIZE;
   case jsonTypeMATRIX4X4R8S:           return jsonTypeMATRIX4X4R8S_STR_SIZE;
#endif
   }

   return 0;
}
#endif
