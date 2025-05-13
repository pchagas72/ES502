#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_KEY_SIZE 32   // 256-bit key
#define AES_IV_SIZE 16    // 128-bit IV
#define AES_BLOCK_SIZE 16 // AES block size

// Function to handle errors
void handleErrors() {
    fprintf(stderr, "Error occurred!\n");
    exit(1);
}

// AES encryption function
int AES_encrypt(unsigned char *input, uint64_t size, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len, ciphertext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize encryption operation with AES-256-CBC
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    // Provide the message to be encrypted and obtain the encrypted output
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, input, (int)size)) handleErrors();
    ciphertext_len = len;

    // Finalize the encryption (handles padding)
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

// AES decryption function
int AES_decrypt(unsigned char *input, uint64_t size, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize decryption operation with AES-256-CBC
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    // Provide the message to be decrypted and obtain the decrypted output
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, input, (int)size)) handleErrors();
    plaintext_len = len;

    // Finalize the decryption (handles padding)
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

// Function to load a file into memory
int loadFile(const char *filename, unsigned char **buffer, uint64_t *size) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    rewind(fp);

    *buffer = (unsigned char *)malloc(*size);
    if (*buffer == NULL) {
        fclose(fp);
        return -1;
    }

    fread(*buffer, 1, *size, fp);
    fclose(fp);

    return 0;
}

// Function to write encrypted/decrypted data to file
void writeFile(const char *filename, unsigned char *content, uint64_t size) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return;
    }
    fwrite(content, 1, size, fp);
    fclose(fp);
}

int main() {
    unsigned char key[AES_KEY_SIZE] = {
        0x7c, 0x7d, 0x39, 0xd6, 0xac, 0xb8, 0xd9, 0x18,
        0xf4, 0xe1, 0xc8, 0x4a, 0x76, 0x8c, 0x05, 0xef,
        0x34, 0xc6, 0x4f, 0xa2, 0x76, 0x76, 0xf3, 0x9e,
        0x94, 0xf1, 0x08, 0x71, 0xb7, 0x5b, 0x7c, 0x3a
    };
    unsigned char iv[AES_IV_SIZE] = {
        0x4d, 0x7f, 0x8e, 0x3b, 0xde, 0xaf, 0x31, 0xc1,
        0x94, 0x75, 0xfe, 0x8e, 0x1a, 0x83, 0xe1, 0x7b
    };

    unsigned char *input_data, *output_data;
    uint64_t input_size, output_size;

    // Load the input file into memory
    if (loadFile("file.txt", &input_data, &input_size) != 0) {
        fprintf(stderr, "Error loading file\n");
        return 1;
    }

    output_data = (unsigned char *)malloc(input_size + AES_BLOCK_SIZE);

    // Encrypt the data
    output_size = AES_encrypt(input_data, input_size, key, iv, output_data);
    printf("Encrypted size: %llu\n", output_size);

    // Write encrypted data to file
    writeFile("file_encrypted.txt", output_data, output_size);

    // Decrypt the data
    unsigned char *decrypted_data = (unsigned char *)malloc(input_size + AES_BLOCK_SIZE);
    uint64_t decrypted_size = AES_decrypt(output_data, output_size, key, iv, decrypted_data);
    printf("Decrypted size: %llu\n", decrypted_size);

    // Write decrypted data to file
    writeFile("file_decrypted.txt", decrypted_data, decrypted_size);

    // Cleanup
    free(input_data);
    free(output_data);
    free(decrypted_data);

    return 0;
}

