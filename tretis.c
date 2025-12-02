#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Struct representando uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Gera uma nova peça aleatória
Peca gerarPeca(int *contadorID) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = (*contadorID)++;
    return nova;
}

// Inicializa a fila
void inicializarFila(Fila *f, int *contadorID) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->itens[i] = gerarPeca(contadorID);
        f->tras = (f->tras + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Inicializa pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Enfileira na fila circular
void enfileirar(Fila *f, Peca p) {
    if (f->quantidade < TAM_FILA) {
        f->itens[f->tras] = p;
        f->tras = (f->tras + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Remove da fila
Peca desenfileirar(Fila *f) {
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// Empilha
int empilhar(Pilha *p, Peca pec) {
    if (p->topo == TAM_PILHA - 1) return 0;
    p->itens[++p->topo] = pec;
    return 1;
}

// Desempilha
int desempilhar(Pilha *p, Peca *saida) {
    if (p->topo == -1) return 0;
    *saida = p->itens[p->topo--];
    return 1;
}

// Troca frente da fila com topo da pilha
void trocarSimples(Fila *f, Pilha *p) {
    if (p->topo == -1) {
        printf("Nao ha pecas na pilha para trocar.\n");
        return;
    }
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
}

// Troca múltipla entre pilha e fila
void trocarMultiplas(Fila *f, Pilha *p) {
    if (p->topo < 2) {
        printf("A pilha nao tem 3 pecas para a troca.\n");
        return;
    }

    if (f->quantidade < 3) {
        printf("A fila nao tem 3 pecas para a troca.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indexFila = (f->frente + i) % TAM_FILA;

        Peca temp = f->itens[indexFila];
        f->itens[indexFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }
}

// Mostrar estado atual
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nFila de pecas:\t");
    for (int i = 0; i < f->quantidade; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }

    printf("\nPilha de reserva (Topo -> Base):\t");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// Menu principal
int main() {
    srand(time(NULL));

    int contadorID = 0;
    Fila fila;
    Pilha pilha;

    inicializarFila(&fila, &contadorID);
    inicializarPilha(&pilha);

    int opcao;

    do {
        mostrarEstado(&fila, &pilha);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca da fila\n");
        printf("2 - Reservar peca (fila -> pilha)\n");
        printf("3 - Usar peca reservada (pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca removida = desenfileirar(&fila);
            enfileirar(&fila, gerarPeca(&contadorID));
            printf("Peca jogada: [%c %d]\n", removida.nome, removida.id);

        } else if (opcao == 2) {
            if (pilha.topo < TAM_PILHA - 1) {
                Peca movida = desenfileirar(&fila);
                empilhar(&pilha, movida);
                enfileirar(&fila, gerarPeca(&contadorID));
                printf("Peca [%c %d] enviada para a pilha\n", movida.nome, movida.id);
            } else {
                printf("A pilha esta cheia!\n");
            }

        } else if (opcao == 3) {
            Peca usada;
            if (desempilhar(&pilha, &usada))
                printf("Peca usada: [%c %d]\n", usada.nome, usada.id);
            else
                printf("Nao ha pecas na pilha!\n");

        } else if (opcao == 4) {
            trocarSimples(&fila, &pilha);

        } else if (opcao == 5) {
            trocarMultiplas(&fila, &pilha);
        }

    } while (opcao != 0);

    printf("Programa encerrado.\n");
    return 0;
}
