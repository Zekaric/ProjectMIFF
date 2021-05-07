
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
        2.2.1 - Binary Syte Sequences<br />
        2.2.2 - Numbers<br />
        2.2.3 - Key Value Composition<br />
            2.2.3.1 - Key<br />
            2.2.3.2 - Value Header<br />
                2.2.3.2.1 - Type Code<br />
                2.2.3.2.2 - Array Count Flag<br />
                2.2.3.2.3 - Compression Flag<br />
            2.2.3.3 - Value<br />
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
    3.1 - Key only<br />
    3.2 - Key-Value Block Begin and End.<br />
    3.3 - Binary values<br />
    3.4 - Embedded files<br />
    3.5 - Type value<br />
    3.6 - String values<br />
    3.7 - Path Values<br />
    3.8 - Boolean values<br />
    3.9 - Simple values<br />
    3.10 - Key Type<br />
    3.11 - User Type<br />
    3.12 - User Data<br />

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

```
"MIFF" \t "." \n
"1" \t "." \n
"TXT" \t "." \n
[Sub-Format Name string] \t "." \n
[Sub-Format Version string] \t "." \n
```

or

```
"MIFF"\n
"1"\n
"BIN"\n
[Sub-Format Name string]\n
[Sub-Format Version string]\n
```

These lines may look a little odd if you do not know anything about the MIFF format.  Anything in quotes (the quotes will not be written out) is what is seen in the file.

\n means a new line character.


* **Line 1** - Format of the file.  MIFF.

* **Line 2** - Version of the file.  Version 1.

* **Line 3** - Representation of the file.  TXT for text, or BIN for binary.

* **Line 4** - Sub-Format name.  A MIFF file is potentially a container for an sub format of the file.

* **Line 5** - Sub-Format version identifier.

If the representation is Binary then right after the \n of the fifth line, the binary format will begin.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain tabs (\t) or new lines (\n).  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8 one character/codepoint can span multiple bytes.  So this name can not exceed this byte size.

## 2.2 - Content


Before going into the specifics of the format we will cover some things at a high level.

### 2.2.1 - Binary Syte Sequences


Everything within "[]" is a byte sequence of a known size.  Each byte seqence is separated but in reality it is immediately following the previous byte sequence.  Nothing extra is added between sequences.

A byte sequence is "[" [Byte count] ":" [Name of this Byte sequence] "]", where Byte count is a number indicating the number of bytes this sequence uses.

### 2.2.2 - Numbers


