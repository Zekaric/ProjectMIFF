/******************************************************************************
file:       MiffLibTest
author:     Robbert de Groot
company:    Zekaric
copyright:  2021, Zekaric

description:
Test program for MiffLib
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

//#include "zlib.h"
#include "mifflib.h"
#include "jsonlib.h"

/******************************************************************************
local:
macro
******************************************************************************/
#if 1
#define INCLUDE_BIN
#endif

#define streq(S1, S2)  (strcmp(S1, S2) == 0)

/******************************************************************************
variable:
******************************************************************************/
static uint8_t _binary[] =
{
        0,   0,   0,
        1,   1,   1,
        2,   2,   2,
        3,   3,   3,
        4,   4,   4,
        5,   5,   5,
        6,   6,   6,
        7,   7,   7,
        8,   8,   8,
        9,   9,   9,
       10,  10,  10,
       11,  11,  11,
       12,  12,  12,
       13,  13,  13,
       14,  14,  14,
       15,  15,  15,
       16,  16,  16,
       17,  17,  17,
       18,  18,  18,
       19,  19,  19,
       20,  20,  20,
       21,  21,  21,
       22,  22,  22,
       23,  23,  23,
       24,  24,  24,
       25,  25,  25,
       26,  26,  26,
       27,  27,  27,
       28,  28,  28,
       29,  29,  29,
       30,  30,  30,
       31,  31,  31,
       32,  32,  32,
       33,  33,  33,
       34,  34,  34,
       35,  35,  35,
       36,  36,  36,
       37,  37,  37,
       38,  38,  38,
       39,  39,  39,
       40,  40,  40,
       41,  41,  41,
       42,  42,  42,
       43,  43,  43,
       44,  44,  44,
       45,  45,  45,
       46,  46,  46,
       47,  47,  47,
       48,  48,  48,
       49,  49,  49,
       50,  50,  50,
       51,  51,  51,
       52,  52,  52,
       53,  53,  53,
       54,  54,  54,
       55,  55,  55,
       56,  56,  56,
       57,  57,  57,
       58,  58,  58,
       59,  59,  59,
       60,  60,  60,
       61,  61,  61,
       62,  62,  62,
       63,  63,  63,
       64,  64,  64,
       65,  65,  65,
       66,  66,  66,
       67,  67,  67,
       68,  68,  68,
       69,  69,  69,
       70,  70,  70,
       71,  71,  71,
       72,  72,  72,
       73,  73,  73,
       74,  74,  74,
       75,  75,  75,
       76,  76,  76,
       77,  77,  77,
       78,  78,  78,
       79,  79,  79,
       80,  80,  80,
       81,  81,  81,
       82,  82,  82,
       83,  83,  83,
       84,  84,  84,
       85,  85,  85,
       86,  86,  86,
       87,  87,  87,
       88,  88,  88,
       89,  89,  89,
       90,  90,  90,
       91,  91,  91,
       92,  92,  92,
       93,  93,  93,
       94,  94,  94,
       95,  95,  95,
       96,  96,  96,
       97,  97,  97,
       98,  98,  98,
       99,  99,  99,
      100, 100, 100,
      101, 101, 101,
      102, 102, 102,
      103, 103, 103,
      104, 104, 104,
      105, 105, 105,
      106, 106, 106,
      107, 107, 107,
      108, 108, 108,
      109, 109, 109,
      110, 110, 110,
      111, 111, 111,
      112, 112, 112,
      113, 113, 113,
      114, 114, 114,
      115, 115, 115,
      116, 116, 116,
      117, 117, 117,
      118, 118, 118,
      119, 119, 119,
      120, 120, 120,
      121, 121, 121,
      122, 122, 122,
      123, 123, 123,
      124, 124, 124,
      125, 125, 125,
      126, 126, 126,
      127, 127, 127,
      128, 128, 128,
      129, 129, 129,
      130, 130, 130,
      131, 131, 131,
      132, 132, 132,
      133, 133, 133,
      134, 134, 134,
      135, 135, 135,
      136, 136, 136,
      137, 137, 137,
      138, 138, 138,
      139, 139, 139,
      140, 140, 140,
      141, 141, 141,
      142, 142, 142,
      143, 143, 143,
      144, 144, 144,
      145, 145, 145,
      146, 146, 146,
      147, 147, 147,
      148, 148, 148,
      149, 149, 149,
      150, 150, 150,
      151, 151, 151,
      152, 152, 152,
      153, 153, 153,
      154, 154, 154,
      155, 155, 155,
      156, 156, 156,
      157, 157, 157,
      158, 158, 158,
      159, 159, 159,
      160, 160, 160,
      161, 161, 161,
      162, 162, 162,
      163, 163, 163,
      164, 164, 164,
      165, 165, 165,
      166, 166, 166,
      167, 167, 167,
      168, 168, 168,
      169, 169, 169,
      170, 170, 170,
      171, 171, 171,
      172, 172, 172,
      173, 173, 173,
      174, 174, 174,
      175, 175, 175,
      176, 176, 176,
      177, 177, 177,
      178, 178, 178,
      179, 179, 179,
      180, 180, 180,
      181, 181, 181,
      182, 182, 182,
      183, 183, 183,
      184, 184, 184,
      185, 185, 185,
      186, 186, 186,
      187, 187, 187,
      188, 188, 188,
      189, 189, 189,
      190, 190, 190,
      191, 191, 191,
      192, 192, 192,
      193, 193, 193,
      194, 194, 194,
      195, 195, 195,
      196, 196, 196,
      197, 197, 197,
      198, 198, 198,
      199, 199, 199,
      200, 200, 200,
      201, 201, 201,
      202, 202, 202,
      203, 203, 203,
      204, 204, 204,
      205, 205, 205,
      206, 206, 206,
      207, 207, 207,
      208, 208, 208,
      209, 209, 209,
      210, 210, 210,
      211, 211, 211,
      212, 212, 212,
      213, 213, 213,
      214, 214, 214,
      215, 215, 215,
      216, 216, 216,
      217, 217, 217,
      218, 218, 218,
      219, 219, 219,
      220, 220, 220,
      221, 221, 221,
      222, 222, 222,
      223, 223, 223,
      224, 224, 224,
      225, 225, 225,
      226, 226, 226,
      227, 227, 227,
      228, 228, 228,
      229, 229, 229,
      230, 230, 230,
      231, 231, 231,
      232, 232, 232,
      233, 233, 233,
      234, 234, 234,
      235, 235, 235,
      236, 236, 236,
      237, 237, 237,
      238, 238, 238,
      239, 239, 239,
      240, 240, 240,
      241, 241, 241,
      242, 242, 242,
      243, 243, 243,
      244, 244, 244,
      245, 245, 245,
      246, 246, 246,
      247, 247, 247,
      248, 248, 248,
      249, 249, 249,
      250, 250, 250,
      251, 251, 251,
      252, 252, 252,
      253, 253, 253,
      254, 254, 254,
      255, 255, 255
};

