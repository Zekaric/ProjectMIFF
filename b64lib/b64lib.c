/******************************************************************************
file:       b64lib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
functions to convert binary data into a base64 string.
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
#include "b64lib.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static BsfB   _isStarted            = bsfFALSE;
static BsfN1  _LetterToValue[128];
static BsfN1  _ValueToLetter[64];

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: bsfGet
******************************************************************************/
BsfB bsfGet(BsfDataGet * const data, BsfN1 * const byte)
{
   if (!_isStarted)
   {
      return bsfFALSE;
   }

   *byte = 0;
   
   switch (data->state)
   {
   case 0:
      // byte to fill      Incoming base64 byte
      // [........]        [......]
      data->byte = data->buffer[data->index++];

      // [111111..]        [111111]
      *byte |= (BsfN1) (_LetterToValue[data->byte] << 2);

      data->byte = data->buffer[data->index++];

      // [11111122]        [22....]
      *byte |= (BsfN1) (_LetterToValue[data->byte] >> 4);

      // Remainder [..2222]
      data->state = 1;
      break;

   case 1:
      // byte to fill      Incoming base64 byte
      // [........]        [..xxxx]

      // [xxxx....]        [..xxxx]
      *byte |= (BsfN1) (_LetterToValue[data->byte] << 4);

      data->byte = data->buffer[data->index++];

      // [xxxx1111]        [1111..]
      *byte |= (BsfN1) (_LetterToValue[data->byte] >> 2);

      // Remainder [....22]
      data->state = 2;
      break;

   case 2:
      // byte to fill      Incoming base64 byte
      // [........]        [....xx]

      // [xx......]        [....xx]
      *byte |= (BsfN1) (_LetterToValue[data->byte] << 6);

      data->byte = data->buffer[data->index++];

      // [xx111111]        [111111]
      *byte |= (BsfN1) (_LetterToValue[data->byte]);

      // Remainder [......]
      data->state = 0;
      break;
   }

   return bsfTRUE;
}

/******************************************************************************
func: bsfPrepGet

Prep the base 64 functions for reading.
******************************************************************************/
BsfDataGet bsfPrepGet(BsfN1 const * const buffer)
{
   BsfDataGet data;

   data.state  = 0;
   data.byte   = 0;
   data.index  = 0;
   data.buffer = buffer;
   
   return data;
}

/******************************************************************************
func: bsfPrepSet

Prep the base 64 functions for Writing.
******************************************************************************/
BsfDataSet bsfPrepSet(BsfN1 * const buffer)
{
   BsfDataSet data;

   data.state  = 0;
   data.byte   = 0;
   data.index  = 0;
   data.buffer = buffer;
   
   return data;
}

/******************************************************************************
func: bsfSet
******************************************************************************/
BsfB bsfSet(BsfDataSet * const data, BsfN1 const byte)
{
   BsfN1 btemp;
   BsfN1 sixbit;

   if (!_isStarted)
   {
      return bsfFALSE;
   }

   btemp = byte;

   switch (data->state)
   {
   case 0:
      // sixbit      Remainder byte  Incoming byte
      // [......]    [........]      [xxxxxxyy]
      
      // [xxxxxx] <----------------- [xxxxxx..]
      sixbit = ((btemp >> 2) & 0x3F);
      data->buffer[data->index++] = _ValueToLetter[sixbit];

      // [xxxxxx]    [..yy....] <--- [......yy]
      data->byte  = (btemp & 0x03) << 4;
      data->state = 1;
      break;

   case 1:
      // sixbit      Remainder byte  Incoming byte
      // [......]    [..rr....]      [xxxxyyyy]

      // [rr....] <- [..rr....]
      // [rrxxxx] <----------------- [xxxx....]
      sixbit = data->byte | ((btemp >> 4) & 0x0F);
      data->buffer[data->index++] = _ValueToLetter[sixbit];

      // [rrxxxx]    [..yyyy..] <--- [....yyyy]
      data->byte  = ((btemp & 0x0F) << 2);
      data->state = 2;
      break;

   case 2:
      // sixbit      Remainder byte  Incoming byte
      // [......]    [..rrrr..]      [xxyyyyyy]

      // [rrrr..] <- [..rrrr..]
      // [rrrrxx] <----------------- [xx......]
      sixbit = data->byte | ((btemp >> 6)  & 0x03);
      data->buffer[data->index++] = _ValueToLetter[sixbit];

      // [yyyyyy] <----------------- [..yyyyyy]
      sixbit = btemp & 0x3F;
      data->buffer[data->index++] = _ValueToLetter[sixbit];

      // [rrrrxx]    [........]      [........]
      data->byte  = 0;
      data->state = 0;
      break;
   }

   data->buffer[data->index] = 0;

   return bsfTRUE;
}

