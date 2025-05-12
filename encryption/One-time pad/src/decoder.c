#include <stdio.h>
#include <stdlib.h>

void decrypt(FILE *encrypted_file, FILE *key_file, FILE *output_file){
    int enc_c;
    while ((enc_c = fgetc(encrypted_file)) != EOF){
        int key_c = fgetc(key_file);
        int decrypted_c = enc_c ^ key_c;

        fputc(decrypted_c, output_file);
    }
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Need input and key.\n");
    } else{
        FILE* encrypted_file = fopen(argv[1], "r");
        FILE* key_file = fopen(argv[2], "r");
        FILE* output_file = fopen("output.out", "w");

        decrypt(encrypted_file, key_file, output_file);

        fclose(encrypted_file);
        fclose(key_file);
        fclose(output_file);
        remove(argv[1]);
        remove(argv[2]);
    }

        return 0;
}
