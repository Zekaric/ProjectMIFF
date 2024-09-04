/******************************************************************************
file:       write
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
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
static JsonN1 _hex[128];

/******************************************************************************
global:
function:
******************************************************************************/
#if 0
/******************************************************************************
func: _JsonGetLine
******************************************************************************/
JsonB _JsonGetLine(Json * const json)
{
   JsonN4  index;
   JsonN1  byte;
   JsonN1 *bufTemp;

   returnFalseIf(!json);

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (json->readBinCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _JsonMemCreateTypeArray(json->readBinCountActual * 2, JsonN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _JsonMemCopyTypeArray(json->readBinCountActual, JsonN1, bufTemp, json->readBinData);
         // Destroy the old buffer.
         _JsonMemDestroy(json->readBinData);

         // Reset the internal buffer to the new larger buffer.
         json->readBinCountActual = json->readBinCountActual * 2;
         json->readBinData        = bufTemp;
      }

      // End of file?
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));
      // End of line, we done reading.
      breakIf(byte == '\n');

      // Add the letter to the byte array.
      json->readBinData[index++] = byte;
   }

   // Need NULL terminator.
   json->readBinCount       = index;
   json->readBinData[index] = 0;

   returnTrue;
}

/******************************************************************************
func: _JsonGetLineSkip
******************************************************************************/
JsonB _JsonGetLineSkip(Json * const json)
{
   JsonN1 byte;

   returnFalseIf(!json);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _JsonGetPart
******************************************************************************/
JsonB _JsonGetPart(Json * const json)
{
   JsonN4  index;
   JsonN1  byte;
   JsonN1 *bufTemp;

   // Nothing left to read for this record.
   returnFalseIf(
      !json ||
      json->isRecordDone);

   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (json->readBinCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _JsonMemCreateTypeArray(json->readBinCountActual * 2, JsonN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _JsonMemCopyTypeArray(json->readBinCountActual, JsonN1, bufTemp, json->readBinData);
         // Destroy the old buffer.
         _JsonMemDestroy(json->readBinData);

         // Reset the internal buffer to the new larger buffer.
         json->readBinCountActual = json->readBinCountActual * 2;
         json->readBinData        = bufTemp;
      }

      // End of file?
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));
      // End of line or part, we done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      json->readBinData[index++] = byte;
   }

   // Need NULL terminator.
   json->readBinCount       = index;
   json->readBinData[index] = 0;

   if (byte == '\n')
   {
      json->isRecordDone = jsonTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _JsonGetArrayCount
******************************************************************************/
JsonB _JsonGetArrayCount(Json * const json, JsonN4 * const count)
{
   returnFalseIf(!count);

   *count = 0;

   returnFalseIf(!json);

   returnFalseIf(!_JsonGetPart(json));

   if (json->readBinData[0] == L'*')
   {
      json->currentRecord.arrayCount = jsonArrayCountUNKNOWN;
      returnTrue;
   }

   json->currentRecord.arrayCount = (JsonN4) _JsonStrToN(json->readBinCount, json->readBinData);

   *count = json->currentRecord.arrayCount;

   returnTrue;
}
#endif

#if 0
/******************************************************************************
func: _JsonGetStr
******************************************************************************/
JsonB _JsonGetStr(Json * const json, JsonStr ** const string)
{
   JsonI4    index;
   JsonN4    keyCount;
   JsonN1    byte;
   JsonN1    unicode[4];
   JsonStr    c2[2];
   JsonStr    str[4];
   JsonN4    ccount;
   JsonN1   *bufTemp;
   JsonB  isFound;

   returnFalseIf(
      !json       ||
      !string);

   isFound       = jsonFALSE;
   unicode[0]    =
      unicode[1] = 
      unicode[2] =
      unicode[3] = 0;
   c2[0]         =
      c2[1]      = 0;
   str[0]         =
      str[1]      = 
      str[1]      = 
      str[4]      = 0;

   // Skip to the start of the string.
   loop
   {
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));

      // Skip over white space.
      continueIf(jsonIS_SPACE(byte));
   }
   returnFalseIf(byte != jsonSTRING_QUOTE_STR[0]);

   // Read in the string in it's entirety.
   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (json->readBinCountActual <= index)
      {
         // Create a new buffer double the size.
         // + 4 to handle the edge case of a unicode escape sequence.
         bufTemp = _JsonMemCreateTypeArray(json->readBinCountActual * 2 + 4, JsonN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _JsonMemCopyTypeArray(json->readBinCountActual, JsonN1, bufTemp, json->readBinData);
         // Destroy the old buffer.
         _JsonMemDestroy(json->readBinData);

         // Reset the internal buffer to the new larger buffer.
         json->readBinCountActual = json->readBinCountActual * 2;
         json->readBinData        = bufTemp;
      }

      // End of file?
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));
      // End of line or part, we done reading.
      breakIf(byte == jsonKEY_VALUE_SEPARATOR_STR[0]);
      
      // End of string
      if (byte == jsonSTRING_QUOTE_STR[0])
      {
         isFound = jsonTRUE;
         break;
      }

      if (byte == jsonSTRING_ESCAPE_STR[0])
      {
         breakIf(!json->getBuffer(json->dataRepo, 1, &byte));

         if      (byte == jsonSTRING_ESCAPE_BACKSLASH_STR[0])
         {
            json->readBinData[index++] = jsonSTRING_ESCAPE_BACKSLASH_STR[0];
         }
         else if (byte == jsonSTRING_ESCAPE_BACKSPACE_STR[0])
         {
            json->readBinData[index++] = '\b';
         }
         else if (byte == jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR[0])
         {
            json->readBinData[index++] = '\r';
         }
         else if (byte == jsonSTRING_ESCAPE_FORMFEED_STR[0])
         {
            json->readBinData[index++] = '\f';
         }
         else if (byte == jsonSTRING_ESCAPE_LINEFEED_STR[0])
         {
            json->readBinData[index++] = '\n';
         }
         else if (byte == jsonSTRING_ESCAPE_QUOTE_STR[0])
         {
            json->readBinData[index++] = jsonSTRING_ESCAPE_QUOTE_STR[0];
         }
         else if (byte == jsonSTRING_ESCAPE_SLASH_STR[0])
         {
            json->readBinData[index++] = jsonSTRING_ESCAPE_SLASH_STR[0];
         }
         else if (byte == jsonSTRING_ESCAPE_TAB_STR[0])
         {
            json->readBinData[index++] = '\t';
         }
         else if (byte == jsonSTRING_ESCAPE_UNICODE_STR[0])
         {
            breakIf(!json->getBuffer(json->dataRepo, 4, unicode));

            c2[0] =
               (_hex[unicode[0]] << 12) +
               (_hex[unicode[1]] <<  8) +
               (_hex[unicode[2]] <<  4) +
               (_hex[unicode[3]] <<  0);
            c2[1] = 0;

            ccount = _JsonC2LetterToC4Letter(c2, &c4);
            ccount = _JsonC4LetterToC1Letter(&c4, &str[0], &str[1], &str[2], &str[3]);

            loop
            {
               json->readBinData[index++] = str[0];
               breakIf(ccount == 1);
               json->readBinData[index++] = str[1];
               breakIf(ccount == 2);
               json->readBinData[index++] = str[2];
               breakIf(ccount == 3);
               json->readBinData[index++] = str[3];
               break;
            }
         }
      }
      else
      {
         // Add the letter to the byte array.
         json->readBinData[index++] = byte;
      }
   }

   // Set the state to be expecting a value of some sort.
   json->readState = jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY;

   // Convert the UTF8 to UTF16
   *string = _JsonMemCreateTypeArray(index + 1, JsonStr);
   returnFalseIf(!*string);
   _JsonC1ToC2(index, json->readBinData, &keyCount, *string);

   returnTrue;
}