static uint64_t _narray[] =
{
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};

static float _reals4[300] =
{
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
};

static double _reals8[300] =
{
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
};

static char *_strings[10] =
{
      "the quick brown fox jumped over the lazy dog.",
      "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      "I fart in your general direction.",
      "No one expects the Spanish Inquisition!",
      "And now for something completely different.",
      "the quick brown fox jumped over the lazy dog.\tTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.\nthe quick brown fox jumped over the lazy dog.",
      "I fart in your general direction.",
      "No one expects the Spanish Inquisition!",
      "And now for something completely different."
};
static int                  _bools[100] =
{
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
};

/******************************************************************************
prototype:
******************************************************************************/
static MiffB    _JsonGetBuffer(     void * const dataRepo, JsonN4 const byteCount, JsonN1       * const byteData);
static void    *_JsonMemCreate(     JsonN4 const memByteCount);
static void     _JsonMemDestroy(    void * const mem);
static JsonB    _JsonTestRead(      JsonStr const * const fileName);
static JsonB    _JsonTestReadObject(Json * const json);
static JsonB    _JsonSetBuffer(     void * const dataRepo, JsonN4 const byteCount, JsonN1 const * const byteData);
static JsonB    _JsonTestWrite(     JsonStr const * const fileName);

static MiffB    _MiffGetBuffer(     void * const dataRepo, MiffN4 const byteCount, MiffN1       * const byteData);
static void    *_MiffMemCreate(     MiffN4 const memByteCount);
static void     _MiffMemDestroy(    void * const mem);
static MiffB    _MiffSetBuffer(     void * const dataRepo, MiffN4 const byteCount, MiffN1 const * const byteData);
static MiffB    _MiffTestRead(      MiffStr const * const fileName);
static MiffB    _MiffTestWrite(     MiffStr const * const fileName);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: main
******************************************************************************/
int main(int acount, char **alist)
{
   int   result;

   acount; alist;

   result = 0;

   // Start miff library.
   if (!miffStart(_MiffMemCreate, _MiffMemDestroy) ||
       !jsonStart(_JsonMemCreate, _JsonMemDestroy))
   {
      result = 1;
      goto DONE;
   }

   // Test the writing.
   if (!_MiffTestWrite("FileTest.miff"))
   {
      result = 2;
      goto DONE;
   }

   if (!_MiffTestRead("FileTest.miff"))
   {
      result = 4;
      goto DONE;
   }

   if (!_JsonTestWrite("FileTest.json"))
   {
      result = 8;
      goto DONE;
   }

   if (!_JsonTestRead("FileTest.json"))
   {
      result = 16;
      goto DONE;
   }

DONE:
   // Stop miff library.
   miffStop();
   jsonStop();

   return result;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _GetBuffer
******************************************************************************/
static JsonB _JsonGetBuffer(void * const dataRepo, JsonN4 const byteCount, JsonN1 * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static MiffB _MiffGetBuffer(void * const dataRepo, MiffN4 const byteCount, MiffN1 * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _MemCreate
******************************************************************************/
void *_JsonMemCreate(JsonN4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

void *_MiffMemCreate(MiffN4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

/******************************************************************************
func: _MemDestroy
******************************************************************************/
void _JsonMemDestroy(void * const mem)
{
   free(mem);
}

void _MiffMemDestroy(void * const mem)
{
   free(mem);
}

/******************************************************************************
func: _SetBuffer
******************************************************************************/
static JsonB _JsonSetBuffer(void * const dataRepo, JsonN4 const byteCount, JsonN1 const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static MiffB _MiffSetBuffer(void * const dataRepo, MiffN4 const byteCount, MiffN1 const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _JsonTestRead
******************************************************************************/
static JsonB JSON_TEST_KEY(Json * const JSON, char const * const KEY) 
{
   JsonStr key[32];

   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return jsonFALSE;
   if (!(jsonGetStr(  JSON, 32, key) && strcmp(key, KEY) == 0))               return jsonFALSE;
   if (jsonGetTypeObj(JSON) != jsonTypeKEY_VALUE_SEPARATOR)                   return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_INT(Json * const JSON, JsonI const VALUE)
{
   JsonI itemp;

   if (jsonGetTypeObj(JSON) != jsonTypeNUMBER_INTEGER)                        return jsonFALSE;
   if (!jsonGetI(     JSON, &itemp))                                          return jsonFALSE;
   if (itemp != VALUE)                                                        return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_NAT(Json * const JSON, JsonN const VALUE)
{
   JsonType type;
   JsonN    ntemp;

   type = jsonGetTypeObj(JSON);
   if (!(type == jsonTypeNUMBER_INTEGER || type == jsonTypeNUMBER_NATURAL))   return jsonFALSE;
   if (!jsonGetN(      JSON, &ntemp))                                         return jsonFALSE;
   if (ntemp != VALUE)                                                        return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_REAL(Json * const JSON, JsonR VALUE)
{
   JsonType type;
   JsonR    rtemp;

   type = jsonGetTypeObj(JSON);
   if (!(type == jsonTypeNUMBER_REAL || type == jsonTypeNUMBER_INTEGER || type == jsonTypeNUMBER_NATURAL))  return jsonFALSE;
   if (!jsonGetR(      JSON, &rtemp))                                         return jsonFALSE;
   if (rtemp != VALUE)                                                        return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_REAL4(Json * const JSON, JsonR4 VALUE)
{
   JsonType type;
   JsonR4   r4temp;

   type = jsonGetTypeObj(JSON);
   if (!(type == jsonTypeNUMBER_REAL || type == jsonTypeNUMBER_INTEGER || type == jsonTypeNUMBER_NATURAL))  return jsonFALSE;
   if (!jsonGetR4(     JSON, &r4temp))                                        return jsonFALSE;
   if (r4temp != VALUE)                                                       return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_STR(Json * const JSON, char const * const VALUE)
{
   int            sindex;
   JsonStrLetter  jletter;
   JsonStr        sletter,
                  stemp[256];

   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return jsonFALSE;
   for (sindex = 0; ; sindex++) 
   {
      jletter = jsonGetStrLetter(JSON, &sletter);
      if (jletter == jsonStrLetterDONE)  break;
      if (jletter == jsonStrLetterERROR) break;
      stemp[sindex] = sletter;
   }
   if (jletter == jsonStrLetterERROR)                                         return jsonFALSE;

   stemp[sindex] = 0;
   if (strcmp(stemp, VALUE) != 0)                                             return jsonFALSE;
   return jsonTRUE;
}

static JsonB JSON_TEST_BIN(Json * const JSON, int binCount, JsonN1 const * const binData)
{
   int    bindex;
   JsonN1 byte;
   JsonN1 binBuffer[3*256];


   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return jsonFALSE;
   if (!jsonGetBinStart(JSON))                                                return jsonFALSE;
   for (bindex = 0; bindex < binCount; bindex++)
   {
      if (!jsonGetBinByte(JSON, &byte))                                       return jsonFALSE;
      binBuffer[bindex] = byte;
   }
   if (!jsonGetBinStop(JSON))                                                 return jsonFALSE;

   if (memcmp(binBuffer, binData, binCount) != 0)                             return jsonFALSE;
   return jsonTRUE;
}

static JsonB _JsonTestRead(JsonStr const * const fileName)
{
   int           index;
   FILE         *file;
   Json         *json;
   JsonB         result;
   JsonType      type;
   char const   *msg;
   JsonI         itemp;
   JsonN         ntemp;
   JsonR         rtemp;
   JsonR4        r4temp;
   JsonStr       stemp[256];

   file   = NULL;
   json   = NULL;
   result = jsonFALSE;

   for (;;)
   {
      msg = "Open file";
      if (fopen_s(&file, fileName, "rb") != 0) break;

      // Set Json up for reading.
      msg = "Create reader";
      json = jsonCreateReader(_JsonGetBuffer, (void *) file);
      if (!json) break;

      // Start of an object.
      msg = "Looking for object start";
      if (jsonGetTypeFile( json) != jsonTypeOBJECT_START)   break;

      msg = "Null";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_NULL)  break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "True";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_TRUE)  break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "False";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_FALSE) break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I 0";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_INT(  json, 0))                        break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I 1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_INT(  json, 1))                        break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I -1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_INT(  json, -1))                       break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I MAX";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_INT(  json, JsonI_MAX))                break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I MIN";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_INT(  json, JsonI_MIN))                break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "N 0";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_NAT(  json, 0))                        break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "N 1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_NAT(  json, 1))                        break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "N MAX";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_NAT(  json, JsonN_MAX))                break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R 0";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, 0.0))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R 1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, 1.0))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
      
      msg = "R -1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, -1.0))                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R PI";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, 3.1415926535897932))       break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R EPS";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, JsonR_EPSILON))            break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R MAX";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL( json, JsonR_MAX))                break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 0";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, 0.0f))                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, 1.0f))                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 -1";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, -1.0f))                    break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 PI";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, 3.1415926535897932f))      break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 EPS";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, JsonR4_EPSILON))           break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 MAX";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_REAL4(json, JsonR4_MAX))               break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "String";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_STR(  json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

#if defined(INCLUDE_BIN)
      msg = "Binary";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (!JSON_TEST_BIN(  json, 3 * 256, _binary))         break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
#endif

      msg = "Bool Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 100; index++)
      {
         type = jsonGetTypeElem(json);
         if (!((type == jsonTypeCONSTANT_TRUE  && _bools[index] == 1) ||
               (type == jsonTypeCONSTANT_FALSE && _bools[index] == 0)))
            break;

         type = jsonGetTypeElem(json);
         if ((index <  99 && type != jsonTypeSEPARATOR) ||
             (index == 99 && type != jsonTypeARRAY_STOP))   
            break;
      }
      if (index != 100)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 256; index++)
      {
         type = jsonGetTypeElem(json);
         if (type != jsonTypeNUMBER_INTEGER ||
             !jsonGetI(json, &itemp)        || 
             (JsonI) _narray[index] != itemp)
            break;
         
         type = jsonGetTypeElem(json);
         if ((index <  255 && type != jsonTypeSEPARATOR) ||
             (index == 255 && type != jsonTypeARRAY_STOP))     
            break;
      }
      if (index != 256)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "N Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 256; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_NATURAL ||
               type == jsonTypeNUMBER_INTEGER)  || 
             !jsonGetN(json, &ntemp)            || 
             _narray[index] != ntemp)
            break;
         
         type = jsonGetTypeElem(json);
         if ((index <  255 && type != jsonTypeSEPARATOR) ||
             (index == 255 && type != jsonTypeARRAY_STOP))     
            break;
      }
      if (index != 256)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 300; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_REAL    || 
               type == jsonTypeNUMBER_INTEGER || 
               type == jsonTypeNUMBER_NATURAL)  ||
             !jsonGetR(json, &rtemp)            || 
             _reals8[index] != rtemp)
            break;
         
         type = jsonGetTypeElem(json);
         if ((index <  299 && type != jsonTypeSEPARATOR) ||
             (index == 299 && type != jsonTypeARRAY_STOP))     
            break;
      }
      if (index != 300)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 300; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_REAL    ||
               type == jsonTypeNUMBER_INTEGER ||
               type == jsonTypeNUMBER_NATURAL)  ||
             !jsonGetR4(json, &r4temp)          ||
             _reals4[index] != r4temp)
            break;
         
         type = jsonGetTypeElem(json);
         if ((index <  299 && type != jsonTypeSEPARATOR) ||
             (index == 299 && type != jsonTypeARRAY_STOP))     
            break;
      }
      if (index != 300)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "String Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 10; index++)
      {
         type = jsonGetTypeElem(json);
         if (type != jsonTypeSTRING_START  ||
             !jsonGetStr(json, 256, stemp) ||
             strcmp(stemp, _strings[index]) != 0)
            break;
         
         type = jsonGetTypeElem(json);
         if ((index <  9 && type != jsonTypeSEPARATOR) ||
             (index == 9 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 10)                                      break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 3; index++)
      {
         if (!JSON_TEST_BIN(json, 3 * 256, _binary))        break;

         type = jsonGetTypeElem(json);
         if ((index <  2 && type != jsonTypeSEPARATOR) ||
             (index == 2 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 3)                                       break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
#endif

      msg = "User Type IntStrReal";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      {
         if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
         if (!jsonGetI(json, &itemp))                       break;
         if (itemp != 42)                                   break;

         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

         if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
         if (!jsonGetStr(json, 256, stemp))                 break;
         if (strcmp(stemp, "Yes, but what is the question?"))  break;

         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

         if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
         if (!jsonGetR(json, &rtemp))                       break;
         if (rtemp != 3.1415926535897932)                   break;

         if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
      }
      if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

      msg = "User Type IntStrReal Array";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      {
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
      }
      if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

      msg = "Block";
      if (!JSON_TEST_KEY(  json, msg))                      break;
      if (jsonGetTypeObj(json) != jsonTypeOBJECT_START)     break;
      {
         msg = "Null";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_NULL)  break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "True";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_TRUE)  break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "False";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeCONSTANT_FALSE) break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I 0";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_INT(  json, 0))                        break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I 1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_INT(  json, 1))                        break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I -1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_INT(  json, -1))                       break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I MAX";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_INT(  json, JsonI_MAX))                break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I MIN";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_INT(  json, JsonI_MIN))                break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "N 0";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_NAT(  json, 0))                        break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "N 1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_NAT(  json, 1))                        break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "N MAX";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_NAT(  json, JsonN_MAX))                break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R 0";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, 0.0))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R 1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, 1.0))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
      
         msg = "R -1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, -1.0))                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R PI";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, 3.1415926535897932))       break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R EPS";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, JsonR_EPSILON))            break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R MAX";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL( json, JsonR_MAX))                break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 0";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, 0.0f))                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, 1.0f))                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 -1";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, -1.0f))                    break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 PI";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, 3.1415926535897932f))      break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 EPS";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, JsonR4_EPSILON))           break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 MAX";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_REAL4(json, JsonR4_MAX))               break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "String";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_STR(  json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (!JSON_TEST_BIN(  json, 3 * 256, _binary))         break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
   #endif

         msg = "Bool Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 100; index++)
         {
            type = jsonGetTypeElem(json);
            if (!((type == jsonTypeCONSTANT_TRUE  && _bools[index] == 1) ||
                  (type == jsonTypeCONSTANT_FALSE && _bools[index] == 0)))
               break;

            type = jsonGetTypeElem(json);
            if ((index <  99 && type != jsonTypeSEPARATOR) ||
                (index == 99 && type != jsonTypeARRAY_STOP))   
               break;
         }
         if (index != 100)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 256; index++)
         {
            type = jsonGetTypeElem(json);
            if (type != jsonTypeNUMBER_INTEGER ||
                !jsonGetI(json, &itemp)        || 
                (JsonI) _narray[index] != itemp)
               break;
         
            type = jsonGetTypeElem(json);
            if ((index <  255 && type != jsonTypeSEPARATOR) ||
                (index == 255 && type != jsonTypeARRAY_STOP))     
               break;
         }
         if (index != 256)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "N Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 256; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_NATURAL ||
                  type == jsonTypeNUMBER_INTEGER)  || 
                !jsonGetN(json, &ntemp)            || 
                _narray[index] != ntemp)
               break;
         
            type = jsonGetTypeElem(json);
            if ((index <  255 && type != jsonTypeSEPARATOR) ||
                (index == 255 && type != jsonTypeARRAY_STOP))     
               break;
         }
         if (index != 256)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 300; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_REAL    || 
                  type == jsonTypeNUMBER_INTEGER || 
                  type == jsonTypeNUMBER_NATURAL)  ||
                !jsonGetR(json, &rtemp)            || 
                _reals8[index] != rtemp)
               break;
         
            type = jsonGetTypeElem(json);
            if ((index <  299 && type != jsonTypeSEPARATOR) ||
                (index == 299 && type != jsonTypeARRAY_STOP))     
               break;
         }
         if (index != 300)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 300; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_REAL    ||
                  type == jsonTypeNUMBER_INTEGER ||
                  type == jsonTypeNUMBER_NATURAL)  ||
                !jsonGetR4(json, &r4temp)          ||
                _reals4[index] != r4temp)
               break;
         
            type = jsonGetTypeElem(json);
            if ((index <  299 && type != jsonTypeSEPARATOR) ||
                (index == 299 && type != jsonTypeARRAY_STOP))     
               break;
         }
         if (index != 300)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "String Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 10; index++)
         {
            type = jsonGetTypeElem(json);
            if (type != jsonTypeSTRING_START  ||
                !jsonGetStr(json, 256, stemp) ||
                strcmp(stemp, _strings[index]) != 0)
               break;
         
            type = jsonGetTypeElem(json);
            if ((index <  9 && type != jsonTypeSEPARATOR) ||
                (index == 9 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 10)                                      break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 3; index++)
         {
            if (!JSON_TEST_BIN(json, 3 * 256, _binary))        break;

            type = jsonGetTypeElem(json);
            if ((index <  2 && type != jsonTypeSEPARATOR) ||
                (index == 2 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 3)                                       break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
   #endif

         msg = "User Type IntStrReal";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

         msg = "User Type IntStrReal Array";
         if (!JSON_TEST_KEY(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         {
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }

         if (jsonGetTypeObj(json) != jsonTypeOBJECT_STOP)       break;
      }

      if (jsonGetTypeObj(json) != jsonTypeOBJECT_STOP)          break;

      msg    = "NO";
      result = jsonTRUE;
      break;
   }
   printf("JSON Read: %s ERROR\n", msg);

   jsonDestroy(json);
   fclose(file);

   return result;
}

/******************************************************************************
func: _JsonTestWrite
******************************************************************************/
static JsonB _JsonTestWrite(JsonStr const * const fileName)
{
   FILE     *file;
   Json     *json;
   JsonB     result;
   JsonI4    index;

   file   = NULL;
   json   = NULL;
   result = jsonFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a json file.
      json = jsonCreateWriter(_JsonSetBuffer, (void *) file, jsonTRUE);
      if (!json)
      {
         break;
      }

      jsonSetObjectStart(json);

      jsonSetKey(              json, "Null");
      jsonSetValueNull(        json);

      jsonSetKey(              json, "True");
      jsonSetValueBool(        json, jsonTRUE);
      jsonSetKey(              json, "False");
      jsonSetValueBool(        json, jsonFALSE);

      jsonSetKey(              json, "I 0");
      jsonSetValueI(           json, 0);
      jsonSetKey(              json, "I 1");
      jsonSetValueI(           json, 1);
      jsonSetKey(              json, "I -1");
      jsonSetValueI(           json, -1);
      jsonSetKey(              json, "I MAX");
      jsonSetValueI(           json, JsonI_MAX);
      jsonSetKey(              json, "I MIN");
      jsonSetValueI(           json, JsonI_MIN);
                    
      jsonSetKey(              json, "N 0");
      jsonSetValueN(           json, 0);
      jsonSetKey(              json, "N 1");
      jsonSetValueN(           json, 1);
      jsonSetKey(              json, "N MAX");
      jsonSetValueN(           json, JsonN_MAX);

      jsonSetKey(              json, "R 0");
      jsonSetValueR(           json, 0.0);
      jsonSetKey(              json, "R 1");
      jsonSetValueR(           json, 1.0);
      jsonSetKey(              json, "R -1");
      jsonSetValueR(           json, -1.0);
      jsonSetKey(              json, "R PI");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetKey(              json, "R EPS");
      jsonSetValueR(           json, JsonR_EPSILON);
      jsonSetKey(              json, "R MAX");
      jsonSetValueR(           json, JsonR_MAX);

      jsonSetKey(              json, "R4 0");
      jsonSetValueR4(          json, 0.0f);
      jsonSetKey(              json, "R4 1");
      jsonSetValueR4(          json, 1.0f);
      jsonSetKey(              json, "R4 -1");
      jsonSetValueR4(          json, -1.0f);
      jsonSetKey(              json, "R4 PI");
      jsonSetValueR4(          json, 3.1415926535897932f);
      jsonSetKey(              json, "R4 EPS");
      jsonSetValueR4(          json, JsonR4_EPSILON);
      jsonSetKey(              json, "R4 MAX");
      jsonSetValueR4(          json, JsonR4_MAX);

      jsonSetKey(              json, "String");
      jsonSetValueStr(         json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
      jsonSetKey(              json, "Binary");
      jsonSetValueBin(         json, 3 * 256, _binary);
#endif

      jsonSetKey(              json, "Bool Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 100; index++)
      {
         jsonSetValueBool(json, _bools[index]);
      }
      jsonSetArrayStop(        json);
      
      jsonSetKey(              json, "I Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 256; index++)
      {
         jsonSetValueI(json, _narray[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "N Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 256; index++)
      {
         jsonSetValueN(json, _narray[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "R Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 300; index++)
      {
         jsonSetValueR(json, _reals8[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "R4 Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 300; index++)
      {
         jsonSetValueR4(json, _reals4[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "String Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 10; index++)
      {
         jsonSetValueStr(json, _strings[index]);
      }
      jsonSetArrayStop(        json);

#if defined(INCLUDE_BIN)
      jsonSetKey(              json, "Binary Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 3; index++)
      {
         jsonSetValueBin(json, 3 * 256, _binary);
      }
      jsonSetArrayStop(        json);
#endif

      jsonSetKey(              json, "User Type IntStrReal");
      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);


      jsonSetKey(              json, "User Type IntStrReal Array");
      jsonSetArrayStart(       json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStop(        json);


      jsonSetKey(        json, "Block");
      jsonSetObjectStart(json);
      {
         jsonSetKey(              json, "Null");
         jsonSetValueNull(        json);

         jsonSetKey(              json, "True");
         jsonSetValueBool(        json, jsonTRUE);
         jsonSetKey(              json, "False");
         jsonSetValueBool(        json, jsonFALSE);

         jsonSetKey(              json, "I 0");
         jsonSetValueI(           json, 0);
         jsonSetKey(              json, "I 1");
         jsonSetValueI(           json, 1);
         jsonSetKey(              json, "I -1");
         jsonSetValueI(           json, -1);
         jsonSetKey(              json, "I MAX");
         jsonSetValueI(           json, JsonI_MAX);
         jsonSetKey(              json, "I MIN");
         jsonSetValueI(           json, JsonI_MIN);
                    
         jsonSetKey(              json, "N 0");
         jsonSetValueN(           json, 0);
         jsonSetKey(              json, "N 1");
         jsonSetValueN(           json, 1);
         jsonSetKey(              json, "N MAX");
         jsonSetValueN(           json, JsonN_MAX);

         jsonSetKey(              json, "R 0");
         jsonSetValueR(           json, 0.0);
         jsonSetKey(              json, "R 1");
         jsonSetValueR(           json, 1.0);
         jsonSetKey(              json, "R -1");
         jsonSetValueR(           json, -1.0);
         jsonSetKey(              json, "R PI");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetKey(              json, "R EPS");
         jsonSetValueR(           json, JsonR_EPSILON);
         jsonSetKey(              json, "R MAX");
         jsonSetValueR(           json, JsonR_MAX);

         jsonSetKey(              json, "R4 0");
         jsonSetValueR4(          json, 0.0f);
         jsonSetKey(              json, "R4 1");
         jsonSetValueR4(          json, 1.0f);
         jsonSetKey(              json, "R4 -1");
         jsonSetValueR4(          json, -1.0f);
         jsonSetKey(              json, "R4 PI");
         jsonSetValueR4(          json, 3.1415926535897932f);
         jsonSetKey(              json, "R4 EPS");
         jsonSetValueR4(          json, JsonR4_EPSILON);
         jsonSetKey(              json, "R4 MAX");
         jsonSetValueR4(          json, JsonR4_MAX);

         jsonSetKey(              json, "String");
         jsonSetValueStr(         json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
         jsonSetKey(              json, "Binary");
         jsonSetValueBin(         json, 3 * 256, _binary);
#endif

         jsonSetKey(              json, "Bool Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 100; index++)
         {
            jsonSetValueBool(json, _bools[index]);
         }
         jsonSetArrayStop(        json);
      
         jsonSetKey(              json, "I Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 256; index++)
         {
            jsonSetValueI(json, _narray[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "N Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 256; index++)
         {
            jsonSetValueN(json, _narray[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "R Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 300; index++)
         {
            jsonSetValueR(json, _reals8[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "R4 Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 300; index++)
         {
            jsonSetValueR4(json, _reals4[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "String Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 10; index++)
         {
            jsonSetValueStr(json, _strings[index]);
         }
         jsonSetArrayStop(        json);

#if defined(INCLUDE_BIN)
         jsonSetKey(              json, "Binary Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 3; index++)
         {
            jsonSetValueBin(json, 3 * 256, _binary);
         }
         jsonSetArrayStop(        json);
#endif

         jsonSetKey(              json, "User Type IntStrReal");
         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);


         jsonSetKey(              json, "User Type IntStrReal Array");
         jsonSetArrayStart(       json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStop(        json);
      }
      jsonSetObjectStop(json);

      jsonSetObjectStop(json);

      result = jsonTRUE;
      break;
   }

   jsonDestroy(json);
   fclose(file);

   return result;
}

/******************************************************************************
func: _MiffTestRead
******************************************************************************/
static MiffB MIFF_TEST_KEY(Miff *miff, char const * const KEY, MiffRecType *recType, MiffN *arrayCount)
{
   MiffStr key[miffKeySIZE];

   memset(key, 0, miffKeySIZE);

   if (!miffGetRecordStart(miff, recType, arrayCount, key)) return miffFALSE;
   if (!streq(key, KEY))                                    return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_NULL_VALUE(Miff *miff)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNULL)        return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_B_VALUE(Miff *miff, MiffB testValue)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUMBER || 
       miffValueGetB(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_I_VALUE(Miff *miff, MiffI testValue)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUMBER || 
       miffValueGetI(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_N_VALUE(Miff *miff, MiffN testValue)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUMBER || 
       miffValueGetN(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_R_VALUE(Miff *miff, MiffR testValue)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUMBER || 
       miffValueIs4(    value)                   ||
       miffValueGetR(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_R4_VALUE(Miff *miff, MiffR4 testValue)
{
   MiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUMBER ||  
       !miffValueIs4(   value)                   || 
       miffValueGetR4(  value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_BIN_VALUE(Miff *miff, MiffN testCount, MiffN1 *testValue)
{
   MiffValue value;
   MiffN1    svalue[256 * 3];

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeBIN &&
       testCount               != miffValueGetBinCount(value))        return miffFALSE;
   if (!miffGetValueBin(miff, miffValueGetBinCount(value), svalue))   return miffFALSE;
   if (memcmp(svalue, testValue, testCount) != 0)           return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_STR_VALUE(Miff *miff, MiffStr *testValue)
{
   MiffValue value;
   MiffN     testValueLen;
   MiffStr   svalue[256];

   testValueLen = strlen(testValue);

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeSTR &&
       testValueLen            != miffValueGetStrCount(value))        return miffFALSE;
   if (!miffGetValueStr(miff, miffValueGetStrCount(value), svalue))   return miffFALSE;
   if (strcmp(testValue, svalue) != 0)                      return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_COUNT(MiffN COUNT, MiffN arrayCount)
{
   if (COUNT != arrayCount) return miffFALSE;
   return miffTRUE;
}

static MiffB MIFF_TEST_NEXT(Miff *miff) 
{
   if (!miffGetRecordEnd(miff)) return miffFALSE;
   return miffTRUE;
}

static MiffB _MiffTestRead(MiffStr const * const fileName)
{
   FILE           *file;
   Miff           *miff;
   MiffB           result;
   MiffStr         subFormatName[miffKeySIZE];
   MiffN           subFormatVersion;
   MiffRecType     recType;
   MiffN           arrayCount;
   MiffN           index;
   char const     *msg;

   file   = NULL;
   miff   = NULL;
   result = miffFALSE;

   for (;;)
   {
      msg = "Open file";
      if (fopen_s(&file, fileName, "rb") != 0) break;

      // Set Miff up for reading.
      msg = "Create reader";
      miff = miffCreateReader(
         miffTRUE,
         _MiffGetBuffer,
         subFormatName,
         &subFormatVersion,
         (void *) file);
      if (!miff) break;

      // Read in the header information.
      //wprintf(L"%S\t%d\n", (char *) subFormatName, (int) subFormatVersion);

      // Unless you are in full control of the format, do not assume that there will be an order
      // to the values.
      msg = "Null";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_NULL_VALUE(miff))                      break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "True";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_B_VALUE(   miff, miffTRUE))            break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "False";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_B_VALUE(   miff, miffFALSE))           break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I 0";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(   miff, 0))                   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I 1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(   miff, 1))                   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I -1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(   miff, -1))                  break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I MAX";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(   miff, MiffI_MAX))           break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I MIN";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(   miff, MiffI_MIN))           break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "N 0";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_N_VALUE(   miff, 0))                   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "N 1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_N_VALUE(   miff, 1))                   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "N MAX";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_N_VALUE(   miff, MiffN_MAX))           break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R 0";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, 0.0))                 break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R 1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, 1.0))                 break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R -1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, -1.0))                break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R PI";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, 3.1415926535897932))  break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R EPS";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, MiffR_EPSILON))       break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R MAX";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R_VALUE(   miff, MiffR_MAX))           break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 0";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, 0.0))                 break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, 1.0))                 break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 -1";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, -1.0))                break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 PI";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, 3.1415926535897932f)) break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 EPS";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, MiffR4_EPSILON))      break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 MAX";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_R4_VALUE(  miff, MiffR4_MAX))          break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "String";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_STR_VALUE(miff, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;
      if (!MIFF_TEST_NEXT(miff))                            break;

#if defined(INCLUDE_BIN)
      msg = "Binary";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_BIN_VALUE(miff, 256 * 3, _binary))     break;
      if (!MIFF_TEST_NEXT(miff))                            break;
#endif

      msg = "Bool Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(100, arrayCount))                break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_B_VALUE(miff, _bools[index]))         break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "I Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(256, arrayCount))                break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_I_VALUE(miff, (int) _narray[index]))  break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "N Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(256, arrayCount))                break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_N_VALUE(miff, _narray[index]))        break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(300, arrayCount))                break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_R_VALUE(miff, _reals8[index]))        break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "R4 Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(300, arrayCount))                break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_R4_VALUE(miff, _reals4[index]))       break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "String Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(10, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!MIFF_TEST_STR_VALUE(miff, _strings[index]))     break;
      }
      if (!MIFF_TEST_NEXT(miff))                            break;

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(3, arrayCount))                  break;
      if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
      if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
      if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
      if (!MIFF_TEST_NEXT(miff))                            break;
#endif

      msg = "User Type IntStrReal";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
      if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
      if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "User Type IntStrReal Array";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (!MIFF_TEST_COUNT(3, arrayCount))                  break;
      if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
      if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
      if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
      if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
      if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
      if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
      if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
      if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
      if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "Block";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (recType != miffRecTypeBLOCK_START)           break;
      if (!MIFF_TEST_NEXT(miff))                            break;
      {
         msg = "Null";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_NULL_VALUE(miff))                      break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "True";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_B_VALUE(   miff, miffTRUE))            break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "False";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_B_VALUE(   miff, miffFALSE))           break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I 0";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(   miff, 0))                   break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I 1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(   miff, 1))                   break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I -1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(   miff, -1))                  break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I MAX";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(   miff, MiffI_MAX))           break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I MIN";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(   miff, MiffI_MIN))           break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "N 0";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_N_VALUE(   miff, 0))                   break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "N 1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_N_VALUE(   miff, 1))                   break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "N MAX";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_N_VALUE(   miff, MiffN_MAX))           break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R 0";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, 0.0))                 break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R 1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, 1.0))                 break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R -1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, -1.0))                break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R PI";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, 3.1415926535897932))  break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R EPS";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, MiffR_EPSILON))       break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R MAX";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R_VALUE(   miff, MiffR_MAX))           break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 0";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, 0.0))                 break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, 1.0))                 break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 -1";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, -1.0))                break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 PI";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, 3.1415926535897932f)) break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 EPS";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, MiffR4_EPSILON))      break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 MAX";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_R4_VALUE(  miff, MiffR4_MAX))          break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "String";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_STR_VALUE(miff, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;
         if (!MIFF_TEST_NEXT(miff))                            break;

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_BIN_VALUE(miff, 256 * 3, _binary))     break;
         if (!MIFF_TEST_NEXT(miff))                            break;
   #endif

         msg = "Bool Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(100, arrayCount))                break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_B_VALUE(miff, _bools[index]))         break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "I Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(256, arrayCount))                break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_I_VALUE(miff, (int) _narray[index]))  break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "N Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(256, arrayCount))                break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_N_VALUE(miff, _narray[index]))        break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(300, arrayCount))                break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_R_VALUE(miff, _reals8[index]))        break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "R4 Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(300, arrayCount))                break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_R4_VALUE(miff, _reals4[index]))       break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "String Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(10, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!MIFF_TEST_STR_VALUE(miff, _strings[index]))     break;
         }
         if (!MIFF_TEST_NEXT(miff))                            break;

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(3, arrayCount))                  break;
         if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
         if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
         if (!MIFF_TEST_BIN_VALUE(miff, 3 * 256, _binary))     break;
         if (!MIFF_TEST_NEXT(miff))                            break;
   #endif

         msg = "User Type IntStrReal";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(1, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
         if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
         if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
         if (!MIFF_TEST_NEXT(miff))                            break;

         msg = "User Type IntStrReal Array";
         if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
         if (!MIFF_TEST_COUNT(3, arrayCount))                  break;
         if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
         if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
         if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
         if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
         if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
         if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
         if (!MIFF_TEST_I_VALUE(  miff, 42))                   break;
         if (!MIFF_TEST_STR_VALUE(miff, "Yes, but what is the question?")) break;
         if (!MIFF_TEST_R_VALUE(  miff, 3.1415926535897932))   break;
         if (!MIFF_TEST_NEXT(miff))                            break;
      }

      msg = "";
      if (!MIFF_TEST_KEY(miff, msg, &recType, &arrayCount)) break;
      if (recType != miffRecTypeBLOCK_STOP)            break;
      if (!MIFF_TEST_NEXT(miff))                            break;

      msg = "NO";
      result = miffTRUE;

      break;
   }

   printf("MIFF Read: %s ERROR\n", msg);

   miffDestroy(miff);
   fclose(file);

   return result;
}