**Natural numbers** (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

**Integer numbers** are numbers are positive and negative whole numbers.

**Real numbers   ** (floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.3 - Key Value Composition


```
[key] [value header] [value]
```

**[key]         ** will be a string that will somewhat uniquely identify the value.

**[value header]** will be a code (binary) or sequence of characters (text) that will dictate what the value will look like.

**[value]       ** will depend on the **[value header]** on what is contained here.

The fields are separated by a single \t character.

#### 2.2.3.1 - Key


Keys are always a single string of any character in UTF8 encoding as long as none are \t and \n.  Whitespace, leading, trailing, and internal to the key string are significant and cannot be trimmed or thinned out.  Keys are limited to being 255 bytes long.  In UTF8 that may not mean 255 letters/characters as some characters may end up requiring more than 1 byte.  I would suggest to limit the whitespace to just " " and nothing else.  Everything else should be a printable character.  We do not need another "Whitespace Language" monster.

```
12345
;':][.,<>]'
a_b_c
$cost
été
This is also a valid key with internal spaces
```

#### 2.2.3.2 - Value Header


A value header is comprised of a type code, an array count, and a compression flag.

```
Text
[type code] \t [array count] \t [compression flag] \n


Binary (2 byte sequence plus a variable number of bytes for array count)
[2B:(cc)(aaa)(ttt tttttttt)][0/1/2/3/4/8/16B: Array Count]


(cc)           -  2 bits, compression flag.
(aaa)          -  3 bits, array flag.
(ttt tttttttt) - 11 bits, type code.
```

##### 2.2.3.2.1 - Type Code


| Binary Type Code | Text Type Code | Description |
| --- | --- | --- |
| 0 | "." | No value, presence or absence of a key is enough information. |
| <br />**Block Types**<br /> |
| 1 | "{" | Key-Value block begins. |
| 2 | "}" | Key-Value block ends. |
| <br />**Basic Types**<br /> |
| 3 | "type" | Type value.  One of these type codes or a user type code. |
| 4 | "define" | A series of key-type pairs.  You can define up to 4031 new types maximum. |
| 5 | """ (Sorry, just a " ) | String (UTF8) data, can be of any length. |
| 6 | "-&#62;" | A specific string value that represents a relative path (relative to the MIFF file location and no backsies "..", local directory or deeper directory.) |
| 7 | "b" | Boolean value. |
| 10   11   12   13   14   15    16    17    18     19 | "i1" "i2" "i3" "i4" "i8" "i16" "i32" "i64" "i128" "i256" | An integer number.  Min and Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for some of these, like i3, but I include these byte counts because they may be useful in certain cases. |
| 20   21   22   23   24   25    26    27    28     29 | "n1" "n2" "n3" "n4" "n8" "n16" "n32" "n64" "n128" "n256" | A natural number.  Ranges from 0 to max value.  Max value will depend on how much the Bytes can encode.  Yes I know there are no native types for a lot of these, like n3, but I include these byte counts because they may be useful in certain cases. |
| 31   33   34   35    36    37    38     39 | "r2" "r4" "r8" "r16" "r32" "r64" "r128" "r256" | A real value.  I do realize there are no standards for r16 - r256 but these are just placeholders for the future if they ever become standard. |
| <br />**Embedded Types**<br /> |
| 40  41   42    43 | "&#42;" "&#42;&#42;" "&#42;&#42;&#42;" "&#42;&#42;&#42;&#42; | Binary data of n4, n8, n16, n32 byte counts in size. |
| 50    51     52      53 | "[&#42;]" "[&#42;&#42;]" "[&#42;&#42;&#42;]" "[&#42;&#42;&#42;&#42;}" | An embedded file of n4, n8, n16, n32 byte counts in size. |
| <br />**User Tyes**<br /> |
| 64 and higher | [user type name] | Depends on what the user type defines. |


##### 2.2.3.2.2 - Array Count Flag


| Encoding Code (Binary (aaa)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| 000 | 1 | A single value follows.  In binary, no array count byte sequence follows. |
| 001, 010, 011, 100, 101, 110 | [number of items] | An array of values follows.  In binary, 001 means a 1 byte natural follows; 010 means a 2 bytes natural follows; 011 means a 3 byte natural follows; 100 means a 4 byte natural follows; 101 means an 8 byte natural follows; 110 means a 16 byte natural follows. |
| 111 | &#42; | An array of values.  This is only ever used when defining a user type.  See User types. |


##### 2.2.3.2.3 - Compression Flag


| Encoding Code (Binary (cc)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| 00 | - | Values are inlined raw with no compression. |
| 01 | . | Values are compressed with zlib as one big chunk. |
| 01 | : | Values are broken down into smaller chunks and then each chunk compressed with zlib. |


#### 2.2.3.3 - Value


The value header generally indicates what will be stored in the value of the key value pair.  It then becomes an issue on how it is represented in the two types of files.  See their respective sections for the details.

## 2.3 - Text Format


The text format is a UTF8 text file.

The format is only somewhat human readable, and only somewhat text editor friendly.  It was not designed to be "human friendly" but rather "debug friendly."  What I mean is that you will be able to look at the format in a text editor, if that text editor is capable of handling really large files with really long lines; and if you know this format, you will be able to debug it.  To a certain degree you should be able to debug the binary version as well since it will not be too different than the text version.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or you may not be seeing the whole file.

All lines are terminated with a '\n'.  NOT an MSDOS/Windows \r\n or a MAC \n\r ending or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.  No exceptions.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF only uses tab (\t) as a separater character.  And only one \t between fields.  Whitespace is not considered a separater character and will be part of a key or value if it is present.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character makes a mark, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Base64


A few items are stored in Base64.  This is to add a little compression to the items in question but mainly it is to ensure binary exactness of the data when writing the values out and reading the values back in.  Unlike normal Base64 which will break the buffer into lines of a certain length, Base64 values in a MIFF are one long continuous stream of characters.  No \n or line breaks of any kind will be present.

### 2.3.5 - Real Number Representation


All real numbers are stored as a Base64 string.  No exceptions.  This is to ensure that when read back into memory from file, the number is exactly as it was when it was stored in the file.  The real value in the MIFF file will be in Big Endian order.

### 2.3.6 - Key Value Format


```
[key] \t [value header] \t [value] \n
```

Any space found in the key that is not a \t will be part of the key.  They will be significant.  Do not strip or reduce them.

There has to be one \t separator between [key] and [value header].

There has to be one \t separator between [value header] and [value].

To terminate the key value pair use a single '\n' character.

To be clear...

**Invalid:** Absolutely no "\n" anywhere before or within the key value line&#42;.  Absolutely no blank lines.  Absolutely no extra \t anywhere in the format.

```
\t[key]\n[value header]\t[value]\n
[key] [value header] [value]\n
```

&#42; There are cases where the [value] will be broken up by '\n' characters but this will be indicated where that will happen.  In general most key value lines live on a single line.

#### 2.3.6.1 - Value Header


Value header will typically look like this.

```
[type code] \t [array count] \t [compression flag]
```

However it can vary slightly depending on the Type Code being used.  See section 3 for accurate representation.

**[type flag]** is a short string as listed in the type code table earlier on in this document.

**[array count]** is a natural value indicating how many values of this given type is present.

**[compression flag]** is a string as listed in the compression flag table earlier in this document.

#### 2.3.6.2 - Value


Values will be discussed lower in section 3.

## 2.4 - Binary


The Binary file will to match the Text file 1-1.

### 2.4.1 - Key Value Format


```
[n1           :key size]
[n1 * key size:key]
[*            :value header]
[*            :value]
```

#### 2.4.1.1 - Key


[key size] defines how long the key is in bytes.  Keys are 1 to 255 Bytes and the Bytes define a UTF8 string.

[key] defines the key of the key value pair.  The length of which was defined by [key size] above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


Value header will typically look like this.

```
[n2:2 bit - compression flag, 3 bit - array flag, 11 bit - type code]
[0,n1,n2,n3,n4,n8,n16:array count]
```

The first 2 bytes holds the type code and array and compression flags.  The 5 high bits of this value are holding the array and compression flags.  The remaining 11 bits are for the type code.  See section on Type Codes, Array Count Flag and Compression Flag earlier in this document for what the type code and flags should equal to.

[array count] byte sequence is only present when storing an array of values.

#### 2.4.1.3 - Value


[&#42;:value] field will vary wildly depending on the [value header].  But it will be a known size given all that information.

# 3 - Fromat: Value Specifics


## 3.1 - Key only


Just a key and no value.  Just like the header lines of the file.

```
Binary                                                     Text


[n1           :key size]                                   [key]\n
[n1 * key size:key]                                        [key]\t.\n
[n2           :value header - 00|000|0]
```

In text, either line will do.  The first line saves 2 bytes.

## 3.2 - Key-Value Block Begin and End.


Key-Value blocks are special.  They are needed to allow nesting of key values.

In a text file, the last line of a Key-Value Block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

In a binary file you will have a key size of 0 to indicate the end of a Key-Value block.

```
Binary                                                     Text


[n1           :key size]                                   [key]\t{\n
[n1 * key size:key]                                        ...
[n2           :value header   - 00|000|1]                  \t}\n
...
[n1           :0]
[n2           :value header   - 00|000|2]


[n1           :key size]                                   [key]\t{\t[array count]\n
[n1 * key size:key]                                        ...
[n2           :value header   - 00|???|1]
[???          :array count]
...
```

Key value bocks can have an Array count.  What that will mean is that there are [array count] key value pairs following.  The next key value pair will not belong to this block.

A block is terminated when a value type of "}" is reached.  The key, if present on this line will be ignored.  Begin and end need to match.  Miss one or the other and the file is corrupt.

Compression flags are never used with this value type.

To be clear on how it works.  In the text file leading separators before the key are unnecessary and are only here for clarity.  Leading separators before a \n are allowed but are wasteful.

```
Binary                                                     Text


[n1     :7]                                                docInfo\t{\n
[n1 * 7 :docInfo]                                          ...
[n2     :00|000|1]                                         \t}\n
...
[n1     :0]
[n2     :00|000|2]


[n1     :6]                                                level1\t{\n
[n1 * 6 :level1]                                           level2\t{\n
[n2     :00|000|1]                                          level3\t{\n
[n1     :6]                                                ...
[n1 * 6 :level2]                                           \t}\n
[n2     :00|000|1]                                          anotherLevel3\t{\n
[n1     :6]                                                ...
[n1 * 6 :level3]                                           \t}\n
[n2     :00|000|1]                                          \t}\n
...                                                        anotherLevel2\t{\n
[n1     :0]                                                ...
[n2     :00|000|2]                                         \t}\n
[n1     :13]                                               \t}\n
[n1 * 13:anotherlevel3]                                     anotherLevel1\t{\n
[n2     :00|000|1]                                         ...
...                                                        \t}\n
[n1     :0]
[n2     :00|000|2]
[n1     :0]
[n2     :00|000|2]
[n1     :6]
[n1 * 6 :anotherlevel2]
[n2     :00|000|1]
...
[n1     :0]
[n2     :00|000|2]
[n1     :0]
[n2     :00|000|2]
[n1     :6]
[n1 * 6 :anotherlevel1]
[n2     :00|000|1]
...
[n1     :0]
[n2     :00|000|2]


[n1     :8]                                                itemList\t{\t2\n
[n1 * 8 :itemList]                                         item\t{\n
[n2     :00|001|1]                                         ...
[n1     :2]                                                \t}\n
[n1     :4]                                                item\t{\n
[n1 * 4 :item]                                             ...
[n2     :00|000|1]                                         \t}\n
...
[n1     :0]
[n2     :00|000|2]
[n1     :4]
[n1 * 4 :item]
[n2     :00|000|1]
...
[n1     :0]
[n2     :00|000|2]
```

## 3.3 - Binary values


```
Binary                                                     Text


[n1                  :key size]                            [key] \t "*" \t "1" \t "-" \t[binary size] \t [Base64 binary data]\n
[n1 * key size       :key]
[n2                  :value header - 00|000|3]
[n4                  :binary size]
[n1 * binary size    :binary data]


[n1                  :key size]                            [key] \t "*" \t [array count] \t "-"\n
[n1 * key size       :key]                                 [binary size] \t [Base64 binary data] \n
[n2                  :value header - 00|???|3]             [binary size] \t [Base64 binary data] \n
[n4                  :array count]                         ...
[n4                  :binary size]
[n1 * binary size    :binary data]
[n4                  :binary size]
[n1 * binary size    :binary data]
...


[n1                  :key size]                            [key] \t "*" \t "1" \t "." \t [binary size] \t [compressed size] \t [Base64 compressed data]\n
[n1 * key size       :key]
[n2                  :value header - 01|00|3]
[n4                  :binary size]
[n4                  :compressed size]
[n1 * compressed size:compressed data]


[n1                  :key size]                            [key] \t "*" \t [array count] \t "." \n
[n1 * key size       :key]                                 [binary size] \t [compressed size] \t [Base64 compressed data] \n
[n2                  :value header - 01|???|3]             [binary size] \t [compressed size] \t [Base64 compressed data] \n
[???                 :array count]
[n4                  :binary size]
[n4                  :compressed size]
[n1 * compressed size:compressed data]
[n4                  :binary size]
[n4                  :compressed size]
[n1 * compressed size:compressed data]
...


[n1                  :key size]                            [key] \t "*" \t "1" \t ":" \t [binary size] \t [chunk byte count] \n
[n1 * key size       :key]                                 [compressed size] \t [Base64 compressed data] \n
[n2                  :value header - 10|000|3]             [compressed size] \t [Base64 compressed data]\n
[n4                  :binary size]                         ...
[n4                  :chunk byte count]
[n4                  :compressed size]
[n1 * compressed size:compressed data]
...
[n4                  :compressed size]
[n1 * compressed size:compressed data]
...


[n1                  :key size]                            [key] \t "*" \t [array count] \t ":" \n
[n1 * key size       :key]                                 [binary size] \t [chunk byte count]\n
[n2                  :value header - 10|???|3]             [compressed size] \t [Base64 compressed data] \n
[???                 :array count]                         [compressed size] \t [Base64 compressed data]\n
[n4                  :binary size]                         ...
[n4                  :chunk byte count]                    [binary size] \t [chunk byte count]\n
[n4                  :compressed size]                     [compressed size] \t [Base64 compressed data]\n
[n1 * compressed size:compressed data]                     ...
...
[n4                  :compressed size]
[n1 * compressed size:compressed data]
[n4                  :binary size]
[n4                  :chunk byte count]
[n4                  :compressed size]
[n1 * compressed size:compressed data]
...
```

The binary data will have a header of 1 natural number (n4) before the binary data.  This value indicates the byte count of the binary data.  This byte count is not included when using Base64 or Zlib compression.  However the binary data will honor Base64 and Zlib compression if used.

```
Binary                                                     Text


[n1       :7]                                              binKey1 ...- 10 ...\n
[n1 * 7   :binKey1]
[n2       :00|00|3]
[n4       :10]
[n1 * 10  :...]


[n1       :7]                                              binKey2 ...= 2\n
[n1 * 7   :binKey2]                                        10 ...\n
[n2       :00|01|3]                                        10 ...\n
[n4       :10]
[n1 * 10  :...]
[n4       :10]
[n1 * 10  :...]


[n1       :7]                                              binKey3 ...z 1024 432 ...\n
[n1 * 7   :binKey3]
[n2       :01|00|3]
[n4       :1024]
[n4       :432]
[n1 * 432 :...]


[n1       :7]                                             binKey4 ...Z 2\n
[n1 * 7   :binKey4]                                       10240 5123 ...\n
[n2       :01|01|3]                                       10240 4603 ...\n
[n4       :10240]
[n4       :5123]
[n1 * 5123:...]
[n4       :10240]
[n4       :4603]
[n1 * 4603:...]


[n1       :7]                                              binKey3 ...c 10000000 100000\n
[n1 * 7   :binKey3]                                        432 ...\n
[n2       :01|00|3]                                        ...
[n4       :10000000]                                       50 ...\n
[n4       :432]
[n1 * 432 :...]
...
[n4       :50]
[n1 * 50  :...]


[n1       :7]                                             binKey4 ...C 2\n
[n1 * 7   :binKey4]                                       10000000 100000\n
[n2       :01|01|3]                                       5123 ...\n
[n4       :10000000]                                      ...
[n4       :1000000]                                       50 ...\n
[n4       :5123]                                          20000000 500000\n
[n1 * 5123:...]                                           4603 ...\n
...                                                       ...
[n4       :50]                                            125 ...\n
[n1 * 50  :...]
[n4       :20000000]
[n4       :500000]
[n4       :4603]
[n1 * 4603:...]
...
[n4       :125]
[n1 * 125 :...]
```

Binary data is dangerous and should be used rarely if at all.  If someone uses them then they should be on the hook to define what the contents of the blob means.  If they do not, then they are being bad citizens and should be shamed!  SHAMED I SAY!

However this is here as a catchall just in case.

## 3.4 - Embedded files


```
Binary                                                     Text


[n1                        :key size]                [key] file- [file type] [byte count] [Base64 file data]\n
[n1 * key size       :key]
[n2                        :00|00|4]
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n1 * byte count           :file data]


[n1                        :key size]                [key] file= [array count]\n
[n1 * key size       :key]                           [three letter file type] [byte count] [Base64 file data]\n
[n2                        :00|01|4]                       [file type] [byte count] [Base64 file data]\n
[n4                        :array count]                   ...
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n1 * byte count           :file data]
[n3                        :three letter file type]
[n4                        :byte count]
[n1 * byte count           :file data]
...


[n1                        :key size]                [key] filez [file type] [byte count] [compressed size] [Base64 compressed data]\n
[n1 * key size       :key]
[n2                        :01|00|4]
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] fileZ [array count]\n
[n1 * key size       :key]                           [file type] [byte count] [compressed size] [Base64 compressed data]\n
[n2                        :01|01|4]                       [file type] [byte count] [compressed size] [Base64 compressed data]\n
[n4                        :array count]                   ...
[n1                        :file type byte count]
[n1 * file type byte count :file type]
[n4                        :byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n3                        :three letter file type]
[n4                        :byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...


[n1                        :key size]                [key] filec [file type] [byte count] [chunk byte count]\n
[n1 * key size       :key]                           [compressed size] [Base64 compressed data]\n
[n2                        :10|00|4]                       [compressed size] [Base64 compressed data]\n
[n1                        :file type byte count]          ...
[n1 * file type byte count :file type]
[n4                        :byte count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] fileC [array count]\n
[n1 * key size       :key]                           [file type] [byte count] [chunk byte count]\n
[n2                        :10|01|4]                       [compressed size] [Base64 compressed data]\n
[n4                        :array count]                   [compressed size] [Base64 compressed data]\n
[n1                        :file type byte count]          ...
[n1 * file type byte count :file type]                     [file type] [byte count] [chunk byte count]\n
[n4                        :byte count]                    [compressed size] [Base64 compressed data]\n
[n4                        :chunk byte count]              ...
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n3                        :three letter file type]
[n4                        :byte count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
```

File option will have a file type identifier, usually the file extension lower case letters only; a natural number for the file size, and then the file that represents the image, copied verbatum into the value portion.

```
Binary                                                     Text


[n1       :5]                                              File1 file- doc 1024 ...\n
[n1 * 5   :File1]
[n2       :00|00|4]
[n1       :3]
[n1 * 3   :doc]
[n4       :1024]
[n1 * 1024:...]


[n1       :9]                                              FileList1 fileZ 2\n
[n1 * 9   :FileList1]                                      htm  2048 517 ...\n
[n2       :00|01|4]                                        docx 4096 4001 ...\n
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
[n2       :01|00|4]
[n1       :3]
[n1 * 3   :css]
[n4       :1024]
[n4       :564]
[n1 * 564 :...]


[n1       :9]                                              FileList2 fileZ 2\n
[n1 * 9   :FileList2]                                      htm  2048 517 ...\n
[n2       :01|01|4]                                        docx 4096 4001 ...\n
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


[n1       :5]                                              File2 filec css 1123456 123456\n
[n1 * 5   :File2]                                          564 ...\n
[n2       :10|00|4]                                        ...
[n1       :3]                                              56 ...\n
[n1 * 3   :css]
[n4       :1123456]
[n4       :123456]
[n4       :564]
[n1 * 564 :...]
...
[n4       :56]
[n1 * 56  :...]


[n1       :9]                                              FileList2 fileC 2\n
[n1 * 9   :FileList2]                                      htm  2123456 123456\n
[n2       :10|01|4]                                        517 ...\n
[n4       :2]                                              ...
[n1       :3]                                              17 ...\n
[n1 * 3   :htm]                                            docx 3123123 123123\n
[n4       :2123456]                                        4001 ...\n
[n4       :123456]                                         ...
[n4       :517]
[n1 * 517 :...]
...
[n4       :17]
[n1 * 17  :...]
[n1       :4]
[n1 * 4   :docx]
[n4       :3123123]
[n4       :123123]
[n4       :4001]
[n1 * 4001:...]
...
```

## 3.5 - Type value


type stores type format string.

```
Binary                                                     Text


[n1                        :key size]                [key] type- [text type code]\n
[n1 * key size       :key]
[n2                        :value header - 00|00|5]
[n2                        :binary type code]


[n1                        :key size]                [key] type= [array count] [text type code]*\n
[n1 * key size       :key]
[n2                        :value header - 00|01|5]
[n4                        :array count]
[n2                        :binary type code]*


[n1                        :key size]                type3 typeZ [array count] [compressed size] [Base64 compressed binary type codes]
[n1 * key size       :key]
[n2                        :value header - 01|01|5]
[n4                        :array count]
[n4                        :compressed size]
[n1 * compressed size:compressed binary type code]


[n1                        :key size]                type3 typeC [array count] [chunk byte count]\n
[n1 * key size       :key]                           [compressed size] [Base64 compressed binary type codes]\n
[n2                        :value header - 01|01|5]        ...
[n4                        :array count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed binary type code]
...
```

There is no chunked or compressed single value case.

The text chunked and compressed type array is using the binary type codes and not the text type codes.

```
Binary                                                     Text


[n1     :5]                                                type1 type- bool\n
[n1 * 5 :type1]
[n2     :00|00|5]
[n2     :10]


[n1     :5]                                                type2 type= 2 bool .\n
[n1 * 5 :type2]
[n2     :00|01|5]
[n4     :2]
[n2     :10]
[n2     :8]


[n1     :5]                                                type3 typeZ 100 31 ...
[n1 * 5 :type3]
[n2     :01|01|5]
[n4     :100]
[n4     :31]
[n1 * 31:...]
```

## 3.6 - String values


Binary case is simpler so we'll talk about that first.

```
[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 00|00|6]
[n4                        :string byte count]
[n1 * string byte count    :string data]


[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 00|01|6]
[n4                        :array count]
[n4                        :string byte count]
[n1 * string byte count    :string data]
[n4                        :string byte count]
[n1 * string byte count    :string data]
...


[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 01|00|6]
[n4                        :string byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 01|01|6]
[n4                        :array count]
[n4 * array count          :string byte count list]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 10|00|6]
[n4                        :string byte count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...


[n1                        :key size]
[n1 * key size       :key]
[n2                        :value header - 10|01|6]
[n4                        :array count]
[n4 * array count          :string byte count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
[n4 * array count          :string byte count]
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
```

The case of a single value just requires a byte count before the actual string data.  The string data will be exactly the byte count size.  Since this is binary, string data is not modified in any way.

The case of an array value just requires a repeat of the byte count string data for as may in the array.

The case of a compressed single value adds a compressed size after the string byte count.  And a compressed byte array of the string follows.

The case of an array of compressed strings is similar to regular array but each string is individually compressed.

Chunking just breaks up the long strings into chunks before compression.

```
[n1     :7]
[n1 * 7 :string1]
[n2     :00|00|6]
[n4     :55]
[n1 * 55:...]


[n1     :7]
[n1 * 7 :string2]
[n2     :00|01|6]
[n4     :2]
[n4     :55]
[n1 * 55:...]
[n4     :55]
[n1 * 55:...]


[n1     :7]
[n1 * 7 :string3]
[n2     :01|00|6]
[n4     :555]
[n4     :67]
[n1 * 67:...]


[n1     :7]
[n1 * 7 :string4]
[n2     :01|01|6]
[n4     :2]
[n4 * 2 :75 65]
[n4     :85]
[n1 * 85:...]
```

The text case is a little more involved.

```
[key] ""- "[string data]\n


[key] ""= [array count]\n
"[string data]\n
...


[key] ""z [string byte count] [compressed size] [Base64 compressed data]\n


[key] ""Z [array count]\n
[string byte count] [compressed size] [Base64 compressed data]\n
[string byte count] [compressed size] [Base64 compressed data]\n
...


[key] ""c [string byte count] [chunk byte count]\n
[compressed size] [Base64 compressed data]\n
[compressed size] [Base64 compressed data]\n
...


[key] ""C [array count]\n1
[string byte count] [chunk byte count]\n
[compressed size] [Base64 compressed data]\n
[compressed size] [Base64 compressed data]\n
...
[string byte count] [chunk byte count]\n
[compressed size] [Base64 compressed data]\n
[compressed size] [Base64 compressed data]\n
...
```

If no compression is used then the string starts right after the " character till the end of the line but not including the new line character at the end of the line.

Multi-line strings are placed on one line.  This is done by escaping new line and cursor return characters as well as some other characters.  See below.

```
\a - bell           - 0x07
\b - backspace      - 0x08
\t - tab            - 0x09
\n - new line       - 0x0a
\v - vertical tab   - 0x0b
\f - form feed      - 0x0c
\r - cursor return  - 0x0d
\e - escape         - 0x1b
\\ - \
```

Examples:

```
string1 ""- "No separators after this character.  Otherwise those separators are part of the string.  This is a single string value.\nBut multiline.\n


string3 ""z 1024 754 ...\n


stringList1 ""= 3\n
"This is string 1, line 1.\r\nThis is string1, line 2.\r\n\n
"This is string 2.\n
"This is string 3.\n


stringList2 ""Z 3\n
53 32 ...\n
42 15 ...\n
75 34 ...\n
```

If the array flag is used, the individual strings will reside on their own line.  Meaning, as soon as the [array count] is written a \n should be present.  Each string is on it's own line.

For an array of strings, Again, the first character of the string will be the " character and then immediately followed by the string.  Any space preceeding the " is ignored.

For a compressed single string, the string is compressed first before converting to Base64.  No escape character or string modification is needed since it is treated like binary data.  A byte count of the string and a compressed size is required to follow.

For a compressed array of strings, each string is concatenated together to form one long buffer.  Then compressed.  After the [array count] there will be a string count for each string in the array followed by the compressed size of this concatenated buffer.

## 3.7 - Path Values


A string defining a relative path (relative to the MIFF file location.)  Folders are separated by a "/" only.  No "." or ".." are allowed as part of the path.

See String Values above for storage rules.  They are the same.

## 3.8 - Boolean values


```
Binary                                                     Text


[n1                        :key size]                [key] bool- [boolean value]\n
[n1 * key size       :key]
[n2                        :value header  - 00|00|10]
[n1                        :boolean value - 'T'|'F']


[n1                        :key size]                [key] bool= [array count] [boolean value] * (array count)\n
[n1 * key size       :key]
[n2                        :value header  - 00|01|10]
[n4                        :array count]
[n1 * (array count / 8)    :boolean value]


[n1                        :key size]                [key] boolZ [array count] [compressed size] [Base64 compressed data]\n
[n1 * key size       :key]
[n2                        :value header  - 01|01|10]
[n4                        :array count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] boolC [array count] [chunk byte count]\n
[n1 * key size       :key]                           [compressed size] [Base64 compressed data]\n
[n2                        :value header  - 10|01|10]       [compressed size] [Base64 compressed data]\n
[n4                        :array count]                   ...
[n4                        :chunk byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
...
```

By default, the value will be 'T' for true, or 'F' for false.

For array values in text files, the value will be a string of 'T' or 'F' letters with no separator in between to save some space.

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


[n1                        :5]                             Bool1 bool- T\n
[n1 * 5                    :Bool1]
[n2                        :00|00|10]
[n1                        :T]
                       

[n1                        :5]                             Bool2 bool= 10 TTTTTFFFFF\n
[n1 * 5                    :Bool2]
[n2                        :00|01|10]
[n4                        :array count]
[n1 * (array count / 8)    :boolean value]


[n1                        :5]                             Bool3 boolZ 1024 25 ...\n
[n1 * 5                    :Bool3]
[n2                        :01|01|10]
[n4                        :array count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
```

## 3.9 - Simple values


Simple value encoding.  Based on what is being stored the byte streams only look slighly different.

```
Binary                                                     Text


[n1                        :key size]                [key] [text type code]- [type value]\n
[n1 * key size       :key]
[n2                        :value header - 00|00|[type code]]
[[byte count]              :type value]


[n1                        :key size]                [key] [text type code]= [array count] [type value] * (array count)\n
[n1 * key size       :key]
[n2                        :value header - 00|01|[type code]]
[n4                        :array count]
[[byte count] * array count:type values]


[n1                        :key size]                [key] [text type code]z [compressed size] [compressed data]\n
[n1 * key size       :key]
[n2                        :value header - 01|00|[type code]]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] [text type code]Z [array count] [compressed size] [compressed data]\n
[n1 * key size       :key]
[n2                        :value header - 01|01|[type code]]
[n4                        :array count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] [text type code]c [chunk byte count]\n
[n1 * key size       :key]                           [compressed size] [compressed data]\n
[n2                        :value header - 10|00|[type code]][compressed size] [compressed data]\n
[n4                        :compressed size]         ...
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] [text type code]C [array count] [chunk byte count]\n
[n1 * key size       :key]                           [compressed size] [compressed data]\n
[n2                        :value header - 10|01|[type code]][compressed size] [compressed data]\n
[n4                        :array count]                   ...
[n4                        :compressed size]
[n1 * compressed size:compressed data]
```

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


The values are simply encoded as such.

```
[byte count:value]
```

Again, multi-byte values are store in big endian.

Arrays of these values are simply repeating the above as many times necessary for the array.

Chunk and Compression flags on a single value are not usually used unless they have a large byte count.

Text representation for a value will be...

| Type Code | Representation |
| --- | --- |
| i&#42;, n&#42; | 1 value in regular digits |
| r&#42; | 1 Base64 stream of the 1 value. |


If using an array flag the above is repeated as many times as there are array elements.  Each array element will be separated by at least one separator.

```
Binary                                                     Text


[n1      :4]                                               1Int i4- 1024\n
[n1 * 4  :1Int]
[n2      :00|00|14]
[i4      :1024]


[n1      :4]                                               *Nat n4= 8 1 2 4 8 16 32 64 128\n
[n1 * 4  :*Nat]
[n2      :00|01|34]
[n4      :8]
[n4      :1]
[n4      :2]
[n4      :4]
[n4      :8]
[n4      :16]
[n4      :32]
[n4      :64]
[n4      :128]


[n1      :5]                                               1Real r4- [Base64 encoded value]\n
[n1 * 5  :1Real]
[n1      :00|00|51]
[r4      :3.14159]
```

## 3.10 - Key Type


A simple key only record.  No value.  If the key exists or doesn't exist, that can be enough information for the format.

```
Binary                                                     Text


[n1                     :key size]                   [key] -\n
[n1 * key size    :key]
[n2                     :value header - 00|00|8]
```

Only - is ever used with this type.

## 3.11 - User Type


A definition of a user type.

```
Binary                                                     Text


[n1                     :key size]                   [key] usertype- [comment]\n
[n1 * key size    :key]                              [key] [value header] [array count] [comment]\n
[n2                     :value header - 00|00|9]           [key] [value header] [array count] [comment]\n
[n2                     :user type code]                   ...
[n1                     :comment byte count]               \n
[n1 * comment byte count:comment]
[n1                     :key size]
[n1 * key size    :key]
[n2                     :value header]
[n4                     :array count]*
[n1                     :comment byte count]
[n1 * comment byte count:comment]
[n1                     :key size]
[n1 * key size    :key]
[n2                     :value header]
[n4                     :array count]*
[n1                     :comment byte count]
[n1 * comment byte count:comment]
...
[n1                     :key size - 0]


[n1                     :key size]                   [key] usertype= [array count] [comment]\n
[n1 * key size    :key]                              [key] [value header] [array count] [comment]\n
[n2                     :value header - 00|01|9]           [key] [value header] [array count] [comment]\n
[n2                     :user type code]                   ...
[n4                     :array count]
[n1                     :comment byte count]
[n1 * comment byte count:comment]
[n1                     :key size]
[n1 * key size    :key]
[n2                     :value header]
[n4                     :array count]*
[n1                     :comment byte count]
[n1 * comment byte count:comment]
[n1                     :key size]
[n1 * key size    :key]
[n2                     :value header]
[n4                     :array count]*
[n1                     :comment byte count]
[n1 * comment byte count:comment]
...
```

In the binary, the [user type code] is something the writer of the MIFF defines.  There is nothing special about this value except that it should be a value between 64 and 4095.  Also, no two user types can have the same user type code.  In the text file, the key for the user type is the user type code.

The difference between the single value and array value is the inclusion of a count.  They both will permit the definition of multiple items.  In the binary case, if not using the array case we need a 0 key size terminator.

[array count] after the [value header] of a type item is optional and only required if the value header defines an array.

Compression flag is never used with this type code.

```
Binary                                                     Text


[n1     :11]                                               TypeContact usertype-\n
[n1 * 11:TypeContact]                                      NameGiven ""-\n
[n2     :00|00|9]                                          NameFamily ""-\n
[n2     :64]                                               Age n1-\n
[n1     :0]                                                EMail ""-\n
-no comment-                                               DataFlags n1= 5\n
[n1     :9]                                                \n
[n1 * 9 :NameGiven]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :10]
[n1 * 10:NameFamily]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :3]
[n1 * 3 :Age]
[n2     :00|00|31]
[n1     :0]
-no comment-
[n1     :5]
[n1 * 5 :EMail]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :9]
[n1 * 9 :DataFlags]
[n2     :00|01|31]
[n4     :5]
[n1     :0]
-no comment-
[n1     :0]


[n1     :11]                                               TypeContact usertype= 5
[n1 * 11:TypeContact]                                      NameGiven ""-\n
[n2     :00|01|9]                                          NameFamily ""-\n
[n2     :64]                                               Age n1-\n
[n4     :5]                                                EMail ""-\n
[n1     :0]                                                DataFlags n1= 5\n
-no comment-
[n1     :9]
[n1 * 9 :NameGiven]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :10]
[n1 * 10:NameFamily]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :3]
[n1 * 3 :Age]
[n2     :00|00|31]
[n1     :0]
-no comment-
[n1     :5]
[n1 * 5 :EMail]
[n2     :00|00|6]
[n1     :0]
-no comment-
[n1     :9]
[n1 * 9 :DataFlags]
[n2     :00|01|31]
[n4     :5]
[n1     :0]
-no comment-
```

If you have a type that is an array but it can be an array of any given size then instead of = use &#42;.

```
Binary                                                     Text


[n1     :5]                                                Image usertype-\n
[n1 * 5 :Image]                                            Width  n4- Width in pixels for the image.\n
[n2     :00|00|9]                                          Height n4- Height in pixels for the image.\n
[n2     :65]                                               Pixels n1* Width * Height * n1 * 3 pixel values in RGB order.\n
[n1     :5]                                                \n
[n1 * 5 :Width]
[n2     :00|00|34]
[n1     :30]
[n1 * 30:Width in pixels for the image.]
[n1     :6]
[n1 * 6 :Height]
[n2     :00|00|34]
[n1     :31]
[n1 * 31:Height in pixels for the image.]
[n1     :6]
[n1 * 3 :Pixels]
[n2     :00|10|31]
[n1     :50]
[n1 * 50:Width * Height * n1 * 3 pixel values in RGB order.]
[n1     :0]
```

In this situation Pixels is a byte array but the size of it is not known at definition.  Rules on its size may be dictated by who ever set the type.  Comments should be used to make it clear.

If you redefine a userType or reuse a user type code then the previous userType will be replaced with the new one

```
point userType-\n
 value r8= 3\n
 \n
...
point userType- This point replaces the above point\n
 east r8-\n
 north r8-\n
 elevation r8-\n
 \n
```

## 3.12 - User Data


```
Binary                                                     Text


[n1                        :key size]                [key] [user type]= [user type data]\n
[n1 * key size       :key]
[n2                        :00|00|user type code]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[*                         :user type data]


[n1                        :key size]                [key] [user type]= [array count]\n
[n1 * key size       :key]                           [user type data]\n
[n2                        :00|01|user type code]          ...
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :array count]
[*                         :user type data]
...


[n1                        :key size]                [key] [user type]z [byte count] [compressed size] [user type data]\n
[n1 * key size       :key]
[n2                        :01|00|user type code]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]


[n1                        :key size]                [key] [user type]Z [array count] [compressed size] [Base64 compressed data]\n
[n1 * key size       :key]
[n2                        :01|01|user type code]
[n1                        :user type byte count]
[n1 * user type byte count :user type]
[n4                        :array count]
[n4                        :byte count]
[n4                        :compressed size]
[n1 * compressed size:compressed data]
```

The value of a user type.  A usertype needs to be defined first.  The values that follow are in the order that the usertype defined them.  If an array of usertypes then each item is on their own line.

In text the values may add new lines.  I.E. string values have the \n as the indicator that the string ends.  Other values may simply add a separator like n4.

If compression is used then the values are collated into a buffer first before compressing.  A byte count is required to know what the uncompressed buffer size was.

```
Binary                                                     Text


[n1      :14]                                              ContactList2 TypeContact-\n
[n1 * 14 :ContactList2]                                    `Robbert\n
[n2      :00|00|64]                                        `de Groot\n
[n4      :2]                                                100\n
[n4      :7]                                               `zekaric@gmail.com\n
[n1 * 7  :Robbert]                                          1 1 2 3 5\n
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


[n1      :14]                                              ContactList2 TypeContact= 2\n
[n1 * 14 :ContactList2]                                    `Robbert\n
[n2      :01|01|64]                                        `de Groot\n
[n4      :2]                                               100\n
[n4      :7]                                               `zekaric@gmail.com\n
[n1 * 7  :Robbert]                                         1 1 2 3 5\n
[n4      :8]                                               `Sherlock\n
[n1 * 8  :de Groot]                                        `Holms\n
[n1      :100]                                             32\n
[n4      :17]                                              `221b@bakerst.uk\n
[n1 * 17 :zekaric@gmail.com]                               10 20 30 40 50\n
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


[n1      :14]                                              ContactList TypeContactz 521 95 ...\n
[n1 * 14 :ContactList1]
[n2      :01|00|64]
[n4      :521]
[n4      :95]
[n1 * 95 :...]


[n1      :14]                                              ContactList2 TypeContactZ 2 5213 954 ...\n
[n1 * 14 :ContactList2]
[n2      :01|01|64]
[n4      :2]
[n4      :5213]
[n4      :954]
[n1 * 954:...]
```
