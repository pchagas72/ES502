#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdint.h>

struct fileStruct {
	uint8_t *buffer;
	uint64_t size;
};
typedef struct fileStruct FileStruct;

void handleFile(char *fileName);
void writeFile(const char *name, uint8_t *content, uint64_t size);
int readFile(const char *name);
void walk(const char *basePath);
int check(const char *name);
