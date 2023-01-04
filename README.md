# Trabalho de Software Básico 2022/2 - Montador

## Alunos

- Leonardo Alves Riether - 19/0032413
- Tiago de Souza Fernandes - 18/0131818

## Desenvolvimento do projeto

- Sistema Operacional: Linux (Manjaro/Ubuntu 22.04)
- Compilador: g++ 11.3.0

## Compilação

Para compilar os arquivos do projeto e gerar o executável do montador, basta utilizar o arquivo makefile no diretório principal:

```
make
``` 

## Uso

O montador pode ser executado com o seguinte comando:

```
./MONTADOR -<ops> <file>
```

A interface oferece 3 operações para o usuário:

- p: Pré-processamento de EQUs e IFs (.PRE)
- m: Pré-processamento até MACROs (.MCR)
- o: Gera o arquivo objeto (.OBJ)

É possível utilizar mais de uma operação na mesma execução:

```
./MONTADOR -pmo examples/bin.ASM
```

O comando acima executará as 3 operações na mesma chamada.

Para mais informações a respeito do funcionamento do montador:
```
./MONTADOR -h
```


## Estrutura do Projeto
- Os arquivos fonte estão em `src/`, os headers, no `include/`, e por fim os testes, em `tests/`
- Existem 5 módulos principais:
    - __lexer__: lê os tokens de um arquivo (já funciona, falta tratamento de erros)
    - __preprocessor__: processa EQUs, IFs e MACROs
    - __parser__: transforma os tokens em um vetor "uniforme" de linhas da forma `[label:] <op> [arg1] [arg2]`
    - __codegen__: gera o código de máquina a partir do vetor de linhas
    - __main__: executa o pipeline de módulos


## Como rodar os testes

Foi utilizado o framework [Catch2](https://github.com/catchorg/Catch2) para realizar testes de unidade.

Os testes podem ser executador utilizando o cmake:

```
./gen-build.sh && ./build/tests
```