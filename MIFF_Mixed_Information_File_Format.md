
# M.I.F.F.: Mixed Information File Format


**`Author:            `** Robbert de Groot

**`Date:              `** 2019-05-15

**`Copyright:         `** 2019, Robbert de Groot

**`License (Library): `** MIT License.

**`License (Document):`** Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)

## Table Of Contents:

**1 - M.I.F.F.**<br />
    1.1 - Discussion<br />
    1.2 - Goals<br />
    1.3 - Disclosure<br />
**2 - MIFF Format: Base**<br />
    2.1 - File Header<br />
    2.2 - Content<br />
        2.2.1 - Numbers<br />
        2.2.2 - Key Value Composition<br />
            2.2.2.1 - Key<br />
            2.2.2.2 - Value Header<br />
                2.2.2.2.1 - Type Code<br />
                2.2.2.2.2 - Array and Compression flag<br />
            2.2.2.3 - Value<br />
    2.3 - Text Format<br />
        2.3.1 - Whitespace Characters<br />
        2.3.2 - Separator Characters<br />
        2.3.3 - Printable Characters<br />
        2.3.4 - Base64<br />
        2.3.5 - Real Number Representation<br />
        2.3.6 - Key Value Format<br />
            2.3.6.1 - Key<br />
            2.3.6.2 - Value Header<br />
            2.3.6.3 - Value<br />
        2.3.7 - Value Specifics<br />
            2.3.7.1 - Key-Value Block<br />
            2.3.7.2 - String values<br />
        2.3.8 - Path Values<br />
            2.3.8.1 - Simple values<br />
            2.3.8.2 - Binary values<br />
            2.3.8.3 - Boolean values<br />
            2.3.8.4 - Image values<br />
                2.3.8.4.1 - img1, imga1, img2, imga2, img4, imga4, imgr4, imgar4<br />
                imgfile<br />
                imgpath<br />
            2.3.8.5 - Type value<br />
            2.3.8.6 - Time values<br />
                2.3.8.6.1 - time<br />
                2.3.8.6.2 - timedate<br />
                2.3.8.6.3 - timehour<br />
                2.3.8.6.4 - times&#42;, timems&#42;, timeus&#42;<br />
            2.3.8.7 - Value stream<br />
            2.3.8.8 - User Type<br />
            2.3.8.9 - User Data<br />
    2.4 - Binary<br />
        2.4.1 - Key Value Format<br />
            2.4.1.1 - Key<br />
            2.4.1.2 - Value Header<br />
            2.4.1.3 - Value<br />
        2.4.2 - Value Specifics<br />
            2.4.2.1 - Key-Value Block<br />
            2.4.2.2 - String, Path, and Relative Path values<br />
            2.4.2.3 - Simple values<br />
            2.4.2.4 - Binary<br />
            2.4.2.5 - Boolean<br />
            2.4.2.6 - Image values<br />
            2.4.2.7 - Type value<br />
            2.4.2.8 - Time value<br />
                2.4.2.8.1 - time<br />
                2.4.2.8.2 - timedate<br />
                2.4.2.8.3 - timetime<br />
            2.4.2.9 - Value stream<br />
            2.4.2.10 - User Type<br />
            2.4.2.11 - User Data<br />
**3 - Design Decisions:**<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals



* **`Simple  `**  The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **`Brief   `**  The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **`Flexible`**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **`Accurate`**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing options which I find lacking in some fashion.

I can be reached at the following two email addresses.

robbert.degroot@hexagon.com<br />zekaric@gmail.com

# 2 - MIFF Format: Base


There are two representations of the format.  A Text file and a Binary file representation.  They will both contain exactly the same data and be exactly the same in feature set.  The Binary file may have the advantage of possibly being slightly more compact and slightly faster in reading and writing.

A MIFF format is essentially a collection of key value pairs.  Values can be a collection of further key value pairs.  Meaning nesting is possible.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This is meant to be a transfer file from one program to another.

Common to both Text and Binary formats, any Byte data that is encode or stored as a binary byte sequence will be in big endian order.  In a text file the data is stored in Base64 but without any line feeds, cursor returns to break up the buffer.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.

```
MIFF_TXT n8 1\n
[Sub-Format Name] n8 [Sub-Format Version]\n
```

or

```
MIFF_BIN n8 1\n
[Sub-Format Name] n8 [Sub-Format Version]\n
```

These lines may look a little odd if you do not know anything about the MIFF format.  In short the first two lines are MIFF Key Value pairs in Text format.