/******************************************************************************
func: _JsonGetC2Key
******************************************************************************/
JsonB _JsonGetC2Key(Json * const json, JsonStr * const key)
{
   JsonN1 keySize;

   returnFalseIf(!key);

   _JsonMemClearTypeArray(jsonKeySIZE, JsonStr, key);

   returnFalseIf(!json);

   returnFalseIf(!_JsonGetPart(json));

   _JsonMemClearTypeArray(jsonKeySIZE, JsonStr, json->currentRecord.name);
   _JsonSTrToKey(
      json->readBinCount,
      json->readBinData,
      &keySize,
      json->currentRecord.name);

   _JsonMemCopyTypeArray(jsonKeySIZE, JsonStr, key, json->currentRecord.name);

   returnTrue;
}

/******************************************************************************
func: _JsonReadType
******************************************************************************/
JsonB _JsonReadType(Json * const json, JsonType * const type, JsonStr * const typeName)
{
   JsonN4 index;
   JsonN1 count;

   returnFalseIf(!type);

   *type = jsonTypeNONE;

   returnFalseIf(!json);

   returnFalseIf(!_JsonGetPart(json));

   returnFalseIf(json->readBinCount == 0);

   // Get the name of the type just in case it is a user type.
   _JsonSTrToKey(json->readBinCount, json->readBinData, &count, typeName);

   forCount (index, jsonTypeCOUNT)
   {
      continueIf(_JsonTypeGetNameSize(index) == 0);

      if (_JsonMemIsEqual(
            _JsonTypeGetNameSize(index), (JsonN1 *) _JsonTypeGetNameC1(index),
            json->readBinCount,     json->readBinData))
      {
         *type = index;

         returnTrue;
      }
   }

   *type = jsonTypeUSER_TYPE;

   returnTrue;
}

