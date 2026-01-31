/**
 * This file is taken from repository:
 * https://github.com/NathanARoss/small-wasm-trimmer
 */
/**
 *  This program allows you to remove specific sections and exports from a Wasm module.
 *  It assumes that all input wasm modules have accurately reported section sizes.
**/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//decode a 32 but unsigned integer according to the LEB128 encoding
unsigned int readVaruint(FILE *in, FILE *out) {
    unsigned int result = 0;
    unsigned int shift = 0;
    
    for (;;) {
        unsigned char byte = getc(in);
        if (out) {
            fputc(byte, out);
        }
        result |= (byte & 0x7F) << shift;

        if ((byte & 0x80) == 0) {
            break;
        }

        shift += 7;
    }

    return result;
}

int writeVaruint(unsigned int value, unsigned char* out) {
    unsigned char* start = out;
    
    do {
      unsigned char byte = value & 0x7F;
      value = value / 128;

      if (value != 0) {/* more bytes to come */
        byte |= 0x80;
      }
      
      *(out++) = byte;
    } while (value != 0);

    return out - start;
  }

int main(int argc, char **argv) {
    //stdin and stdout must be files because we are processing binary data
    if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
        fprintf(stderr, "Usage: small-wasm-trimmer [OPTION]... < input.wasm > output.wasm\n");
        fprintf(stderr, "  --remove-sections CODE1 CODE2 ...\tRemove a list of sections.\n");
        fprintf(stderr, "                                   \tIf this argument is absent, section 0 is removed by default.\n");
        fprintf(stderr, "                                   \tIf no list is provided, then no sections are removed.\n");
        fprintf(stderr, "  --remove-exports NAME1 NAME2 ...\tRemove exports by a list of field names.\n");
        fprintf(stderr, "ex: small-wasm-trimmer --remove-sections 0 4 --remove-exports __heap_base __data_end  < data.wasm > out.wasm\n");
        return -1;
    }

    //remove section 0 by default
    int sectionsToIgnore = 1;
    
    int exportsToIgnoreStart = 0;
    int exportsToIgnoreEnd = 0;

    int argumentType = 0;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--remove-sections") == 0) {
            argumentType = 1;
            sectionsToIgnore = 0;
            continue;
        }
        if (strcmp(argv[i], "--remove-exports") == 0) {
            argumentType = 2;
            exportsToIgnoreStart = i + 1;
            exportsToIgnoreEnd = i + 1;
            continue;
        }

        if (argumentType == 1) {
            //any non-integer inputs will be treated as section 0 (custom)
            int section = atoi(argv[i]);
            sectionsToIgnore |= 1 << section;
        }
        if (argumentType == 2) {
            exportsToIgnoreEnd = i + 1;
        }
    }

    int header[2];
    const int elementsRead = fread(&header, sizeof(int), 2, stdin);

    if (elementsRead != 2 || header[0] != 0x6d736100 || header[1] != 0x01) {
        fputs("Input file is not valid wasm\n", stderr);
        return -1;
    }

    //write header back out
    fwrite(&header, sizeof(int), 2, stdout);

    unsigned int sizeReduction = 0;
    for (;;) {
        const char sectionCode = getc(stdin);
        if (sectionCode == EOF) {
            break;
        }

        //skip over custom sections
        if ((sectionsToIgnore & (1 << sectionCode)) != 0) {
            const unsigned int sectionSize = readVaruint(stdin, (FILE*) 0);
            fprintf(stderr, "removing section %i (%i bytes)\n", sectionCode, sectionSize + 2);
            fseek(stdin, sectionSize, SEEK_CUR);
            sizeReduction += sectionSize + 2;
        } else if (sectionCode == 7 && exportsToIgnoreStart != exportsToIgnoreEnd) {
            const unsigned int sectionSize = readVaruint(stdin, (FILE*) 0);
            unsigned char retainedData[sectionSize];
            unsigned char* writePosition = retainedData;

            const unsigned int count = readVaruint(stdin, (FILE*) 0);
            unsigned int newCount = 0;

            //copy the entries form stdin to retainedData only if the name of the field
            //is not on the --remove-exports list
            for (int i = 0; i < count; ++i) {
                const unsigned int fieldLen = readVaruint(stdin, (FILE*) 0);
                unsigned char fieldStr[fieldLen + 1];
                for (int j = 0; j < fieldLen; ++j) {
                    fieldStr[j] = getc(stdin); //assumes ASCII field names
                }
                fieldStr[fieldLen] = 0;

                int isRemoved = 0;
                for (int j = exportsToIgnoreStart; j < exportsToIgnoreEnd; ++j) {
                    if (strcmp(argv[j], fieldStr) == 0) {
                        isRemoved = 1;
                        break;
                    }
                }

                if (isRemoved == 1) {
                    fprintf(stderr, "removing export \"%s\"\n", fieldStr);
                    const unsigned char kind = getc(stdin);
                    const unsigned int index = readVaruint(stdin, (FILE*) 0);
                    continue;
                }
                ++newCount;

                writePosition += writeVaruint(fieldLen, writePosition);

                memcpy(writePosition, fieldStr, fieldLen);
                writePosition += fieldLen;

                const unsigned char kind = getc(stdin);
                *writePosition++ = kind;

                const unsigned int index = readVaruint(stdin, (FILE*) 0);
                writePosition += writeVaruint(index, writePosition);
            }

            if (newCount != 0) {
                fputc(sectionCode, stdout);

                const int countLEBLength = (32 - __builtin_clz(newCount) + 6) / 7; //ceil(log2(newCount) / 7)
                const int newSectionSize = writePosition - retainedData + countLEBLength;
                fprintf(stderr, "export section reduced by %i bytes\n", sectionSize - newSectionSize);
                sizeReduction += sectionSize - newSectionSize;

                int length = writeVaruint(newSectionSize, writePosition);
                fwrite(writePosition, sizeof(char), length, stdout);

                length = writeVaruint(newCount, writePosition);
                fwrite(writePosition, sizeof(char), length, stdout);
                
                fwrite(&retainedData, sizeof(char), writePosition - retainedData, stdout);
            } else {
                fprintf(stderr, "Exports section removed (%i bytes).\n", sectionSize);
                sizeReduction += sectionSize + 1;
            }
        } else {
            //copy the entire section body without decoding it
            fputc(sectionCode, stdout);

            const unsigned int sectionSize = readVaruint(stdin, stdout);
            // fprintf(stderr, "Section %i: %i bytes\n", sectionCode, sectionSize);

            //if copying byte-by-byte becomes a performance issue, I'll buffer this
            for (unsigned int i = 0; i < sectionSize; ++i) {
                fputc(getc(stdin), stdout);
            }
        }
    }

    fprintf(stderr, "size reduction: %i bytes\n", sizeReduction);

    return 0;
}