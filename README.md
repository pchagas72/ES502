# ES502

## Livros

- Black Mass, por VX-underground, <a href="https://vx-underground.org/">site do grupo</a>

## Ransomwares

### First

O esqueleto de um ransomware, utiliza Fernet para encriptar todos os arquivos de um diretório raíz (onde o arquivo é executado) e exige uma senha para desencriptar, é simples e busca apenas encriptar arquivos.

A versão em C está em estado de testes com diferentes métodos de criptografia.

### TODO python:
- Utilizar key como senha. [OK]
- Usar uma key conhecida pelo operador e deletar ela logo após encriptar. [OK]
- Adicionar UI. 
- Refazer em C ou Go para gerar binário coerente. [OK]

### TODO C:
- Adicionar plarygroud com diferentes criptografias [OK]
- Adicionar mais métodos de criptografias
- Cifra de César [OK]
- RSA
- Adicionar sistema de flags para o playground [OK]
- Melhorar sistema de flags
