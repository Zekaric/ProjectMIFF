
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
        2.2.3 - Record Composition<br />
            2.2.3.1 - Key<br />
            2.2.3.2 - Type Code<br />
            2.2.3.3 - Array Count<br />
            2.2.3.4 - Compression Flag<br />
            2.2.3.5 - Value<br />
    2.3 - Text Format<br />
        2.3.1 - Whitespace Characters<br />
        2.3.2 - Separator Characters<br />
        2.3.3 - Printable Characters<br />
        2.3.4 - Base64<br />
        2.3.5 - Real Number Representation<br />
        2.3.6 - Record Composition<br />
    2.4 - Binary<br />
        2.4.1 - Key Value Format<br />
            2.4.1.1 - Key<br />
            2.4.1.2 - Value Header<br />
            2.4.1.3 - Array Count<br />
            2.4.1.4 - Chunk Byte Count<br />
            2.4.1.5 - Value<br />
            2.4.1.6 - Special Case<br />
**3 - Fromat: Value Specifics**<br />
    3.1 - Key-Value Block Begin and End.<br />
    3.2 - Basic Values<br />
    3.3 - String values<br />
    3.4 - Define User Type<br />
    3.5 - User Data<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals



* **Simple**    The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief**     The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

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

Common to both Text and Binary formats, any byte data that is encode or stored as a binary byte sequence will be in big endian order.  In a text file the data is stored in Base64 like encoding.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.

![image](indent1.png)MIFF![image](nl.png)<br />![image](indent1.png)1![image](nl.png)<br />![image](indent1.png)TXT![image](nl.png)<br />![image](indent1.png)[Sub-Format Name string]![image](nl.png)<br />![image](indent1.png)[Sub-Format Version string]![image](nl.png)

or

![image](indent1.png)MIFF![image](nl.png)<br />![image](indent1.png)1![image](nl.png)<br />![image](indent1.png)BIN![image](nl.png)<br />![image](indent1.png)[Sub-Format Name string]![image](nl.png)<br />![image](indent1.png)[Sub-Format Version string]![image](nl.png)

![image](tab.png) means a tab character.  ![image](nl.png) means a new line character.  The &gt; is not part of the line, is it denoting the start of the line.


* **Line 1** - Format of the file.  MIFF.

* **Line 2** - Version of the file.  Version 1.

* **Line 3** - Representation of the file.  TXT for text, or BIN for binary.

* **Line 4** - Sub-Format name.  A MIFF file is potentially a container for an sub format of the file.

* **Line 5** - Sub-Format version identifier.

If the representation is Binary then right after the ![image](nl.png) of the fifth line, the binary format will begin.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain ![image](tab.png)s or ![image](nl.png)s.  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8 one character/codepoint can span multiple bytes.  So this name can not exceed this byte count.

## 2.2 - Content


Before going into the specifics of the format we will cover some things at a high level.

### 2.2.1 - Binary Syte Sequences


Everything within &ldquo;&lt;&rdquo; and &ldquo;&gt;&rdquo; is a byte sequence of a known byte count.  Each byte seqence is immediately followed by the next byte sequence.  Nothing extra is added between sequences.

A byte sequence is represented by &ldquo;&lt;&rdquo; [byte count] &ldquo;:&rdquo; [name of this byte sequence] &ldquo;&gt;&rdquo;, where byte count is a number indicating the number of bytes this sequence uses.  This byte count can be represented by a value defined in an earlier byte sequence.

### 2.2.2 - Numbers