/******************************************************************************
func: _MiffTestWrite
******************************************************************************/
static MiffB _MiffTestWrite(MiffStr const * const fileName)
{
   FILE     *file;
   Miff     *miff;
   MiffB  result;

   file   = NULL;
   miff   = NULL;
   result = miffFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a miff file.
      miff = miffCreateWriter(miffTRUE, _MiffSetBuffer, "MiffTestFile", 1, (void *) file);
      if (!miff)
      {
         break;
      }

      miffSetRecordNull(           miff, "Null");

      miffSetRecordBool(           miff, "True",     miffTRUE);
      miffSetRecordBool(           miff, "False",    miffFALSE);
                             
      miffSetRecordI(              miff, "I 0",      0);
      miffSetRecordI(              miff, "I 1",      1);
      miffSetRecordI(              miff, "I -1",     -1);
      miffSetRecordI(              miff, "I MAX",    MiffI_MAX);
      miffSetRecordI(              miff, "I MIN",    MiffI_MIN);
                             
      miffSetRecordN(              miff, "N 0",      0);
      miffSetRecordN(              miff, "N 1",      1);
      miffSetRecordN(              miff, "N MAX",    MiffN_MAX);
                             
      miffSetRecordR(              miff, "R 0",     0.0);
      miffSetRecordR(              miff, "R 1",     1.0);
      miffSetRecordR(              miff, "R -1",    -1.0);
      miffSetRecordR(              miff, "R PI",    3.1415926535897932);
      miffSetRecordR(              miff, "R EPS",   MiffR_EPSILON);
      miffSetRecordR(              miff, "R MAX",   MiffR_MAX);

      miffSetRecordR4(             miff, "R4 0",    0.0);
      miffSetRecordR4(             miff, "R4 1",    1.0);
      miffSetRecordR4(             miff, "R4 -1",   -1.0);
      miffSetRecordR4(             miff, "R4 PI",   3.1415926535897932f);
      miffSetRecordR4(             miff, "R4 EPS",  MiffR4_EPSILON);
      miffSetRecordR4(             miff, "R4 MAX",  MiffR4_MAX);
                             
      miffSetRecordStr(            miff, "String",  "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
#if defined(INCLUDE_BIN)
      miffSetRecordBinBuffer(      miff, "Binary",  256 * 3, _binary);
#endif
                             
      miffSetRecordBoolArray(      miff, "Bool Array",   100,  _bools);

      miffSetRecordIArray(         miff, "I Array",      256,  _narray);
      miffSetRecordNArray(         miff, "N Array",      256,  _narray);

      miffSetRecordRArray(         miff, "R Array",      300,  _reals8);
      miffSetRecordR4Array(        miff, "R4 Array",     300,  _reals4);

      miffSetRecordStrArray(       miff, "String Array", 10,   _strings);

#if defined(INCLUDE_BIN)
      miffSetRecordStartBinArray(  miff, "Binary Array", 3);
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetRecordStop(           miff);
#endif

      miffSetRecordStartValue(     miff, "User Type IntStrReal");
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
      miffSetRecordStop(           miff);

      miffSetRecordStartValueArray(miff, "User Type IntStrReal Array", 3);
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
                                    
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
                                    
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
      miffSetRecordStop(           miff);

      miffSetRecordBlockStart(     miff, "Block");
      {
         miffSetRecordNull(           miff, "Null");

         miffSetRecordBool(           miff, "True",     miffTRUE);
         miffSetRecordBool(           miff, "False",    miffFALSE);
                             
         miffSetRecordI(              miff, "I 0",      0);
         miffSetRecordI(              miff, "I 1",      1);
         miffSetRecordI(              miff, "I -1",     -1);
         miffSetRecordI(              miff, "I MAX",    MiffI_MAX);
         miffSetRecordI(              miff, "I MIN",    MiffI_MIN);
                             
         miffSetRecordN(              miff, "N 0",      0);
         miffSetRecordN(              miff, "N 1",      1);
         miffSetRecordN(              miff, "N MAX",    MiffN_MAX);
                             
         miffSetRecordR(              miff, "R 0",     0.0);
         miffSetRecordR(              miff, "R 1",     1.0);
         miffSetRecordR(              miff, "R -1",    -1.0);
         miffSetRecordR(              miff, "R PI",    3.1415926535897932);
         miffSetRecordR(              miff, "R EPS",   MiffR_EPSILON);
         miffSetRecordR(              miff, "R MAX",   MiffR_MAX);

         miffSetRecordR4(             miff, "R4 0",    0.0);
         miffSetRecordR4(             miff, "R4 1",    1.0);
         miffSetRecordR4(             miff, "R4 -1",   -1.0);
         miffSetRecordR4(             miff, "R4 PI",   3.1415926535897932f);
         miffSetRecordR4(             miff, "R4 EPS",  MiffR4_EPSILON);
         miffSetRecordR4(             miff, "R4 MAX",  MiffR4_MAX);
                             
         miffSetRecordStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
#if defined(INCLUDE_BIN)
         miffSetRecordBinBuffer(      miff, "Binary",   256 * 3, _binary);
#endif
                             
         miffSetRecordBoolArray(      miff, "Bool Array",   100,  _bools);

         miffSetRecordIArray(         miff, "I Array",      256,  _narray);
         miffSetRecordNArray(         miff, "N Array",      256,  _narray);

         miffSetRecordRArray(         miff, "R Array",      300,  _reals8);
         miffSetRecordR4Array(        miff, "R4 Array",     300,  _reals4);

         miffSetRecordStrArray(       miff, "String Array", 10,   _strings);

#if defined(INCLUDE_BIN)
         miffSetRecordStartBinArray(  miff, "Binary Array", 3);
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetRecordStop(           miff);
#endif

         miffSetRecordStartValue(     miff, "User Type IntStrReal");
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
         miffSetRecordStop(           miff);

         miffSetRecordStartValueArray(miff, "User Type IntStrReal Array", 3);
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
                                      
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
                                      
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
         miffSetRecordStop(           miff);
      }
      miffSetRecordBlockStop(miff);

      result = miffTRUE;
      break;
   }

   miffDestroy(miff);
   fclose(file);

   return result;
}
