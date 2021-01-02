
# M.I.F.F.: Mixed Information File Format


**Author:** Robbert de Groot

**Date:** 2019-05-15

**Copyright:** 2019, Robbert de Groot

**License (Library):** MIT License.

**License (Document):** Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)

## Table Of Contents:

**1 - M.I.F.F.**<br />
    1.1 - Discussion<br />
    1.2 - Goals<br />
    1.3 - Disclosure<br />
**2 - MIFF Format: Base**<br />
    2.1 - File Header<br />
    2.2 - Content<br />
        2.2.1 - Key Value Format<br />
            2.2.1.1 - Key<br />
            2.2.1.2 - Value Header<br />
                2.2.1.2.1 - Type Value<br />
                2.2.1.2.2 - Array and Compression flag<br />
            2.2.1.3 - Value<br />
    2.3 - Text Format<br />
        2.3.1 - Whitespace Characters<br />
        2.3.2 - Separator Characters<br />
        2.3.3 - Printable Characters<br />
        2.3.4 - Key Value Format<br />
            2.3.4.1 - Key<br />
            2.3.4.2 - Value Header<br />
            2.3.4.3 - Value<br />
        2.3.5 - Value Specifics<br />
            2.3.5.1 - Key-Value Block, []<br />
            2.3.5.2 - String values, ""<br />
            2.3.5.3 - Simple values<br />
            2.3.5.4 - Binary values, ...<br />
            2.3.5.5 - Boolean values, bool<br />
            2.3.5.6 - Image values, img&#42;<br />
            2.3.5.7 - Type value, type<br />
            2.3.5.8 - Time values, ymd, hms, ymdhms<br />
            2.3.5.9 - Value stream, [...]<br />
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
**3 - Design Decisions:**<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.

It started life because of a need for a reduced data storage format for larger amounts of data.  See Mining Interchange MIFF since that was the reason for this document.

## 1.2 - Goals



* **Simple**  The format should be simple for the developers to export their data.  And still fairly simple to re-import that data.

* **Brief**  The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  A years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

## 1.3 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was a thought on developing something in place of existing options.

I can be reached at the following two email addresses.

robbert.degroot@hexagon.com<br />zekaric@gmail.com

# 2 - MIFF Format: Base


There are two representations of the format.  A Text file and a Binary file representation.  They will both contain exactly the same data and be exactly the same in feature set.  The Binary file may have the advantage of possibly being slightly more compact and slightly faster in reading and writing.

A MIFF format is essentially a collection of key value pairs.  Values can be a collection of further key value pairs.  Meaning nesting can be possible.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file random accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This is just to be a transfer file from one program to another.

Common to both Text and Binary formats, any Byte data that is encode or stored as a binary byte sequence will be in network (big endian) order.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.

```
"MIFF       1TXT\n[Sub-Format Name]\n[Sub-Format Version]\n"
```

or

```
"MIFF       1BIN\n[Sub-Format Name]\n[Sub-Format Version]\n"
```

"MIFF" indicates the base file format type.

"       1" indicates the MIFF file format version.  Currently at version 1.  This will only use whole numbers instead of the usual XX.XX.XX.XX versioning of software.  This is written in 8 char hex.

"TXT\n" and "BIN\n" indicate the content organization.  TXT for text, and BIN for binary.  "\n" is a line feed character.  This is part of the string.  MIFF Text files will only use \n for line endings and never \n\r or \r\n or any other character.

[Sub-Format Name] will be a UTF8 string and defines the format of what is being stored in the file.

[Sub-Format Version] will always be an unsigned 4 byte value.  Like the MIFF version, this is written in 8 char hex.

## 2.2 - Content


Before going into the specifics of the text format and the binary format we will cover what a key value pair will look like at a high level.

### 2.2.1 - Key Value Format


```
[key] [value header] [value]\n
```

**[key]** will be a string that will somewhat uniquely identify the value.  No longer than 255 Bytes in UTF8 format.  Longer keys will be truncated.

**[value header]** will be an code of characters that will dictate what the value will look like.

**[value]** will depend on the **[value header]** on what is contained here.

#### 2.2.1.1 - Key


Keys are always a single string of characters and are limited to printable UNICODE code points.  Meaning no whitespace of any kind inside a key.

