# ES502

## Livros

- Black Mass, por VX-underground, <a href="https://vx-underground.org/">site do grupo</a>

## Ransomwares

### First

O esqueleto de um ransomware, utiliza Fernet para encriptar todos os arquivos de um diretório raíz (onde o arquivo é executado) e exige uma senha para desencriptar, é simples e busca apenas encriptar arquivos.

### TODO:
- Utilizar key como senha.
- Usar uma key conhecida pelo operador e deletar ela logo após encriptar. 
- Adicionar UI.
- Refazer em C ou Go para gerar binário coerente.
