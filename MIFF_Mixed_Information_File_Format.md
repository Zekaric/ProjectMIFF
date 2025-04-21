# M.I.F.F.: Mixed Information File Format

```
Author:             Robbert de Groot
Date:               2019-05-15
Copyright:          2019, Robbert de Groot
License (Library):  MIT License.
License (Document): Creative Commons Attribution-NoDerivs.
                    CC BY-ND
                    https://creativecommons.org/licenses/by-nd:4.0
```

## Table Of Contents:

- [M.I.F.F.: Mixed Information File Format](#miff-mixed-information-file-format)
  - [Table Of Contents:](#table-of-contents)
- [1 - M.I.F.F.](#1---miff)
  - [1.1 - Discussion](#11---discussion)
  - [1.2 - Goals](#12---goals)
  - [1.3 - Design Decisions:](#13---design-decisions)
  - [1.4 - Disclosure](#14---disclosure)
- [2 - Format](#2---format)
  - [2.1 - File Header](#21---file-header)
  - [2.2 - Content](#22---content)
    - [2.2.2 - Key Section](#222---key-section)
    - [Block Start, Array Count, Grouping Count section](#block-start-array-count-grouping-count-section)
    - [2.2.3 - Value Section](#223---value-section)
  - [Examples](#examples)
- [Notes](#notes)
  - [Comparing with JSON.](#comparing-with-json)
    - [File Size](#file-size)
    - [Comments](#comments)
  - [Comments on MIFF](#comments-on-miff)


# 1 - M.I.F.F.

## 1.1 - Discussion

**What is the purpose of M.I.F.F. (MIFF)?**

MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals

* **Simple**    The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief**     The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **Accurate**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Design Decisions:

**Why not XML or JSON?**

I find XML is too verbose.

JSON is much better than XML and is a reasonable alternative to XML.  I do have a few issues that I found out with JSON.  JSON format doesn't specify any limits on numbers.  Which is great.  However implementations can do what they like when it comes to numbers they can't handle.  I.E. Javascript change handle 64 bit integers and unsigned integers.  So it tends to write those values out as strings.  The JSON format has no problems but Javascript does.  JSON format doesn't have NAN, Infinity defined for any of the numbers.  So those too are written out as strings and more worryingly, there is no standard on how to write that information out.

Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I do not claim MIFF is better, it is just different.

**Why big endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was big endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another, accurately.

## 1.4 - Disclosure

I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2 - Format

A MIFF File is a UTF8 text file.

Each line in the text file is a 'record' or piece of data.  A record is delimited by [tb], '\t', tab characters and is terminated by a UNIX [nl], '\n', new line character.  The contents of the file is a collection of key value pairs.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This file is meant to be a data transfer file format between softwared packages or a general format to store data.  It could be used as a native format if desired, just like where JSON is used.

Any data that is encode or stored as a binary byte sequence will be in big endian order.

## 2.1 - File Header

There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.  The header is 4 lines long.

```
MIFF[tb]1[nl]
[Sub-Format Name string][tb][Sub-Format Version integer][nl]
```

[nl] means a new line character.

* **Line 1** - Format of the file: "MIFF", tab, Version of the file: "1"
* **Line 2** - Sub-Format name, tab, Sub-Format version integer.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain [tb]s or [nl]s.  Leading spaces are trimmed.  Trailing spaces should not exist but if they do then it is part of the format name.  Spaces internal to the format name are also allowed and are significant.

I say 255 bytes instead of characters because in UTF8, one UNICODE character or codepoint can span multiple bytes.  So this name can not exceed this byte count.

## 2.2 - Content

Each record (a line in the file) is split into 2 to 3 sections.  The key for the line, an optional array and/or grouping count, and the value section.

Each section is separated by a [tb].  The entire line ends in a [nl].

```
[key][tb][value][nl]
-or-
[key][tb][array and/or grouping count][tb][value][nl]
```

### 2.2.2 - Key Section

Keys are any string that is up to 255 bytes long.  I say bytes because in UTF8 format, UNICODE characters can take up multiple bytes.  So as long as the key is not larger than 255 bytes, the key is valid.  The key can have spaces or any special characters you like except [tb]s and [nl]s.  Leading spaces are trimmed and are not included.  Trailing spaces should not exist and will become part of the key if they do exist.  Internal spaces within the key are part of the key.

Special key of **}** will indicate a closing of a block.

### Block Start, Array Count, Grouping Count section

Block start will look like...

```
{
```

...followed by a [nl].  The key is the name of the block.

An Array count will look like...

```
[10
```

It will start with a "[" and followed immediately by a natural number 0 or larger.  Why is 0 possible.  It might be significant for the reader to know that this is an array but it is deliberately empty.

Grouping count will look like...

```
(3
```

It will start with a "(" and followed immediately by a natrual number 2 or larger.  What is grouping needed for?  It is intended to convey how many items are per array item.  Lets say we have a user type like a point which has 3 double values.

So if we had a point list of 52 points we could see the following...

```
[52 (3
```

If both are provided then they are separated by a space.  Array specification is always first.

If the size of the array is unknown then you can provide...

```
[*
```

This will tell reader that the writer didn't know the size of the array before writing and to proceed with a potentially variable array size.

Is either strictly necessary?  No.  You can still have a key value that still dictates an array and not specified beforehand.  It just helps the reader to allocate space properly for faster reading.  Sometimes grouping may not be as 'neat' as a fixed size.  Then it will need to be determined by the format of the value by the subformat of the file.  However it is generally assumed if array or grouping is missing then their respective values are 1 but it is not a strict rule.

By default, if this section is missing, it is generally assumed that the array count and grouping count is 1 but that is not a strict rule.  Often, by what the key is referring to, the value could be a known type of specified values.  Set value section.

### 2.2.3 - Value Section

The sub-format of the file should be dictating what values follow.  This should be defined somewhere in the documentation of the sub-format.

There can be 0 or more values associated with the key.  Again depending on what the sub-format documenation dictates.  Each value will be separated by a [tb].

If there is a count in the line header other than 1, then there should be multiple copies of whatever type that is associated with the key.

Values have a simple format.

```
[header][valueData]
```

The header is a single character.

The valueData will depend on the header.

| Header | Default |Description |
| --- | --- | --- |
| ~ | | The value data is unset/null.  Nothing else follows the header. |
| 0 1 2 3 4 5 6 7 8 9 . e -<br />I i<br />N<br />R r C c ? | 0 |A number.<br />A boolean value this will be 0 (false) or 1 (true).<br />An integer value will be a number between INT_MIN to INT_MAX.  "I" is short for INT_MAX, and "i" is short for INT_MIN.<br />A natural value will be a nbumber between 0 and UINT_MAX.  "N" is short for UINT_MAX.<br />A small real value  will be written simply.  E.G.: 0, 1.1, -1.5.  <br />A large real value will be written in scientific notation.  E.G.: 3.1415e128.  "R" is short for DBL_MAX, "r" is short for -DBL_MAX, "C" is short for infinity, "c" is short for -infinity, and "?" is short for not a number. |
| "[count] | empty string |The value data will be a string.  There is no closing **"**.  [count] denotes the size of the string in bytes but it can be optional.  If the string is less than 4K in size this count can be omitted.  [count] can be a "*" to indicate that the length of the string is unknown and should be obtained from reading the string carefully.<br />A single space follows the [count] value and the string immediately follows up to the next [tb] or [nl] which ever is next.  All [tb], [nl], and '\' characters inside the string are escaped.  Meaning a [tb] inside the string will be '\t', a [nl] inside teh string will be a '\n', and a '\' inside the string will be a '\\'.  Nothing else is escaped and should be written as is in the file. |
| .[count] | empty buffer |The value data is a binary buffer.  [count] denotes the size of the binary buffer in bytes but it can be optional.  If the buffer is less than 4K in size this count can be omitted.  [count] can be a "*" to indicate that the size of the buffer is unknown and should be obtained from reading the buffer carefully.<br />A single space follows the [count] value and the binary buffer in hexidecimal encoding immediately follows. |
| [Any other letter] | | Bad format |

Each key can potentially have 0 or more values.  If more than one value present for a key, then each value is separated by a [tb] character.

## Examples

```
- A null value.
Null[tb]~[nl]
-
- A boolean true without an explicit setting.  If this was missing then isVisible is false.
isVisible[nl]
-
- Or explicitly setting the value.  True is 1, False is 0.
isVisible[tb]1[nl]
isOpen[tb]0[nl]
-
- A true and false array.
True And False[tb][2[tb]1[tb]0[nl]
-
- Some integers, reals.
Numbers[tb][7[tb]0[tb]1[tb]-1[tb]i[tb]0.[tb]3.14159[tb]R[tb]?[nl]
-
- Some strings.
Strings[tb][3[tb]" The quick brown fox\njumped over the lazy dog.[tb]" Salut, c'est été.[tb]" 你好马[nl]
-
- Some binary data.
binary buffer[tb]. FEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEF[nl]
```

# Notes

## Comparing with JSON.

### File Size

File size from the test program which dumps out every data type MIFF can support and also dumps out the exact same data to a similar JSON file.

| File | Size In Bytes Text | Size In Bytes Zipped |
| --- | --- | --- |
| JSON compressed to one line | 32,335 | 2,178 |
| JSON Miff style formatted | 32,550 | 2,316 |
| MIFF | 29,432 | 2,191 |


JSON compressed to one line removed quite a bit of formatting bytes.  When compressed this difference becomes almost negligable.

### Comments

Doing an apples to apples comparison with JSON, MIFF saves quite a few bytes in the file size.  Compressed, MIFF fares slightly better but not an easy win.  We aren't talking huge differences.

Where MIFF saves some space.

| Part                     | Comments | Size | Readable | Speed |
| ---                      | ---      | ---  | ---      | ---   |
| Header                   | MIFF has a header with versions<br />JSON does not have a header. | + JSON | + MIFF | - |
| Keys                     | MIFF does not need keys to be enclosed in quotes.<br />JSON encloses keys within quotes. | + MIFF | - | - |
| Array Count              | MIFF has an array count and grouping count.<br />JSON does not have such concept. | + JSON | + MIFF | + MIFF |
| Record Ender             | MIFF needs just a single [nl].<br />JSON will depend on formatting. | - | - | - |
| True, False, Null        | MIFF uses "1", "0", "~".<br />JSON uses "true", "false", "null" | + MIFF | - | + MIFF |
| Numbers                  | MIFF and JSON writes numbers roughtly the same | - | - | - |
| Numbers - Special Values | Infinity, -Infinity, Not a Number, Maximum and Minimum<br />MIFF has constants for these values.<br />JSON has no standard for these values.  Implementations vary. | + MIFF | + MIFF | + MIFF |
| Array of objects         | MIFF will require you to just make duplicate key blocks<br />JSON has object definition built in. | + JSON | + JSON | - |
| Allow for duplicate keys in a block/object | MIFF is intended to be read in sequence.  Duplicate use of keys is expected.<br />JSON doesn't forbid it but if you want a JSON to be read correctly by every importer then you should treat the data as if it is an object in memory.  So you need to make an array of objects instead.<br />Neither is better, just need to be aware either file's intended use. | - | - | - |

**Is MIFF better?**

No.  It will depend on how much value you place on readability and editability of the format.

I suspect MIFF will have a slight advantage over reading and writing speed over JSON because there is less processing of numbers from the text format to binary in memory representation since the text format of the number is basically binary.

The question is, how important is it to read the file and edit it manually?  Maybe for testing and debugging but in general, common everyday users are never going to.  So for the most part, whoever is actually reading the file will be a developer or similar.  Granted the real numbers will be a pain in the ass even for developers.

## Comments on MIFF

**Why not use Base64 for binary data?**

I started with base 64.  The problem is that base64 encoding adds a complication.  And in the end, zipping up a similar file with base64 encoded binary data results in a larger zip file.  Or at least with my basic test with the test program.  It seems Hex dumping of binary data results in better data (deflate) compression.  I suspect it is because half byte hex dumping matches the raw byte data more closely and results in a lot more pattern matching; whereas the base64 encoding, which shifts bits around and causes, what appears to be, a little more 'randomness', give compression algorithms a harder time.  Compression algorithms do not like randomness.  Base64 saves in text file size but negatively affects compressed file sizes.

So I figured, why bother with the added complexity of base64.  If you want to save space, compress the file.  Text files are inefficient to begin with.