```
12345
;':][.,<>]'
a_b_c
$cost
```

The key can be nonsense or just a number.  As long it does not include any whitespace.

#### 2.2.1.2 - Value Header


There are quite a few different value types and encodings.  Each value is prepended with a header explaining the data you will be reading.

Value header will be described in the respective Text and Binary sections.  Just be aware there is a type value, array and compression flag.

##### 2.2.1.2.1 - Type Value


| n2 Value (Binary) | Type Code (Text) | Description |
| --- | --- | --- |
| **Block Types** | 	
 | 	
 |
| 1 | [] | Key-Value block. |
| 2 | [...] | Value stream.  The contents deviates from the rest of the format.  Representations will explained where they are used. |
| 3 | ... | Binary data. |
| **Basic Types** | 	
 | 	
 |
| 4 | type | Type value.  One of these type codes. |
| 5 | "" | String (UTF8) data. |
| 6 | bool | Boolean value. |
| 11, 12, 13, 14, 15, 16, 17, 18,<br />19, 20, 21, 22, 23, 24, 25, 26 | i1, i2, i3, i4, i5, i6, i7, i8,<br />i9, i10, i11, i12, i13, i14, i15, i16 | An integer value using 1 to 16 Bytes.  Ranges for a negative max value to a positive max value for a signed integer value. |
| 31, 32, 33, 34, 35, 36, 37, 38,<br />39, 40, 41, 42, 43, 44, 45, 46 | n1, n2, n3, n4, n5, n6, n7, n8,<br />n9, n10, n11, n12, n13, n14, n15, n16 | A natural value (unsigned integer) using 1 to 16 Bytes.  Ranges from 0 to max value. |
| 50, 51, 52 | r4, r8, r10 | A real value using 4 (float), 8 (double), or 10 (long double) Bytes. |
| **Complex Types** | 	
 | 	
 |
| 60, 61 | dms, dms6 | Degree, Minute, Second.  For dms, degree is an i2, minute is an n1, and second is and n1.  Form dms+ the second is changed to an n3 for fractional portion ranging from 0.00000 to 59.99999. |
| 70 | guid | Id value as a GUID. |
| 80 | path | String (UTF8) defining a relative path (relative to the MIFF file location.)  Folders are separated by a "/" only.  No "." or ".." are allowed as part of the path. |
| 90, 91,<br />92, 93,<br />94, 95,<br />96, 97 | rgb1, rgba1,<br />rgb2, rgba2,<br />rgb4, rgba4,<br />rgbr4, rgbar4 | Color using n1, n2, n4, or r4 per component.  In byte order Red, Green, Blue, and Alpha (if alpha is required.) |
| 100, 102, 103, 104 | cmyk1, cmyk2, cmyk4, cmykr4 | Color using n1, n2, n4, or r4 per component.  In byte order Cyan, Magenta, Yellow, and Black. |
| 110, 111,<br />112, 113,<br />114, 115,<br />116, 117 | img1, imga1,<br />img2, imga2,<br />img4, imga4,<br />imgr4, imgar4 | An image using rgbn1, rgban1, rgbr4, and rgbar4 values.  A width and height follows that are both n4 each. |
| 120 | imgfile | An image file inlined.  Keep to known formats, PNG for non-photographs, and regular JPG for photographs. |
| 121, 122 | imgpath | An image file path.  Follows path rules. |
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


##### 2.2.1.2.2 - Array and Compression flag


| Encoding Code (Binary) | Encoding Code (Text) | Description |
| --- | --- | --- |
| &#42;0 | - | Single value, depending on type of data, values is stored readable in text format or Base64 encoded. |
| &#42;1 | = | Array of values, depending on type of data, values is stored readable in text format or Base64 encoded. |
| 0&#42; | z | Single value.  Data is Zlib compressed then Base64 encoded. |
| 1&#42; | Z | Array of values.  Array is Zlib compressed then Base64 encoded. |


**[array count]** will only present when =, and Z are used.  This will be an unsigned integer value indicating how many items are in the array.

**[compressed size]** will only present when z and Z are used.  This will be an unsigned integer value indicating the raw, in memory byte count of the compressed data buffer.

= is used to indicate an array of values.  If used the array is either too small to really need compression or too random that compression will not really reduce the byte count.

