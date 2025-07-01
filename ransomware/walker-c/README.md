# Walker.c

## Propósito

Este foi um ransomware criado para a cadeira ES502 de segurança da informação, com o professor Sidney Lopes do 
Departamento de Eletrônica e Sistemas, na Universidade Federal de Pernambuco. E tem como propósito o estudo de 
métodos criptográficos e como eles podem ser aplicados em maliciosos como Ransomwares.

## Utilização

Ao compilar o binário, ele deve ser executado com flags, isto é "--encode" e "--decode".

Ao utilizar "ransom --encode" o programa irá codificar todos os arquivos DO CAMINHO ONDE FOI EXECUTADO, na explicação do código 
pode-se encontrar a razão por isto. 

Já utilizando "ransom --decode" o programa irá pedir uma chave e decodificar todos os arquivos do caminho.

## Explicação do código

Por agora, vamos por partes seguindo o caminho do repositório:

### Main.c

No "main" o código simplesmente checa a flag que o usuário utilizou e retorna a função adequada para ser executada.

### FileHandler

Neste arquivo, funções essenciais são implementadas, como as "walk" e "infect".

### Função "Walk"

Esta função lê todos os arquivos em árvore de definido diretório, e para cada um deles, dependendo da opção do usuário (encode ou decode), infecta ou limpa cada um dos arquivos.

### Função "Infect"

A função infect utiliza criptografia AES para infectar os arquivos que a função Walk encontrar, ao final de todos os arquivos, codifica-se a chave AES utilizada com RSA, e a chave privada apenas o atacante tem acesso.

### Função "Clean"

Esta função faz o contrário da "Infect", uma infecta, a outra limpa. Esta função pede uma chave privada de RSA e limpa o computador infectado.
