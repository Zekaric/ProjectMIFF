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

#include "jsonlib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define loop                                       for (;;)
#define forCount(    INDEX, COUNT)                 for ((INDEX) = 0;         (INDEX) < (COUNT); (INDEX)++)
#define forCountDown(INDEX, COUNT)                 for ((INDEX) = COUNT - 1; (INDEX) >= 0;      (INDEX)--)
#define once                                       for (int __count__ = 0; __count__ < 1; __count__++)

#define breakIf(   EXP)                            if (EXP) { break;    }
#define continueIf(EXP)                            if (EXP) { continue; }
#define gotoIf(    EXP, LABEL)                     if (EXP) { goto LABEL; }

#if !defined(min)
#define min(A, B)                                  (((A) < (B)) ? (A) : (B))
#define max(A, B)                                  (((A) < (B)) ? (B) : (A))
#endif

#define returnFalse                                return jsonBoolFALSE
#define returnNull                                 return NULL
#define returnTrue                                 return jsonBoolTRUE
#define returnIf(     EXP, VALUE)                  if (EXP) { return (VALUE); }
#define return0If(    EXP)                         if (EXP) { return 0;       }
#define returnFalseIf(EXP)                         if (EXP) { returnFalse;    }
#define returnNullIf( EXP)                         if (EXP) { returnNull;     }
#define returnTrueIf( EXP)                         if (EXP) { returnTrue;     }
#define returnVoidIf( EXP)                         if (EXP) { return;         }

#define U_LITERAL_HELP(STR)                        L ## STR
#define U_LITERAL(     STR)                        U_LITERAL_HELP(STR)

#define jsonARRAY_START_STR                        "["
#define jsonARRAY_STOP_STR                         "]"
#define jsonOBJECT_START_STR                       "{"
#define jsonOBJECT_STOP_STR                        "}"
#define jsonSEPARATOR_STR                          ","
#define jsonKEY_VALUE_SEPARATOR_STR                ":"
#define jsonSTRING_QUOTE_STR                       "\""
#define jsonSTRING_ESCAPE_STR                      "\\"
#define jsonSTRING_ESCAPE_QUOTE_STR                "\""
#define jsonSTRING_ESCAPE_SLASH_STR                "\\"
#define jsonSTRING_ESCAPE_BACKSLASH_STR            "/"
#define jsonSTRING_ESCAPE_BACKSPACE_STR            "b"
#define jsonSTRING_ESCAPE_FORMFEED_STR             "f"
#define jsonSTRING_ESCAPE_LINEFEED_STR             "n"
#define jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR      "r"
#define jsonSTRING_ESCAPE_UNICODE_STR              "u"
#define jsonSTRING_ESCAPE_TAB_STR                  "t"
#define jsonNUMBER_EXPONENT_1_STR                  "E"
#define jsonNUMBER_EXPONENT_2_STR                  "e"
#define jsonNUMBER_PLUS_STR                        "+"
#define jsonNUMBER_MINUS_STR                       "-"
#define jsonNUMBER_DECIMAL_STR                     "."

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
JsonBool  _JsonC1EncodedToC1(     JsonN4  * const c1Count, JsonC1       * const c1);
JsonBool  _JsonC1ToC1Encoded(     JsonN4    const c1Count, JsonC1 const * const c1, JsonN4 * const c1eCount, JsonC1 ** const c1e);
void      _JsonC1ToC2(            JsonN4    const c1Count, JsonC1 const * const c1, JsonN4 * const c2Count,  JsonC2 *  const c2);
void      _JsonC1ToC2Key(         JsonN4    const c1Count, JsonC1 const * const c1, JsonN1 * const c2Count,  JsonC2 *  const c2);
JsonI8    _JsonC1ToI(             JsonN4    const c1Count, JsonC1 const * const c1);
JsonN8    _JsonC1ToN(             JsonN4    const c1Count, JsonC1 const * const c1);
JsonC2   *_JsonC2Append(          JsonC2 const * const a, JsonC2 const * const b, JsonC2 const * const c);
JsonC2   *_JsonC2Clone(           JsonN4    const c2Count, JsonC2 const * const c2);
JsonBool  _JsonC2ToC1(            JsonN4    const c2Count, JsonC2 const * const c2, JsonN4 * const c1Count, JsonC1 ** const c1);
JsonBool  _JsonC2ToC1Key(         JsonN4    const c2Count, JsonC2 const * const c2, JsonN1 * const c1Count, JsonC1 *  const c1);
JsonN4    _JsonC1LetterToC4Letter(JsonC1 const * const c1, JsonC4 * const c4);
JsonN4    _JsonC2LetterToC4Letter(JsonC2 const * const c2, JsonC4 * const c4);
JsonN4    _JsonC4LetterToC1Letter(JsonC4 const c4Letter, JsonC1 * const a, JsonC1 * const b, JsonC1 * const c, JsonC1 * const d);
JsonN4    _JsonC4LetterToC2Letter(JsonC4 const c4Letter, JsonC2 * const a, JsonC2 * const b);