z, and Z will generate a Base64 encoded string in the Text format.  This string lives on one line, there are no '\n' embedded in the encoding.

What is Base64 encoded and what isn't.  If the value being stored is a floating point value, it will be Base64 encoded to maintain precision of the value.  All string, integer, or natural (unsigned integer) numbers are written unencoded.

The 2 bits in the Binary format are the upper 2 bits of the type flag.

#### 2.2.1.3 - Value


The value header generally indicates what will be stored in the value of the key value pair.  It then becomes an issue on how it is represented in the two types of files.  See their respective sections for the details.

## 2.3 - Text Format


The text format is a UTF8 text file.  UTF8 is the 1 byte UNICODE format for text.  The base format only uses the ASCII characters (first 128 UNICODE character codes).  UTF8 mainly comes into play with the string data being stored.

The format is only somewhat human readable, and only somewhat text editor friendly, but it really is not meant to be "human friendly."  What I mean is that you will be able to look at the format in a text editor (if the text editor is capable of handling really large files with really long lines) and if you know this format, you will be able to debug it.  To a certain degree you should be able to debug the binary version as well since it will not be too different than the text version.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or may not be seeing the whole file.

All lines are terminated with a UNIX '\n'.  NOT an MSDOS/Windows \r\n or a MAC \n\r ending or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.  No exceptions.

'\r' (Cursor Return) should not appear anywhere in the format unless writted explicitely as '\', 'r' characters.  Meaning that it has been 'escaped' and that will only be in string values.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF limits what white space characters can be used as separators.

```
' ' (space), '\t' (Tab)
```

If you see any other whitespace characters used as a separator in the MIFF file then the file is not MIFF.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character makes a mark, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Key Value Format


The text file key value format looks like this.

```
[key] [value header] [value]\n
```

There can be leading separators before the [key] but these will be ignored.

There has to be at least one separator between [key] and [value header].

There has to be at least one separator between [value header] and the [value].

Any extra separators in the line will be ignored.

To terminate the key value pair use a single '\n' character.

To be clear...

**Valid:** spaces and tabs can be used to indent and separate the parts.

```
\t\t[key]\t[value header]\t\t\t[value]\n
```

**Invalid:**Absolutely no "\n" anywhere before or within the key value line&#42;.  '\n' characters indicate a termination of a kv block so any extra '\n' characters will cause problems.

```
\n\n\n[key]\n[value header]\n[value]\n
\n[key] [value header] [value]\n
```

&#42; There are cases where the [value] will be broken up by '\n' characters but this will be indicated below.

#### 2.3.4.1 - Key


See MIFF Format: Base / Content / Key Value Format / Key section for key information.

#### 2.3.4.2 - Value Header


As defined above the value header is as follows

```
[type value][array and compression flag] [array count] [compressed size]
```

[type flag] and [array and compression flag] is a string.  No space between the two.

If combined with '-' then the value is a single value, possibly Base64 encoded if it represents floating point data.

If combined with '=' then the value is an array of the given type, possibly Base64 encoded if it represents floating point data.

If combined with 'z' or 'Z' then the value is treated as binary, Byte swapped to be Big Endian, compressed using zlib, and encoded into a string using Base64.

[array count] will be a natural number (unsigned integer) value in readable UTF8 indicating how many values in the array.  Only present when =, or Z are used.

[compressed size] will be a natural number in readable UTF8 indicating the raw, in memory byte count of the compressed data.  Only present when z and Z are used.

Examples: (adding \n to indicate the necessity of this line.)

```
CompressedGraphic imgn1z 564 64 64 [Base64 zip compressed byte stream]\n


SingleNatural n1- 128\n


SingleString ""- This is a string.\n<- This is written as \ and n but means newline.  This is really a multiline string!  This \ and n is not ->\n


ArrayOfPoints xyr4= 5 10 20 0 20 20 0 20 50 0 10\n
```

#### 2.3.4.3 - Value


Some of the types are a bit more complex so they will have a specific representation in the text file.

### 2.3.5 - Value Specifics


#### 2.3.5.1 - Key-Value Block, []


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

#### 2.3.5.2 - String values, ""


String values are sort of the odd one out with all the other data types.  The other data types have a known size based on their input while strings are variable in size.  So encoding them will be slightly different.

