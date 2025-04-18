#ifndef LIBPAK_H
#define LIBPAK_H

#include <stdio.h>
#include <libdeflate.h>

typedef struct {
    uint8_t nameHash[20]; // sha1 takes 160 bits
    uint64_t offset; // number of bytes before the data of this element in the pak file
    uint64_t compressedSize;
    uint64_t decompressedSize;
} PakElementHeader;

typedef struct {
    void* data;
    size_t dataSize;
} PakElementData;

typedef struct {
    FILE* file;
    struct libdeflate_compressor* compressor;
    PakElementHeader* header;
    size_t headerSize;
    size_t headerCount;
    void* compressedDataPool;
    size_t compressedDataPoolSize;
} PakCompressor;

typedef struct {
    FILE* file;
    struct libdeflate_decompressor* decompressor;
    PakElementHeader* header;
    size_t headerCount;
    void* compressedDataPool;
    size_t compressedDataPoolSize;
} PakReader;

#define PAK_MEMORY_CHUNK_SIZE 8192
#define PAK_ELEMENT_CHUNK_SIZE 128

// supported compression levels range is [0; 12]
PakCompressor pakCompressorInit(char* pakFilePath, uint8_t compressionLevel);
void pakCompressorAddData(PakCompressor* compressor, char* name, void* data, size_t size);
void pakCompressorAddFile(PakCompressor* compressor, char* path);
void pakCompressorFinish(PakCompressor* compressor);

void pakDecompress(char* pakFilePath);

PakReader pakReaderInit(char* pakFilePath);
// it is callee's responsibility to free the element data it got
PakElementData pakReaderRead(PakReader* reader, char* name);
void pakReaderFree(PakReader* reader);

#endif
