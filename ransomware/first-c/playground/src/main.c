#include <stdio.h>
#include "./encryption/encryption.h"
#include "./reduntant/redundant.h"

int main(){
    FileStruct file;
    int encoder_ID = ask_encoder();
    walk("./", &file, encoder_ID);
    return 0;
}
