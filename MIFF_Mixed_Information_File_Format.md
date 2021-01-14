
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
    1.3 - Design Decisions:<br />
    1.4 - Disclosure<br />
**2 - Format: Base**<br />
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
            2.3.6.1 - Value Header<br />
            2.3.6.2 - Value<br />
    2.4 - Binary<br />
        2.4.1 - Key Value Format<br />
            2.4.1.1 - Key<br />
            2.4.1.2 - Value Header<br />
            2.4.1.3 - Value<br />
**3 - Fromat: Value Specifics**<br />
    3.1 - Key-Value Block<br />
    3.2 - Value Stream<br />
    3.3 - Binary values<br />
    3.4 - Embedded files<br />
    3.5 - Type value<br />
    3.6 - String values<br />
    3.7 - Boolean values<br />
    3.8 - Simple values<br />
    3.9 - Path Values<br />
    3.10 - Image values<br />
        3.10.1 - In file images<br />
        3.10.2 - Embedded image files<br />
        3.10.3 - Image file link<br />
    3.11 - User Type<br />
    3.12 - User Data<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals



* **`Simple  `**  The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **`Brief   `**  The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **`Flexible`**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **`Accurate`**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Design Decisions:


****Why not XML or JSON?****<br>
    I find XML are too verbose.  JSON is much better than XML.  Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I can't claim MIFF is better, just different.

****Why Big Endian for multibyte data types?****<br>
    In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows NT.  At that time the architecture on some of the other platforms was Big Endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a Little Endian machine which Windows has always been.  The problems that we faced were trivial to solve but just very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler.


## 1.4 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing options which I find lacking in some fashion.

I can be reached at the following two email addresses.

robbert.degroot@hexagon.com<br />zekaric@gmail.com

# 2 - Format: Base


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