Before the string starts you need to define the escape character being used.  This is needed to encode tab, cursor return, line feed characters.

If no compression is used then, all strings will have their tab, cursor return, and line feed characters replaced with the above escape character + "t", "r", or "n" respectively when written to the file.  If your string already has a escape charater + t, r, or n inside you need to pick a different escape character.

Strings are placed on one line even if they define a multi-line string.  With the above replacement of cursor return and line feeds this can be done.

There can only be no separator between the escape character and the start of the string.  If separators exists they are part of the string.

'=' array flag, the individual strings will reside on their own line.  Meaning, as soon as a string is finished '\n' will immediately follow.  There will be as many lines as there are array elements.

For an array of strings, the first character of the the next line will be the start of the first string in the array.  The line after that will be the second string.  No lines will have any leading spaces.  Any space visible is part of the string.

'z' encoding, the string is compressed first before converting to Base64.

'Z' encoding, unlike '=', Each string in the array is compressed individually and placed on their own line.

```
string1 ""- \<-separator character being used.  No separators after this character.  Otherwise those separators are part of the string.  This is a single string value.\nBut multiline.\n


string2 ""- ~Example of changing the escape character.~nThis is a new line\n


stringList ""= 3\n
^This is string 1, line 1.^r^nThis is string1, line 2.^r^n\n
/This is string 2.\n
\This is string 3.  Escape characters can change\n
```

#### 2.3.5.3 - Simple values


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

If Base64 encoded then byte swap to Big Endian order before encoding.

If using 'z' or 'Z' encoding flag, byte swap to Big Endian order, compress the values using zlib, and then write out the Base64 string representation of the compressed buffer.

#### 2.3.5.4 - Binary values, ...


The binary data will have a header of 1 natural number before the binary data.  This value indicates the byte count of the binary data.  This byte count is not included when using Base64 or Zlib compression.  However the binary data will honor Base64 and Zlib compression if used.

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

#### 2.3.5.5 - Boolean values, bool


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

#### 2.3.5.6 - Image values, img&#42;


**imgn1, imgan1, imgn2, imgan2, imgn4, imgan4, imgr4, imgar4**

```
[pixel count width] [pixel count height] [pixel buffer]
```

image options will have a header of 2 natural numbers before the image data.  These values indicate width and height of the graphic.  These values are not included in the Base64 or Zlib compression if compression is used.  However the image data will honor Base64 or Zlib compression if used.

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

**imgfile**

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

**imgpath, imgrelpath**

```
[string of path to file]
```

Image path graphic option simply contains a fully qualified path or relative path from the MIFF file to an image file.  See string storage for string format, namely escape character.

```
imageFileLink imgrelpath- ~\path\to\image\file\image.png\n


imageFileLinkList imgrelpath= 2\n
~\path\to\image\file\image1.png\n
~\path\to\image\file\image2.png\n
```

#### 2.3.5.7 - Type value, type


type stores type format string.

```
typeValue type- mat3r4
```

#### 2.3.5.8 - Time values, ymd, hms, ymdhms


Store time values.

```
ymd [YYYY] [MM] [DD]\n
hms [HH] [MM] [SS]\n
ymdhms [YYYY] [MM] [DD] [HH] [MM] [SS]\n
```

ymd values are only stored one way in Text, "YYYY MM DD" where YYYY, MM, DD are natural numbers.  MM and DD are 0 padded.

hms values are only stored one way in Text, "HH MM SS" where HH, MM, SS are natural numbers.  HH is 24 hour Greenwich Time zone, no daylights correction.

```
date ymdhms 2019 01 31 20 30 40\n
```

#### 2.3.5.9 - Value stream, [...]


A Value Stream is a departure of the base format.  It is meant to be used to compress the data storage more with a simpler key value structure which will be dictated by the format that uses MIFF as a base format.

Keys will typically be 1 byte.  Values will be a known sized based on the key or other information of the format.

## 2.4 - Binary


The Binary file will to match the Text file 1-1.  Everything within '[]' is a byte sequence of a known size.  Each byte seqence is separated here with a comma but is immediately following after previous byte sequence.

A byte sequence is [[Byte count]:[Name of this Byte sequence]], where Byte count is a number indicating the number of bytes this sequence uses.