First line is the MIFF format and style for the file and the version number of the MIFF format.

Second line is the sub format of the MIFF file and the version of the sub format.

"MIFF_TXT" on the first line indicates that after the second line we will see key value pairs in text MIFF representation.  "MIFF_BIN" means that after the \n of the second line we will see the key value pairs in the binary MIFF representation.

"n8" indicates that the version number is a natural number using 8 Bytes.  Natural number being unsigned integer for programmers.

Currently MIFF is at version 1.  This will only use whole numbers instead of the usual XX.XX.XX.XX versioning of software.  This is written as a readable decimal number.

[Sub-Format Name] will be a UTF8 string and defines the format of what is being stored in the file.  Do not include spaces in this name.

Like the first line we have "n8" and a [Sub-Format Version].

## 2.2 - Content


Before going into the specifics of the format we will cover some things at a high level.

### 2.2.1 - Numbers


Natural numbers (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

Integer numbers are numbers are positive and negative whole numbers.

Real numbers (floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.2 - Key Value Composition


```
[key] [value header] [value]\n
```

**[key]         ** will be a string that will somewhat uniquely identify the value.  No longer than 255 Bytes in UTF8 format.  Longer keys will be truncated.

**[value header]** will be an code (binary) or sequence of characters (text) that will dictate what the value will look like.

**[value]       ** will depend on the **[value header]** on what is contained here.

#### 2.2.2.1 - Key


Keys are always a single string of characters and are limited to printable UNICODE code points.  Meaning no whitespace of any kind inside a key.

```
12345
;':][.,<>]'
a_b_c
$cost
ฤ돥
```

The key can be nonsense or just a number.  As long it does not include any whitespace.  It is not limited to just the English character set.

#### 2.2.2.2 - Value Header


A value header is comprised of a type code and an array and compression flag.

##### 2.2.2.2.1 - Type Code


| Binary Type Code | Text Type Code | Description |
| --- | --- | --- |
| <br />**Block Types**<br /> | 	
 | 	
 |
| 1 | [] | Key-Value block. |
| 2 | [...] | Value stream.  The contents deviates from the rest of the format.  Representations will explained where they are used. |
| 3 | ... | Binary data. |
| <br />**Basic Types**<br /> |
| 4 | type | Type value.  One of these type codes. |
| 5 | "" | String (UTF8) data. |
| 6 | bool | Boolean value. |
| 11, 12, 13, 14, 15, 16, 17, 18,<br />19, 20, 21, 22, 23, 24, 25, 26 | i1, i2, i3, i4, i5, i6, i7, i8,<br />i9, i10, i11, i12, i13, i14, i15, i16 | An integer number using 1 to 16 Bytes.  Min and Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for a lot of these, like i3, but I include these byte counts because they may be useful to someone. |
| 31, 32, 33, 34, 35, 36, 37, 38,<br />39, 40, 41, 42, 43, 44, 45, 46 | n1, n2, n3, n4, n5, n6, n7, n8,<br />n9, n10, n11, n12, n13, n14, n15, n16 | A natural number using 1 to 16 Bytes.  Ranges from 0 to max value.  Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for a lot of these, like i3, but I include these byte counts because they may be useful to someone. |
| 50, 51, 52 | r4, r8, r10 | A real value using 4 (float), 8 (double), or 10 (long double) Bytes. |
| <br />**Complex Types**<br /> |
| 60, 61 | dms, dms6, dms8 | Degree, Minute, Second.  For dms, degree is an i2, minute is an n1, and second is and n1.  Form dms6 the second is changed to an n3 for fractional portion ranging from 0.00000 to 59.99999.  dms8, similar to dms6 but s is an n5 ranging from 0.0000000000 to 59.9999999999.  For reference, equator is 40 075 Km long.  1 degree is about 111.32 Km.  1 minute is about 1.86 Km.  1 second is about 30.92 m.  0.00001s (dms6) is about 0.3092 mm (if my math is correct.) |
| 70 | guid | Id value as a GUID. |
| 80 | path | A relative path (relative to the MIFF file location.) |
| 90, 91,<br />92, 93,<br />94, 95,<br />96, 97 | rgb1, rgba1,<br />rgb2, rgba2,<br />rgb4, rgba4,<br />rgbr4, rgbar4 | Color using n1, n2, n4, or r4 per component.  In byte order Red, Green, Blue, and Alpha (if alpha is required.) |
| 100, 102, 103, 104 | cmyk1, cmyk2, cmyk4, cmykr4 | Color using n1, n2, n4, or r4 per component.  In byte order Cyan, Magenta, Yellow, and Black. |
| 110, 111,<br />112, 113,<br />114, 115,<br />116, 117 | img1, imga1,<br />img2, imga2,<br />img4, imga4,<br />imgr4, imgar4 | An image using rgbn1, rgban1, rgbr4, and rgbar4 values.  A width and height follows that are both n4 each. |
| 120 | imgfile | An image file inlined.  Keep to known formats, PNG for non-photographs, and regular JPG for photographs. |
| 121 | imgpath | An image file path.  Follows path rules. |
| 130, 131 | mat3r4, mat3r8 | 3 x 3 matrix using r4 or r8 values.  Written out left to right, top to bottom order.  Meaning, top left value of the matrix is written first, then second element of the top row, and so on. |
| 140, 141 | mat4r4, mat4r8 | 4 x 4 matrix using r4 or r8 values. |
| 150, 151 | matNr4, matNr8 | N x N matrix using r4 or r8 values.  First value defines N and it will be an n4. |
| 160, 161 | matNMr4, matNMr8 | N x M matrix using r4 or r8 values.  First two values defines N and M and they will be an n4. |
| 170, 171, 175, 176 | v2r4, v2r8, cplx4, cplx8 | 2 value item using r4 or r8.  Meaning (value1, value2), (Cartesian X, Cartesian Y), (Easting, Northing), (Texture U, Texture V), (Latitude, Longitude), complex number (Real, Imaginary).  Cartesian meaning mathematical XYZ and not Computer Graphics XYZ.  Which means you may need to YZ swap. |
| 180, 181 | v2i4, v2i8 | 2 value item using i4 or i8. |
| 190, 191 | v2n4, v2n8 | 2 value item using n4 or n8. |
| 200, 201 | v3r4, v3r8 | 3 value item using r4 or r8.  Meaning (value1, value2, value3), (Cartesian X, Cartesian Y, Cartesian Z), (Easting, Northing, Elevation (Z)), (Texture U, Texture V, Texture W), (Latitude, Longitude, Elevation (Z)).  Cartesian meaning mathematical XYZ and not Computer Graphics XYZ.  Which means you may need to YZ swap. |
| 210, 211 | v3i4, v3i8 | 3 value item using i4 or i8. |
| 220, 221 | v3n4, v3n8 | 3 value item using n4 or n8. |
| 230, 231, 235, 236 | v4r4, v4r8, quat4, quat8 | 4 value item using r4 or r8.  Meaning (value1, value2, value3, value4), (Cartesian X, Cartesian Y, Cartesian Z, W), quaternion value (Real, I, J, K).  Cartesian meaning mathematical XYZ and not Computer Graphics XYZ.  Which means you may need to YZ swap. |
| 240, 241 | v4i4, v4i8 | 4 value item using i4 or i8. |
| 250, 251 | v4n4, v4n8 | 4 value item using n4 or n8. |
| 260, 261, 262 | time, timedate, timehour | Time as a full date and time, date only, time only |
| 270, 271, 272,<br />275, 276, 277 | times4, timems4, timeus4,<br />times8, timems8, timeus8 | A time difference as a second value, millisecond value, microsecond value. |
| <br />**User Type**<br /> |
| 7 | userType | A series of key-type pairs. |
| 8 | userData | Which type to use and then a series of values. |


##### 2.2.2.2.2 - Array and Compression flag


| Encoding Code (Binary) | Encoding Code (Text) | Description |
| --- | --- | --- |
| &#42;0 | - | A single value follows. |
| &#42;1 | = | An array of values follows. |
| 0&#42; | z | A single value compressed with zlib follows. |
| 1&#42; | Z | An array of values compressed with zlib follows. |


#### 2.2.2.3 - Value


The value header generally indicates what will be stored in the value of the key value pair.  It then becomes an issue on how it is represented in the two types of files.  See their respective sections for the details.

## 2.3 - Text Format


The text format is a UTF8 text file.

The format is only somewhat human readable, and only somewhat text editor friendly.  It was not designed to be "human friendly" but rather "debug friendly."  What I mean is that you will be able to look at the format in a text editor, if the text editor is capable of handling really large files with really long lines; and if you know this format, you will be able to debug it.  To a certain degree you should be able to debug the binary version as well since it will not be too different than the text version.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or you may not be seeing the whole file.

All lines are terminated with a '\n'.  NOT an MSDOS/Windows \r\n or a MAC \n\r ending or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.  No exceptions.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF limits what white space characters can be used as separators.

```
' ' (Space), '\t' (Tab)
```

If you see any other whitespace characters used as a separator in the MIFF file then the file is not MIFF.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character makes a mark, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Base64


A few items are stored in Base64.  This is to add a little compression to the items in question or to ensure exactness of the data when reading the values back in.  Unlike normal Base64 which will break the buffer into lines of a certain length, Base64 values in a MIFF are one long continuous stream of characters.  No \n or line breaks of any kind will be present.

### 2.3.5 - Real Number Representation


All real numbers are stored as a Base64 string.  No exceptions.  This is to ensure that when read back into memory from file, the number is exactly as it was when it was stored in the file.  The real value in the MIFF file will be in Big Endian order.

### 2.3.6 - Key Value Format


```
[key] [value header] [value]\n
```

There can be leading separators before the [key] but these will be ignored.

There has to be at least one separator between [key] and [value header].

There has to be at least one separator between [value header] and the [value].

Any extra separators in the line will be ignored.

To terminate the key value pair use a single '\n' character.

To be clear...

**Valid:** spaces and tabs can be used to indent and separate the parts although rather wasteful of space.

```
\t\t[key]\t[value header]\t\t\t[value]\n
```

**Invalid:** Absolutely no "\n" anywhere before or within the key value line&#42;.  Absolutely no blank lines as they have specific meaning in the format.

```
\n\n\n[key]\n[value header]\n[value]\n
\n[key] [value header] [value]\n
```

&#42; There are cases where the [value] will be broken up by '\n' characters but this will be indicated where that will happen.  In general most key value lines live on a single line.

#### 2.3.6.1 - Key


See section 2.2.2.1.

#### 2.3.6.2 - Value Header


```
[type code][array and compression flag] [[array count]] [[compressed byte count]]
```

**[type flag]** and **[array and compression flag]** is a string.  No space between the two.

[array count] will be a natural number (n4) value in readable UTF8 indicating how many values in the array.  Only present when =, or Z are used.

[compressed byte count] will be a natural number (n4) in readable UTF8 indicating the byte count of the compressed data.  Only present when z and Z are used.

#### 2.3.6.3 - Value


Some of the types are a bit more complex so they will have a specific representation in the text file.

### 2.3.7 - Value Specifics


#### 2.3.7.1 - Key-Value Block


Key-Value blocks are special.  They are needed to allow nesting of key values.  The last line of a block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

```
docInfo []-\n
title  ""- M.I.F.F.: Mixed Information File Format\n
author ""- Robbert de Groot\n
\n
```

To be clear on how it works.  Leading separators before the key are unnecessary and are only here for clarity.  Leading separators before a \n are allowed but are wasteful.

```
level1 []-\n
level2 []-\n
level3 []-\n
...
\n
anotherLevel3 []-\n
\n
\n
anotherLevel2 []-\n
\n
\n
anotherLevel1 []-\n
...
\n
```

Key value bocks can have an '=' array specifier and a array count.  This is useful for an array of a complex type.

```
itemList []= 2\n
item []-\n
name ""- Item A\n
value i4- 0\n
\n
item []-\n
name ""- item B\n
value i4- 10\n
\n
\n
```

z, and Z are never used with "[]".

#### 2.3.7.2 - String values


String values are odd.  If no compression is used then before the string starts you need to define the escape character being used.  This is needed to encode tab, cursor return, line feed and other control characters.  All the usual control characters will escaped using the control character provided.  If your string already has a escape charater combination inside and it should not be treated as a control character then you need to pick a different escape character.  Control character need not be 'printable'.

Strings are placed on one line even if they define a multi-line string.  With the above escapement of these control characters this can be done.

There can only be no separator between the escape character and the start of the string.  If separators exists between this escape character and the start of the string, then those separators are part of the string.

If the '=' array flag is used, the individual strings will reside on their own line.  Meaning, as soon as the [array count] is written a \n should be present.  Each string is on it's own line following the key line.

For an array of strings, Again, the first character of the string will be the escape character and then immediately followed by the string.

'z' encoding, the string is compressed first before converting to Base64.  No escape character needed.  A byte count and a compressed byte count is required to follow.

'Z' encoding, unlike '=', Each string in the array is compressed individually and placed on their own line.  Each line also includes a byte count for the string and a compressed byte count for the string.

```
string1 ""- \<-escape character being used.  No separators after this character.  Otherwise those separators are part of the string.  This is a single string value.\nBut multiline.\n


string2 ""- ~Example of changing the escape character.~nThis is a new line\n


string3 ""z [byte count] [compressed byte count] [Base64 encoded compressed string]\n


stringList1 ""= 3\n
^This is string 1, line 1.^r^nThis is string1, line 2.^r^n\n
/This is string 2.\n
@This is string 3.  Escape characters can change\n


stringList2 ""Z 3 [byte count] [compressed byte count] [Base64 encoded compressed string list]\n
```

See binary case for defining the list for the compressed array.

### 2.3.8 - Path Values


A string defining a relative path (relative to the MIFF file location.)  Folders are separated by a "/" only.  No "." or ".." are allowed as part of the path.  I am still investigating if there is a universal way to define paths.  UNC may be it but I haven't convinced myself yet.

See strings for storage rules.

#### 2.3.8.1 - Simple values


| Type Code | Representation |
| --- | --- |
| i1 - i16, n1 - n16 | 1 human readable number. |
| r4, r8, r10 | 1 Base64 encoded floating point number |
| v2i4, v2i8, v2n4, v2n8 | 2 human readable numbers separated by at least one separator. |
| v2r4, v2r8, cplx4, cplx8 | 2 Base64 encoded floating point numbers. |
| v3i4, v3i8, rgbn1, rgbn2, rgbn4 | 3 human readable numbers separated by at least one separator. |
| v3r4, v3r8, rgbr4 | 3 Base64 encoded floating point numbers. |
| v4i4, v4i8, v4n4, v4n8, rgban1, rgban2, rgban4, cmykn1, cmykn2, cmykn4 | 4 human readable numbers separated by at least one separator. |
| v4r4, v4r8, quat4, quat8, rgbar4, cmykr4 | 4 Base64 encoded floating point numbers. |
| mat3r4, mat3r8 | 9 Base64 encoded floating point numbers.  Written out left to right, top to bottom order. Meaning, top left value of the matrix is written first, then second element of the top row, and so on. |
| mat4r4, mat4r8 | 16 Base64 encoded floating point numbers.  Written out left to right, top to bottom order. Meaning, top left value of the matrix is written first, then second element of the top row, and so on. |


If using '=' array flag the above is repeated as many times as there are array elements.  Each array element will be separated by at least one separator.

```
OneInt               i4-       1024\n
ManyInt              i4=     8 1 2 4 8 16 32 64 128\n
OneMatrix            mat3r4-   [Base64 encoded matrix]\n
ManyMatrix           mat3r4= 3 [Base64 encoded matrix] [Base64 encoded matrix] [Base64 encoded matrix]\n
ManyMatrixCompressed mat4r8Z 9 798 [Base64 encoded compressed data]\n
```

rgbr4 and rgbar4, the values are typically in the range between 0.0 and 1.0.

If Base64 encoded then data will be in Big Endian order.

If using 'z' or 'Z' encoding flag, data is in Big Endian order, compressed using zlib, and encoded in Base64.

#### 2.3.8.2 - Binary values


The binary data will have a header of 1 natural number (n4) before the binary data.  This value indicates the byte count of the binary data.  This byte count is not included when using Base64 or Zlib compression.  However the binary data will honor Base64 and Zlib compression if used.

```
binKey1 bin- 10 [Base64 encoded binary data]\n


binKey2 bin= 2\n
10 [Base64 encoded binary data]\n
10 [Base64 encoded binary data]\n


binKey3 binz 1024 432 [Base64 encoded binary data]\n*


binKey2 binZ 2\n
10240 5123 [Base64 encoded binary data]\n
10240 4603 [Base64 encoded binary data]\n
```

Binary data is dangerous and should be used rarely if at all.  If someone uses them then they should be on the hook to define what the contents of the blob means.  If they do not, then they are being bad citizens and should be shamed!  SHAMED I SAY!

However this is here as a catchall just in case.

#### 2.3.8.3 - Boolean values


Boolean data is a little different than the basic types above.

By default, the value will be 't' for true, or 'f' for false.

'=' encoding flag, the value will be a string of 't' or 'f' letters with no separator in between to save some space.

'Z' will take the bitmap and compress it first before encoding it with Base64.  Bitmap being...

```
Byte 0                            Byte 1
+---+---+---+---+---+---+---+---+ +---  ...
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | | 8
+---+---+---+---+---+---+---+---+ +--- ...
2^7 2^6 ...                 2^0   2^8 ...
```

0, 1, 2, ... Indicates the index into the array of booleans.  So each bit will either be 1 (true) or 0 (false).  This byte stream then gets converted to Base64.

'z' encoding flag is never used with this type.

```
OneBoolean  bool-    t\n
ManyBoolean bool= 10 tttttfffff\n
```

#### 2.3.8.4 - Image values


##### 2.3.8.4.1 - img1, imga1, img2, imga2, img4, imga4, imgr4, imgar4


```
[pixel count width] [pixel count height] [pixel buffer]
```

**[pixel count width]** and **[pixel count height]** are natural numbers (n4) before the image data.

**[pixel buffer]** will always be Base64 encoded and one long stream.  Do not break down in rows.

Images are written out left to right, top to bottom.  Top left pixel written out first, then the second pixel from the left top row is next, and so on.

Array of graphics will place each graphic on a separate line similar to string encoding.

```
SingleGraphic imgn1- 2 2 [Base64 encoded data]\n


ArrayOfGraphics imgn1= 2\n
2 2 [Base64 encoded data]\n
2 2 [Base64 encoded data]\n


SingleCompressedGraphic imgn1z 564 2 2 [Base64 encoded data]\n


ArrayOfCompressedGraphics imgn1Z 2\n
564 2 2 [Base64 encoded data]\n
564 2 2 [Base64 encoded data]\n
```

##### imgfile


```
[three letter file format] [file size] [file data]
```

Image file option will have a 3 letter image identifier for the file type, a natural number for the file size, and then the file that represents the image, copied verbatum into the value portion.  There is never any compression applied.  The data is always Base64 encoded.

```
imageFile1 imgfile- png 1024 [Base64 encoding of a PNG file data]\n


imageFile2 imgfilez bmp 564 1024 [Base64 encoding of a BMP file data]\n


imageFileList imgfile= 2\n
jpg 2048 [Base64 encoding of a JPG file 1 data]\n
tif 4096 [Base64 encoding of a TIFF file 2 data]\n
```

##### imgpath


```
[string of path to file]
```

Image path graphic option simply contains a relative path from the MIFF file to an image file.  See path for storage rules.

```
imageFileLink imgrelpath- *\path\to\image\file\image.png\n


imageFileLinkList imgrelpath= 2\n
*\path\to\image\file\image1.png\n
*\path\to\image\file\image2.png\n
```

#### 2.3.8.5 - Type value


type stores type format string.

```
typeValue type- mat3r4
```

#### 2.3.8.6 - Time values


##### 2.3.8.6.1 - time


```
[YYYY] [MM] [DD] [hh] [mm] [ss]\n
```

##### 2.3.8.6.2 - timedate


```
[YYYY] [MM] [DD]\n
```

##### 2.3.8.6.3 - timehour


```
[hh] [mm] [ss]\n
```

All values are natural numbers and are 0 padded.

[hh] is 24 hour Greenwich Time zone, no daylights correction.

```
timeValue time 2019 01 31 20 30 40\n
```

##### 2.3.8.6.4 - times&#42;, timems&#42;, timeus&#42;


```
[i4 or i8]
```

A time difference value in seconds, milliseconds, and microseconds.

#### 2.3.8.7 - Value stream


A Value Stream is a departure of the base format.  It is meant to be used to compress the data storage more with a simpler key value structure which will be dictated by the format that uses MIFF as a base format.

Keys will typically be 1 byte.  Values will be a known sized based on the key or other information of the format.

#### 2.3.8.8 - User Type


A definition of a type.

[item count] [[key] [type code]]&#42;

**[item count]** is a natural number (n4) which indicates how many [key] and [type code] pairs that follow.

```
ContactType usertype- 5 NameGiven ""- NameFamily ""- Age n1- EMail ""- DataFlags n1= 5\n*
```

#### 2.3.8.9 - User Data


The value of a user type.  A usertype needs to be defined first.  The values that follow are in the order that the usertype defined them.  If an array of usertypes then each item is on their own line.

```
Contact1 userdata-\n
`Robbert\n
`de Groot\n
100\n
`zekaric@gmail.com\n
1 1 2 3 5\n


Contact2 userdataz [byte count] [compressed byte count] [Base64 encoded string]\n


ContactList userdata= 2 ContactType\n
`Robbert\n
`de Groot\n
100\n
`zekaric@gmail.com\n
1 1 2 3 5\n
`Sherlock\n
`Holms\n
32\n
`221b@bakerst.uk\n
10 20 30 40 50\n


ContactList userdataZ 2 [byte count] [compressed byte count] ContactType [Base64 encoded string]\n
```

How to compress the user data if it includes strings of indeterminant lenght?  See Binary case for userdata.  When you have to binary representation, compress that and Base64 encode it.

## 2.4 - Binary


The Binary file will to match the Text file 1-1.

Everything within '[]' is a byte sequence of a known size.  Each byte seqence is separated  but is immediately following after previous byte sequence.  Nothing extra is added between sequences.

A byte sequence is [[Byte count]:[Name of this Byte sequence]], where Byte count is a number indicating the number of bytes this sequence uses.

### 2.4.1 - Key Value Format


```
[n1:key byte count] [n1 * key byte count:key] [n2:value header] [[n4:array count]] [[n4:compress byte count]] [*:value]
```

#### 2.4.1.1 - Key


[key byte count] defines how long the key is in bytes.  Keys are 1 to 255 Bytes and they define a UTF8 string.

[key] defines the key of the key value pair.  The length of which was defined by [key byte count] above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


[value header] is 2 bytes.

The 2 high bits of this value are holding the array or compression flags.  The remaining 14 bits are for the type code.  See section 2.2.2.2 above for what the flags and type code should equal to.

[array count] and [compress buffer byte count] are only present when certain value header conditions are present.  Meaning [array count] byte sequence is only present when storing an array of values; [compress buffer byte count] is only present when the value is compressed.  If neither of these are present in the value header then these byte sequences are not present.

Both [array count] and [compress buffer byte count] are natural numbers (n4.)

#### 2.4.1.3 - Value


[&#42;:value] field will vary wildly depending on the [value header], [array count], and [compress buffer byte count] values.  But it will be a known size given all that information.

### 2.4.2 - Value Specifics


#### 2.4.2.1 - Key-Value Block


There technically is no value.  The next byte after the [value header] is the start of the first key in the block.

A block is terminated when the [key byte count] is equal to 0.

```
[8],["itemList"],[0x1],
[key string length != 0],...
[key string length != 0],...
[key string length != 0],...
[key string length = 0] <- end of itemList block
```

In the above I have defined an "itemList" key which starts the key value block.  If the next key value has a key byte count that is &#62; 0 then it is part of the itemList's key value block.  The last key byte count is 0 which indicates that the block is done.  Anything that follows the block is a new key value outside of the itemList block.

#### 2.4.2.2 - String, Path, and Relative Path values


```
[[n4:string byte count][n1 * string byte count:string]]]
```

Each string is prefixed with a [string byte count], the byte count of the UTF8 string.  The UTF8 byte stream follows and should be exactly the length defined by string byte count.

Array of strings will have as many of these pairs as found in the array.

If compression is used, the string length is not part of the compressed data.

If compression on an array of string is used then...

```
[n1:key byte count][n1 * key byte count:key][b110.0101][n4:array count][n4:byte count][n4:compressed byte count][compressed buffer]
```

#### 2.4.2.3 - Simple values


Simple value encoding.  Based on what is being stored the byte streams only look slighly different.

| Type | Byte Count |
| --- | --- |
| i1, n1 | 1 |
| i2, n2 | 2 |
| rgb1 | 3 (3 x 1 Byte) |
| i4, n4, r4 | 4 |
| rgba1 | 4 (4 x 1 Byte) |
| rgb2 | 6 (3 x 2 Byte) |
| i8, n8, r8 | 8 |
| v2i4, cplx4 | 8 (2 x 4 Byte) |
| rgba2 | 8 (4 x 2 Byte) |
| v3r4, v3i4, v3n4, rgb4, rgbr4 | 12 (3 x 4 Byte) |
| v2r8, cplx8 | 16 (2 x 8 Byte) |
| v4r4, quat4, rgba4, rgbar4, cmyk4, cmykr4 | 16 (4 x 4 Byte) |
| v3r8 | 24 (3 x 8 Byte) |
| v4r8, quat8 | 32 (4 x 8 Byte) |
| mat3r4 | 36 (9 x 4 Byte) |
| mat3r8 | 64 (9 x 8 Byte) |
| mat4r4 | 72 (16 x 4 Byte) |
| mat4r8 | 128 (16 x 8 Byte) |


The values are simply encoded as such.

```
[byte count:value]
```

Again, multi-byte values are store in big endian.

Arrays of these values are simply repeating the above as many times necessary for the array.

Compression on a single value is not usually used.  I may be tries with the larger types like mat4r8.

#### 2.4.2.4 - Binary


Binary encoding is like string encoding.  We need a byte count before the buffer data.

```
[4:byte count]
[byte count:buffer data]
```

byte count will never be compressed when compression is used but buffer data will.

Array of binaries will have as many of these pairs as found in the array.

#### 2.4.2.5 - Boolean


Boolean data is a little different than the basic types above.

**Single Value**

```
[1:Byte]
```

The value will be ASCII 't' for true, or ASCII 'f' for false.

**Array of Values**

```
[(Array Count) / 8:Bytes]
```

'=' encoding flag will mean the flags are encoded in to a bit map where...

```
Byte 0                            Byte 1
+---+---+---+---+---+---+---+---+ +---  ...
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | | 8
+---+---+---+---+---+---+---+---+ +--- ...
2^7 2^6 ...                 2^0   2^8 ...
```

0, 1, 2, ... indicates the index into the array and how it relates to the bytes.  So each bit will either be 1 (true) or 0 (false).  This byte stream then gets converted to Base64.

'Z' will take the above bitmap and compress it first before encoding it with Base64.

#### 2.4.2.6 - Image values


**imgn1, imgan1, imgn2, imgan2, imgn4, imgan4, imgr4, imgar4**

```
[n4:width]
[n4:height]
[width * height * [rgb1|rgba1|rgb2|rgba2|rgb4|rgba4|rgbr4|rgbar4]:graphic data]
```

Raw graphic data size will be width &#42; height &#42; size of the color item.

**imgfile**

```
[n1 * 3:three letter file format]
[n4:image file size]
[image file size:binary buffer of the image file]
```

It is only recommended to use compression flag on a file type that does not already use an efficient compression method.  Meaning, PNG and JPG are already highly compressed.  Adding the compression flag to these file types will not save a lot of memory.  BMP is an uncompressed file and would benefit greatly from a complression flag.

**imgpath**

```
[n4:string length]
[n1 * string length:file path]
```

#### 2.4.2.7 - Type value


```
[n2:type value]
```

The type value.

#### 2.4.2.8 - Time value


Store time values.

##### 2.4.2.8.1 - time


```
[i2:Year Value]
[n1:Month Value]
[n1:Day Value]
[n1:Hour Value]
[n1:Minute Value]
[n1:Second Value]
```

##### 2.4.2.8.2 - timedate


```
[i2:Year Value]
[n1:Month Value]
[n1:Day Value]
```

##### 2.4.2.8.3 - timetime


```
[n1:Hour Value]
[n1:Minute Value]
[n1:Second Value]
```

Year value is the present year as is.  Month value is the month value starting at 1 being January.  Similarly Day starts at 1.

Hour value is 24 hour Greenwich Time zone, no daylights correction starting at 0 hour.  Minute and Second are both starting at 0.

#### 2.4.2.9 - Value stream


A Value Stream is a departure of the base format.  It is meant to be used to compress the data storage more with a simpler key value structure which will be dictated by the format that uses MIFF as a base format.

Keys will typically be 1 byte.  Values will be a known sized based on the key or other information of the format.

#### 2.4.2.10 - User Type


```
[n4:key-type count][[n1:key byte count][key byte count:key][n2:type code][n4:array count]+]*
```

Defining a unique type for the format.

There will be key-type count pairs of key and type code.

Depending on the type code, if array of types is defined, an array count may be present.

#### 2.4.2.11 - User Data


```
[n1:user type byte count][user type byte count:user type name][*:value]*
```

Providing the values of a user type.  A user type must be defined before setting the values.

The values will depend on what was defined by the user types.  The values are in the order as they were defined by the user type.

# 3 - Design Decisions:


****Why not XML or JSON?****<br>
    I did not take XML as a format because it is too verbose.  JSON is very flexible but I also find it is still unnecessarily verbose.  Better than XML but still unacceptable in my opinion.  Considering the data, I do not want a format that would cause too much data bloat.  There will always be some bloat happening but it should be mitigated in my opinion.

****Why network order for multibyte data types?****<br>
    In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows NT.  At that time the architecture on some of the other platforms was Big Endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a Little Endian machine which Windows has always been.  The problems that we faced were trivial to solve but just very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler.

