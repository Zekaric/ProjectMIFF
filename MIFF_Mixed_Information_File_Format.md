
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
        2.2.2 - Record Composition<br />
            2.2.2.1 - Key<br />
            2.2.2.2 - Type Code<br />
            2.2.2.3 - Array Size<br />
            2.2.2.4 - Compression Flag<br />
            2.2.2.5 - Value<br />
    2.3 - Text Format<br />
        2.3.1 - Whitespace Characters<br />
        2.3.2 - Separator Characters<br />
        2.3.3 - Printable Characters<br />
        2.3.4 - Base64<br />
        2.3.5 - Real Number Representation<br />
        2.3.6 - Record Composition<br />
**3 - Fromat: Value Specifics**<br />
    3.1 - Key-Value Block Begin and End.<br />
    3.2 - Type value<br />
    3.3 - String values<br />
    3.4 - Boolean values<br />
    3.5 - Simple values<br />
    3.6 - User Type<br />
    3.7 - User Data<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals



* **Simple  **  The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief   **  The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **Accurate**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Design Decisions:


**Why not XML or JSON?**

I find XML is too verbose.  JSON is much better than XML and is a reasonable alternative to XML.  Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I do not claim MIFF is better, it is just different.

**Why Big Endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was Big Endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different Endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another.

## 1.4 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2 - Format: Base


There are two representations of the format.  A Text file representation and a Binary file representation.  They will both contain exactly the same data and be exactly the same in feature set.  The Binary file may have the advantage of possibly being slightly more compact and slightly faster in reading and writing.

A MIFF format is essentially a collection of key value pairs.  Values are typed.  Nesting and user types are allowed.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This is meant to be a transfer file format to move data from one program to another.

Common to both Text and Binary formats, any Byte data that is encode or stored as a binary byte sequence will be in big endian order.  In a text file the data is stored in Base64 like encoding.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.

![image](indent.png)MIFF![image](nl.png)<br />![image](indent.png)1![image](nl.png)<br />![image](indent.png)TXT![image](nl.png)<br />![image](indent.png)[Sub-Format Name string]![image](nl.png)<br />![image](indent.png)[Sub-Format Version string]![image](nl.png)

![image](tab.png) means a tab character.  ![image](nl.png) means a new line character.


* **Line 1** - Format of the file.  MIFF.

* **Line 2** - Version of the file.  Version 1.

* **Line 3** - Representation of the file.  TXT for text, or BIN for binary.

* **Line 4** - Sub-Format name.  A MIFF file is potentially a container for an sub format of the file.

* **Line 5** - Sub-Format version identifier.

If the representation is Binary then right after the \n of the fifth line, the binary format will begin.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain tabs (\t, ![image](tab.png)) or new lines (\n, ![image](nl.png)).  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8 one character/codepoint can span multiple bytes.  So this name can not exceed this byte size.

Binary files will be explored at a later date.

## 2.2 - Content


Before going into the specifics of the format we will cover some things at a high level.

### 2.2.1 - Numbers