Any spaces between [:] sequences are just for formatting.  No extra bytes are defined between consecutive sequences.

### 2.4.1 - Key Value Format


```
[n1:key string byte count] [n1 * key string byte count:key value] [n2:value header] [[n4:array count]] [[n4:compress buffer byte count]] [*:value]
```

#### 2.4.1.1 - Key


[key string byte count] is the first byte sequence and defines how long the key string is in bytes.  Keys can not be longer than 255 characters.  They need to be at least 1 character.

[key] is the second byte sequence and defines the key of the key value pair.  The length of which was defined by [key string byte count] above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


[value header] is the third byte sequence and is 2 bytes.  The 2 high bits of this value are holding the array or compression flags.  The rest of the bits are for the type itself.  See section 2.2.1.2 above.

[array count] and [compress buffer byte count] are only present when certain value header conditions are present.  Meaning [array count] byte sequence is only present when storing an array of values; [compress buffer byte count] is only present when the value is compressed.  If neither of these are present in the value header then these byte sequences are not present.

Both [array count] and [compress buffer byte count] are 4 byte unsigned integers stored in network order.

#### 2.4.1.3 - Value


[&#42;:value] field will vary wildly depending on the [value header], [array count], and [compress buffer byte count] values.  But it will be a known size given all that information.

### 2.4.2 - Value Specifics


#### 2.4.2.1 - Key-Value Block


This indicates the start of a Key-Value block.  There technically is no value.  The next byte after the [value header] is the start of the first key in the block.

A block is terminated when the [key string length] is equal to 0.

```
[8],["itemList"],[0x1],
[key string length != 0],...
[key string length != 0],...
[key string length != 0],...
[key string length = 0] <- end of itemList block
```

Hopefully the above explains it.  I have defined an "itemList" key which starts the key value block.  If the next key value has a key string length that is &#62; 0 then it is part of the itemList key value block.  The last key string length is 0 which indicates that the block is done.  Anything that follows the block is a new key value outside of the itemList block.

#### 2.4.2.2 - String, Path, and Relative Path values


```
[[n4:string length][n1 * string length:string]]]
```

Each string is prefixed with a [string length], the byte count of the UTF8 string.  Then you just dump out the string in UTF8 format.

Array of strings will have as many of these pairs as found in the array.

If compression is used, the string length is not part of the compressed data.

Path values should be a device independent as possible.

#### 2.4.2.3 - Simple values


Simple value encoding.  Based on what is being stored the byte streams only look slighly different.

| Type | Byte Count |
| --- | --- |
| i1, n1 | 1 |
| i2, n2 | 2 |
| rgbn1 | 3 (3 x 1 Byte) |
| i4, n4, r4 | 4 |
| rgban1 | 4 (4 x 1 Byte) |
| rgbn2 | 6 (3 x 2 Byte) |
| i8, n8, r8 | 8 |
| v2i4, cplx4 | 8 (2 x 4 Byte) |
| v3r4, v3i4, v3n4, rgbn4, rgbr4 | 12 (3 x 4 Byte) |
| v2r8, cplx8 | 16 (2 x 8 Byte) |
| v4r4, quat4, rgban4, rgbar4, cmykn4, cmykr4 | 16 (4 x 4 Byte) |
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
[width * height * [rgbn1|rgban1|rgbn2|rgban2|rgbn4|rgban4|rgbr4|rgbar4]:graphic data]
```

Raw graphic data size will be width &#42; height &#42; size of the color item.

**imgfile**

```
[n1 * 3:three letter file format]
[n4:image file size]
[image file size:binary buffer of the image file]
```

**imgpath, imgrelpath**

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

# 3 - Design Decisions:


****Why not XML or JSON?****<br>
    I did not take XML as a format because it is too verbose.  JSON is very flexible but I also find it is still unnecessarily verbose.  Better than XML but still unacceptable in my opinion.  Considering the data, I do not want a format that would cause too much data bloat.  There will always be some bloat happening but it should be mitigated in my opinion.

****Why network order for multibyte data types?****<br>
    In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows NT.  At that time the architecture on some of the other platforms was Big Endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a Little Endian machine which Windows has always been.  The problems that we faced were trivial to solve but just very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler.

