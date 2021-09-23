
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
**3 - Format: Value Specifics**<br />
    3.1 - Key-Value Block Begin and End.<br />
    3.2 - Basic Values<br />
    3.3 - String values<br />
    3.4 - Variable values<br />

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

```
&gt; MIFF![image](nl.png)
&gt; 1![image](nl.png)
&gt; TXT![image](nl.png)
&gt; [Sub-Format Name string]![image](nl.png)
&gt; [Sub-Format Version string]![image](nl.png)
```

or

```
&gt; MIFF![image](nl.png)
&gt; 1![image](nl.png)
&gt; BIN![image](nl.png)
&gt; [Sub-Format Name string]![image](nl.png)
&gt; [Sub-Format Version string]![image](nl.png)
```

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
;':][.,&lt;&gt;]&#96;
a_b_c
$cost
été
This is also a valid key with internal spaces
```

#### 2.2.3.2 - Type Code


| Binary Type Code | Text Type Code | Description |
| --- | --- | --- |
| <br />**Block Types**<br /> |
| 1 | { | Key-Value block begins. |
| 0 | } | Key-Value block ends. |
| <br />**Basic Value Types**<br /> |
| 2 | type | Type value.  One of these type codes or a user type code. |
| 5 | " | String (UTF8) data, can be of any length. |
| 8 | b | Boolean value. |
| 10 11 12 13 14 15 16 17 18 19 | i1 i2 i3 i4 i8 i16 i32 i64 i128 i256 | An integer number.  Min and Max value will depend on how much the bytes can encode.  Yes I know there are no native types for some of these, like i3, but I include these byte counts because they may be useful in certain cases. |
| 20 21 22 23 24 25 26 27 28 29 | n1 n2 n3 n4 n8 n16 n32 n64 n128 n256 | A natural number.  Ranges from 0 to max value.  Max value will depend on how much the bytes can encode.  Yes I know there are no native types for a lot of these, like n3, but I include these byte counts because they may be useful in certain cases. |
| 33 34 | r4 r8 | A real value. |
| 40 41 42 43 | abi1 abi2 abi4 abi8 | A 2D point using integers. |
| 50 51 52 53 | abn1 abn2 abn4 abn8 | A 2D point using naturals. |
| 62 63 | abr4 abr8 | A 2D point using reals. |
| 70 71 72 73 | abci1 abci2 abci4 abci8 | A 3D point using integers. |
| 80 81 82 83 | abcn1 abcn2 abcn4 abcn8 | A 3D point using naturals. |
| 92 93 | abcr4 abcr8 | A 3D point using reals. |
| 100 101 102 103 | abcdi1 abcdi2 abcdi4 abcdi8 | A 4D point using integers. |
| 110 111 112 113 | abcdn1 abcdn2 abcdn4 abcdn8 | A 4D point using naturals. |
| 122 123 | abcdr4 abcdr8 | A 4D point using reals. |
| 132 133 | mat2x2r4 mat2x2r8 | A 2x2 r4 or r8 matrix |
| 142 143 | mat3x3r4 mat3x3r8 | A 3x3 r4 or r8 matrix |
| 152 153 | mat4x4r4 mat4x4r8 | A 4x4 r4 or r8 matrix |
| <br />** Variable Value Type**<br /> |
| 7 | v | Variable data follows.  Could be and combination of any Value type.  What follows should be defined somewhere in the format somewhere before this is used. |


#### 2.2.3.3 - Array Count


| Encoding Code (Binary bits(aaa)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| 000 | 1 | A single value follows.  In binary, no array count present. |
| 001 010 011 100 101 110 | [number of items] | An array of values follows.  In binary, 001 means an n1 count follows; 010 means an n2 count follows; 011 means an n4 count follows; 100 means an n8 count follows; 101 means an n16 count follows; 110 means an n32 count follows. |
| 111 | &#42; | An unknown count array of values follows or used when defining a user type.  See User types. |


#### 2.2.3.4 - Compression Flag


| Encoding Code (Binary bit(c)) | Encoding Code (Text) | Description |
| --- | --- | --- |
| 0 | - | Values are inlined raw with no compression. |
| 1 | : | Values are broken down into smaller chunks and then each chunk compressed with zlib. |


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


```
&gt; [type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)[compression flag]![image](tab.png)[value]![image](nl.png)
```

Any space found in the key that is not a ![image](tab.png) will be part of the key.  They will be significant.  Do not strip or reduce them.

There has to be one ![image](tab.png) separator between each part.

How the value appears will depend on the definition here.

To be clear...

**Invalid:** Absolutely no ![image](nl.png) anywhere before or within the key value line&#42;.  Absolutely no blank lines.  Absolutely no extra ![image](tab.png) anywhere in the format.

```
&gt; [type code]![image](tab.png)![image](tab.png)[key]![image](nl.png)[array count] [compression flag]![image](tab.png)[value]![image](nl.png)
```

What [value] will look like will depend on what is being stored for the record.  Values will be discussed lower in section 3.

## 2.4 - Binary


The Binary file byte sequence is not too different than the text file organization.  The infromation stored will not be much different.

### 2.4.1 - Key Value Format


```
&lt;n2                 :value header(aaa c tttt tttt tttt)&gt;
&lt;n1                 :key byte count&gt;
&lt;key byte count + 1 :key&gt;
&lt;n1/n2/n4/n8/n16/n32:array count (if aaa above is not 000)&gt;
&lt;n4                 :chunk byte count (if c above is 1)&gt;
&lt;&#42;                  :value&gt;
```

#### 2.4.1.1 - Key


&lt;n1:key byte count&gt; defines how long the key is in bytes.  Keys are 1 to 256 bytes and the bytes define a UTF8 string.

&lt;key byte count + 1:key&gt; defines the key of the key value pair.  The length of which was defined by &lt;key byte count&gt; above.  No null terminator is included in the key.

#### 2.4.1.2 - Value Header


&lt;n2:value header&gt; Value header is a 2 byte value.

The high three bits (aaa) defines how the array count value is stored.  See section 2.2.3.3.

The 4th high bit (c) defines is chunked compression is being used.  See section 2.2.3.4.

The remaining bits define the type of the key value.  0-63 type values are reserved, see section 2.2.3.2.  User types are values 64 - 4095.

#### 2.4.1.3 - Array Count


&lt;n1/n2/n4/n8/n16/n32:array count&gt; byte sequence is only present when storing an array of values.  It can be an n1 to n32 value in size depending on what is set in the value header for (aaa).

#### 2.4.1.4 - Chunk Byte Count


&lt;n4:chunk byte count&gt; Chunk Byte Count will only be present if compression is being used as indicated by the (c) flag in the value header.

#### 2.4.1.5 - Value


&lt;&#42;:value&gt; field will vary wildly depending on the &lt;value header&gt;.  See section 3 for how the values are represented.

#### 2.4.1.6 - Special Case


Key Value Block End will just have the value header and nothing else follows.  (aaa) will be 000, (c) will be 0, and (tttttttttttt) will be 0.

# 3 - Format: Value Specifics


## 3.1 - Key-Value Block Begin and End.


Key-Value blocks are special.  They are needed to allow nesting of key values.

In a text file, the last line of a Key-Value Block needs to be a comletely empty line with nothing on it but a \n to indicate that the block is terminated.

In a binary file you will have a key byte count of 0 to indicate the end of a Key-Value block.

**Text**

```
&gt; {![image](tab.png)[key]![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
```

**Binary**

```
&lt;n2            :value header (000 0 1)&gt;
&lt;n1            :key byte count&gt;
&lt;key byte count:key&gt;
...
&lt;n2            :value header (000 0 0)&gt;
```

A block is terminated when a value type of "}" is reached.  There is no key for this line value type.  Every begin block value type record requires an end block value type record, there must not be extras of each in the file.

Array Count and Compression flags are never used with this value type.

**Example 1:<br />Text**

```
&gt; {![image](tab.png)docInfo![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 1&gt;&lt;n1 :7&gt;&lt;7 :docInfo&gt;
...
&lt;n2 :000 0 0&gt;<br />
```

**Example 2:<br />Text**

```
&gt; {![image](tab.png)level1![image](nl.png)
&gt; {![image](tab.png)level2![image](nl.png)
&gt; {![image](tab.png)level3![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel3![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel2![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel1![image](nl.png)
&gt; ...
&gt; }![image](nl.png)<br />
```

**Binary**

```
&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level1&gt;
&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level2&gt;
&lt;n2 :000 0 1&gt;&lt;n1 :6&gt;&lt;6 :level3&gt;
...
&lt;n2 :000 0 0&gt;
&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel3&gt;
...
&lt;n2 :000 0 0&gt;
&lt;n2 :000 0 0&gt;
&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel2&gt;
...
&lt;n2 :000 0 0&gt;
&lt;n2 :000 0 0&gt;
&lt;n2 :000 0 1&gt;&lt;n1 :13&gt;&lt;13 :anotherlevel1&gt;
...
&lt;n2 :000 0 0&gt;
```

## 3.2 - Basic Values


Basic value encoding.  Based on what is being stored the byte streams only look slighly different.

**Text**

```
&gt; [text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[type value]![image](nl.png)
&gt; [text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)
&gt; [text type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)-(![image](tab.png)[type value])&#42;![image](nl.png)
&gt; [text type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)
```

**Binary**

```
&lt;n2               :value header (000 0 [type code])&gt;
&lt;n1               :key byte count&gt;
&lt;key byte count   :key&gt;
&lt;[type byte count]:[type data]&gt;


&lt;n2               :value header (000 1 [type code])&gt;
&lt;n1               :key byte count&gt;
&lt;key byte count   :key&gt;
&lt;n4               :chunk byte count&gt;
(&lt;n4                   :compressed byte count&gt;
 &lt;compressed byte count:binary compressed data&gt;)&#42;


&lt;n2               :value header (aaa 0 [type code])&gt;
&lt;n1               :key byte count&gt;
&lt;key byte count   :key&gt;
&lt;n??              :array count&gt;
&lt;[type byte count] &#42; array count:[type data]&gt;


&lt;n2               :value header (aaa 1 [type code])&gt;
&lt;n1               :key byte count&gt;
&lt;key byte count   :key&gt;
&lt;n??              :array count&gt;
&lt;n4               :chunk byte count&gt;
(&lt;n4                   :compressed byte count&gt;
 &lt;compressed byte count:binary compressed data&gt;)&#42;
```

type byte counts for the simple types will be one of...

| Type | byte Count |
| --- | --- |
| b, i1, n1 | 1 |
| type, i2, n2, abi1, abn1 | 2 |
| abci1, abcn1 | 3 |
| i4, n4, r4, abi2, abn2, abcdi1, abcdn2 | 4 |
| abci1, abcn1 | 6 |
| i8, n8, r8, abi4, abn4, abr4, abcdi2, abcdn2 | 8 |
| abci4, abcn4, abcr4 | 12 |
| i16, n16, abi8, abn8, abr8, abcdi4, abcdn4, abcdr4, mat2x2r4r | 16 |
| abci8, abcn8, abcr8 | 24 |
| i32, n32, abcdi8, abcdn8, abcdr8, mat2x2r8 | 32 |
| mat3x3r4 | 36 |
| i64, n64, mat4x4r4 | 64 |
| mat3x3r8 | 72 |
| i128, n128, mat4x4r8 | 128 |
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

```
&gt; type![image](tab.png)type1![image](tab.png)1![image](tab.png)-![image](tab.png)b![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 2&gt;&lt;n1 :5&gt;&lt;5 :type1&gt;&lt;n2 :8&gt;
```

**Example 2:<br />Text**

```
&gt; type![image](tab.png)type2![image](tab.png)2![image](tab.png)-![image](tab.png)b![image](tab.png)n4![image](nl.png)
```

**Binary**

```
&lt;n2 :001 0 5&gt;&lt;n1 :5&gt;&lt;5 :type2&gt;&lt;n1 :2&gt;&lt;n2 &#42; 2 :8 23&gt;
```

**Example 3:<br />Text**

```
type![image](tab.png)type3![image](tab.png)100000![image](tab.png):![image](tab.png)10000![image](tab.png)
  2342![image](tab.png)[Base64 stream]![image](tab.png)
  4546![image](tab.png)[Base64 stream]![image](nl.png)
```

**Binary**

```
&lt;n2 :011 1 5&gt;&lt;n1 :5&gt;&lt;5 :type3&gt;&lt;n4 :100000&gt;&lt;n4 :10000&gt;
  &lt;n4 :2342&gt;&lt;2342 :binary compressed data&gt;
  &lt;n4 :4546&gt;&lt;4546 :binary compressed data&gt;
```

**Example 4:<br />Text**

```
&gt; b![image](tab.png)Bool1![image](tab.png)1![image](tab.png)-![image](tab.png)T![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 8&gt;&lt;n1 :5&gt;&lt;5 :Bool1&gt;&lt;n1 :1&gt;
```

**Example 5:<br />Text**

```
&gt; b![image](tab.png)Bool2![image](tab.png)10![image](tab.png)-![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](nl.png)
```

**Binary**

```
&lt;n2 :001 0 8&gt;&lt;n1 :5&gt;&lt;5 :Bool5&gt;&lt;n1 :10&gt;&lt;n1 &#42; 10 :1 1 1 1 1 0 0 0 0 0&gt;
```

**Example 6:<br />Text**

```
&gt; i4![image](tab.png)1Int![image](tab.png)1![image](tab.png)-![image](tab.png)1024![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 13&gt;&lt;n1 :4&gt;&lt;4 :1Int&gt;&lt;n4 :1024&gt;
```

**Example 7:<br />Text**

```
&gt; n4![image](tab.png)&#42;Nat![image](tab.png)8![image](tab.png)-![image](tab.png)1![image](tab.png)2![image](tab.png)4![image](tab.png)8![image](tab.png)16![image](tab.png)32![image](tab.png)64![image](tab.png)128![image](nl.png)
```

**Binary**

```
&lt;n2 :001 0 23&gt;&lt;n1 :4&gt;&lt;4 :&#42;Nat&gt;&lt;n1 :8&gt;&lt;n4 &#42; 8 :1 2 4 8 16 32 64 128&gt;
```

**Example 8:<br />Text**

```
&gt; r4![image](tab.png)1Real![image](tab.png)1![image](tab.png)-![image](tab.png)[Base64 encoded value]![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 33&gt;&lt;n1 :5&gt;&lt;5 :1Real&gt;&lt;r4 :[binary r4 value]&gt;
```

**Example 9:<br />Text**

```
&gt; r8![image](tab.png)&#42;Real![image](tab.png)3![image](tab.png)-![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](nl.png)
```

**Binary**

```
&lt;n2 :001 0 34&gt;&lt;n1 :5&gt;&lt;5 :&#42;Real&gt;&lt;n1 :3&gt;&lt;n8 &#42; 3 :[binary r8 value] &#42; 3&gt;
```

## 3.3 - String values


**Text**

```
&gt; "![image](tab.png)[key]![image](tab.png)1![image](tab.png)-![image](tab.png)[string value]![image](nl.png)
&gt; "![image](tab.png)[key]![image](tab.png)1![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)
&gt; "![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)-(![image](tab.png)[string value])&#42;![image](nl.png)
&gt; "![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png):![image](tab.png)[chunk byte count](![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)
```

**Binary**

```
&lt;n2             :value header (000 0 5)&gt;
&lt;n1             :key byte count&gt;
&lt;key byte count :key&gt;
&lt;(n1)&#42;          :[string value]![image](nl.png)&gt;


&lt;n2             :value header (000 1 5)&gt;
&lt;n1             :key byte count&gt;
&lt;key byte count :key&gt;
&lt;n4             :chunk byte count&gt;
(&lt;n4                   :compressed byte count&gt;
&lt;compressed byte count:binary compressed data&gt;)&#42;


&lt;n2             :value header (aaa 0 5)&gt;
&lt;n1             :key byte count&gt;
&lt;key byte count :key&gt;
&lt;n??            :array count&gt;
&lt;(n1)&#42;          :[string value](![image](tab.png)[string value])&#42;![image](nl.png)&gt;


&lt;n2             :value header (aaa 1 5)&gt;
&lt;n1             :key byte count&gt;
&lt;key byte count :key&gt;
&lt;n??            :array count&gt;
&lt;n4             :chunk byte count&gt;
(&lt;n4                   :compressed byte count&gt;
&lt;compressed byte count:binary compressed data&gt;)&#42;
```

In both text and binary cases the string is preprocessed before storing.  All tabs and new line characters are escaped.  This way when you see an actual tab or new line in the record, these are used to separate strings in an array of strings or to terminate the record.

In a compressed byte array, the tabs in the stream will separate the strings like they do in the uncompressed text record.  The new line character will indicate the end of the string.

```
\t - tab            - 0x09
\n - new line       - 0x0a
\\ - \
```

**Example 1:<br />Text**

```
&gt; "![image](tab.png)string1![image](tab.png)1![image](tab.png)-![image](tab.png)This is line1.\nThis is line 2.![image](nl.png)
```

**Binary**

```
&lt;n2 :000 0 5&gt;&lt;n1 :7&gt;&lt;7 :string1&gt;&lt;(n1)&#42; :This is line1.\nThis is line 2.![image](nl.png)&gt;
```

**Example 2:<br />Text**

```
&gt; "![image](tab.png)string2![image](tab.png)1![image](tab.png):![image](tab.png)1000000![image](tab.png)23424![image](tab.png)[Base64 stream]![image](tab.png)45894![image](tab.png)[Base64 stream]![image](nl.png)
```

**Binary**

```
&lt;n2 :001 1 5&gt;&lt;n1 :7&gt;&lt;7 :string2&gt;&lt;n4 :1000000&gt;
&lt;n4 :23424&gt;&lt;23424 :binary compressed data&gt;
&lt;n4 :45894&gt;&lt;45894 :binary compressed data&gt;
```

**Example 3:<br />Text**

```
&gt; "![image](tab.png)stringList1![image](tab.png)3![image](tab.png)-![image](tab.png)This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)
```

**Binary**

```
&lt;n2 :001 0 5&gt;&lt;n1 :11&gt;&lt;11 :stringList1&gt;&lt;n1 :3&gt;
&lt;(n1)&#42; :This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)&gt;
```

**Example 4:<br />Text**

```
&gt; "![image](tab.png)stringList2![image](tab.png)3![image](tab.png):![image](tab.png)1000![image](tab.png)252![image](tab.png)[Base64 stream]![image](tab.png)543![image](tab.png)[Base64 stream]![image](nl.png)
```

**Binary**

```
&lt;n2 :001 1 5&gt;&lt;n1 :11&gt;&lt;11 :stringList2&gt;&lt;n1 :3&gt;&lt;n4 :1000&gt;
&lt;n4 :252&gt;&lt;252 :binary compressed data&gt;
&lt;n4 :543&gt;&lt;543 :binary compressed data&gt;
```

## 3.4 - Variable values


Variable records should be used sparingly or when using the basic types would make the format too unweildy.  The contents of the variable record should be defined somewhere in the format documentation or through other records in the file so that the file reader will be able to parse the contents properly and safely.

**Text**

```
&gt; v![image](tab.png)[key]![image](tab.png)&#42;![image](tab.png)-![image](tab.png)[tab delimited values]![image](nl.png)
&gt; v![image](tab.png)[key]![image](tab.png)&#42;![image](tab.png):![image](tab.png)[chunk byte count]![image](tab.png)(![image](tab.png)[compressed byte count]![image](tab.png)[Base64 stream])&#42;![image](nl.png)
```

**Binary**

todo.
