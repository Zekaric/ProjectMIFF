/******************************************************************************
file:       base64
author:     Robbert de Groot
copyright:  2021, 

description:
functions to convert binary data into a base64 string.
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
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static MiffN4  _base64State               = 0;
static MiffN1  _base64Byte                = 0;
static MiffN1  _base64LetterToValue[128];
static MiffN1  _base64ValueToLetter[64];

/******************************************************************************
prototype:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _Base64Restart
******************************************************************************/
void _Base64Restart(void)
{
   _base64State = 0;
   _base64Byte  = 0;
}

/******************************************************************************
func: _Base64Get
******************************************************************************/
MiffBool _Base64Get(Miff * const miff, Miff1 * const byte)
{
   byte->n = 0;
   
   switch (_base64State)
   {
   case 0:
      // byte to fill      Incoming base64 byte
      // [........]        [......]

      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &_base64Byte));

      // [111111..]        [111111]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 2);

      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &_base64Byte));

      // [11111122]        [22....]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] >> 4);

      // Remainder [..2222]
      _base64State = 1;
      break;

   case 1:
      // byte to fill      Incoming base64 byte
      // [........]        [..xxxx]

      // [xxxx....]        [..xxxx]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 4);

      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &_base64Byte));

      // [xxxx1111]        [1111..]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] >> 2);

      // Remainder [....22]
      _base64State = 2;
      break;

   case 2:
      // byte to fill      Incoming base64 byte
      // [........]        [....xx]

      // [xx......]        [....xx]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 6);

      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, &_base64Byte));

      // [xx111111]        [111111]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte]);

      // Remainder [......]
      _base64State = 0;
      break;
   }

   returnTrue;
}

/******************************************************************************
func: _Base64Set
******************************************************************************/
MiffBool _Base64Set(Miff * const miff, Miff1 * const byte)
{
   MiffN1 sixbit;

   switch (_base64State)
   {
   case 0:
      // Remainder byte
      // [........]
      
      sixbit = byte->n >> 2;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));

      // Remainder [......xx]
      _base64Byte  = byte->n;
      _base64State = 1;
      break;

   case 1:
      // Remainder byte
      // [......xx]

      sixbit = (_base64Byte & 0x3) | byte->n >> 4;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));

      // Remainder [....xxxx]
      _base64Byte  = byte->n;
      _base64State = 2;
      break;

   case 2:
      // Remainder byte
      // [....xxxx]

      sixbit = (_base64Byte & 0xf) | byte->n >> 6;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));

      sixbit = byte->n & 0x3f;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));

      // Remainder [........]
      _base64Byte  = 0;
      _base64State = 0;
      break;
   }

   returnTrue;
}

/******************************************************************************
func: _Base64SetEnd

Send out the remainder bits.
******************************************************************************/
MiffBool _Base64SetEnd(Miff * const miff)
{
   MiffN1 sixbit;

   switch (_base64State)
   {
   case 0:
      // Remainder byte
      // [........]

      // Nothing to do.
      break;

   case 1:
      // Remainder byte
      // [......xx]

      sixbit = _base64Byte & 0x3;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));
      break;

   case 2:
      // Remainder byte
      // [....xxxx]

      sixbit = _base64Byte & 0xf;
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &_base64ValueToLetter[sixbit]));
      break;
   }

   _base64Byte  = 0;
   _base64State = 0;

   returnTrue;
}
