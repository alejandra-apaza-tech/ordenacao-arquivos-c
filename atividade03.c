/* ======================================================================
   ATIVIDADE AVALIATIVA 03 - ORDENACAO + ARQUIVOS EM C   (VERSAO 2)

   Os 5 passos do enunciado:
     1) Criar e gravar o arquivo dados.txt com numeros desordenados
     2) Ler esses numeros do arquivo para um vetor
     3) Escolher o algoritmo (Bubble, Insertion, Selection, Merge, Quick)
     4) Ordenar e salvar o resultado em ordenado.txt
     5) Alterar um valor do dados.txt e reordenar

   Melhorias desta versao:
     - Se o usuario digitar LETRA onde se espera numero, o programa
       avisa e pergunta de novo (antes ele travava).
     - O dados.txt so e criado automaticamente se NAO existir. Assim
       os dados do usuario nao sao apagados cada vez que abre.
     - Ao criar um novo dados.txt, o arquivo so e gravado DEPOIS de
       todos os numeros serem digitados (nao apaga se der erro).
     - Menu para consultar (ver) os dois arquivos.

   Conceitos usados (planos de aula):
     - Aula 08: arquivos  -> FILE *, fopen, fprintf, fscanf, fclose
     - Aula 09: ponteiros -> *, &, passagem por referencia
     - Aula 11: algoritmos de ordenacao
     - Aula 12: menu com switch/case
   ====================================================================== */

#include <stdio.h>   // printf, scanf, FILE, fopen, fprintf, fscanf, fclose
#include <stdlib.h>  // malloc, free (Merge Sort) e exit

#define MAX 100                      // maximo de numeros no vetor
#define ARQ_DADOS    "dados.txt"     // arquivo de ENTRADA (desordenado)
#define ARQ_ORDENADO "ordenado.txt"  // arquivo de SAIDA (ordenado)


/* ======================================================================
   PARTE 1 - LEITURA SEGURA DO TECLADO
   ====================================================================== */

/* Joga fora o que sobrou digitado no teclado ate o Enter.
   Serve para limpar uma letra errada que ficou "presa". */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // nao faz nada: so le e descarta
    }
}

/* Mostra uma mensagem e le um numero inteiro.
   Se o usuario digitar letra, avisa e PERGUNTA DE NOVO, ate vir
   um numero valido. Devolve o numero lido. */
int lerInteiro(const char *mensagem) {
    int valor;
    int ok;

    while (1) {
        printf("%s", mensagem);
        ok = scanf("%d", &valor);       // scanf devolve 1 se leu um numero

        if (ok == 1) {                  // deu certo
            limparBuffer();
            return valor;
        }
        if (ok == EOF) {                // teclado fechado: evita repetir pra sempre
            printf("\nEntrada encerrada.\n");
            exit(0);
        }
        printf("Entrada invalida! Digite apenas numeros inteiros.\n");
        limparBuffer();                 // descarta a letra digitada
    }
}


/* ======================================================================
   PARTE 2 - FUNCOES DE ARQUIVO (Aula 08)
   ====================================================================== */

/* Devolve 1 se o arquivo existe e 0 se nao existe.
   Tenta abrir com "r" (leitura): se abrir, existe. */
int arquivoExiste(const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}

/* Grava um vetor num arquivo, um numero por linha.
   Modo "w": cria o arquivo ou APAGA o conteudo antigo. */
void gravarVetor(const char *nome, int v[], int n) {
    int i;
    FILE *fp = fopen(nome, "w");        // fp = "apelido" do arquivo aberto

    if (fp == NULL) {                   // NULL = nao conseguiu abrir
        printf("ERRO: nao consegui gravar %s\n", nome);
        return;
    }

    for (i = 0; i < n; i++) {
        fprintf(fp, "%d\n", v[i]);      // fprintf escreve DENTRO do arquivo
    }

    fclose(fp);                         // OBRIGATORIO: fecha e salva
}

/* Cria o dados.txt com 10 numeros desordenados, mas SO se ele
   ainda nao existir (para nao apagar dados do usuario). */
