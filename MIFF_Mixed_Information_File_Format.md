# M.I.F.F.: Mixed Information File Format <!-- omit in toc -->

```
Author:             Robbert de Groot
Date:               2019-05-15
Copyright:          2019, Robbert de Groot
License (Library):  MIT License.
License (Document): Creative Commons Attribution-NoDerivs.
                    CC BY-ND
                    https://creativecommons.org/licenses/by-nd:4.0
```

## Table Of Contents: <!-- omit in toc -->

- [1. M.I.F.F.](#1-miff)
  - [1.1. Discussion](#11-discussion)
  - [1.2. Goals](#12-goals)
  - [1.3. Design Decisions:](#13-design-decisions)
  - [1.4. Disclosure](#14-disclosure)
- [2. Format](#2-format)
  - [2.1. File Header](#21-file-header)
  - [2.2. Content](#22-content)
    - [2.2.1. Value](#221-value)
  - [2.3. Example](#23-example)
- [3. Notes](#3-notes)
  - [3.1. Comparing with JSON.](#31-comparing-with-json)
    - [3.1.1. File Size](#311-file-size)
    - [3.1.2. Comments](#312-comments)
  - [3.2. Comments on MIFF](#32-comments-on-miff)


# 1. M.I.F.F.

## 1.1. Discussion

**What is the purpose of M.I.F.F. (MIFF)?**

MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2. Goals

* **Simple**    The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief**     The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **Accurate**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3. Design Decisions:

**Why not XML or JSON?**

I find XML is too verbose.

JSON is much better than XML and is a reasonable alternative to XML.  I do have a few issues that I found out with JSON.  JSON format doesn't specify any limits on numbers.  Which is great.  However implementations can do what they like when it comes to numbers they can't handle.  I.E. Javascript change handle 64 bit integers and unsigned integers.  So it tends to write those values out as strings.  The JSON format has no problems but Javascript does.  JSON format doesn't have NAN, Infinity defined for any of the numbers.  So those too are written out as strings and more worryingly, there is no standard on how to write that information out.

Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I do not claim MIFF is better, it is just different.

**Why big endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was big endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another, accurately.

## 1.4. Disclosure

I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2. Format

A **MIFF File is a UTF8 text file**.

**Each line** in the text file **is a record**.  Each record is **terminated with a** [nl], '\n' **new line character**.

**A record is a collection of values**.  Each value is **separated by a** [tb], '\t', **tab** character.

**A record is** generally **a key-value pair**.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This file is meant to be a data transfer file format between software packages or a general format to store data.  It could be used as a native format if desired, just like where JSON is used, but performance will most likely not be better than something more tailor made.

Any data that is encode or stored as a binary byte sequence will be defined by the subformat of the file.

## 2.1. File Header

First two records of the file are always the header lines.

```
MIFF[tb]1[nl]
[value: Sub-Format Name text string][tb][value: Sub-Format Version integer][nl]
```

* **Line 1** - Format of the file: text string value "MIFF", tab, Version of the file: natural value "1"
* **Line 2** - Sub-Format name: text string value, tab, Sub-Format version: natural value.

The sub-format name is not limited to any characters.  Text strings are always in UTF8 format.  The only limit to the format name is that it can not be longer than 4096 bytes and it cannot contain raw [tb]s or [nl]s.  Leading spaces are trimmed.  Trailing spaces should not exist but if they do then it is part of the format name.  Spaces internal to the format name are also allowed and are significant.

I say 4096 bytes instead of characters because in UTF8, one UNICODE letter or codepoint can span multiple bytes.  So this name can not exceed this byte count.  These header lines will be encoded like other text values found in the records.  See how a record and record values are encoded.

## 2.2. Content

Each record (a line in the file) is a collection of values.  The first value can be considered a 'key value' for the record.

Each value is separated by a [tb].  The entire line ends in a [nl].  For example...

```
[value][nl]
[value][tb][value][nl]
[value][tb][value][tb][value]nl]
[value][tb][value][tb][value][tb][value][nl]
...
```

### 2.2.1. Value

The sub-format of the file should be dictating what key values follow.  This should be defined somewhere in the documentation of the sub-format.  Like Mining Information MIFF file format.

There can be 0 or more values other than the key value associated with the record.  Again depending on what the sub-format documenation dictates.  Each value will be separated by a [tb].

If there is a count in the line header other than 1, then there should be multiple copies of whatever type that is associated with the key.

Values have a simple format.

```
[header][valueData]
```

The header is a single character.

The valueData will depend on the header.

| Header | Default |Description |
| --- | --- | --- |
| ~ | | The value is null.<br /><br />Nothing else follows the header. |
| { | | A block start.<br /><br />Normally nothing else follows the header.  Used as the first value after a key value but may also follow array specifiers.  But could possibly be used to encode a more complicated data structure in which case a stop block will also be included.  But as the format author, keep things simple. |
| } | | A block stop.<br /><br />This is often used without a key value.  Meaning it is often in the key value spot of a record.  A key is not necessary as it will simply close the last block that was started.  If a key value is provided, no check to see if it matches the start block key is made. |
| [[count] | | An array specifier.<br /><br>The values following are part of an array.  [count] can be * if unknown.  Multiple array specifiers can be used to indicate nested arrays.<br /><br />The reader and writer of the format will need to validate if the item count of the values that follow are actually matching the count provided.<br /><br />(This value is not necessary if subformat is explicit about what a value for a key should be.  Can be used to make the format more obvious or allow readers to allocate space.) |
| ([count] | | A user type.<br /><br />[count] should be a number of values in the user type. <!-- or [type name] should be a simple text string for the user type.  This can follow an array specifier.--><br /><br />The reader and writer of the format will need to validate if the values that follow match the user type.<br /><br />(This value is not necessary if the subformat is explicit about what the value for a key should be.  Can be used to make the format more obvious or allow readers to allocate proper space.  <!-- It is encouraged to include a record defining [type name] before its use in the file, but again, not strictly necessary as long as it is adequately documented somewhere.-->) |
| 0 1 2 3 4 5 6 7 8 9 + -<br />+I -I +N +R -R +C -C ? | 0 |The value is a number.<br /><br />A boolean value this will be 0 (false) or 1 (true).<br /><br />An integer value will be a number between INT64_MIN to INT64_MAX.<br />**+I** is short for MAX value.<br />**-I** is short for MIN value.<br /><br />A natural value will be a number between 0 and UINT64_MAX.<br />**+N** is short for MAX value.<br /><br />A small real value  will be written simply.  E.G.: 0, 1.1, -1.5.  <br />A large real value will be written in scientific notation.  E.G.: 3.1415e128.<br />**+R** is short for DBL_MAX.<br />**-R** is short for -DBL_MAX.<br />**+C** is short for infinity.<br />**-C** is short for -infinity.<br />**?** is short for not a number. |
| `[count] | empty buffer |The value is a binary buffer.<br /><br/>[count] denotes the size of the binary buffer in bytes but it can be optional.  If the buffer is less than 4K in size this count can be omitted.  If the count is unknown at write time, [count] can be a "*" to indicate that the size of the buffer is unknown and should be obtained from reading the buffer carefully.<br />A single space follows the [count] and the binary buffer data is in hexidecimal encoding immediately following that space. |
| Any other letter than 0123456789{}\`"([+-<br />"[count] | empty string | The value is a text string.<br /><br/>If the header is a letter other than 0123456789{}\`"([+- then the header letter is part of the text and the entire text string is up to the next \t or \r.  In this case the length of the text string is limited ot 4096 bytes.<br /><br />If the string starts with 0123456789{}\`([+- then we need to use the "[count] header.  [count] gives you the size of the text string in bytes (not letters.) [count] can be omitted if the byte count of the text string is not larger than 4096.  [count] can be * to indicate that the size of the text string is unknown and should be obtained from reading the buffer carefully.<br /><br />A single space follows the [count] and the text string continues until the \t or \n.<br /><br />All [tb], [nl], and '\' characters inside the string are escaped.  Meaning a [tb] inside the string will be '\t', a [nl] inside teh string will be a '\n', and a '\' inside the string will be a '\\'.  Nothing else is escaped and should be written as is in the file. |

## 2.3. Example

In this example, the first value of the record is a key value.  The values that follow are related to this key value.

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
Strings[tb][3[tb]The quick brown fox\njumped over the lazy dog.[tb]Salut, c'est été.[tb]你好马[nl]
-
- Some binary data.
binary buffer[tb]` FEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEF[nl]
```

# 3. Notes

## 3.1. Comparing with JSON.

### 3.1.1. File Size

File size from the test program which dumps out every data type MIFF can support and also dumps out the exact same data to a similar JSON file.

| File | Size In Bytes Text | Size In Bytes Zipped |
| --- | --- | --- |
| JSON formatted to one line | 32,335 | 2,178 |
| JSON formatted for human readability | 32,550 | 2,316 |
| MIFF | 24,665 | 2,011 |

MIFF file size for simlar data does save quite a few bytes.  Even when compared to JSON and even when compared to JSON in its minimal form.  Zip compression saves over 100 bytes but since this is a made up file and not real world data I don't know if the difference will be significant in the long run.  Uncompressed, MIFF might have an argument over JSON.

### 3.1.2. Comments

Doing an apples to apples comparison with JSON, MIFF saves some bytes in the file size.  Compressed, MIFF fares slightly better but not an easy win.  We aren't talking huge differences.

**Where MIFF better than JSON?**

| Part                     | Comments | Size | Readable | Speed |
| ---                      | ---      | ---  | ---      | ---   |
| Header                   | Header and subformat header<br />MIFF has a header with versions<br />JSON does not have a header.  Better hope you know what you are reading. | + JSON | + MIFF | - |
| Keys                     | MIFF does not need keys and text strings to be enclosed in quotes and most do not need the "[count] prefix.<br />JSON encloses all keys and text strings within quotes. | + MIFF | - | - |
| Array Count              | MIFF has an array count and grouping count option.<br />JSON does not have such concept. | + JSON | + MIFF | + MIFF |
| Record Ender             | MIFF needs just a single [nl].<br />JSON will depend on formatting. | - | - | - |
| True, False, Null        | MIFF uses "1", "0", "~".<br />JSON uses "true", "false", "null" | + MIFF | - | + MIFF |
| Numbers                  | MIFF and JSON writes numbers roughtly the same | - | - | - |
| Numbers - Special Values | Infinity, -Infinity, Not a Number, Maximum, and Minimum<br />MIFF has constants for these values.<br />JSON has no standard for these values.  Implementations vary. | + MIFF | + MIFF | + MIFF |
| Array of objects         | MIFF will require you to just make duplicate key blocks however a block can have an array specifier<br />JSON has object definition built in more elegantly. | + JSON | + JSON | - |
| Allow for duplicate keys in a block/object | MIFF is intended to be read in sequence.  Duplicate use of keys is expected.<br />JSON doesn't forbid it but if you want a JSON to be read correctly by every importer then you should treat the data as if it is an actual object in memory.  So you need to make an array of objects instead.<br />I give JSON a slight advantage for readability here. | - | + JSON | - |

**Is MIFF better?**

No.

I suspect MIFF might have a slight advantage over reading and writing speed over JSON but only slightly.  Probably the difference is not anything to write about.  However MIFF format and parsing is way simpler than JSON.

The header of the file will tell you what the file contains.  JSON has no such thing other than the file name giving you some hint as to what is contained in the file.  OR you structured the JSON to have a small layer to provide a key to the overall object that the file contains.  At least with MIFF, the header tells you it is a MIFF file and what information is store in the file with the subformat header line.  To me this would be important information.  Otherwise, an importer of the data may not know if it is receiving data it is expecting.

JSON doesn't include any size information for arrays or strings.  This leaves it up to the importer to guess how large an array or string should be.  At least with MIFF you can allocate the size before reading.  Nicer for the importer.

JSON doesn't handle numbers quite right.  But this is because JavaScript doesn't handle numbers quite right.  Encoding some special values is not standard.  You can't encode all 64 bit integers or naturals.  Often, for JSON, if a number can't be represented, it is turned into a string.  This adds complication for the importer and exporter of JSON data.  This is where MIFF, I believe, does it right.  At least if all you need is a number that can fit into 64 bit representations (double, int64_t, uint64_t.)  Both formats have issues if you have numbers that are larger than 64 bit types of the C/C++/C#/and similar languages.  However MIFF can be update to handle that.

JSON does array of objects better.  I can't argue that point.  JSON is closer to a programming language in that respect so it naturally does it better.  But this also, in a way, handcuffs JSON as well.  It's not really a data file but a in memory object dump file.  It is great if the data in file matches the in memory representation.  But it is restrictive if you want to deviate from there.

MIFF, I will say, limits how you can write out the file.  It will be fairly predictable while JSON could include a lot of space and formatting or none at all.  I'm not sure which would have the upper hand here but I like MIFF's 1 line 1 record.  Which means, when comparing minimal forms, MIFF's readability is probably more human readable than JSON's minimal form.  But this is just an objective opinion.  MIFF has a advantage that finding differences might be easier for diff programs as MIFF is really only written and appears one way while JSON can vary wildly.

## 3.2. Comments on MIFF

**Why not use Base64 for binary data?**

I started with base 64.  The problem is that base64 encoding adds a complication.  And in the end, zipping up a similar file with base64 encoded binary data results in a larger zip file.  Or at least with my basic test with the test program.  It seems Hex dumping of binary data results in better data (deflate) compression.  I suspect it is because half byte hex dumping matches the raw byte data more closely and results in a lot more pattern matching; whereas the base64 encoding, which shifts bits around and causes, what appears to be, a little more 'randomness', give compression algorithms a harder time.  Compression algorithms do not like randomness.  Base64 saves in text file size but negatively affects compressed file sizes.

So I figured, why bother with the added complexity of base64.  If you want to save space, compress the file.  Text files are inefficient to begin with.
