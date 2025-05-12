#include <stdio.h>
#include <stdlib.h>

void encrypt(FILE *input_file, FILE *encrypted_file, FILE *key_file){
    int c;
    while((c=fgetc(input_file)) != EOF){
        int key = rand();
        int encrypted_c = c ^ key;

        fputc(key, key_file);
        fputc(encrypted_c, encrypted_file);
    }
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Need file\n");
    } else{
        char *input_file_name = argv[1];
        FILE *input_file = fopen(input_file_name, "r");
        FILE *encrypted_file = fopen("file.out", "w");
        FILE *key_file = fopen("key.out","w");

        encrypt(input_file, encrypted_file, key_file);

        fclose(input_file);
        fclose(encrypted_file);
        fclose(key_file);
        remove(argv[1]);
    }

        return 0;
}
