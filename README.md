# ES502

## Livros

- Black Mass, por VX-underground, <a href="https://vx-underground.org/">site do grupo</a>

## Ransomwares

### First

O esqueleto de um ransomware, utiliza Fernet para encriptar todos os arquivos de um diretório raíz (onde o arquivo é executado) e exige uma senha para desencriptar, é simples e busca apenas encriptar arquivos.

A versão em C está em estado de testes com diferentes métodos de criptografia.

### TODO fist:
- Utilizar key como senha. [OK]
- Usar uma key conhecida pelo operador e deletar ela logo após encriptar. [OK]
- Refazer em C ou Go para gerar binário coerente. [OK]

### TODO playground:
- Adicionar playgroud com diferentes criptografias [OK]
- Adicionar Cifra de César [OK]
- Adicionar sistema de flags para o playground [OK]
- Adicionar AES [OK]
- Criar e esconder key do AES [OK]
- Criptografar chave AES com RSA [OK]
- Melhorar sistema de flags [OK]
- Adicionar --help [OK]
- Adicionar input de key para o XOR [OK]

### TODO walker-c:
Agora que o playground ficou pronto, fazer um arquivo principal com as seguintes caraterísticas:

- Utilizar novas chaves [OK]
- Criptografar com AES e RSA, escondendo RSA [OK]
- Adicionar --path
- Esconder o malware em um arquivo comum
