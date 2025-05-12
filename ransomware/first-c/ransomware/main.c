#include <stdio.h>
#include "./encryption/encryption.h"
#include "./reduntant/redundant.h"

int main(){
    FileStruct file;
    walk("./", &file);
    return 0;
}
