This file attempts to document the Rayman 2: The Great Escape file formats and engine as best possible.
Note: OpenRayman could not have been possible without the amazing szymski on GitHub (https://github.com/szymski/Rayman2Lib), who seems to have reverse engineered the encoding algorithm used in Rayman 2 and the file formats that are used within the engine. Most of this information was scrapped together from the Rayman2Lib repository.


## Progress


Some progress have been made in decoding and reading files.
Note: some of this information may not be incorrect, as i have not verified any of this information yet


## Formats


.cnt = archive
.gf (graphics file?) (TODO: rayman2lib has both gf3 and gf?) = textures
.sna = ? scripts / events / level / everything?!?! (TODO: figure out what rayman2lib is doing with seeking)
.dat = pointers to file?!? file database?!?!
.dsb = same?!? scripts?!?!
.gpt = ?!?!?!?
.bnm = probably sound files (based off of rayman2lib)


## Decoding


Some files used by the Rayman 2: The Great Escape engine are encoded via a simple encoding algorithm.


The general formula for decoding these files (as provided by Rayman2Lib) in psuedo code is

```
skip int32 (magic number)
magic = 1790299257
for each byte
	byte ^= (magic >> 8) & 255
	magic = 16807 * (magic ^ 0x75BD924) - 0x7FFFFFFF * ((magic ^ 0x75BD924u) / 0x1F31D)
```


## .cnt files


.cnt files are archive files, used for storing textures (more?).


The structure of a .cnt file in psuedo code is as follows:

```
uint32 directory_count
uint32 file_count
uint16 signature
uint8 xor_key

directories
	uint32 name_length
	char[] name

uint8 version_id

files
	uint32 directory;
	uint32 name_length;
	char[] name
	uint32 unknown (TODO)
	uint32 pointer (in file)
	uint32 size
```

## .gf files


.gf files are texture files (graphics file), in RGB24 or RGBA32 format.


The structure of a .gf file in psuedo code is as follows:

```
uint32 signature
uint32 width
uint32 height
uint8 channels
uint8 repeat_byte

for each pixel
	uint8 color
	if color equals repeat_byte
		uint8 color
		uint8 repeat_count
```