**Natural numbers** (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

**Integer numbers** are numbers are positive and negative whole numbers.

**Real numbers   ** (floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.2 - Record Composition


A record basically defines a key - value pair.  However the composition of that record is composed of a few parts.

**[key]             ** will be a string that will somewhat uniquely identify the value.

**[type code]       ** will be a code (BIN) or sequence of characters (TXT) that will dictate what the value will look like as well as a compression setting and array size setting.

**[array size]      ** will be a number or special character to indicate the number of values of a particular type is associated with the key.

**[compression flag]** will be a setting on how the value is compressed, if at all.

**[value]           ** will depend on the the other parts so it can vary in appearance.

#### 2.2.2.1 - Key


Keys are always a single string of any character in UTF8 encoding as long as none are \t and \n.  Whitespace, leading, trailing, and internal to the key string are significant and cannot be trimmed or thinned out.  Keys are limited to being 255 bytes long.  In UTF8 that may not mean 255 letters/characters as some characters may end up requiring more than 1 byte.  I would suggest to limit the whitespace to just " " and nothing else.  Everything else should be a printable character.  We do not need another "Whitespace Language" monster.

```
12345
;':][.,<>]'
a_b_c
$cost
été
This is also a valid key with internal spaces
```

#### 2.2.2.2 - Type Code


| Text Type Code | Description |
| --- | --- |
| <br />**Block Types**<br /> |
| { | Key-Value block begins. |
| } | Key-Value block ends. |
| <br />**Basic Types**<br /> |
| type | Type value.  One of these type codes or a user type code. |
| define | A series of key-type pairs.  You can define up to 4031 new types maximum. |
| " | String (UTF8) data, can be of any length. |
| b | Boolean value. |
| i1 i2 i3 i4 i8 i16 i32 i64 i128 i256 | An integer number.  Min and Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for some of these, like i3, but I include these byte counts because they may be useful in certain cases. |
| n1 n2 n3 n4 n8 n16 n32 n64 n128 n256 | A natural number.  Ranges from 0 to max value.  Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for a lot of these, like n3, but I include these byte counts because they may be useful in certain cases. |
| r2 r4 r8 | A real value. |
| <br />**User Tyes**<br /> |
| [user type name] | Depends on what the user type defines.  A define must exist first. |


#### 2.2.2.3 - Array Size


| Encoding Code (Text) | Description |
| --- | --- |
| 1 | A single value follows.  In binary, no array size byte sequence follows. |
| [number of items] | An array of values follows.  In binary, 001 means a 1 byte natural follows; 010 means a 2 bytes natural follows; 011 means a 3 byte natural follows; 100 means a 4 byte natural follows; 101 means an 8 byte natural follows; 110 means a 16 byte natural follows. |
| &#42; | An unknown array of values follows or used when defining a user type.  See User types. |


#### 2.2.2.4 - Compression Flag


| Encoding Code (Text) | Description |
| --- | --- |
| - | Values are inlined raw with no compression. |
| . | Values are compressed with zlib as one big chunk. |
| : | Values are broken down into smaller chunks and then each chunk compressed with zlib. |


#### 2.2.2.5 - Value


See the types respective sections for details.

## 2.3 - Text Format


The text format is a UTF8 text file.

The format is only somewhat human readable, and only somewhat text editor friendly.  It was not designed to be "human friendly" but rather "debug friendly."  What I mean is that you will be able to look at the format in a text editor, if that text editor is capable of handling really large files with really long lines; and if you know this format, you will be able to debug it.  To a certain degree you should be able to debug the binary version as well since it will not be too different than the text version.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or you may not be seeing the whole file.

All lines are terminated with a single \n (![image](nl.png)).  NOT an MSDOS/Windows \r\n or a MAC \n\r or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.  No exceptions.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF only uses tab (\t, ![image](tab.png)) as a separater character.  And only one ![image](tab.png) between fields.  Whitespace is not considered a separater character and will be part of a key or value if it is present.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character causes ink to be placed on the paper, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Base64


A few items are stored in Base64.  This is to add a little compression to the items in question but mainly it is to ensure binary exactness of the data when writing the values out and reading the values back in.  Unlike normal Base64 which will break the buffer into lines of a certain length, Base64 values in a MIFF are one long continuous stream of characters.  No \n or line breaks of any kind will be present.

### 2.3.5 - Real Number Representation


All real numbers are stored as a Base64 string.  No exceptions.  This is to ensure that when read back into memory from file, the number is exactly as it was when it was stored in the file.  The real value in the MIFF file will be in Big Endian order.

### 2.3.6 - Record Composition


![image](indent.png)[type code]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)[compression flag]![image](tab.png)[value]![image](nl.png)

Any space found in the key that is not a ![image](tab.png) will be part of the key.  They will be significant.  Do not strip or reduce them.

There has to be one ![image](tab.png) separator between each part.

How the value appears will depend on the definition here.

To be clear...

**Invalid:** Absolutely no ![image](nl.png) anywhere before or within the key value line&#42;.  Absolutely no blank lines.  Absolutely no extra ![image](tab.png) anywhere in the format.

![image](indent.png)[type code]![image](tab.png)![image](tab.png)[key]![image](nl.png)[array size] [compression flag]![image](tab.png)[value]![image](nl.png)

What [value] will look like will depend on what is being stored for the record.  Values will be discussed lower in section 3.

# 3 - Fromat: Value Specifics


## 3.1 - Key-Value Block Begin and End.


Key-Value blocks are special.  They are needed to allow nesting of key values.

In a text file, the last line of a Key-Value Block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

In a binary file you will have a key size of 0 to indicate the end of a Key-Value block.

![image](indent.png){![image](tab.png)[key]![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)

Key value bocks can have an array size.  What that will mean is that there are [array size] key value pairs following.  The next key value pair after the last key value will not belong to this block.

A block is terminated when a value type of "}" is reached or array size key values have been read.  The key for this line, if present, will be ignored.  So it should really be empty to save space.  Every begin requires an end, there must not be extras of each in the file.

Compression flags are never used with this value type.

![image](indent.png){![image](tab.png)docInfo![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)

![image](indent.png){![image](tab.png)level1![image](nl.png)<br />![image](indent.png){![image](tab.png)level2![image](nl.png)<br />![image](indent.png){![image](tab.png)level3![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)<br />![image](indent.png){![image](tab.png)anotherLevel3![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)<br />![image](indent.png)}![image](nl.png)<br />![image](indent.png){![image](tab.png)anotherLevel2![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)<br />![image](indent.png)}![image](nl.png)<br />![image](indent.png){![image](tab.png)anotherLevel1![image](nl.png)<br />![image](indent.png)...<br />![image](indent.png)}![image](nl.png)<br />

## 3.2 - Type value


type stores type format string.

![image](indent.png)type![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[text type code]![image](nl.png)

![image](indent.png)type![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)-(![image](tab.png)[text type code])&#42;![image](nl.png)

![image](indent.png)type![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png).![image](tab.png)[compressed size]![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)type![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

There is no chunked or compressed single value case.

The text chunked and compressed type array is using the binary type codes and not the text type codes.

![image](indent.png)type![image](tab.png)type1![image](tab.png)1![image](tab.png)-![image](tab.png)b![image](nl.png)

![image](indent.png)type![image](tab.png)type2![image](tab.png)2![image](tab.png)-![image](tab.png)b![image](tab.png)n4![image](nl.png)

![image](indent.png)type![image](tab.png)type3![image](tab.png)100![image](tab.png).![image](tab.png)31![image](tab.png)...![image](nl.png)

![image](indent.png)type![image](tab.png)type4![image](tab.png)100000![image](tab.png):![image](tab.png)1000![image](tab.png)2342![image](tab.png)...![image](tab.png)4546![image](tab.png)...![image](nl.png)

## 3.3 - String values


![image](indent.png)"![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[string value]![image](nl.png)

![image](indent.png)"![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent.png)"![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)-|img (![image](tab.png)[string value])&#42;![image](nl.png)

![image](indent.png)"![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

If no compression is used then the string starts right after the ![image](tab.png) character till the end of the line but not including the new line character at the end of the line.

Multi-line strings are placed on one line.  This is done by escaping new line characters, tab characters and the escape character when the string is written out without any compression.

If compression is used then we will still escape these characters.  Note however there is no "." compression option when dealing with a string or an array of strings.  This is because a string length is never consistent so chunked compression is only available.  Strings are stored in the compressed buffer in the same way as it is stored in the uncompressed string.

```
\t - tab            - 0x09
\n - new line       - 0x0a
\\ - \
```

Examples:

![image](indent.png)"![image](tab.png)string1![image](tab.png)1![image](tab.png)-![image](tab.png)This is line1.\nThis is line 2.![image](nl.png)

![image](indent.png)"![image](tab.png)string3![image](tab.png)1![image](tab.png):![image](tab.png)1000000![image](tab.png)23424![image](tab.png)[Base64 stream]\n![image](tab.png)458947![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)"![image](tab.png)stringList1![image](tab.png)3![image](tab.png)-![image](tab.png)This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)

![image](indent.png)"![image](tab.png)stringList3![image](tab.png)3![image](tab.png):![image](tab.png)1000![image](tab.png)252![image](tab.png)[Base64 stream]![image](tab.png)543![image](tab.png)[Base64 stream]![image](nl.png)

## 3.4 - Boolean values


![image](indent.png)b![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[boolean value]![image](nl.png)

![image](indent.png)b![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)-(![image](tab.png)[boolean value])&#42;![image](nl.png)

![image](indent.png)b![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)[compressed size]![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)b![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

By default, the value will be 'T' for true, or 'F' for false.

For array values in text files, the value will be a string of 'T' or 'F' letters with a separator in between.

For array values in binary , the same string will compressed.

'z' encoding flag is never used with this type.

![image](indent.png)b![image](tab.png)Bool1![image](tab.png)1![image](tab.png)-![image](tab.png)T![image](nl.png)

![image](indent.png)b![image](tab.png)Bool2![image](tab.png)10![image](tab.png)-![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](nl.png)

![image](indent.png)b![image](tab.png)Bool3![image](tab.png)1024![image](tab.png).![image](tab.png)25![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)b![image](tab.png)Bool4![image](tab.png)100000![image](tab.png):![image](tab.png)1000![image](tab.png)234![image](tab.png)[Base64 stream]![image](tab.png)2934![image](tab.png)[Base64 stream]![image](nl.png)

## 3.5 - Simple values


Simple value encoding.  Based on what is being stored the byte streams only look slighly different.

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[type value]![image](nl.png)

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png).![image](tab.png)[compressed size]![image](tab.png)[compressed data]![image](nl.png)

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)-(![image](tab.png)[type value])&#42;![image](nl.png)

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png).![image](tab.png)[compressed size]![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)[text type code]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

binary sizes for the simple types will be one of...

| Type | Byte Count |
| --- | --- |
| i1, n1 | 1 |
| i2, n2 | 2 |
| i3, n3 | 3 |
| i4, n4, r4 | 4 |
| i8, n8, r8 | 8 |
| i16, n16, r16 | 16 |
| i32, n32, r32 | 32 |
| i64, n64, r64 | 64 |
| i128, n128, r128 | 128 |
| i256, n256, r256 | 256 |


Text representation for a value will be...

| Type Code | Representation |
| --- | --- |
| i&#42;, n&#42; | 1 value in regular digits. |
| r&#42; | 1 Base64 stream of the 1 value. |


If using an array flag the above is repeated as many times as there are array elements.  Each array element will be separated by one \t.

![image](indent.png)i4![image](tab.png)1Int![image](tab.png)1![image](tab.png)-![image](tab.png)1024![image](nl.png)

![image](indent.png)n4![image](tab.png)&#42;Nat![image](tab.png)8![image](tab.png)-![image](tab.png)1![image](tab.png)2![image](tab.png)4![image](tab.png)8![image](tab.png)16![image](tab.png)32![image](tab.png)64![image](tab.png)128![image](nl.png)

![image](indent.png)r4![image](tab.png)1Real![image](tab.png)1![image](tab.png)-![image](tab.png)[Base64 encoded value]![image](nl.png)

## 3.6 - User Type


A definition of a user type.

![image](indent.png)define![image](tab.png)[key]![image](tab.png)[member count]![image](tab.png)-(![image](tab.png)[type code]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)[compression type])&#42;![image](nl.png)

Compression is never used with this type code.

![image](indent.png)define![image](tab.png)TypeContact![image](tab.png)6![image](tab.png)-![image](tab.png)"![image](tab.png)NameGiven![image](tab.png)1![image](tab.png)-![image](tab.png)"![image](tab.png)NameFamily![image](tab.png)1![image](tab.png)-![image](tab.png)n1![image](tab.png)Age![image](tab.png)1![image](tab.png)-![image](tab.png)"![image](tab.png)EMail![image](tab.png)1![image](tab.png)-![image](tab.png)n1![image](tab.png)DataFlags![image](tab.png)5![image](tab.png)-![image](tab.png)i4![image](tab.png)Streamed data![image](tab.png)&#42;![image](tab.png):![image](nl.png)

![image](indent.png)define![image](tab.png)Image![image](tab.png)3![image](tab.png)-![image](tab.png)n4![image](tab.png)Width![image](tab.png)1![image](tab.png)-![image](tab.png)n4![image](tab.png)Height![image](tab.png)1![image](tab.png)-![image](tab.png)n1![image](tab.png)Pixels![image](tab.png)&#42;![image](tab.png):![image](tab.png)1000000![image](nl.png)

![image](indent.png)define![image](tab.png)Binary Blob![image](tab.png)2![image](tab.png)-![image](tab.png)n4![image](tab.png)Byte Count![image](tab.png)1![image](tab.png)-![image](tab.png)n1![image](tab.png)Byte Data![image](tab.png)&#42;![image](tab.png):![image](tab.png)1000000![image](nl.png)

As shown in the above, "Pixels" or "Byte Data" is an array of values but this array appears to be variable in length and we do not know its size when we defined the type.  The size will be provided when the data for the type is provided.

![image](indent.png)define![image](tab.png)point![image](tab.png)1![image](tab.png)-![image](tab.png)r4![image](tab.png)value![image](tab.png)3![image](tab.png)-![image](nl.png)

...

![image](indent.png)define![image](tab.png)point![image](tab.png)3![image](tab.png)-![image](tab.png)r8![image](tab.png)east![image](tab.png)1![image](tab.png)-![image](tab.png)r8![image](tab.png)north![image](tab.png)1![image](tab.png)-![image](tab.png)r8![image](tab.png)elevation![image](tab.png)1![image](tab.png)-![image](nl.png)

If you redefine a userType then the previous userType will be replaced with the new one.

## 3.7 - User Data


![image](indent.png)[user type]![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[user type data]![image](nl.png)

![image](indent.png)[user type]![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent.png)[user type]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png)-![image](tab.png)[user type data]![image](nl.png)

![image](indent.png)[user type]![image](tab.png)[key]![image](tab.png)[array size]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed size]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

The value of a user type.  A "define" needs to be present before it can be used.  The values that follow are in the order that the usertype defined them.

If compression is used then the values are collated into a buffer first before compressing.  A byte count is required to know what the uncompressed buffer size was.

![image](indent.png)TypeContact![image](tab.png)Contact1![image](tab.png)1![image](tab.png)-![image](tab.png)Robbert![image](tab.png)de Groot![image](tab.png)100![image](tab.png)zekaric@gmail.com![image](tab.png)1![image](tab.png)1![image](tab.png)2![image](tab.png)3![image](tab.png)5![image](nl.png)

![image](indent.png)TypeContact![image](tab.png)Contact2![image](tab.png)1![image](tab.png):![image](tab.png)10000![image](tab.png)521![image](tab.png)[Base64 stream]![image](nl.png)

![image](indent.png)TypeContact![image](tab.png)ContactList1![image](tab.png)2![image](tab.png)-![image](tab.png)Robbert![image](tab.png)de Groot![image](tab.png)100![image](tab.png)zekaric@gmail.com![image](tab.png)1![image](tab.png)1![image](tab.png)2![image](tab.png)3![image](tab.png)5![image](tab.png)Sherlock![image](tab.png)Holms![image](tab.png)32![image](tab.png)221b@bakerst.uk![image](tab.png)10![image](tab.png)20![image](tab.png)30![image](tab.png)40![image](tab.png)50![image](nl.png)

![image](indent.png)TypeContact![image](tab.png)ContactList3![image](tab.png)2![image](tab.png):![image](tab.png)100000![image](tab.png)5213![image](tab.png)[Base64 stream]![image](tab.png)32784![image](tab.png)[Base64 stream]![image](nl.png)