/******************************************************************************
func: bsfSetEnd

Send out the remainder bits.
******************************************************************************/
BsfB bsfSetEnd(BsfDataSet * const data)
{
   BsfN1 sixbit;

   if (!_isStarted)
   {
      return bsfFALSE;
   }

   switch (data->state)
   {
   case 0:
      // Remainder byte
      // [........]

      // Nothing to do.
      break;

   case 1:
   case 2:
      // Remainder byte
      // [..xx....]
      // [..xxxx..]

      sixbit = data->byte;
      data->buffer[data->index++] = _ValueToLetter[sixbit];
      break;
   }

   data->byte                = 0;
   data->state               = 0;
   data->buffer[data->index] = 0;

   return bsfTRUE;
}

/******************************************************************************
func: bsfStart
******************************************************************************/
void bsfStart(void)
{
   BsfN1 index;

   index = 0;
   _LetterToValue['A'] = index++;
   _LetterToValue['B'] = index++;
   _LetterToValue['C'] = index++;
   _LetterToValue['D'] = index++;
   _LetterToValue['E'] = index++;
   _LetterToValue['F'] = index++;
   _LetterToValue['G'] = index++;
   _LetterToValue['H'] = index++;
   _LetterToValue['I'] = index++;
   _LetterToValue['J'] = index++;
   //10
   _LetterToValue['K'] = index++;
   _LetterToValue['L'] = index++;
   _LetterToValue['M'] = index++;
   _LetterToValue['N'] = index++;
   _LetterToValue['O'] = index++;
   _LetterToValue['P'] = index++;
   _LetterToValue['Q'] = index++;
   _LetterToValue['R'] = index++;
   _LetterToValue['S'] = index++;
   _LetterToValue['T'] = index++;
   //20
   _LetterToValue['U'] = index++;
   _LetterToValue['V'] = index++;
   _LetterToValue['W'] = index++;
   _LetterToValue['X'] = index++;
   _LetterToValue['Y'] = index++;
   _LetterToValue['Z'] = index++;
   _LetterToValue['a'] = index++;
   _LetterToValue['b'] = index++;
   _LetterToValue['c'] = index++;
   _LetterToValue['d'] = index++;
   //30
   _LetterToValue['e'] = index++;
   _LetterToValue['f'] = index++;
   _LetterToValue['g'] = index++;
   _LetterToValue['h'] = index++;
   _LetterToValue['i'] = index++;
   _LetterToValue['j'] = index++;
   _LetterToValue['k'] = index++;
   _LetterToValue['l'] = index++;
   _LetterToValue['m'] = index++;
   _LetterToValue['n'] = index++;
   //40
   _LetterToValue['o'] = index++;
   _LetterToValue['p'] = index++;
   _LetterToValue['q'] = index++;
   _LetterToValue['r'] = index++;
   _LetterToValue['s'] = index++;
   _LetterToValue['t'] = index++;
   _LetterToValue['u'] = index++;
   _LetterToValue['v'] = index++;
   _LetterToValue['w'] = index++;
   _LetterToValue['x'] = index++;
   //50
   _LetterToValue['y'] = index++;
   _LetterToValue['z'] = index++;
   _LetterToValue['0'] = index++;
   _LetterToValue['1'] = index++;
   _LetterToValue['2'] = index++;
   _LetterToValue['3'] = index++;
   _LetterToValue['4'] = index++;
   _LetterToValue['5'] = index++;
   _LetterToValue['6'] = index++;
   _LetterToValue['7'] = index++;
   //60
   _LetterToValue['8'] = index++;
   _LetterToValue['9'] = index++;
   _LetterToValue['+'] = index++;
   _LetterToValue['/'] = index++;

   index = 0;
   _ValueToLetter[index++] = 'A';
   _ValueToLetter[index++] = 'B';
   _ValueToLetter[index++] = 'C';
   _ValueToLetter[index++] = 'D';
   _ValueToLetter[index++] = 'E';
   _ValueToLetter[index++] = 'F';
   _ValueToLetter[index++] = 'G';
   _ValueToLetter[index++] = 'H';
   _ValueToLetter[index++] = 'I';
   _ValueToLetter[index++] = 'J';
   _ValueToLetter[index++] = 'K';
   _ValueToLetter[index++] = 'L';
   _ValueToLetter[index++] = 'M';
   _ValueToLetter[index++] = 'N';
   _ValueToLetter[index++] = 'O';
   _ValueToLetter[index++] = 'P';
   _ValueToLetter[index++] = 'Q';
   _ValueToLetter[index++] = 'R';
   _ValueToLetter[index++] = 'S';
   _ValueToLetter[index++] = 'T';
   _ValueToLetter[index++] = 'U';
   _ValueToLetter[index++] = 'V';
   _ValueToLetter[index++] = 'W';
   _ValueToLetter[index++] = 'X';
   _ValueToLetter[index++] = 'Y';
   _ValueToLetter[index++] = 'Z';
   _ValueToLetter[index++] = 'a';
   _ValueToLetter[index++] = 'b';
   _ValueToLetter[index++] = 'c';
   _ValueToLetter[index++] = 'd';
   _ValueToLetter[index++] = 'e';
   _ValueToLetter[index++] = 'f';
   _ValueToLetter[index++] = 'g';
   _ValueToLetter[index++] = 'h';
   _ValueToLetter[index++] = 'i';
   _ValueToLetter[index++] = 'j';
   _ValueToLetter[index++] = 'k';
   _ValueToLetter[index++] = 'l';
   _ValueToLetter[index++] = 'm';
   _ValueToLetter[index++] = 'n';
   _ValueToLetter[index++] = 'o';
   _ValueToLetter[index++] = 'p';
   _ValueToLetter[index++] = 'q';
   _ValueToLetter[index++] = 'r';
   _ValueToLetter[index++] = 's';
   _ValueToLetter[index++] = 't';
   _ValueToLetter[index++] = 'u';
   _ValueToLetter[index++] = 'v';
   _ValueToLetter[index++] = 'w';
   _ValueToLetter[index++] = 'x';
   _ValueToLetter[index++] = 'y';
   _ValueToLetter[index++] = 'z';
   _ValueToLetter[index++] = '0';
   _ValueToLetter[index++] = '1';
   _ValueToLetter[index++] = '2';
   _ValueToLetter[index++] = '3';
   _ValueToLetter[index++] = '4';
   _ValueToLetter[index++] = '5';
   _ValueToLetter[index++] = '6';
   _ValueToLetter[index++] = '7';
   _ValueToLetter[index++] = '8';
   _ValueToLetter[index++] = '9';
   _ValueToLetter[index++] = '+';
   _ValueToLetter[index++] = '/';

   _isStarted = bsfTRUE;
}

/******************************************************************************
func: bsfStop
******************************************************************************/
void bsfStop(void)
{
   // Nothing to do.
   _isStarted = bsfFALSE;
}
