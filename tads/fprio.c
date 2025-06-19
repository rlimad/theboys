// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"


// descreve um nodo da fila de prioridades
struct fpnodo_t {
    void *item;
    struct fpnodo_t *prox;
    int prioridade;
    int tipo;
};

// descreve uma fila de prioridades
struct fprio_t {
    struct fpnodo_t *inicio;
    int tamanho;
};

// cria uma fila vazia.
struct fprio_t *fprio_cria () {

    struct fprio_t *nova_fila = malloc (sizeof(struct fprio_t));

    if(!nova_fila) {
        return NULL;
    }

    memset (nova_fila, 0, sizeof(struct fprio_t));

    return nova_fila;

}

struct fprio_t *fprio_destroi (struct fprio_t *f) {

    // verifica se eh vazia 
    if(!f) {
        return NULL;
    }

    struct fpnodo_t *atual = f->inicio;
    while (atual != NULL) {
        struct fpnodo_t *proximo = atual->prox;
        free(atual->item); // libera item
        free(atual); // libera o no 
        atual = proximo; // avanca
    }

    //libera a estrutura principal 
    free(f);
    return NULL;
}

int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio) {
    // verifica se fila e item sao nulos
    if (!f || !item) 
        return -1;

    // chega se nao foi feita dupla insercao 
    for (struct fpnodo_t *aux = f->inicio; aux; aux = aux->prox) {
        if (aux->item == item)
            return -1;
    }

    // cria no
    struct fpnodo_t *novo = malloc(sizeof *novo);
    if (!novo) 
        return -1;
    novo->item = item;
    novo->tipo = tipo;
    novo->prioridade = prio;
    novo->prox = NULL;

    // Encontra posição de inserção
    struct fpnodo_t *anterior = NULL; // antes da cabeça
    struct fpnodo_t *atual  = f->inicio;
    while (atual && atual->prioridade < prio) {
        anterior = atual; 
        atual  = atual->prox;
    }
    while (atual && atual->prioridade == prio) {
        anterior = atual; 
        atual  = atual->prox;
    }

    // faz o link
    if (!anterior) {
        // insere no inicio
        novo->prox = f->inicio;
        f->inicio  = novo;
    } else {
        // insere entre anterior e atual
        novo->prox  = anterior->prox;
        anterior->prox  = novo;
    }

    f->tamanho++;
    return f->tamanho;
}

// retira o primeiro item da fila 
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {

    // verificações 
    if(!f || !tipo || !prio ) {
        return NULL;
    }

    if(f->tamanho == 0) {
        return NULL;
    }

    // guarda os valores solicitados
    struct fpnodo_t *aux = f->inicio;
    void *itenzin = aux->item;
    *tipo = aux->tipo;
    *prio = aux->prioridade;

    f->inicio = aux->prox; // remove

    f->tamanho--;

    free(aux);

    return itenzin;
}

// retorna o tamanho da fila
int fprio_tamanho (struct fprio_t *f) {

    if(!f) {
        return -1;
    }

    return f->tamanho;
}

// imprime a fila de acordo com o formato solicitado (tipo prio)
void fprio_imprime (struct fprio_t *f) {

    if (!f) return;

    struct fpnodo_t *aux = f->inicio;

    while(aux) {
        printf("(%d %d)", aux->tipo, aux->prioridade);
        if(aux->prox != NULL) {
            printf(" "); // espaco caso nulo
        }
        aux = aux->prox; // avança
    }
}   