#define   _JsonC1GetCount(STR)                           ((JsonN4) strlen((char const *)    STR))
#define   _JsonC2GetCount(STR)                           ((JsonN4) wcslen((wchar_t const *) STR))

void     *_JsonMemCreate(         JsonN4 const memByteCount);
void      _JsonMemDestroy(        void * const mem);
JsonBool  _JsonMemIsEqual(        JsonN4 const countA, JsonN1 const * const memA, JsonN4 const countB, JsonN1 const * const memB);
void      _JsonMemStart(          JsonMemCreate const memCreateFunc, JsonMemDestroy const memDestroyFunc);
void      _JsonMemStop(           void);

#define   _JsonMemClearType(             TYPE, MEM)               memset((MEM), 0,               sizeof(TYPE))
#define   _JsonMemClearTypeArray( COUNT, TYPE, MEM)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define   _JsonMemCopyTypeArray(  COUNT, TYPE, DST, SRC)          memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define   _JsonMemCreateType(            TYPE)           (TYPE *) _JsonMemCreate(                    sizeof(TYPE))
#define   _JsonMemCreateTypeArray(COUNT, TYPE)           (TYPE *) _JsonMemCreate(          (COUNT) * sizeof(TYPE))

JsonBool  _JsonReadLine(          Json       * const json);
JsonBool  _JsonReadLineSkip(      Json       * const json);
JsonBool  _JsonReadPart(          Json       * const json);
JsonBool  _JsonReadArrayCount(    Json       * const json, JsonN4   * const count);
JsonBool  _JsonReadC2Key(         Json       * const json, JsonC2   * const key);
JsonBool  _JsonReadR4(            Json       * const json, JsonR4   * const value);
JsonBool  _JsonReadR4S(           Json       * const json, JsonR4   * const value);
JsonBool  _JsonReadR8(            Json       * const json, JsonR8   * const value);
JsonBool  _JsonReadR8S(           Json       * const json, JsonR8   * const value);

JsonBool  _JsonWriteC1(           Json const * const json, JsonC1 const * const value);
JsonBool  _JsonWriteC2(           Json const * const json, JsonC2 const * const value);
JsonBool  _JsonWriteC2Key(        Json const * const json, JsonC2 const * const key);
JsonBool  _JsonWriteI(            Json       * const json, JsonI8 const value);
JsonBool  _JsonWriteIndent(       Json       * const json);
JsonBool  _JsonWriteN(            Json       * const json, JsonN8 const value);
JsonBool  _JsonWriteR4(           Json       * const json, JsonR4 const value);
JsonBool  _JsonWriteR8(           Json       * const json, JsonR8 const value);
JsonBool  _JsonWriteString(       Json       * const json, JsonC2 const * const value);

#endif
