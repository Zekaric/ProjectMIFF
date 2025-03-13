/******************************************************************************
file:       local
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

#if !defined(LIBJSON_LOCAL)
#define      LIBJSON_LOCAL

/******************************************************************************
include:
******************************************************************************/
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "b64lib.h"
#include "jsonlib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define jsonARRAY_START_STR                        "["
#define jsonARRAY_STOP_STR                         "]"
#define jsonOBJECT_START_STR                       "{"
#define jsonOBJECT_STOP_STR                        "}"
#define jsonSEPARATOR_STR                          ","
#define jsonKEY_VALUE_SEPARATOR_STR                ":"
#define jsonSTRING_QUOTE_STR                       "\""
#define jsonSTRING_ESCAPE_STR                      "\\"
#define jsonSTRING_ESCAPE_QUOTE_STR                "\\\""
#define jsonSTRING_ESCAPE_BACK_SLASH_STR           "\\\\"
#define jsonSTRING_ESCAPE_FORWARD_SLASH_STR        "\\/"
#define jsonSTRING_ESCAPE_BACKSPACE_STR            "\\b"
#define jsonSTRING_ESCAPE_FORMFEED_STR             "\\f"
#define jsonSTRING_ESCAPE_LINEFEED_STR             "\\n"
#define jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR      "\\r"
#define jsonSTRING_ESCAPE_UNICODE_STR              "\\u"
#define jsonSTRING_ESCAPE_TAB_STR                  "\\t"
#define jsonNUMBER_EXPONENT_1_STR                  "E"
#define jsonNUMBER_EXPONENT_2_STR                  "e"
#define jsonNUMBER_PLUS_STR                        "+"
#define jsonNUMBER_MINUS_STR                       "-"
#define jsonNUMBER_DECIMAL_STR                     "."

#define jsonARRAY_START_CHAR                       '['
#define jsonARRAY_STOP_CHAR                        ']'
#define jsonOBJECT_START_CHAR                      '{'
#define jsonOBJECT_STOP_CHAR                       '}'
#define jsonSEPARATOR_CHAR                         ','
#define jsonKEY_VALUE_SEPARATOR_CHAR               ':'
#define jsonSTRING_QUOTE_CHAR                      '\"'
#define jsonBACK_SLASH_CHAR                        '\\'
#define jsonFOREWARD_SLASH_CHAR                    '/'
#define jsonNUMBER_EXPONENT_1_CHAR                 'E'
#define jsonNUMBER_EXPONENT_2_CHAR                 'e'
#define jsonNUMBER_PLUS_CHAR                       '+'
#define jsonNUMBER_MINUS_CHAR                      '-'
#define jsonNUMBER_DECIMAL_CHAR                    '.'

#define jsonIS_SPACE(BYTE) (BYTE == ' ' || BYTE == '\t' || BYTE == '\n' || BYTE == '\r')

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Gb     _JsonEatSpace(                  Gjson       * const json);

Gb     _JsonGetChar(                   Gjson       * const json);
GjsonType  _JsonGetFalse(                  Gjson       * const json);
GjsonType  _JsonGetNull(                   Gjson       * const json);
GjsonType  _JsonGetNumber(                 Gjson       * const json);
GjsonType  _JsonGetNumberInteger(          Gjson       * const json, Gstr * const str);
GjsonType  _JsonGetNumberReal(             Gjson       * const json, Gstr * const str);
GjsonType  _JsonGetTrue(                   Gjson       * const json);

Gb     _JsonIsSpace(                   Gjson       * const json);

void     *_JsonMemCreate(                 Gn8 const memByteCount);
void      _JsonMemDestroy(                void * const mem);
void      _JsonMemStart(                  GmemCloc const memCreateFunc, GmemDloc const memDestroyFunc);
void      _JsonMemStop(                   void);

#define   _JsonMemClearType(             TYPE, MEM)               memset((MEM), 0,               sizeof(TYPE))
#define   _JsonMemClearTypeArray( COUNT, TYPE, MEM)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define   _JsonMemCopyTypeArray(  COUNT, TYPE, DST, SRC)          memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define   _JsonMemCreateType(            TYPE)           (TYPE *) _JsonMemCreate(                    sizeof(TYPE))
#define   _JsonMemCreateTypeArray(COUNT, TYPE)           (TYPE *) _JsonMemCreate(          (COUNT) * sizeof(TYPE))

Gb     _JsonStrStart(                  void);
void      _JsonStrStop(                   void);
Gn1    _JsonStrToHex(                  Gn1 const value);
Gn8     _JsonStrToN(                    Gstr const * const str);

#define   _JsonStrGetCount(STR)   ((Gn4) strlen((char const *)    STR))

_locale_t _JsonLocaleGet(        void);

Gb     _JsonSetBuffer(        Gjson const * const json, Gn8 const bufCount, Gn1 const * const buf);
Gb     _JsonSetI(             Gjson       * const json, Gi8 const value);
Gb     _JsonSetIndent(        Gjson       * const json);
Gb     _JsonSetN(             Gjson       * const json, Gn8 const value);
Gb     _JsonSetNewLine(       Gjson       * const json);
Gb     _JsonSetR(             Gjson       * const json, Gr8 const value);
Gb     _JsonSetR4(            Gjson       * const json, Gr4 const value);

#endif
