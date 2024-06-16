/******************************************************************************
file:       base64
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
#include "local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static MiffN1  _base64LetterToValue[128];
static MiffN1  _base64ValueToLetter[64];

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffBase64Restart
******************************************************************************/
MiffBase64Data _MiffBase64Restart(MiffN1 * const value)
{
   MiffBase64Data data;

   data.state  = 0;
   data.byte   = 0;
   data.index  = 0;
   data.buffer = value;
   
   return data;
}

/******************************************************************************
func: _MiffBase64Get
******************************************************************************/
MiffB _MiffBase64Get(MiffBase64Data * const data, MiffN1 * const byte)
{
   *byte = 0;
   
   switch (data->state)
   {
   case 0:
      // byte to fill      Incoming base64 byte
      // [........]        [......]
      data->byte = data->buffer[data->index++];

      // [111111..]        [111111]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte] << 2);

      data->byte = data->buffer[data->index++];

      // [11111122]        [22....]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte] >> 4);

      // Remainder [..2222]
      data->state = 1;
      break;

   case 1:
      // byte to fill      Incoming base64 byte
      // [........]        [..xxxx]

      // [xxxx....]        [..xxxx]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte] << 4);

      data->byte = data->buffer[data->index++];

      // [xxxx1111]        [1111..]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte] >> 2);

      // Remainder [....22]
      data->state = 2;
      break;

   case 2:
      // byte to fill      Incoming base64 byte
      // [........]        [....xx]

      // [xx......]        [....xx]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte] << 6);

      data->byte = data->buffer[data->index++];

      // [xx111111]        [111111]
      *byte |= (MiffN1) (_base64LetterToValue[data->byte]);

      // Remainder [......]
      data->state = 0;
      break;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffBase64Set
******************************************************************************/
MiffB _MiffBase64Set(MiffBase64Data * const data, MiffN1 const byte)
{
   MiffN1 btemp;
   MiffN1 sixbit;

   btemp = byte;

   switch (data->state)
   {
   case 0:
      // sixbit      Remainder byte  Incoming byte
      // [......]    [........]      [xxxxxxyy]
      
      // [xxxxxx] <----------------- [xxxxxx..]
      sixbit = ((btemp >> 2) & 0x3F);
      data->buffer[data->index++] = _base64ValueToLetter[sixbit];

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
      data->buffer[data->index++] = _base64ValueToLetter[sixbit];

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
      data->buffer[data->index++] = _base64ValueToLetter[sixbit];

      // [yyyyyy] <----------------- [..yyyyyy]
      sixbit = btemp & 0x3F;
      data->buffer[data->index++] = _base64ValueToLetter[sixbit];

      // [rrrrxx]    [........]      [........]
      data->byte  = 0;
      data->state = 0;
      break;
   }

   data->buffer[data->index] = 0;

   returnTrue;
}

/******************************************************************************
func: _MiffBase64SetEnd

Send out the remainder bits.
******************************************************************************/
MiffB _MiffBase64SetEnd(MiffBase64Data * const data)
{
   MiffN1 sixbit;

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
      data->buffer[data->index++] = _base64ValueToLetter[sixbit];
      break;
   }

   data->byte                = 0;
   data->state               = 0;
   data->buffer[data->index] = 0;

   returnTrue;
}

/******************************************************************************
func: _MiffBase64Start
******************************************************************************/
void _MiffBase64Start(void)
{
   MiffN1 index;

   _MiffMemClearTypeArray(128, MiffN1, _base64LetterToValue);
   index = 0;
   _base64LetterToValue['A'] = index++;
   _base64LetterToValue['B'] = index++;
   _base64LetterToValue['C'] = index++;
   _base64LetterToValue['D'] = index++;
   _base64LetterToValue['E'] = index++;
   _base64LetterToValue['F'] = index++;
   _base64LetterToValue['G'] = index++;
   _base64LetterToValue['H'] = index++;
   _base64LetterToValue['I'] = index++;
   _base64LetterToValue['J'] = index++;
   //10
   _base64LetterToValue['K'] = index++;
   _base64LetterToValue['L'] = index++;
   _base64LetterToValue['M'] = index++;
   _base64LetterToValue['N'] = index++;
   _base64LetterToValue['O'] = index++;
   _base64LetterToValue['P'] = index++;
   _base64LetterToValue['Q'] = index++;
   _base64LetterToValue['R'] = index++;
   _base64LetterToValue['S'] = index++;
   _base64LetterToValue['T'] = index++;
   //20
   _base64LetterToValue['U'] = index++;
   _base64LetterToValue['V'] = index++;
   _base64LetterToValue['W'] = index++;
   _base64LetterToValue['X'] = index++;
   _base64LetterToValue['Y'] = index++;
   _base64LetterToValue['Z'] = index++;
   _base64LetterToValue['a'] = index++;
   _base64LetterToValue['b'] = index++;
   _base64LetterToValue['c'] = index++;
   _base64LetterToValue['d'] = index++;
   //30
   _base64LetterToValue['e'] = index++;
   _base64LetterToValue['f'] = index++;
   _base64LetterToValue['g'] = index++;
   _base64LetterToValue['h'] = index++;
   _base64LetterToValue['i'] = index++;
   _base64LetterToValue['j'] = index++;
   _base64LetterToValue['k'] = index++;
   _base64LetterToValue['l'] = index++;
   _base64LetterToValue['m'] = index++;
   _base64LetterToValue['n'] = index++;
   //40
   _base64LetterToValue['o'] = index++;
   _base64LetterToValue['p'] = index++;
   _base64LetterToValue['q'] = index++;
   _base64LetterToValue['r'] = index++;
   _base64LetterToValue['s'] = index++;
   _base64LetterToValue['t'] = index++;
   _base64LetterToValue['u'] = index++;
   _base64LetterToValue['v'] = index++;
   _base64LetterToValue['w'] = index++;
   _base64LetterToValue['x'] = index++;
   //50
   _base64LetterToValue['y'] = index++;
   _base64LetterToValue['z'] = index++;
   _base64LetterToValue['0'] = index++;
   _base64LetterToValue['1'] = index++;
   _base64LetterToValue['2'] = index++;
   _base64LetterToValue['3'] = index++;
   _base64LetterToValue['4'] = index++;
   _base64LetterToValue['5'] = index++;
   _base64LetterToValue['6'] = index++;
   _base64LetterToValue['7'] = index++;
   //60
   _base64LetterToValue['8'] = index++;
   _base64LetterToValue['9'] = index++;
   _base64LetterToValue['+'] = index++;
   _base64LetterToValue['/'] = index++;

   index = 0;
   _base64ValueToLetter[index++] = 'A';
   _base64ValueToLetter[index++] = 'B';
   _base64ValueToLetter[index++] = 'C';
   _base64ValueToLetter[index++] = 'D';
   _base64ValueToLetter[index++] = 'E';
   _base64ValueToLetter[index++] = 'F';
   _base64ValueToLetter[index++] = 'G';
   _base64ValueToLetter[index++] = 'H';
   _base64ValueToLetter[index++] = 'I';
   _base64ValueToLetter[index++] = 'J';
   _base64ValueToLetter[index++] = 'K';
   _base64ValueToLetter[index++] = 'L';
   _base64ValueToLetter[index++] = 'M';
   _base64ValueToLetter[index++] = 'N';
   _base64ValueToLetter[index++] = 'O';
   _base64ValueToLetter[index++] = 'P';
   _base64ValueToLetter[index++] = 'Q';
   _base64ValueToLetter[index++] = 'R';
   _base64ValueToLetter[index++] = 'S';
   _base64ValueToLetter[index++] = 'T';
   _base64ValueToLetter[index++] = 'U';
   _base64ValueToLetter[index++] = 'V';
   _base64ValueToLetter[index++] = 'W';
   _base64ValueToLetter[index++] = 'X';
   _base64ValueToLetter[index++] = 'Y';
   _base64ValueToLetter[index++] = 'Z';
   _base64ValueToLetter[index++] = 'a';
   _base64ValueToLetter[index++] = 'b';
   _base64ValueToLetter[index++] = 'c';
   _base64ValueToLetter[index++] = 'd';
   _base64ValueToLetter[index++] = 'e';
   _base64ValueToLetter[index++] = 'f';
   _base64ValueToLetter[index++] = 'g';
   _base64ValueToLetter[index++] = 'h';
   _base64ValueToLetter[index++] = 'i';
   _base64ValueToLetter[index++] = 'j';
   _base64ValueToLetter[index++] = 'k';
   _base64ValueToLetter[index++] = 'l';
   _base64ValueToLetter[index++] = 'm';
   _base64ValueToLetter[index++] = 'n';
   _base64ValueToLetter[index++] = 'o';
   _base64ValueToLetter[index++] = 'p';
   _base64ValueToLetter[index++] = 'q';
   _base64ValueToLetter[index++] = 'r';
   _base64ValueToLetter[index++] = 's';
   _base64ValueToLetter[index++] = 't';
   _base64ValueToLetter[index++] = 'u';
   _base64ValueToLetter[index++] = 'v';
   _base64ValueToLetter[index++] = 'w';
   _base64ValueToLetter[index++] = 'x';
   _base64ValueToLetter[index++] = 'y';
   _base64ValueToLetter[index++] = 'z';
   _base64ValueToLetter[index++] = '0';
   _base64ValueToLetter[index++] = '1';
   _base64ValueToLetter[index++] = '2';
   _base64ValueToLetter[index++] = '3';
   _base64ValueToLetter[index++] = '4';
   _base64ValueToLetter[index++] = '5';
   _base64ValueToLetter[index++] = '6';
   _base64ValueToLetter[index++] = '7';
   _base64ValueToLetter[index++] = '8';
   _base64ValueToLetter[index++] = '9';
   _base64ValueToLetter[index++] = '+';
   _base64ValueToLetter[index++] = '/';
}

/******************************************************************************
func: _MiffBase64Stop
******************************************************************************/
void _MiffBase64Stop(void)
{
   // Nothing to do.
}
