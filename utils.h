#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR::FILE::READ: Could not open file \"%s\".\n", path);
        return NULL;
    }

    // seek to the end of the file to 
    // know how big the buffer should be
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // read the file
    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR::FILE::MEMORY: Not enough memory to read \"%s\".\n", path);
        return NULL;
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "ERROR::FILE::READ: Could not read file \"%s\".\n", path);
        return NULL;
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

#endif /* ifndef UTILS_H
#define UTILS_H */
