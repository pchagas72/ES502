#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include "../encryption/encryption.h"

struct fileStruct {
	uint8_t *buffer;
	uint64_t size;
};
typedef struct fileStruct FileStruct;

void walk(const char *basePath, FileStruct *file, int encoder_ID, char *args[]);
int loadFile(const char *name, FileStruct *file);
void writeFile(const char *name, uint8_t *content, uint64_t size);
void handleFile(char *fileName, FileStruct *file, int encoder_ID, char *args[]);
int checkFile(const char *name);