**Natural numbers** (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

**Integer numbers** are numbers are positive and negative whole numbers.

**Real numbers   **(floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.2 - Key Value Composition


```
[key] [value header] [value]\n
```

**[key]         ** will be a string that will somewhat uniquely identify the value.  A key can not be longer than 255 Bytes in UTF8 format.  Longer keys will be truncated.

**[value header]** will be an code (binary) or sequence of characters (text) that will dictate what the value will look like.

**[value]       ** will depend on the **[value header]** on what is contained here.

#### 2.2.2.1 - Key


Keys are always a single string of characters and are limited to printable UNICODE code points.  Meaning no whitespace of any kind inside a key.

```
12345
;':][.,<>]'
a_b_c
$cost
ɃtɃ
```

The key can be anything and in any language.  As long it does not include any whitespace.  It should use printable characters only.

#### 2.2.2.2 - Value Header


A value header is comprised of a type code and an array and compression flag.

##### 2.2.2.2.1 - Type Code


| Binary Type Code | Text Type Code | Description |
| --- | --- | --- |
| <br />**Block Types**<br /> |
| 1 | [] | Key-Value block. |
| 2 | [...] | Value stream.  The contents deviates from the rest of the format.  Representations will explained where they are used. |
| <br />**Embedded Types**<br /> |
| 3 | ... | Binary data. |
| 4 | file | File data. |
| <br />**Basic Types**<br /> |
| 5 | type | Type value.  One of these type codes. |
| 6 | "" | String (UTF8) data. |
| 7 | bool | Boolean value. |
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
| 8 | userType | A series of key-type pairs. |
| 9 | userData | Which type to use and then a series of values. |


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
' ' (Space)
'\t' (Tab)
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

#### 2.3.6.1 - Value Header


Value header will typically look like this.

```
[type code][array and compression flag] [[array count]] [[compressed byte count]]
```

However it can vary slightly depending on the Type Code being used.  See section 3 for accurate representation.

**[type flag]** and **[array and compression flag]** is a string.  No space between the two.

[array count] will be a natural number (n4) value in readable UTF8 indicating how many values in the array.  Only present when =, or Z are used.

[compressed byte count] will be a natural number (n4) in readable UTF8 indicating the byte count of the compressed data.  Only present when z and Z are used.

#### 2.3.6.2 - Value


Values will be discussed lower in section 3.

## 2.4 - Binary


The Binary file will to match the Text file 1-1.

Everything within '[]' is a byte sequence of a known size.  Each byte seqence is separated  but is immediately following after previous byte sequence.  Nothing extra is added between sequences.

A byte sequence is [[Byte count]:[Name of this Byte sequence]], where Byte count is a number indicating the number of bytes this sequence uses.

### 2.4.1 - Key Value Format


```
[n1                 :key byte count]
[n1 * key byte count:key]
[*                  :value header]
[*                  :value]
```

#### 2.4.1.1 - Key


[key byte count] defines how long the key is in bytes.  Keys are 1 to 255 Bytes and the Bytes define a UTF8 string.

[key] defines the key of the key value pair.  The length of which was defined by [key byte count] above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


Value header will typically look like this.

```
[n2:1 bit - compression flag, 1 bit - array flag, 14 bit - type code]
[n4:array count]
[n4:compressed byte count]
```

The first 2 bytes holds the type code and array and compression flags.  The 2 high bits of this value are holding the array and compression flags.  The remaining 14 bits are for the type code.  See section 2.2.2.2 above for what the flags and type code should equal to.

[array count] and [compressed byte count] are only present when certain value header conditions are present.  Meaning [array count] byte sequence is only present when storing an array of values; [compressed byte count] is only present when the value is compressed.  If neither of these are present in the value header then these byte sequences are not present.

Both [array count] and [compressed byte count] are natural numbers (n4) and this arrangement is the general case.  See section 3 for accurate representation.

#### 2.4.1.3 - Value


[&#42;:value] field will vary wildly depending on the [value header], [array count], and [compress buffer byte count] values.  But it will be a known size given all that information.

# 3 - Fromat: Value Specifics


## 3.1 - Key-Value Block


Key-Value blocks are special.  They are needed to allow nesting of key values.

In a text file, the last line of a Key-Value Block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

In a binary file you will have a key byte count of 0 to indicate the end of a Key-Value block.

```
Binary                                                     Text


[n1                 :key byte count]                       [key] []-\n
[n1 * key byte count:key]                                   ...
[n2                 :value header   - 0|0|1]               \n
...
[n1                 :key byte count - 0]


[n1                 :key byte count]                       [key] []= [array count]\n
[n1 * key byte count:key]                                   ...
[n2                 :value header   - 0|1|1]
[n4                 :array count]
...
```

Key value bocks can have an Array flag set.  This is useful for an array of a complex type.

In this case there is no blank line (text) or 0 key count (binary) terminator since the key value count is known.

Compression flag is never used with this value type.

To be clear on how it works.  In the text file leading separators before the key are unnecessary and are only here for clarity.  Leading separators before a \n are allowed but are wasteful.

```
Binary                                                     Text


[n1     :7]                                                docInfo []-\n
[n1 * 7 :docInfo]                                           ...
[n2     :0|0|1]                                            \n
...
[n1     :0]


[n1     :6]                                                level1 []-\n
[n1 * 6 :level1]                                            level2 []-\n
[n2     :0|0|1]                                              level3 []-\n
[n1     :6]                                                   ...
[n1 * 6 :level2]                                             \n
[n2     :0|0|1]                                              anotherLevel3 []-\n
[n1     :6]                                                   ...
[n1 * 6 :level3]                                             \n
[n2     :0|0|1]                                             \n
...                                                         anotherLevel2 []-\n
[n1     :0]                                                  ...
[n1     :13]                                                \n
[n1 * 13:anotherlevel3]                                    \n
[n2     :0|0|1]                                            anotherLevel1 []-\n
...                                                         ...
[n1     :0]                                                \n
[n1     :0]
[n1     :6]
[n1 * 6 :anotherlevel2]
[n2     :0|0|1]
...
[n1     :0]
[n1     :0]
[n1     :6]
[n1 * 6 :anotherlevel1]
[n2     :0|0|1]
...
[n1     :0]


[n1     :8]                                                itemList []= 2\n
[n1 * 8 :itemList]                                          item []-\n
[n2     :0|1|1]                                              ...
[n4     :2]                                                 \n
[n1     :4]                                                 item []-\n
[n1 * 4 :item]                                               ...
[n2     :0|0|1]                                             \n
...
[n1     :0]
[n1     :4]
[n1 * 4 :item]
[n2     :0|0|1]
...
[n1     :0]
```

## 3.2 - Value Stream


A Value Stream is a departure of the base format.  It is meant to be used to compress the data storage more with a simpler key value structure which will be dictated by the format that uses MIFF as a base format.

Keys will typically be 1 byte.  Values will be a known sized based on the key or other information of the format.

## 3.3 - Binary values


```
Binary                                                     Text
                                                 

[n1                        :key byte count]                [key ...- [binary byte count] [Base64 binary data]\n
[n1 * key byte count       :key]
[n2                        :value header - 0|0|3]
[n4                        :binary byte count]
[n1 * binary byte count    :binary data]
                                                               

[n1                        :key byte count]                [key] ...= [array count]\n
[n1 * key byte count       :key]                           [binary byte count] [Base64 binary data]\n
[n2                        :value header - 0|1|3]          [binary byte count] [Base64 binary data]\n
[n4                        :array count]
[n4                        :binary byte count]
[n1 * binary byte count    :binary data]
[n4                        :binary byte count]
[n1 * binary byte count    :binary data]
...
                                                                  

[n1                        :key byte count]                [key] ...z [binary byte count] [compressed byte count] [Base64 compressed data]\n
[n1 * key byte count       :key]
[n2                        :value header - 1|0|3]
[n4                        :binary byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
                                                                  

[n1                        :key byte count]                [key] ...Z [array count]\n
[n1 * key byte count       :key]                           [binary byte count] [compressed byte count] [Base64 compressed data]\n
[n2                        :value header - 1|1|3]          [binary byte count] [compressed byte count] [Base64 compressed data]\n
[n4                        :array count]
[n4                        :binary byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
[n4                        :binary byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
...
```

The binary data will have a header of 1 natural number (n4) before the binary data.  This value indicates the byte count of the binary data.  This byte count is not included when using Base64 or Zlib compression.  However the binary data will honor Base64 and Zlib compression if used.

```
Binary                                                     Text


[n1       :7]                                              binKey1 ...- 10 ...\n
[n1 * 7   :binKey1]
[n2       :0|0|3]
[n4       :10]
[n1 * 10  :...]
                        

[n1       :7]                                              binKey2 ...= 2\n
[n1 * 7   :binKey2]                                        10 ...\n
[n2       :0|1|3]                                          10 ...\n
[n4       :10]
[n1 * 10  :...]
[n4       :10]
[n1 * 10  :...]
                        

[n1       :7]                                              binKey3 ...z 1024 432 ...\n
[n1 * 7   :binKey3]
[n2       :1|0|3]
[n4       :1024]
[n4       :432]
[n1 * 432 :...]
                        

[n1       :7]                                             binKey4 ...Z 2\n
[n1 * 7   :binKey4]                                       10240 5123 ...\n
[n2       :1|1|3]                                         10240 4603 ...\n
[n4       :10240]
[n4       :5123]
[n1 * 5123:...]
[n4       :10240]
[n4       :4603]
[n1 * 4603:...]
```

Binary data is dangerous and should be used rarely if at all.  If someone uses them then they should be on the hook to define what the contents of the blob means.  If they do not, then they are being bad citizens and should be shamed!  SHAMED I SAY!

However this is here as a catchall just in case.

## 3.4 - Embedded files


```
Binary                                                     Text


[n1                        :key byte count]                [key] file- [file type] [byte count] [Base64 file data]\n
[n1 * key byte count       :key]
[n2                        :0|0|4]
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n1 * byte count           :file data]


[n1                        :key byte count]                [key] file= [array count]\n
[n1 * key byte count       :key]                           [three letter file type] [byte count] [Base64 file data]\n
[n2                        :0|1|4]                         [file type] [byte count] [Base64 file data]\n
[n4                        :array count]                   ...
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n1 * byte count           :file data]
[n3                        :three letter file type]
[n4                        :byte count]
[n1 * byte count           :file data]
...


[n1                        :key byte count]                [key] filez [file type] [byte count] [compressed byte count] [Base64 compressed data]\n
[n1 * key byte count       :key]
[n2                        :1|0|4]
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]


[n1                        :key byte count]                [key] fileZ [array count]\n
[n1 * key byte count       :key]                           [file type] [byte count] [compressed byte count] [Base64 compressed data]\n
[n2                        :1|1|4]                         [file type] [byte count] [compressed byte count] [Base64 compressed data]\n
[n4                        :array count]                   ...
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
[n3                        :three letter file type]
[n4                        :byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
...
```

File option will have a file type identifier, usually the file extension lower case letters only; a natural number for the file size, and then the file that represents the image, copied verbatum into the value portion.

```
Binary                                                     Text


[n1       :5]                                              File1 file- doc 1024 ...\n
[n1 * 5   :File1]
[n2       :0|0|4]
[n1       :3]
[n1 * 3   :doc]
[n4       :1024]
[n1 * 1024:...]


[n1       :9]                                              FileList1 fileZ 2\n
[n1 * 9   :FileList1]                                      htm  2048 517 ...\n
[n2       :0|1|4]                                          docx 4096 4001 ...\n
[n4       :2]
[n1       :3]
[n1 * 3   :htm]
[n4       :2048]
[n1 * 2048:...]
[n1       :4]
[n1 * 4   :docx]
[n4       :4096]
[n1 * 4096:...]


[n1       :5]                                              File2 filez css 1024 564 ...\n
[n1 * 5   :File2]
[n2       :1|0|4]
[n1       :3]
[n1 * 3   :css]
[n4       :1024]
[n4       :564]
[n1 * 564 :...]


[n1       :9]                                              FileList2 fileZ 2\n
[n1 * 9   :FileList2]                                      htm  2048 517 ...\n
[n2       :1|1|4]                                          docx 4096 4001 ...\n
[n4       :2]
[n1       :3]
[n1 * 3   :htm]
[n4       :2048]
[n4       :517]
[n1 * 517 :...]
[n1       :4]
[n1 * 4   :docx]
[n4       :4096]
[n4       :4001]
[n1 * 4001:...]
```

## 3.5 - Type value


type stores type format string.

```
Binary                                                     Text
                                                 

[n1                        :key byte count]                [key] type- [text type code]\n
[n1 * key byte count       :key]
[n2                        :value header - 0|0|5]
[n2                        :binary type code]
                                                           

[n1                        :key byte count]                [key] type= [array count] [text type code]*\n
[n1 * key byte count       :key]
[n2                        :value header - 0|1|5]
[n4                        :array count]
[n2                        :binary type code]*
                                                           

[n1                        :key byte count]                type3 typeZ 100 31 [Base64 compressed binary type codes]
[n1 * key byte count       :key]
[n2                        :value header - 1|1|5]
[n4                        :array count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed binary type code]
```

There is no compressed single value case.

The text compressed type array is using the binary type codes and not the text type codes.

```
Binary                                                     Text


[n1     :5]                                                type1 type- mat3r4\n
[n1 * 5 :type1]
[n2     :0|0|5]
[n2     :130]
                                

[n1     :5]                                                type2 type= 2 mat3r8 v3r8\n
[n1 * 5 :type2]
[n2     :0|1|5]
[n4     :2]
[n2     :131]
[n2     :201]
                                

[n1     :5]                                                type3 typeZ 100 31 ...
[n1 * 5 :type3]
[n2     :1|1|5]
[n4     :100]
[n4     :31]
[n1 * 31:...]
```

## 3.6 - String values


Binary case is simpler so we'll talk about that first.

```
[n1                        :key byte count]
[n1 * key byte count       :key]
[n2                        :value header - 0|0|6]
[n4                        :string byte count]
[n1 * string byte count    :string data]


[n1                        :key byte count]
[n1 * key byte count       :key]
[n2                        :value header - 0|1|6]
[n4                        :array count]
[n4                        :string byte count]
[n1 * string byte count    :string data]
[n4                        :string byte count]
[n1 * string byte count    :string data]
...


[n1                        :key byte count]
[n1 * key byte count       :key]
[n2                        :value header - 1|0|6]
[n4                        :string byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]


[n1                        :key byte count]
[n1 * key byte count       :key]
[n2                        :value header - 1|1|6]
[n4                        :array count]
[n4 * array count          :string byte count list]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
```

The case of a single value just requires a byte count before the actual string data.  The string data will be exactly the byte count side.  Since this is binary, string data is not modified in any way.

The case of an array value just requires a repeat of the byte count string data for as may in the array.

The case of a compressed single value adds a compressed byte count after the string byte count.  And a compressed byte array of the string follows.

The difficult case is of the compressed array value.  In this case we have an array count followed by the lengths of each string in that array.  Then a compressed byte count of all the strings concatenated together, with not extra characters added, and then compressed.

```
[n1     :7]
[n1 * 7 :string1]
[n2     :0|0|6]
[n4     :55]
[n1 * 55:...]


[n1     :7]
[n1 * 7 :string2]
[n2     :0|1|6]
[n4     :2]
[n4     :55]
[n1 * 55:...]
[n4     :55]
[n1 * 55:...]


[n1     :7]
[n1 * 7 :string3]
[n2     :1|0|6]
[n4     :555]
[n4     :67]
[n1 * 67:...]


[n1     :7]
[n1 * 7 :string4]
[n2     :1|1|6]
[n4     :2]
[n4 * 2 :75 65]
[n4     :85]
[n1 * 85:...]
```

The text case is a little more involved.

```
[key] ""- [escape character][string data]\n


[key] ""= [array count]\n
[excape character][string data]\n
...


[key] ""z [string byte count] [compressed byte count] [Base64 compressed data]\n


[key] ""Z [array count] [string byte count] * (array count) [compressed byte count] [Base64 compressed data]\n
```

If no compression is used then before the string starts you need to define the escape character.  This is needed to encode control characters like tabs, new lines, cursor return etc.  All the usual control characters will escaped using the control character provided.  If your string already has a escape charater combination inside and it should not be treated as a control character, then you need to pick a different escape character.  Control characters need not be 'printable' but it would help if it was.

Multi-line strings are placed on one line.  With the escapement of these control characters (\n, \r etc.) this can be done.

There can only be no separators between the escape character and the start of the string.  If separators exist between this escape character and the start of the string, then those separators are part of the string.

```
string1 ""- \<-escape character being used.  No separators after this character.  Otherwise those separators are part of the string.  This is a single string value.\nBut multiline.\n


string2 ""- ~Example of changing the escape character.~nThis is a new line\n


string3 ""z 1024 754 ...\n


stringList1 ""= 3\n
^This is string 1, line 1.^r^nThis is string1, line 2.^r^n\n
/This is string 2.\n
@This is string 3.  Escape characters can change\n


stringList2 ""Z 3 53 42 75 85 ...\n
```

If the array flag is used, the individual strings will reside on their own line.  Meaning, as soon as the [array count] is written a \n should be present.  Each string is on it's own line following the key line.

For an array of strings, Again, the first character of the string will be the escape character and then immediately followed by the string.

For a compressed single string, the string is compressed first before converting to Base64.  No escape character or string modification is needed since it is treated like binary data.  A byte count of the string and a compressed byte count is required to follow.

For a compressed array of strings, each string is concatenated together to form one long buffer.  Then compressed.  After the [array count] there will be a string count for each string in the array followed by the compressed byte count of this concatenated buffer.

## 3.7 - Boolean values


```
Binary                                                     Text


[n1                        :key byte count]                [key] bool- [boolean value]
[n1 * key byte count       :key]
[n2                        :value header  - 0|0|7]
[n1                        :boolean value - 't'|'f']


[n1                        :key byte count]                [key] bool= [array count] [boolean value] * (array count)
[n1 * key byte count       :key]
[n2                        :value header  - 0|1|7]
[n4                        :array count]
[n1 * (array count / 8)    :boolean value]


[n1                        :key byte count]                [key] boolZ [array count] [compressed byte count] [Base64 compressed data]
[n1 * key byte count       :key]
[n2                        :value header  - 1|1|7]
[n4                        :array count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
```

By default, the value will be 't' for true, or 'f' for false.

For array values in text files, the value will be a string of 't' or 'f' letters with no separator in between to save some space.

For array values in binary , the value will be made into a bitmap first.  See diagram below for bit assignment.

For compressed array value, a bitmap is made and then compress.  See diagram below for bit assignment.

```
Byte 0                            Byte 1
+---+---+---+---+---+---+---+---+ +---  ...
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | | 8
+---+---+---+---+---+---+---+---+ +--- ...
 2^7 2^6 ...                 2^0   2^8 ...
```

0, 1, 2, ... Indicates the index into the array of booleans.  So each bit will either be 1 (true) or 0 (false).

'z' encoding flag is never used with this type.

```
Binary                                                     Text


[n1                        :5]                             Bool1 bool- t\n
[n1 * 5                    :Bool1]
[n2                        :0|0|7]
[n1                        :t]
                       

[n1                        :5]                             Bool2 bool= 10 tttttfffff\n
[n1 * 5                    :Bool2]
[n2                        :0|1|7]
[n4                        :array count]
[n1 * (array count / 8)    :boolean value]


[n1                        :5]                             Bool3 boolZ 1024 25 ...\n
[n1 * 5                    :Bool3]
[n2                        :1|1|7]
[n4                        :array count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
```

## 3.8 - Simple values


Simple value encoding.  Based on what is being stored the byte streams only look slighly different.

```
Binary                                                     Text


[n1                        :key byte count]                [key] [text type code]- [type value]
[n1 * key byte count       :key]
[n2                        :value header - 0|0|[type code]]
[[byte count]              :type value]


[n1                        :key byte count]                [key] [text type code]= [array count] [type value] * (array count)
[n1 * key byte count       :key]
[n2                        :value header - 0|1|[type code]]
[n4                        :array count]
[[byte count] * array count:type values]


[n1                        :key byte count]                [key] [text type code]z [compressed byte count] [compressed data]
[n1 * key byte count       :key]
[n2                        :value header - 1|0|[type code]]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]


[n1                        :key byte count]                [key] [text type code]Z [array count] [compressed byte count] [compressed data]
[n1 * key byte count       :key]
[n2                        :value header - 1|1|[type code]]
[n4                        :array count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
```

Binary Byte counts for the simple types will be one of...

| Type | Byte Count |
| --- | --- |
| i1, n1 | 1 |
| i2, n2 | 2 |
| rgb1, dms | 3 (3 x 1 Byte). rgb&#42; is ordered red, green, blue always.  dms&#42; is ordered degree, minute, second always. |
| i4, n4, r4 | 4 |
| rgba1, cmyk1 | 4 (4 x 1 Byte). rgba&#42; is ordered red, green, blue, alpha always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| rgb2 | 6 (3 x 2 Byte). rgb&#42; is ordered red, green, blue always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| dms6 | 6 (2 Byte, 1 Byte, 3 Byte) |
| i8, n8, r8 | 8 |
| v2i4, cplx4 | 8 (2 x 4 Byte). cplx&#42; is ordered real, i always. |
| dms8 | 6 (2 Byte, 1 Byte, 5 Byte) |
| rgba2, cmyk2 | 8 (4 x 2 Byte). rgba&#42; is ordered red, green, blue, alpha always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| v3r4, v3i4, v3n4, rgb4, rgbr4 | 12 (3 x 4 Byte). rgb&#42; is ordered red, green, blue always. |
| v2r8, cplx8 | 16 (2 x 8 Byte). cplx&#42; is ordered real, i always. |
| v4r4, quat4, rgba4, rgbar4, cmyk4, cmykr4 | 16 (4 x 4 Byte). rgba&#42; is ordered red, green, blue, alpha always. quant&#42; is ordered real, i, j, k always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| v3r8 | 24 (3 x 8 Byte) |
| v4r8, quat8 | 32 (4 x 8 Byte). quant&#42; is ordered real, i, j, k always. |
| mat3r4 | 36 (9 x 4 Byte) |
| mat3r8 | 64 (9 x 8 Byte) |
| mat4r4 | 72 (16 x 4 Byte) |
| guid | 128 |
| mat4r8 | 128 (16 x 8 Byte) |


The values are simply encoded as such.

```
[byte count:value]
```

Again, multi-byte values are store in big endian.

Arrays of these values are simply repeating the above as many times necessary for the array.

Compression on a single value is not usually used.  I may be tries with the larger types like mat4r8.

Text representation for a value will be...

| Type Code | Representation |
| --- | --- |
| i1 - i16, n1 - n16, times4, timems4, timeus4, times8, timems8, timeus8 | 1 value.  times&#42; is a second count, timems&#42; is a millisecond count, timeus&#42; is a microsecond count. |
| r4, r8, r10, guid | 1 Base64 stream of the 1 value. |
| v2i4, v2i8, v2n4, v2n8 | 2 values separated by at least one separator. |
| v2r4, v2r8, cplx4, cplx8 | 1 Base64 stream of the 2 values.  cplx is ordered real, i always. |
| v3i4, v3i8, rgbn1, rgbn2, rgbn4, dms, dm6, dms8, timedate, timehour | 3 values separated by at least one separator.  rgb&#42; is ordered red, green, blue always.  dms6 and dms8 second values are naturals internally so writing them out as a floating point loses no precision.  timedate is ordered year month day values always.  timehour is ordered hour minute second values always.  Month starts at 1.  Day starts at 1.  Hour starts at 0 (midnight) and is always in 24 hour time.  Time zone is always Greenwich and never adjusted to daylights. |
| v3r4, v3r8, rgbr4 | 1 Base64 steam of the 3 values.  rgb&#42; is ordered red, green, blue always. |
| v4i4, v4i8, v4n4, v4n8, rgban1, rgban2, rgban4, cmykn1, cmykn2, cmykn4 | 4 values separated by at least one separator.  rgba&#42; is ordered red, green, blue, alpha always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| v4r4, v4r8, quat4, quat8, rgbar4, cmykr4 | 1 Base64 stream of the 4 values.  quant&#42; is ordered real, i, j, k always.  rgba&#42; is ordered red, green, blue, alpha always.  cmyk&#42; is ordered cyan, magenta, yellow, black always. |
| time | 6 values separated by at least one separator.  time is ordered year month day hour minute second always.  Month starts at 1.  Day starts at 1.  Hour starts at 0 (midnight) and is always in 24 hour time.  Time zone is always Greenwich and never adjusted to daylights. |
| mat3r4, mat3r8 | 1 Base64 stream of the 9 values.  Written out left to right, top to bottom order. Meaning, top left value of the matrix is written first, then second element of the top row, and so on. |
| mat4r4, mat4r8 | 1 Base64 stream of the 16 values.  Written out left to right, top to bottom order. Meaning, top left value of the matrix is written first, then second element of the top row, and so on. |


If using an array flag the above is repeated as many times as there are array elements.  Each array element will be separated by at least one separator.

```
Binary                                                     Text


[n1      :4]                                               1Int i4- 1024\n
[n1 * 4  :1Int]
[n2      :0|0|14]
[i4      :1024]


[n1      :4]                                               *Nat n4= 8 1 2 4 8 16 32 64 128\n
[n1 * 4  :NNat]
[n2      :0|1|34]
[n4      :8]
[n4      :1]
[n4      :2]
[n4      :4]
[n4      :8]
[n4      :16]
[n4      :32]
[n4      :64]
[n4      :128]


[n1      :7]                                               1Matrix mat3r4- [Base64 matrix]\n
[n1 * 7  :1Matrix]
[n2      :0|0|130]
[r4 * 9  :matrix data]


[n1      :7]                                               *Matrix mat3r4= 3 [Base64 matrix] [Base64 matrix] [Base64 matrix]\n
[n1 * 7  :*Matrix]
[n2      :0|1|130]
[n4      :3]
[r4 * 9  :matrix data]
[r4 * 9  :matrix data]
[r4 * 9  :matrix data]


[n1      :7]                                               zMatrix mat4r8Z 81 ...\n
[n1 * 7  :ZMatrix]
[n2      :1|0|141]
[n4      :81]
[n1 * 81 :...]


[n1      :7]                                               ZMatrix mat4r8Z 9 798 ...\n
[n1 * 7  :ZMatrix]
[n2      :1|1|141]
[n4      :9]
[n4      :798]
[n1 * 798:...]
```

## 3.9 - Path Values


A string defining a relative path (relative to the MIFF file location.)  Folders are separated by a "/" only.  No "." or ".." are allowed as part of the path.  I am still investigating if there is a universal way to define paths.  UNC may be it but I haven't convinced myself yet.

See strings (section 3.6) for storage rules.

## 3.10 - Image values


### 3.10.1 - In file images


Type codes img1, imga1, img2, imga2, img4, imga4, imgr4, imgar4 define an image inside the MIFF file.

```
Binary                                                     Text


[n1                        :key byte count]                [key] [type code]- [pixel count width] [pixel count height] [Base64 pixel data]\n
[n1 * key byte count       :key]
[n2                        :0|0|type code]
[n4                        :pixel count width]
[n4                        :pixel count height]
[n1 * image byte count     :pixel data]


[n1                        :key byte count]                [key] [type code]= [array count]\n
[n1 * key byte count       :key]                           [pixel count width] [pixel count height] [Base64 pixel data]\n
[n2                        :0|1|type code]                 [pixel count width] [pixel count height] [Base64 pixel data]\n
[n4                        :array count]                   ...
[n4                        :pixel count width]
[n4                        :pixel count height]
[n1 * image byte count     :pixel data]
[n4                        :pixel count width]
[n4                        :pixel count height]
[n1 * image byte count     :pixel data]
...


[n1                        :key byte count]                [key] [type code]z [pixel count width] [pixel count height] [compressed byte count] [Base64 compressed data]\n
[n1 * key byte count       :key]
[n2                        :1|0|type code]
[n4                        :pixel count width]
[n4                        :pixel count height]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]


[n1                        :key byte count]                [key] [type code]Z [array count]\n
[n1 * key byte count       :key]                           [pixel count width] [pixel count height] [compressed byte count] [Base64 compressed data]\n
[n2                        :1|1|type code]                 [pixel count width] [pixel count height] [compressed byte count] [Base64 compressed data]\n
[n4                        :array count]                   ...
[n4                        :pixel count width]
[n4                        :pixel count height]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
[n4                        :pixel count width]
[n4                        :pixel count height]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
...
```

**[pixel buffer]** will always be Base64 encoded and one long stream.  Do not break down in rows.

Images are written out left to right, top to bottom.  Top left pixel written out first, then the second pixel from the left top row is next, and so on.  There is no byte alignment or packing.

Array of graphics will place each graphic on a separate line similar to string encoding.

```
Binary                                                     Text


[n1         :6]                                            1Image imgn1- 800 600 ...\n
[n1 * 6     :1Image]
[n2         :0|0|110]
[n4         :640]
[n4         :480]
[n1 * 307200:...]


[n1         :6]                                            NImage imgn1= 2\n
[n1 * 6     :ZImage]                                       800 600 ...\n
[n2         :0|1|110]                                      640 480 ...\n
[n4         :2]
[n4         :800]
[n4         :600]
[n1 * 480000:...]
[n4         :640]
[n4         :480]
[n1 * 307200:...]


[n1         :6]                                            zImage imgn1Z 564 640 480 ...\n
[n1 * 6     :zImage]
[n2         :1|0|110]
[n4         :640]
[n4         :480]
[n4         :885]
[n1 * 885   :...]


[n1         :6]                                            ZImage imgn1Z 2\n
[n1 * 6     :ZImage]                                       564 640 480 ...\n
[n2         :1|1|110]                                      885 640 480 ...\n
[n4         :2]
[n4         :640]
[n4         :480]
[n4         :564]
[n1 * 564   :...]
[n4         :640]
[n4         :480]
[n4         :885]
[n1 * 885   :...]
```

### 3.10.2 - Embedded image files


Embed an image file.  See Embedded file (section 3.4) for storage rules.

It is only recommended to use compression flag on a file type that does not already use an efficient compression method.  Meaning, PNG and JPG are already highly compressed.  Adding the compression flag to these file types will not give you much gain.  BMP is an uncompressed file and would benefit greatly from a complression flag.

### 3.10.3 - Image file link


Image path option simply contains a relative path from the MIFF file to an image file.  See path (section 3.9) and string (section 3.6) for storage rules.

## 3.11 - User Type


A definition of a user type.

```
Binary                                                     Text


[n1                 :key byte count]                       [key] usertype- [key] [value header] [array count] [key] [value header] [array count] ...\n
[n1 * key byte count:key]
[n2                 :value header - 0|0|8]
[n1                 :key byte count]
[n1 * key byte count:key]
[n2                 :value header]
[n4                 :array count]*
[n1                 :key byte count]
[n1 * key byte count:key]
[n2                 :value header]
[n4                 :array count]*
...
[n1                 :key byte count - 0]


[n1                 :key byte count]                       [key] usertype= [array count] [key] [value header] [array count] [key] [value header] [array count] ...\n
[n1 * key byte count:key]
[n2                 :value header - 0|1|8]
[n4                 :array count]
[n1                 :key byte count]
[n1 * key byte count:key]
[n2                 :value header]
[n4                 :array count]*
[n1                 :key byte count]
[n1 * key byte count:key]
[n2                 :value header]
[n4                 :array count]*
...
```

The different between the single value and array value is the inclusion of a count.  They both will permit the definition of multiple items.  In the binary case, if not using the array case we need a 0 key byte count terminator.

[array count] after the [value header] of a type item is optional and only required if the value header defines an array.

Compression flag is never used with this type code.

```
Binary                                                     Text


[n1     :11]                                               TypeContact usertype- NameGiven ""- NameFamily ""- Age n1- EMail ""- DataFlags n1= 5\n
[n1 * 11:TypeContact]
[n2     :0|0|8]
[n1     :9]
[n1 * 9 :NameGiven]
[n2     :0|0|6]
[n1     :10]
[n1 * 10:NameFamily]
[n2     :0|0|6]
[n1     :3]
[n1 * 3 :Age]
[n2     :0|0|31]
[n1     :5]
[n1 * 5 :EMail]
[n2     :0|0|6]
[n1     :9]
[n1 * 9 :DataFlags]
[n2     :0|1|31]
[n4     :5]
[n1     :0]


[n1     :11]                                               TypeContact usertype= 5 NameGiven ""- NameFamily ""- Age n1- EMail ""- DataFlags n1= 5\n
[n1 * 11:TypeContact]
[n2     :0|1|8]
[n4     :5]
[n1     :9]
[n1 * 9 :NameGiven]
[n2     :0|0|6]
[n1     :10]
[n1 * 10:NameFamily]
[n2     :0|0|6]
[n1     :3]
[n1 * 3 :Age]
[n2     :0|0|31]
[n1     :5]
[n1 * 5 :EMail]
[n2     :0|0|6]
[n1     :9]
[n1 * 9 :DataFlags]
[n2     :0|1|31]
[n4     :5]
```

## 3.12 - User Data


```
Binary                                                     Text


[n1                        :key byte count]                [key] userdata= [user type] [user type data]\n
[n1 * key byte count       :key]
[n2                        :0|0|9]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[*                         :user type data]


[n1                        :key byte count]                [key] userdata= [user type] [array count]\n
[n1 * key byte count       :key]                           [user type data]\n
[n2                        :0|1|9]                         ...
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :array count]
[*                         :user type data]
...


[n1                        :key byte count]                [key] userdataz [user type] [byte count] [compressed byte count] [user type data]\n
[n1 * key byte count       :key]
[n2                        :1|0|9]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]


[n1                        :key byte count]                [key] userdataZ [user type] [array count] [compressed byte count] [Base64 compressed data]\n
[n1 * key byte count       :key]
[n2                        :1|1|9]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :array count]
[n4                        :byte count]
[n4                        :compressed byte count]
[n1 * compressed byte count:compressed data]
```

The value of a user type.  A usertype needs to be defined first.  The values that follow are in the order that the usertype defined them.  If an array of usertypes then each item is on their own line.

In text the values may add new lines.  I.E. string values have the \n as the indicator that the string ends.  Other values may simply add a separator like n4.

If compression is used then the values are collated into a buffer first before compressing.  A byte count is required to know what the uncompressed buffer size was.

```
Binary                                                     Text


[n1      :14]                                              ContactList2 userdata- TypeContact\n
[n1 * 14 :ContactList2]                                    `Robbert\n
[n2      :1|1|9]                                           `de Groot\n
[n1 * 11 :TypeContact]                                     100 `zekaric@gmail.com\n
[n4      :2]                                               1 1 2 3 5\n
[n4      :7]
[n1 * 7  :Robbert]
[n4      :8]
[n1 * 8  :de Groot]
[n1      :100]
[n4      :17]
[n1 * 17 :zekaric@gmail.com]
[n1      :1]
[n1      :1]
[n1      :2]
[n1      :3]
[n1      :5]


[n1      :14]                                              ContactList2 userdata= TypeContact 2\n
[n1 * 14 :ContactList2]                                    `Robbert\n
[n2      :1|1|9]                                           `de Groot\n
[n1 * 11 :TypeContact]                                     100 `zekaric@gmail.com\n
[n4      :2]                                               1 1 2 3 5\n
[n4      :7]                                               `Sherlock\n
[n1 * 7  :Robbert]                                         `Holms\n
[n4      :8]                                               32 `221b@bakerst.uk\n
[n1 * 8  :de Groot]                                        10 20 30 40 50\n
[n1      :100]
[n4      :17]
[n1 * 17 :zekaric@gmail.com]
[n1      :1]
[n1      :1]
[n1      :2]
[n1      :3]
[n1      :5]
[n4      :8]
[n1 * 8  :Sherlock]
[n4      :5]
[n1 * 5  :Holms]
[n1      :32]
[n4      :15]
[n1 * 15 :221b@bakerst.uk]
[n1      :10]
[n1      :20]
[n1      :30]
[n1      :40]
[n1      :50]


[n1      :14]                                              ContactList userdataz TypeContact 521 95 ...\n
[n1 * 14 :ContactList1]
[n2      :1|0|9]
[n1 * 11 :TypeContact]
[n4      :521]
[n4      :95]
[n1 * 95 :...]


[n1      :14]                                              ContactList2 userdataZ TypeContact 2 5213 954 ...\n
[n1 * 14 :ContactList2]
[n2      :1|1|9]
[n1 * 11 :TypeContact]
[n4      :2]
[n4      :5213]
[n4      :954]
[n1 * 954:...]
```
