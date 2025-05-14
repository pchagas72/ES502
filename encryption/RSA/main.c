#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define PLAIN_TEXT_LEN 32

void handle_openssl_error() {
    ERR_print_errors_fp(stderr);
    abort();
}

int main() {
    EVP_PKEY_CTX *ctx = NULL;
    EVP_PKEY *pkey = NULL;
    unsigned char plaintext[PLAIN_TEXT_LEN] = "This_is_32_bytes_of_plaintext!!";
    unsigned char encrypted[512];
    unsigned char decrypted[PLAIN_TEXT_LEN + 1] = {0};
    size_t encrypted_len = sizeof(encrypted), decrypted_len = sizeof(decrypted);

    // 1. Generate RSA Key
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0 ||
        EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        handle_openssl_error();
    }
    EVP_PKEY_CTX_free(ctx);

    // 2. Print Private Key
    printf("\n-----BEGIN PRIVATE KEY-----\n");
    PEM_write_PrivateKey(stdout, pkey, NULL, NULL, 0, NULL, NULL);
    printf("-----END PRIVATE KEY-----\n");

    // 3. Print Public Key
    printf("\n-----BEGIN PUBLIC KEY-----\n");
    PEM_write_PUBKEY(stdout, pkey);
    printf("-----END PUBLIC KEY-----\n");

    // 4. Encrypt with public key
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0 ||
        EVP_PKEY_encrypt(ctx, NULL, &encrypted_len, plaintext, PLAIN_TEXT_LEN) <= 0 ||
        EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, plaintext, PLAIN_TEXT_LEN) <= 0) {
        handle_openssl_error();
    }
    EVP_PKEY_CTX_free(ctx);

    printf("\nEncrypted (%zu bytes):\n", encrypted_len);
    for (size_t i = 0; i < encrypted_len; i++) {
        printf("%02x", encrypted[i]);
    }
    printf("\n");

    // 5. Decrypt with private key
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0 ||
        EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0 ||
        EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) {
        handle_openssl_error();
    }
    EVP_PKEY_CTX_free(ctx);

    printf("\nDecrypted: %s\n", decrypted);

    EVP_PKEY_free(pkey);
    return 0;
}