/******************************************************************************
func: _JsonGetR4
******************************************************************************/
JsonB _JsonGetR4(Json * const json, JsonR4 * const value)
{
   Json4      vtemp;
   JsonBase64Data data;

   returnFalseIf(!_JsonGetPart(json));

   data = _JsonBase64Restart(json->readBinData);

   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[0]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[1]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[2]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[3]));

   _JsonByteSwap4(json, &vtemp);

   *value = vtemp.r;

   returnTrue;
}

/******************************************************************************
func: _JsonGetR4S
******************************************************************************/
JsonB _JsonGetR4S(Json * const json, JsonR4 * const value)
{
   JsonStr *ctemp;

   returnFalseIf(!_JsonGetPart(json));

   *value = (JsonR4) strtod((char *) json->readBinData, (char **) &ctemp);

   returnTrue;
}

/******************************************************************************
func: _JsonGetR
******************************************************************************/
JsonB _JsonGetR(Json * const json, JsonR8 * const value)
{
   Json8      vtemp;
   JsonBase64Data data;

   returnFalseIf(!_JsonGetPart(json));

   data = _JsonBase64Restart(json->readBinData);

   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[0]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[1]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[2]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[3]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[4]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[5]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[6]));
   returnFalseIf(!_JsonBase64Get(&data, &vtemp.byte[7]));

   _JsonByteSwap8(json, &vtemp);

   *value = vtemp.r;

   returnTrue;
}

/******************************************************************************
func: _JsonGetRS
******************************************************************************/
JsonB _JsonGetRS(Json * const json, JsonR8 * const value)
{
   JsonStr *ctemp;

   returnFalseIf(!_JsonGetPart(json));

   *value = strtod((char *) json->readBinData, (char **) &ctemp);

   returnTrue;
}
#endif

/******************************************************************************
func: _JsonGetStart
******************************************************************************/
JsonB _JsonGetStart(void)
{
   _hex['0']    = 0x0;
   _hex['1']    = 0x1;
   _hex['2']    = 0x2;
   _hex['3']    = 0x3;
   _hex['4']    = 0x4;
   _hex['5']    = 0x5;
   _hex['6']    = 0x6;
   _hex['7']    = 0x7;
   _hex['8']    = 0x8;
   _hex['9']    = 0x9;
   _hex['a']    = 
      _hex['A'] = 0xa;
   _hex['b']    =
      _hex['B'] = 0xb;
   _hex['c']    =
      _hex['C'] = 0xc;
   _hex['d']    =
      _hex['D'] = 0xd;
   _hex['e']    =
      _hex['E'] = 0xe;
   _hex['f']    =
      _hex['F'] = 0xf;

   returnTrue;
}

/******************************************************************************
func: _JsonGetStop
******************************************************************************/
void _JsonGetStop(void)
{
   return;
}
