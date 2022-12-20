# Montador
Brought to you by @LeoRiether and @Tiagosf00

## Estrutura do Projeto
- Os arquivos fonte estão em `src/`, os headers, no `include/`, e por fim os testes, em `tests/`
- Existem 5 módulos principais:
    - __lexer__: lê os tokens de um arquivo (já funciona, falta tratamento de erros)
    - __preprocessor__: processa EQUs, IFs e MACROs
    - __parser__: transforma os tokens em um vetor "uniforme" de linhas da forma `[label:] <op> [arg1] [arg2]`
    - __codegen__: gera o código de máquina a partir do vetor de linhas
    - __main__: executa o pipeline de módulos
