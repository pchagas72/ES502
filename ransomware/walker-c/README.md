
# Documentação Técnica – Walker.c

## Propósito

O *Walker.c* é um ransomware desenvolvido como parte da disciplina **ES502 - Segurança da Informação**, ministrada pelo professor **Sidney Lopes**, do Departamento de Eletrônica e Sistemas da **Universidade Federal de Pernambuco (UFPE)**. Seu objetivo é proporcionar uma experiência prática no uso de técnicas criptográficas, demonstrando como elas podem ser aplicadas tanto para segurança quanto para fins ofensivos, como no funcionamento de ransomwares.

---

## Utilização

Após a compilação, o binário pode ser executado com os seguintes parâmetros:

```bash
./walker --encode     # Criptografa arquivos do diretório atual
./walker --decode     # Descriptografa os arquivos
./walker --help       # Exibe a ajuda
```

As versões curtas também são suportadas:

```bash
./walker -e
./walker -d
./walker -h
```

> ⚠️ O programa opera **apenas no diretório atual**, ou seja, todos os arquivos presentes no diretório de execução serão afetados. Isso foi uma decisão intencional e está justificada no código.

---

## Estrutura de Diretórios

A organização dos arquivos do projeto segue um padrão modular, separando as responsabilidades por pasta. Abaixo está um resumo da estrutura, baseada em `find .`:

```
.
├── Makefile                   # Script de compilação
├── bin/
│   └── ransom                 # Binário compilado
├── system/
│   └── file.txt               # Arquivo de exemplo para teste
├── src/
│   ├── main.c                 # Ponto de entrada do programa
│   ├── ransom.key             # Chave RSA privada (exemplo)
│   ├── keys/
│   │   ├── AES.key            # Chave AES temporária
│   │   └── public_key.key     # Chave pública RSA
│   ├── FileHandler/
│   │   ├── FileHandler.c      # Manipulação de arquivos
│   │   └── FileHandler.h
│   └── encryption/
│       ├── encryption.h       # Header central de criptografia
│       ├── AES/
│       │   ├── AES.c          # Implementação de AES
│       │   └── AES.h
│       └── RSA/
│           ├── RSA.c          # Implementação de RSA
│           └── RSA.h
```

Essa estrutura permite isolar as funções relacionadas à criptografia (`AES`, `RSA`) e ao controle de arquivos (`FileHandler`) de forma organizada. O diretório `bin/` contém o executável final, e `system/` pode ser usado como ambiente de teste.

---

## Estrutura do Código

### `main.c`

Este arquivo é o ponto de entrada do programa. Ele interpreta os argumentos de linha de comando utilizando `getopt_long` e direciona o fluxo para as funções apropriadas.

- `--encode` ou `-e`: inicia a criptografia dos arquivos.
- `--decode` ou `-d`: solicita uma chave privada e inicia a descriptografia.
- `--help` ou `-h`: exibe uma mensagem de uso.

Dependendo da flag escolhida, o programa chama a função `walk`, que percorre os arquivos e aplica a operação desejada. A criptografia ou descriptografia das chaves AES é feita após o processo de arquivo, via `RSA_encrypt` ou `RSA_decrypt`.

---

### `FileHandler.c`

Este módulo agrupa as funções responsáveis pela manipulação dos arquivos no sistema de diretórios.

#### Função `walk`

Percorre recursivamente todos os arquivos do diretório onde o programa foi executado. Para cada arquivo encontrado, chama a função adequada: `infect` (criptografar) ou `clean` (descriptografar), dependendo da operação selecionada.

#### Função `infect`

Criptografa um arquivo usando AES e, ao final, prepara a chave AES para ser criptografada com RSA. Cada arquivo é sobrescrito com sua versão criptografada.

#### Função `clean`

Faz o processo inverso da `infect`. Recebe a chave privada RSA, descriptografa a chave AES previamente salva e utiliza-a para restaurar os arquivos criptografados ao seu estado original.

---

### `AES.c`

Este arquivo contém as rotinas de criptografia e descriptografia com **AES (Advanced Encryption Standard)**.

#### Função `encryptFile`

Criptografa o conteúdo de um arquivo usando AES no modo CBC. Um IV (vetor de inicialização) aleatório é utilizado para aumentar a segurança. O resultado substitui o conteúdo original do arquivo.

#### Função `decryptFile`

Descriptografa o conteúdo de um arquivo criptografado com AES, restaurando seu conteúdo original. A operação depende da chave AES correta e do IV usado na criptografia.

Essas funções são invocadas pelas funções `infect` e `clean` no `FileHandler.c`.

---

### `RSA.c`

Implementa a criptografia assimétrica **RSA**, usada exclusivamente para proteger a chave AES.

#### Função `RSA_encrypt`

Criptografa a chave AES com uma chave pública RSA. Essa chave criptografada pode ser armazenada com segurança ou transmitida ao atacante.

#### Função `RSA_decrypt`

Recebe uma chave privada RSA e a utiliza para descriptografar a chave AES. Essa operação é crítica para permitir que os arquivos criptografados possam ser recuperados no modo `--decode`.

---

## Considerações Finais

Este projeto foi construído com propósitos exclusivamente educacionais, para demonstrar o funcionamento interno de ransomwares e a aplicação de técnicas criptográficas modernas. Qualquer uso indevido deste código pode configurar crime, conforme previsto na legislação brasileira. O autor não se responsabiliza por usos fora do contexto acadêmico.

---
