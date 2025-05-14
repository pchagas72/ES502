#include "./AES.h"

void handleErrors() {
    fprintf(stderr, "Error occurred!\n");
    exit(1);
}

int AES_encrypt(unsigned char *input, uint64_t size, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len, ciphertext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize encryption operation, AES 256
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    // Starts encryption
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, input, (int)size)) handleErrors();
    ciphertext_len = len;

    // Finalize the encryption, adds padding
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int AES_loadFile(const char *filename, unsigned char **buffer, uint64_t *size) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not read file %s",filename);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    *size = ftell(fp);
    rewind(fp);
	printf("File: %s size loaded\n", filename);

    *buffer = (unsigned char *)malloc(*size);
    if (*buffer == NULL) {
        fclose(fp);
        return -1;
    }

    fread(*buffer, 1, *size, fp);
	printf("File: %s buffer loaded\n", filename);
    fclose(fp);

    return 0;
}

int AES_decrypt(unsigned char *input, uint64_t size, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;
    // Does almost the exact same thing as encrypt, but in reverse

    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, input, (int)size)) handleErrors();
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

void AES_writeFile(const char *filename, unsigned char *content, uint64_t size) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return;
    }
    fwrite(content, 1, size, fp);
    fclose(fp);
}

void AES_generateKey(unsigned char *key){
    if (access("./keys/AES.key", F_OK) != 0){
        if (RAND_bytes(key,32) != 1){
            printf("Could not generate random key\n");
            free(key);
        } else{
            printf("Generated key\n");
            FILE *keyFile = fopen("./keys/AES.key", "wb");
            fputs(key, keyFile);
            fclose(keyFile);
            printf("Written keyfile\n");
        }
    } else{
        printf("Key exists, reading key\n");
        FILE *keyFile = fopen("./keys/AES.key", "rb");
        fread(key, sizeof(unsigned char *), 32, keyFile);
    }
}