void criarDadosIniciais() {
    int valores[10] = {42, 7, 19, 88, 3, 56, 21, 94, 12, 35};

    if (arquivoExiste(ARQ_DADOS)) {
        printf("Arquivo %s ja existe. Usando os dados dele.\n", ARQ_DADOS);
        return;
    }

    gravarVetor(ARQ_DADOS, valores, 10);
    printf("Arquivo %s criado com 10 numeros desordenados.\n", ARQ_DADOS);
}

/* Le os numeros do dados.txt para o vetor v (Aula 09: ponteiros).
     - v  : vetor ja e um endereco, entao a funcao altera o original
     - *n : ponteiro para o 'n' de quem chamou. A funcao escreve nele
            a quantidade lida, SEM usar return.
   Modo "r": o arquivo precisa existir. */
void lerDados(int v[], int *n) {
    FILE *fp = fopen(ARQ_DADOS, "r");
    int valor;

    *n = 0;                             // *n = "conteudo do endereco n"

    if (fp == NULL) {
        printf("ERRO: nao consegui abrir %s\n", ARQ_DADOS);
        return;
    }

    // fscanf devolve 1 quando consegue ler um numero
    while (*n < MAX && fscanf(fp, "%d", &valor) == 1) {
        v[*n] = valor;
        (*n)++;                         // parenteses: aumenta o conteudo
    }

    fclose(fp);
}

