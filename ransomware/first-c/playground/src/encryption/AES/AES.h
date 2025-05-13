#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE    32
#define AES_IV_SIZE     16
#define AES_BLOCK_SIZE  16

void handleErrors();

int AES_encrypt(unsigned char *input, uint64_t size,
                unsigned char *key, unsigned char *iv,
                unsigned char *ciphertext);

int decrypt(uint8_t *input, uint8_t ciphertext_len,
            unsigned char *key, unsigned char *iv,
            unsigned char *plaintext);

int AES_decrypt(unsigned char *input, uint64_t size,
                unsigned char *key, unsigned char *iv,
                unsigned char *plaintext);

int AES_loadFile(const char *filename, unsigned char **buffer,
                 uint64_t *size);

void AES_writeFile(const char *filename, unsigned char *content,
                   uint64_t size);