**Natural numbers** (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

**Integer numbers** are numbers are positive and negative whole numbers.

**Real numbers**    (floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.3 - Record Composition


A record basically defines a key - value pair.  However the composition of that record is composed of a few parts.

**[key]**              will be a string that will somewhat uniquely identify the value.

**[type code]**        will be a code (BIN) or sequence of characters (TXT) that will dictate what the value will look like as well as a compression setting and array count setting.

**[array count]**      will be a number or special character to indicate the number of values of a particular type is associated with the key.

**[compression flag]** will be a setting on how the value is compressed, if at all.

**[value]**            will depend on the the other parts so it can vary in appearance.

#### 2.2.3.1 - Key


Keys are always a single string of any character in UTF8 encoding as long as none are ![image](tab.png) and ![image](nl.png).  Whitespace, leading, trailing, and internal to the key string are significant and cannot be trimmed or thinned out.  Keys are limited to being 255 bytes long.  In UTF8 that may not mean 255 letters/characters as some characters may end up requiring more than 1 byte.  I would suggest to limit the whitespace to just spaces and nothing else.  Everything else should be a printable character.  We do not need another "Whitespace Language" monster.

```
12345
;':][.,<>]'
a_b_c
$cost
été
This is also a valid key with internal spaces
```

#### 2.2.3.2 - Type Code


| Binary Type Code | Text Type Code | Description |
| --- | --- | --- |
| <br />**Block Types**<br /> |
| `1` | `{` | Key-Value block begins. |
| `0` | `}` | Key-Value block ends. |
| <br />**Basic Types**<br /> |
| `2` | `type` | Type value.  One of these type codes or a user type code. |
| `3` | `define` | A series of key-type pairs.  You can define up to 4031 new types maximum. |
| `5` | `"` | String (UTF8) data, can be of any length. |
| `8` | `b` | Boolean value. |
| `10 11 12 13 14 15 16 17 18 19` | `i1 i2 i3 i4 i8 i16 i32 i64 i128 i256` | An integer number.  Min and Max value will depend on how much the bytes can encode.  Yes I know there are no native types for some of these, like i3, but I include these byte counts because they may be useful in certain cases. |
| `20 21 22 23 24 25 26 27 28 29` | `n1 n2 n3 n4 n8 n16 n32 n64 n128 n256` | A natural number.  Ranges from 0 to max value.  Max value will depend on how much the bytes can encode.  Yes I know there are no native types for a lot of these, like n3, but I include these byte counts because they may be useful in certain cases. |
| `33 34` | `r4 r8` | A real value. |
| <br />**User Tyes**<br /> |
| `64 and higher` | `[user type name]` | Depends on what the user type defines.  A define must exist first. |


#### 2.2.3.3 - Array Count


| Encoding Code (Binary bits(aaa)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| `000` | `1` | A single value follows.  In binary, no array count present. |
| `001 010 011 100 101 110` | `[number of items]` | An array of values follows.  In binary, 001 means an n1 count follows; 010 means an n2 count follows; 011 means an n4 count follows; 100 means an n8 count follows; 101 means an n16 count follows; 110 means an n32 count follows. |
| `111` | `&#42;` | An unknown count array of values follows or used when defining a user type.  See User types. |


#### 2.2.3.4 - Compression Flag


| Encoding Code (Binary bit(c)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| `0` | `-` | Values are inlined raw with no compression. |
| `1` | `:` | Values are broken down into smaller chunks and then each chunk compressed with zlib. |


#### 2.2.3.5 - Value


See the types respective sections for details.

## 2.3 - Text Format


The text format is a UTF8 text file.

The format is only somewhat human readable, and only somewhat text editor friendly.  It was not designed to be "human friendly" but rather "debug friendly."  What I mean is that you will be able to look at the format in a text editor, if that text editor is capable of handling really large files with really long lines; and if you know this format, you will be able to debug it.  To a certain degree you should be able to debug the binary version as well since it will not be too different than the text version.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or you may not be seeing the whole file.

All lines are terminated with a single ![image](nl.png) (\n).  NOT an MSDOS/Windows Cursor Return-New Line pair (\r\n) or a MAC \n\r or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF only uses ![image](tab.png) as a separater character.  And only one ![image](tab.png) between fields.  Whitespace is not considered a separater character and will be part of a key or value if it is present.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character causes ink to be placed on the paper, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Base64


A few items are stored in Base64.  This is to add a little compression to the items in question but mainly it is to ensure binary exactness of the data when writing the values out and reading the values back in.  Unlike normal Base64 which will break the buffer into lines of a certain length, Base64 values in a MIFF are one long continuous stream of characters.  No \n or line breaks of any kind will be present.

### 2.3.5 - Real Number Representation


All real numbers are stored as a Base64 string.  No exceptions.  This is to ensure that when read back into memory from file, the number is exactly as it was when it was stored in the file.  The real value in the MIFF file will be in Big Endian order.

### 2.3.6 - Record Composition


![image](indent1.png)[type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)[compression flag]![image](tab.png)[value]![image](nl.png)

Any space found in the key that is not a ![image](tab.png) will be part of the key.  They will be significant.  Do not strip or reduce them.

There has to be one ![image](tab.png) separator between each part.

How the value appears will depend on the definition here.

To be clear...

**Invalid:** Absolutely no ![image](nl.png) anywhere before or within the key value line&#42;.  Absolutely no blank lines.  Absolutely no extra ![image](tab.png) anywhere in the format.

![image](indent1.png)[type code]![image](tab.png)![image](tab.png)[key]![image](nl.png)[array count] [compression flag]![image](tab.png)[value]![image](nl.png)

What [value] will look like will depend on what is being stored for the record.  Values will be discussed lower in section 3.

## 2.4 - Binary


The Binary file byte sequence is not too different than the text file organization.  The infromation stored will not be much different.

### 2.4.1 - Key Value Format


![image](indent1.png)&lt;n2                 :value header(aaa c tttt tttt tttt)&gt;<br />![image](indent.png)&lt;n1                 :key byte count&gt;<br />![image](indent.png)&lt;key byte count + 1 :key&gt;<br />![image](indent.png)&lt;n1/n2/n4/n8/n16/n32:array count (if aaa above is not 000)&gt;<br />![image](indent.png)&lt;n4                 :chunk byte count (if c above is 1)&gt;<br />![image](indent.png)&lt;&#42;                  :value&gt;

#### 2.4.1.1 - Key


`&lt;n1:key byte count&gt;` defines how long the key is in bytes.  Keys are 1 to 256 bytes and the bytes define a UTF8 string.

`&lt;key byte count + 1:key&gt;` defines the key of the key value pair.  The length of which was defined by `&lt;key byte count&gt;` above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


`&lt;n2:value header&gt;` Value header is a 2 byte value.

The high three bits (aaa) defines how the array count value is stored.  See section 2.2.3.3.

The 4th high bit (c) defines is chunked compression is being used.  See section 2.2.3.4.

The remaining bits define the type of the key value.  0-63 type values are reserved, see section 2.2.3.2.  User types are values 64 - 4095.

#### 2.4.1.3 - Array Count


`&lt;n1/n2/n4/n8/n16/n32:array count&gt;` byte sequence is only present when storing an array of values.  It can be an n1 to n32 value in size depending on what is set in the value header for (aaa).

#### 2.4.1.4 - Chunk Byte Count


`&lt;n4:chunk byte count&gt;` Chunk Byte Count will only be present if compression is being used as indicated by the (c) flag in the value header.

#### 2.4.1.5 - Value


`&lt;&#42;:value&gt;` field will vary wildly depending on the `&lt;value header&gt;`.  See section 3 for how the values are represented.

#### 2.4.1.6 - Special Case


Key Value Block End will just have the value header and nothing else follows.  (aaa) will be 000, (c) will be 0, and (tttttttttttt) will be 0.

# 3 - Fromat: Value Specifics


## 3.1 - Key-Value Block Begin and End.


Key-Value blocks are special.  They are needed to allow nesting of key values.

In a text file, the last line of a Key-Value Block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

In a binary file you will have a key byte count of 0 to indicate the end of a Key-Value block.

**Text**

![image](indent1.png){![image](tab.png)[key]![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2            :value header (000 0 1)&gt;<br />![image](indent.png)&lt;n1            :key byte count&gt;<br />![image](indent.png)&lt;key byte count:key&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2            :value header (000 0 0)&gt;

A block is terminated when a value type of "}" is reached.  There is no key for this line value type.  Every begin block value type record requires an end block value type record, there must not be extras of each in the file.

Array Count and Compression flags are never used with this value type.

**Example 1:<br />Text**

![image](indent1.png){![image](tab.png)docInfo![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 1&gt;&lt;n1 :7&gt;&lt;7 :docInfo&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />

**Example 2:<br />Text**

![image](indent1.png){![image](tab.png)level1![image](nl.png)<br />![image](indent1.png){![image](tab.png)level2![image](nl.png)<br />![image](indent1.png){![image](tab.png)level3![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)<br />![image](indent1.png){![image](tab.png)anotherLevel3![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)<br />![image](indent1.png)}![image](nl.png)<br />![image](indent1.png){![image](tab.png)anotherLevel2![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)<br />![image](indent1.png)}![image](nl.png)<br />![image](indent1.png){![image](tab.png)anotherLevel1![image](nl.png)<br />![image](indent1.png)...<br />![image](indent1.png)}![image](nl.png)<br />

**Binary**

![image](indent1.png)&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level1&gt;<br />![image](indent.png)&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level2&gt;<br />![image](indent.png)&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level3&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />![image](indent.png)&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel3&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />![image](indent.png)&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel2&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />![image](indent.png)&lt;n2 :000 0 0&gt;<br />![image](indent.png)&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel1&gt;<br />![image](indent.png)...<br />![image](indent.png)&lt;n2 :000 0 0&gt;

## 3.2 - Basic Values


Basic value encoding.  Based on what is being stored the byte streams only look slighly different.

**Text**

![image](indent1.png)[text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[type value]![image](nl.png)

![image](indent1.png)[text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent1.png)[text type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)-(![image](tab.png)[type value])&#42;![image](nl.png)

![image](indent1.png)[text type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2               :value header (000 0 [type code])&gt;<br />![image](indent.png)&lt;n1               :key byte count&gt;<br />![image](indent.png)&lt;key byte count   :key&gt;<br />![image](indent.png)&lt;[type byte count]:[type data]&gt;

![image](indent1.png)&lt;n2               :value header (000 1 [type code])&gt;<br />![image](indent.png)&lt;n1               :key byte count&gt;<br />![image](indent.png)&lt;key byte count   :key&gt;<br />![image](indent.png)&lt;n4               :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count:binary compressed data&gt;)&#42;

![image](indent1.png)&lt;n2               :value header (aaa 0 [type code])&gt;<br />![image](indent.png)&lt;n1               :key byte count&gt;<br />![image](indent.png)&lt;key byte count   :key&gt;<br />![image](indent.png)&lt;n??              :array count&gt;<br />![image](indent.png)&lt;[type byte count] &#42; array count:[type data]&gt;

![image](indent1.png)&lt;n2               :value header (aaa 1 [type code])&gt;<br />![image](indent.png)&lt;n1               :key byte count&gt;<br />![image](indent.png)&lt;key byte count   :key&gt;<br />![image](indent.png)&lt;n??              :array count&gt;<br />![image](indent.png)&lt;n4               :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count:binary compressed data&gt;)&#42;

type byte counts for the simple types will be one of...

| Type | byte Count |
| --- | --- |
| b, i1, n1 | 1 |
| type, i2, n2 | 2 |
| i3, n3 | 3 |
| i4, n4, r4 | 4 |
| i8, n8, r8 | 8 |
| i16, n16 | 16 |
| i32, n32 | 32 |
| i64, n64 | 64 |
| i128, n128 | 128 |
| i256, n256 | 256 |


Text representation for a value will be...

| Type Code | Text Representation | Binary Representation |
| --- | --- | --- |
| type | text string for the type. | 2 byte binary value. |
| b | 'T' or 'F'. | 1 byte per value. 1 or 0 |
| i&#42;, n&#42; | 1 value in regular digits. | Byte count of type.  Big endian order. |
| r&#42; | 1 Base64 stream of the 1 value.  Big endian order. | Byte count of type.  Big endian order. |


If using an array flag the above is repeated as many times as there are array elements.  Each array element will be separated by one ![image](tab.png) in text mode.

**Example 1:<br />Text**

![image](indent1.png)type![image](tab.png)type1![image](tab.png)1![image](tab.png)-![image](tab.png)b![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 2&gt;&lt;n1 :5&gt;&lt;5 :type1&gt;&lt;n2 :8&gt;

**Example 2:<br />Text**

![image](indent1.png)type![image](tab.png)type2![image](tab.png)2![image](tab.png)-![image](tab.png)b![image](tab.png)n4![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 5&gt;&lt;n1 :5&gt;&lt;5 :type2&gt;&lt;n1 :2&gt;&lt;n2 &#42; 2 :8 23&gt;

**Example 3:<br />Text**

![image](indent1.png)type![image](tab.png)type3![image](tab.png)100000![image](tab.png):![image](tab.png)10000![image](tab.png)<br />![image](indent.png)![image](indent.png)2342![image](tab.png)[Base64 stream]![image](tab.png)<br />![image](indent.png)![image](indent.png)4546![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :011 1 5&gt;&lt;n1 :5&gt;&lt;5 :type3&gt;&lt;n4 :100000&gt;&lt;n4 :10000&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :2342&gt;&lt;2342 :binary compressed data&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :4546&gt;&lt;4546 :binary compressed data&gt;

**Example 4:<br />Text**

![image](indent1.png)b![image](tab.png)Bool1![image](tab.png)1![image](tab.png)-![image](tab.png)T![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 8&gt;&lt;n1 :5&gt;&lt;5 :Bool1&gt;&lt;n1 :1&gt;

**Example 5:<br />Text**

![image](indent1.png)b![image](tab.png)Bool2![image](tab.png)10![image](tab.png)-![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 8&gt;&lt;n1 :5&gt;&lt;5 :Bool5&gt;&lt;n1 :10&gt;&lt;n1 &#42; 10 :1 1 1 1 1 0 0 0 0 0&gt;

**Example 6:<br />Text**

![image](indent1.png)i4![image](tab.png)1Int![image](tab.png)1![image](tab.png)-![image](tab.png)1024![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 13&gt;&lt;n1 :4&gt;&lt;4 :1Int&gt;&lt;n4 :1024&gt;

**Example 7:<br />Text**

![image](indent1.png)n4![image](tab.png)&#42;Nat![image](tab.png)8![image](tab.png)-![image](tab.png)1![image](tab.png)2![image](tab.png)4![image](tab.png)8![image](tab.png)16![image](tab.png)32![image](tab.png)64![image](tab.png)128![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 23&gt;&lt;n1 :4&gt;&lt;4 :&#42;Nat&gt;&lt;n1 :8&gt;&lt;n4 &#42; 8 :1 2 4 8 16 32 64 128&gt;

**Example 8:<br />Text**

![image](indent1.png)r4![image](tab.png)1Real![image](tab.png)1![image](tab.png)-![image](tab.png)[Base64 encoded value]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 33&gt;&lt;n1 :5&gt;&lt;5 :1Real&gt;&lt;r4 :[binary r4 value]&gt;

**Example 9:<br />Text**

![image](indent1.png)r8![image](tab.png)&#42;Real![image](tab.png)3![image](tab.png)-![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 34&gt;&lt;n1 :5&gt;&lt;5 :&#42;Real&gt;&lt;n1 :3&gt;&lt;n8 &#42; 3 :[binary r8 value] &#42; 3&gt;

## 3.3 - String values


**Text**

![image](indent1.png)"![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[string value]![image](nl.png)

![image](indent1.png)"![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent1.png)"![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)-(![image](tab.png)[string value])&#42;![image](nl.png)

![image](indent1.png)"![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2             :value header (000 0 5)&gt;<br />![image](indent.png)&lt;n1             :key byte count&gt;<br />![image](indent.png)&lt;key byte count :key&gt;<br />![image](indent.png)&lt;(n1)&#42;          :[string value]![image](nl.png)&gt;

![image](indent1.png)&lt;n2             :value header (000 1 5)&gt;<br />![image](indent.png)&lt;n1             :key byte count&gt;<br />![image](indent.png)&lt;key byte count :key&gt;<br />![image](indent.png)&lt;n4             :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count:binary compressed data&gt;)&#42;

![image](indent1.png)&lt;n2             :value header (aaa 0 5)&gt;<br />![image](indent.png)&lt;n1             :key byte count&gt;<br />![image](indent.png)&lt;key byte count :key&gt;<br />![image](indent.png)&lt;n??            :array count&gt;<br />![image](indent.png)&lt;(n1)&#42;          :[string value](![image](tab.png)[string value])&#42;![image](nl.png)&gt;

![image](indent1.png)&lt;n2             :value header (aaa 1 5)&gt;<br />![image](indent.png)&lt;n1             :key byte count&gt;<br />![image](indent.png)&lt;key byte count :key&gt;<br />![image](indent.png)&lt;n??            :array count&gt;<br />![image](indent.png)&lt;n4             :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count:binary compressed data&gt;)&#42;

In both text and binary cases the string is preprocessed before storing.  All tabs and new line characters are escaped.  This way when you see an actual tab or new line in the record, these are used to separate strings in an array of strings or to terminate the record.

In a compressed byte array, the tabs in the stream will separate the strings like they do in the uncompressed text record.  The new line character will indicate the end of the string.

```
\t - tab            - 0x09
\n - new line       - 0x0a
\\ - \
```

**Example 1:<br />Text**

![image](indent1.png)"![image](tab.png)string1![image](tab.png)1![image](tab.png)-![image](tab.png)This is line1.\nThis is line 2.![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 5&gt;&lt;n1 :7&gt;&lt;7 :string1&gt;&lt;(n1)&#42; :This is line1.\nThis is line 2.![image](nl.png)&gt;

**Example 2:<br />Text**

![image](indent1.png)"![image](tab.png)string2![image](tab.png)1![image](tab.png):![image](tab.png)1000000![image](tab.png)23424![image](tab.png)[Base64 stream]![image](tab.png)45894![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 1 5&gt;&lt;n1 :7&gt;&lt;7 :string2&gt;&lt;n4 :1000000&gt;<br />![image](indent.png)&lt;n4 :23424&gt;&lt;23424 :binary compressed data&gt;<br />![image](indent.png)&lt;n4 :45894&gt;&lt;45894 :binary compressed data&gt;

**Example 3:<br />Text**

![image](indent1.png)"![image](tab.png)stringList1![image](tab.png)3![image](tab.png)-![image](tab.png)This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 5&gt;&lt;n1 :11&gt;&lt;11 :stringList1&gt;&lt;n1 :3&gt;<br />![image](indent.png)&lt;(n1)&#42; :This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)&gt;

**Example 4:<br />Text**

![image](indent1.png)"![image](tab.png)stringList2![image](tab.png)3![image](tab.png):![image](tab.png)1000![image](tab.png)252![image](tab.png)[Base64 stream]![image](tab.png)543![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 1 5&gt;&lt;n1 :11&gt;&lt;11 :stringList2&gt;&lt;n1 :3&gt;&lt;n4 :1000&gt;<br />![image](indent.png)&lt;n4 :252&gt;&lt;252 :binary compressed data&gt;<br />![image](indent.png)&lt;n4 :543&gt;&lt;543 :binary compressed data&gt;

## 3.4 - Define User Type


**Text**

![image](indent1.png)define![image](tab.png)[key]![image](tab.png)[member count]![image](tab.png)-(![image](tab.png)[type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)[compression type])&#42;![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2             :value header (aaa 0 3)&gt;<br />![image](indent.png)&lt;n1             :key byte count&gt;<br />![image](indent.png)&lt;key byte count :key&gt;<br />![image](indent.png)&lt;n??            :array count&gt;<br />![image](indent.png)&lt;n2             :user type code&gt;<br />![image](indent.png)(&lt;n2                            :member variable value header (aaa c [type code])&gt;<br />![image](indent.png) &lt;n1                            :member variable key byte count&gt;<br />![image](indent.png) &lt;member variable key byte count:member variable key&gt;<br />![image](indent.png) &lt;n??                           :array count if (aaa) not 000 or 111&gt;<br />![image](indent.png) &lt;n4                            :chunk byte count if (c) is 1&gt;)&#42;

In the binary, the [user type code] is something the writer of the MIFF defines.  There is nothing special about this value except that it should be a value between 64 and 4095.  Also, no two user types can have the same user type code.

Compression is never used with this type code.

**Example 1:<br />Text**

![image](indent1.png)define![image](tab.png)TypeContact![image](tab.png)5![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)"![image](tab.png)NameGiven![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)"![image](tab.png)NameFamily![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n1![image](tab.png)Age![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)"![image](tab.png)EMail![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n1![image](tab.png)DataFlags![image](tab.png)5![image](tab.png)-![image](nl.png)

Note: There are no new lines or spacing for indentation.  The above would all be on a signle line where each character follows right after each other.  It is only shown this way to make it easier to see the structure.

**Binary**

![image](indent1.png)&lt;n2 :001 0  3&gt;&lt;n1 :11&gt;&lt;11 :TypeContact&gt;&lt;n1 :6&gt;&lt;n2 :64&gt;<br />![image](indent.png)&lt;n2 :000 0  5&gt;&lt;n1 : 9&gt;&lt; 9 :NameGiven&gt;<br />![image](indent.png)&lt;n2 :000 0  5&gt;&lt;n1 :10&gt;&lt;10 :NameFamily&gt;<br />![image](indent.png)&lt;n2 :000 0 20&gt;&lt;n1 : 3&gt;&lt; 3 :Age&gt;<br />![image](indent.png)&lt;n2 :000 0  5&gt;&lt;n1 : 5&gt;&lt; 5 :Email&gt;<br />![image](indent.png)&lt;n2 :001 0 20&gt;&lt;n1 : 9&gt;&lt; 9 :DataFlags&gt;&lt;n1 :5&gt;

**Example 2:<br />Text**

![image](indent1.png)define![image](tab.png)Image![image](tab.png)3![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n4![image](tab.png)Width![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n4![image](tab.png)Height![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n1![image](tab.png)Pixels![image](tab.png)&#42;![image](tab.png):![image](tab.png)1000000![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0  3&gt;&lt;n1 :5&gt;&lt;5 :Image&gt;&lt;n1 :3&gt;&lt;n2 :65&gt;<br />![image](indent.png)&lt;n2 :000 0 23&gt;&lt;n1 :5&gt;&lt;5 :Width&gt;<br />![image](indent.png)&lt;n2 :000 0 23&gt;&lt;n1 :6&gt;&lt;6 :Height&gt;<br />![image](indent.png)&lt;n2 :111 1 20&gt;&lt;n1 :6&gt;&lt;6 :Pixels&gt;&lt;n4 :1000000&gt;

**Example 3:<br />Text**

![image](indent1.png)define![image](tab.png)Binary Blob![image](tab.png)2![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n4![image](tab.png)Byte Count![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)n1![image](tab.png)Byte Data![image](tab.png)&#42;![image](tab.png):![image](tab.png)1000000![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0  3&gt;&lt;n1 :11&gt;&lt;11 :Binary Blob&gt;&lt;n1 :2&gt;&lt;n2 :66&gt;<br />![image](indent.png)&lt;n2 :000 0 23&gt;&lt;n1 :10&gt;&lt;10 :Byte Count&gt;<br />![image](indent.png)&lt;n2 :111 1 20&gt;&lt;n1 : 9&gt;&lt; 9 :Byte Data&gt;&lt;n4 :1000000&gt;

As shown in the above, "Pixels" or "Byte Data" is an array of values but this array appears to be variable in length and we do not know its byte count when we defined the type.  The byte count will be provided when the data for the type is provided.

What would happen if a type is redefined in a MIFF file like...

![image](indent1.png)define![image](tab.png)point![image](tab.png)1![image](tab.png)-![image](tab.png)r4![image](tab.png)value![image](tab.png)3![image](tab.png)-![image](nl.png)

![image](indent.png)...

![image](indent1.png)define![image](tab.png)point![image](tab.png)3![image](tab.png)-![image](tab.png)r8![image](tab.png)east![image](tab.png)1![image](tab.png)-![image](tab.png)r8![image](tab.png)north![image](tab.png)1![image](tab.png)-![image](tab.png)r8![image](tab.png)elevation![image](tab.png)1![image](tab.png)-![image](nl.png)

In both text and binary, if you redefine a user type then the previous user type will be replaced with the new one.  However this case is discouraged.

Can user types be nested?  Yes, you can add an existing user type to an existing user type.

Can user types be recursive or cyclic?  Technically this can happen but it is not allowed.  The reason being is that this will lead to an infinite loop and eventually a stack overflow or similar error.  There is code that will test for this and will stop an import or export when detected.

## 3.5 - User Data


The value of a user type.  A "define" needs to be present before it can be used.  The values that follow are in the order that the user type defined them and are similarly displayed in the form that the user type defined them.

**Text**

![image](indent1.png)[user type]![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[user type data]![image](nl.png)

![image](indent1.png)[user type]![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

![image](indent1.png)[user type]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)-(![image](tab.png)[user type data])&#42;![image](nl.png)

![image](indent1.png)[user type]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2                    :000 0 [user type code]&gt;<br />![image](indent.png)&lt;n1                    :key byte count&gt;<br />![image](indent.png)&lt;key byte count        :key&gt;[user type data]

![image](indent1.png)&lt;n2                    :000 1 [user type code]&gt;<br />![image](indent.png)&lt;n1                    :key byte count&gt;<br />![image](indent.png)&lt;key byte count        :key&gt;<br />![image](indent.png)&lt;n4                    :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count: compressed date&gt;)&#42;

![image](indent1.png)&lt;n2                    :aaa 0 [user type code]&gt;<br />![image](indent.png)&lt;n1                    :key byte count&gt;<br />![image](indent.png)&lt;key byte count        :key&gt;<br />![image](indent.png)&lt;n??                   :array count&gt;<br />![image](indent.png)[user type data]

![image](indent1.png)&lt;n2                    :aaa 1 [user type code]&gt;<br />![image](indent.png)&lt;n1                    :key byte count&gt;<br />![image](indent.png)&lt;key byte count        :key&gt;<br />![image](indent.png)&lt;n??                   :array count&gt;<br />![image](indent.png)&lt;n4                    :chunk byte count&gt;<br />![image](indent.png)(&lt;n4                   :compressed byte count&gt;<br />![image](indent.png) &lt;compressed byte count: compressed date&gt;)&#42;

If compression is used then the values are collated into a buffer first before compressing.  Taking the types from the above section that defined them...

**Example 1:<br />Text**

![image](indent1.png)TypeContact![image](tab.png)Contact1![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)Robbert![image](tab.png)de Groot![image](tab.png)100![image](tab.png)zekaric@gmail.com![image](tab.png)1![image](tab.png)1![image](tab.png)2![image](tab.png)3![image](tab.png)5![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 64&gt;&lt;n1 :8&gt;&lt;8 :Contact1&gt;<br />![image](indent.png)![image](indent.png)&lt;(n1)&#42; :Robbert![image](nl.png)&gt;&lt;(n1)&#42; :de Groot![image](nl.png)&gt;&lt;n1 :100&gt;&lt;(n1)&#42; :zekaric@gmail.com![image](nl.png)&gt;&lt;n1 &#42; 5:1 2 3 4 5&gt;

**Example 2:<br />Text**

![image](indent1.png)TypeContact![image](tab.png)Contact2![image](tab.png)1![image](tab.png):![image](tab.png)10000![image](tab.png)<br />![image](indent.png)![image](indent.png)521![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 1 64&gt;&lt;n1 :8&gt;&lt;8 :Contact1&gt;&lt;n4 :10000&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :521&gt;&lt;521 :compressed binary data&gt;

**Exmaple 3:<br />Text**

![image](indent1.png)TypeContact![image](tab.png)ContactList1![image](tab.png)2![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)Robbert![image](tab.png)de Groot![image](tab.png)100![image](tab.png)zekaric@gmail.com![image](tab.png)1![image](tab.png)1![image](tab.png)2![image](tab.png)3![image](tab.png)5![image](tab.png)<br />![image](indent.png)![image](indent.png)Sherlock![image](tab.png)Holms![image](tab.png)32![image](tab.png)221b@bakerst.uk![image](tab.png)10![image](tab.png)20![image](tab.png)30![image](tab.png)40![image](tab.png)50![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 0 64&gt;&lt;n1 :12&gt;&lt;12 :ContactList1&gt;&lt;n1 :2&gt;<br />![image](indent.png)![image](indent.png)&lt;(n1)&#42; :Robbert![image](nl.png)&gt;&lt;(n1)&#42; :de Groot![image](nl.png)&gt;&lt;n1 :100&gt;&lt;(n1)&#42; :zekaric@gmail.com![image](nl.png)&gt;&lt;n1 &#42; 5:1 2 3 4 5&gt;<br />![image](indent.png)![image](indent.png)&lt;(n1)&#42; :Sherlock![image](nl.png)&gt;&lt;(n1)&#42; :Holms![image](nl.png)&gt;&lt;n1 :32&gt;&lt;(n1)&#42; :221b@bakerst.uk![image](nl.png)&gt;&lt;n1 &#42; 5:10 20 30 40 50&gt;

**Example 4:<br />Text**

![image](indent1.png)TypeContact![image](tab.png)ContactList2![image](tab.png)2![image](tab.png):![image](tab.png)100000![image](tab.png)<br />![image](indent.png)![image](indent.png)5213![image](tab.png)[Base64 stream]![image](tab.png)<br />![image](indent.png)![image](indent.png)32784![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :001 1 64&gt;&lt;n1 :12&gt;&lt;12 :ContactList2&gt;&lt;n1 :2&gt;&lt;n4 :100000&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :5213&gt;&lt;5213 :compressed binary data&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :32784&gt;&lt;32784 :compressed binary data&gt;

**Example 5:<br />Text**

![image](indent1.png)Image![image](tab.png)Portrait![image](tab.png)1![image](tab.png)-![image](tab.png)<br />![image](indent.png)![image](indent.png)256![image](tab.png)256![image](tab.png)17899![image](tab.png)[Base64 stream]![image](nl.png)

**Binary**

![image](indent1.png)&lt;n2 :000 0 65&gt;&lt;n1 :8&gt;&lt;8 :Portrait&gt;<br />![image](indent.png)![image](indent.png)&lt;n4 :256&gt;&lt;n4 :256&gt;&lt;n4 :17899&gt;&lt;17899 :compressed binary data&gt;
