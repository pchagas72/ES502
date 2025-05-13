#include <stdio.h>
#include "./encryption/encryption.h"
#include "./reduntant/redundant.h"

#define XOR 1
#define CAESAR_ENC 2
#define CAESAR_DEC 3

int check_flags(char *args[]){
    if (strcmp(args[1],"--XOR") == 0){
        return XOR;
    }
    if (strcmp(args[1], "--caesar") == 0){
        if (strcmp(args[2],"encode") == 0){
            return CAESAR_ENC;
        } else if (strcmp(args[2], "decode") == 0){
            return CAESAR_DEC;
        } 
    }
    return -1;
}

int main(int argc, char *argv[]){
    FileStruct file;
    int encoder_ID = check_flags(argv);
    walk("./", &file, encoder_ID, argv);
    return 0;
}