/* Mostra o vetor na tela */
void mostrarVetor(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

/* Mostra na tela o conteudo de um arquivo de numeros */
void mostrarArquivo(const char *nome) {
    FILE *fp = fopen(nome, "r");
    int valor;
    int achou = 0;

    printf("\n--- %s ---\n", nome);

    if (fp == NULL) {
        printf("[arquivo ainda nao existe]\n");
        return;
    }

    while (fscanf(fp, "%d", &valor) == 1) {
        printf("%d ", valor);
        achou = 1;
    }
    if (!achou) {
        printf("[arquivo vazio]");
    }
    printf("\n");

    fclose(fp);
}


/* ======================================================================
   PARTE 3 - OS 5 ALGORITMOS DE ORDENACAO (Aula 11)
   ====================================================================== */

/* 1) BUBBLE SORT (bolha)
   Compara vizinhos e troca se estiverem fora de ordem.
   A cada passada, o MAIOR valor "flutua" ate o final. O(n^2). */
void bubbleSort(int v[], int n) {
    int i, j, aux;
    for (i = 0; i < n - 1; i++) {            // passadas
        for (j = 0; j < n - i - 1; j++) {    // vizinhos
            if (v[j] > v[j + 1]) {           // fora de ordem?
                aux = v[j];                  // aux = "copo extra" p/ trocar
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

/* 2) INSERTION SORT (insercao)
   Como ordenar cartas na mao: pega uma e encaixa no lugar certo
   entre as que ja estao ordenadas. O(n^2). */
void insertionSort(int v[], int n) {
    int i, j, chave;
    for (i = 1; i < n; i++) {
        chave = v[i];                        // carta a encaixar
        j = i - 1;
        while (j >= 0 && v[j] > chave) {     // empurra os maiores p/ direita
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;                    // encaixa no buraco certo
    }
}

/* 3) SELECTION SORT (selecao)
   Acha o MENOR de todos e poe na primeira posicao; depois o menor
   do resto vai para a segunda, e assim por diante. O(n^2). */
void selectionSort(int v[], int n) {
    int i, j, min, aux;
    for (i = 0; i < n - 1; i++) {
        min = i;                             // supoe que o menor e o atual
        for (j = i + 1; j < n; j++) {
            if (v[j] < v[min]) {
                min = j;                     // achou um menor
            }
        }
        aux = v[i];                          // troca
        v[i] = v[min];
        v[min] = aux;
    }
}

/* 4) MERGE SORT (intercalacao) - "dividir e conquistar"
   Divide ao meio ate sobrar 1 elemento e junta as partes ja
   ordenadas. O(n log n). Usa memoria extra. */
void merge(int v[], int inicio, int meio, int fim) {
    int tam = fim - inicio + 1;
    int *aux = (int *) malloc(tam * sizeof(int));   // vetor temporario
    int i = inicio, j = meio + 1, k = 0;

    if (aux == NULL) {                       // sem memoria: nao faz nada
        printf("ERRO: sem memoria no Merge Sort\n");
        return;
    }

    while (i <= meio && j <= fim) {          // pega sempre o menor
        if (v[i] <= v[j]) aux[k++] = v[i++];
        else              aux[k++] = v[j++];
    }
    while (i <= meio) aux[k++] = v[i++];     // sobras da esquerda
    while (j <= fim)  aux[k++] = v[j++];     // sobras da direita

    for (i = inicio, k = 0; i <= fim; i++, k++) {
        v[i] = aux[k];                       // devolve ao vetor original
    }
    free(aux);                               // libera a memoria
}

void mergeSort(int v[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSort(v, inicio, meio);          // metade esquerda
        mergeSort(v, meio + 1, fim);         // metade direita
        merge(v, inicio, meio, fim);         // junta as duas
    }
}

/* 5) QUICK SORT (ordenacao rapida)
   Escolhe um PIVO (o do meio). Menores ficam a esquerda dele e
   maiores a direita; repete nas duas partes. Media O(n log n). */
void quickSort(int v[], int inicio, int fim) {
    int i = inicio, j = fim;
    int pivo = v[(inicio + fim) / 2];
    int aux;

    while (i <= j) {
        while (v[i] < pivo) i++;             // acha um maior que o pivo
        while (v[j] > pivo) j--;             // acha um menor que o pivo
        if (i <= j) {
            aux = v[i];                      // troca de lado
            v[i] = v[j];
            v[j] = aux;
            i++;
            j--;
        }
    }
    if (inicio < j) quickSort(v, inicio, j);
    if (i < fim)    quickSort(v, i, fim);
}


/* ======================================================================
   PARTE 4 - MENUS E OPCOES (Aula 12: switch/case)
   ====================================================================== */

/* Mostra o menu dos algoritmos e devolve a escolha (1 a 5).
   Repete a pergunta ate o usuario digitar um valor valido. */
int escolherAlgoritmo() {
    int op;

    printf("\n--- ESCOLHA O ALGORITMO ---\n");
    printf("1 - Bubble Sort\n");
    printf("2 - Insertion Sort\n");
    printf("3 - Selection Sort\n");
    printf("4 - Merge Sort\n");
    printf("5 - Quick Sort\n");

    do {
        op = lerInteiro("Escolha (1 a 5): ");
        if (op < 1 || op > 5) {
            printf("Opcao invalida! Use um numero de 1 a 5.\n");
        }
    } while (op < 1 || op > 5);

    return op;
}

/* Aplica no vetor o algoritmo escolhido (switch/case) */
void ordenarVetor(int v[], int n, int op) {
    switch (op) {
        case 1:
            printf("Algoritmo: BUBBLE SORT\n");
            bubbleSort(v, n);
            break;                 // break: impede de cair no proximo case
        case 2:
            printf("Algoritmo: INSERTION SORT\n");
            insertionSort(v, n);
            break;
        case 3:
            printf("Algoritmo: SELECTION SORT\n");
            selectionSort(v, n);
            break;
        case 4:
            printf("Algoritmo: MERGE SORT\n");
            mergeSort(v, 0, n - 1);
            break;
        case 5:
            printf("Algoritmo: QUICK SORT\n");
            quickSort(v, 0, n - 1);
            break;
        default:                   // qualquer outro valor
            printf("Algoritmo invalido!\n");
    }
}

/* OPCAO 1: cria um novo dados.txt com numeros digitados pelo usuario.
   Primeiro junta tudo no vetor; so no FINAL grava o arquivo.
   Assim, se algo der errado no meio, o arquivo antigo nao e apagado. */
void criarNovoArquivo() {
    int v[MAX];
    int n, i;
    char mensagem[40];

    printf("\n--- CRIAR NOVO %s ---\n", ARQ_DADOS);

    do {
        n = lerInteiro("Quantos numeros quer gravar (1 a 100)? ");
        if (n < 1 || n > MAX) {
            printf("Quantidade invalida! Use de 1 a %d.\n", MAX);
        }
    } while (n < 1 || n > MAX);

    for (i = 0; i < n; i++) {
        sprintf(mensagem, "Numero %d: ", i + 1);
        v[i] = lerInteiro(mensagem);
    }

    gravarVetor(ARQ_DADOS, v, n);
    printf("Arquivo %s gravado com %d numeros.\n", ARQ_DADOS, n);
}

/* OPCAO 2: mostra o conteudo dos dois arquivos */
void consultarArquivos() {
    mostrarArquivo(ARQ_DADOS);
    mostrarArquivo(ARQ_ORDENADO);
}

/* OPCAO 3: le dados.txt, ordena com o algoritmo escolhido e salva
   o resultado em ordenado.txt (o dados.txt NAO e alterado). */
void executarOrdenacao() {
    int v[MAX];
    int n;
    int op;

    lerDados(v, &n);                // &n = ENDERECO de n (ponteiro)

    if (n == 0) {
        printf("Nenhum dado para ordenar. Crie o %s primeiro (opcao 1).\n",
               ARQ_DADOS);
        return;
    }

    op = escolherAlgoritmo();

    printf("\nAntes  : ");
    mostrarVetor(v, n);

    ordenarVetor(v, n, op);

    printf("Depois : ");
    mostrarVetor(v, n);

    gravarVetor(ARQ_ORDENADO, v, n);
    printf("Resultado salvo em %s\n", ARQ_ORDENADO);
}

/* OPCAO 4: altera UM valor dentro do dados.txt e depois reordena */
void alterarValor() {
    int v[MAX];
    int n, i, pos, novo;

    lerDados(v, &n);
    if (n == 0) {
        printf("Nenhum dado para alterar. Crie o %s primeiro (opcao 1).\n",
               ARQ_DADOS);
        return;
    }

    printf("\nValores atuais em %s:\n", ARQ_DADOS);
    for (i = 0; i < n; i++) {
        printf("  posicao %d -> %d\n", i + 1, v[i]);
    }

    do {
        pos = lerInteiro("Qual posicao quer alterar? ");
        if (pos < 1 || pos > n) {
            printf("Posicao invalida! Use de 1 a %d.\n", n);
        }
    } while (pos < 1 || pos > n);

    novo = lerInteiro("Novo valor: ");

    v[pos - 1] = novo;              // vetor comeca em 0, por isso pos - 1
    gravarVetor(ARQ_DADOS, v, n);   // grava a alteracao no dados.txt
    printf("%s atualizado!\n", ARQ_DADOS);

    printf("\nAgora vamos reordenar com os dados novos.\n");
    executarOrdenacao();            // reordena e salva no ordenado.txt
}


/* ======================================================================
   PROGRAMA PRINCIPAL
   ====================================================================== */
int main() {
    int opcao;

    // Passo 1: cria o dados.txt (so se ainda nao existir)
    criarDadosIniciais();

    do {
        printf("\n========== MENU - ATIVIDADE 03 ==========\n");
        printf("1 - Criar novo dados.txt (digitar numeros)\n");
        printf("2 - Consultar arquivos (dados.txt e ordenado.txt)\n");
        printf("3 - Ordenar dados e gerar ordenado.txt\n");
        printf("4 - Alterar um valor do dados.txt e reordenar\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Escolha: ");

        switch (opcao) {
            case 1:
                criarNovoArquivo();
                break;
            case 2:
                consultarArquivos();
                break;
            case 3:
                executarOrdenacao();
                break;
            case 4:
                alterarValor();
                break;
            case 0:
                printf("Encerrando. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente de novo.\n");
        }
    } while (opcao != 0);           // repete ate digitar 0

    return 0;
}
