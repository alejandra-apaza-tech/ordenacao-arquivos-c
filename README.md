# Atividade 03 - Ordenação de dados em arquivos (C)

Programa em C que grava números em um arquivo de texto, lê esses dados, ordena com o algoritmo escolhido pelo usuário e salva o resultado em outro arquivo.

**Autor(a):** Nicole Alejandra

## O que o programa faz

1. Cria o arquivo `dados.txt` com números desordenados (automaticamente, se ele ainda não existir).
2. Lê os números do arquivo para um vetor.
3. Deixa o usuário escolher o algoritmo de ordenação.
4. Ordena e salva o resultado em `ordenado.txt`, sem alterar o arquivo original.
5. Permite alterar um valor do `dados.txt` e reordenar.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `atividade03.c` | Código-fonte do programa |
| `dados.txt` | Números originais, desordenados (entrada) |
| `ordenado.txt` | Números já ordenados (saída) |

## Algoritmos de ordenação

| Algoritmo | Ideia | Complexidade média |
|---|---|---|
| Bubble Sort | Compara vizinhos e troca; o maior "sobe" para o final | O(n²) |
| Insertion Sort | Encaixa cada elemento na posição certa de uma parte já ordenada | O(n²) |
| Selection Sort | Procura o menor elemento e o coloca no início | O(n²) |
| Merge Sort | Divide o vetor ao meio e junta as partes ordenadas | O(n log n) |
| Quick Sort | Usa um pivô: menores à esquerda, maiores à direita | O(n log n) |

## Como compilar e executar

É necessário ter um compilador C (por exemplo, o `gcc`).

```bash
gcc atividade03.c -o atividade03
./atividade03
```

No Windows (PowerShell), execute com `.\atividade03.exe`.

## Menu

```
1 - Criar novo dados.txt (digitar números)
2 - Consultar arquivos (dados.txt e ordenado.txt)
3 - Ordenar dados e gerar ordenado.txt
4 - Alterar um valor do dados.txt e reordenar
0 - Sair
```

Ao criar um novo arquivo (opção 1), digite um número por linha.

## Exemplo

Conteúdo do `dados.txt`:

```
42 7 1 88 3 56 21 94 12 35
```

Conteúdo do `ordenado.txt` após a ordenação:

```
1 3 7 12 21 35 42 56 88 94
```

## Conceitos utilizados

- Manipulação de arquivos: `FILE *`, `fopen`, `fprintf`, `fscanf`, `fclose` e os modos `"r"` e `"w"`
- Ponteiros e passagem por referência (`*n` e `&n`)
- Algoritmos de ordenação
- Menu interativo com `switch/case`
- Validação de entrada: se o usuário digitar letra onde se espera número, o programa pede novamente
