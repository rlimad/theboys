// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"
 
struct item_t {
  // define a estrutura interna do item
  int valor;
  struct item_t *ant;
  struct item_t *prox;

} ;

// estrutura de uma lista
struct lista_t {
  // define a estrutura interna da lista
  struct item_t *primeiro;
  struct item_t *ultimo;
  int tam;
} ;

struct lista_t *lista_cria () {
  
  // aloca 
  struct lista_t *nova_lista = malloc (sizeof(struct lista_t));

  // verifica alocacao
  if(!nova_lista) {
    return NULL;
  }

  // inicializa todos os campos com 0 
  memset (nova_lista, 0, sizeof(struct lista_t));

  return nova_lista;

 }


 struct lista_t *lista_destroi(struct lista_t *lst) {
  
  if (!lst) {
    return NULL;
  }

  // libera os nos
  struct item_t *atual = lst->primeiro;
  while (atual != NULL) {
      struct item_t *proximo = atual->prox;  // salva o prox
      free(atual);                           // libera o no atual
      atual = proximo;                       // avança
  }

  // libera a estrutura principal
  free(lst);
  return NULL;

}

int lista_insere (struct lista_t *lst, int item, int pos) {

  if(!lst) {
    return -1;
  }

  struct item_t *novo_elem = malloc (sizeof(struct item_t));
  if (!novo_elem) return -1;
  memset(novo_elem, 0, sizeof(struct item_t));
  novo_elem->valor = item;

  // caso lista vazia
  if (lst->tam == 0) {
    novo_elem->ant = novo_elem->prox = NULL;
    lst->primeiro = lst->ultimo = novo_elem;
    lst->tam = 1;
    return 1;
  }
 
  if(pos == 0) {
    // inicio 
    novo_elem->prox = lst->primeiro;
    lst->primeiro->ant = novo_elem;
    novo_elem->ant = NULL;
    lst->primeiro = novo_elem;
  }

  else if (pos == -1 || pos >= lst->tam) {
    // fim
    novo_elem->ant = lst->ultimo;
    lst->ultimo->prox = novo_elem;
    novo_elem->prox = NULL;
    lst->ultimo = novo_elem;
  }

  // inserir antes do no que ocupa pos - meio
  else {
    
    struct item_t *aux = lst->primeiro;

    for(int i=0; i < pos; i++) {
      aux = aux->prox;
    }

    novo_elem->prox = aux;
    novo_elem->ant = aux->ant;
    aux->ant->prox = novo_elem;
    aux->ant = novo_elem;

  }

  lst->tam++;
  return lst->tam;
  
}

int lista_retira(struct lista_t *lst, int *item, int pos) {
  
  if (!lst || !item || lst->tam == 0) return -1;

  struct item_t *aux;

  // caso remove do início
  if (pos == 0) {
    aux = lst->primeiro;
    *item = aux->valor;
    lst->primeiro = aux->prox;
    if (lst->primeiro != NULL)
      lst->primeiro->ant = NULL;
    else
      lst->ultimo = NULL;  // lista ficou vazia
  }
  
  // caso remove do fim
  else if (pos == -1 || pos >= lst->tam) {
    aux = lst->ultimo;
    *item = aux->valor;
    lst->ultimo = aux->ant;
    if (lst->ultimo != NULL)
      lst->ultimo->prox = NULL;
    else
      lst->primeiro = NULL;
  }
  
  // caso remove do meio
  else {
    
    aux = lst->primeiro;
    for (int i = 0; i < pos; i++) {
      aux = aux->prox;
    }

    *item = aux->valor;
    aux->ant->prox = aux->prox;
    aux->prox->ant = aux->ant;

  }

  free(aux);
  lst->tam--;
  return lst->tam;

}

int lista_consulta(struct lista_t *lst, int *item, int pos) {
  
  // erro se lista ou item invalido, ou lista vazia
  if (!lst || !item || lst->tam == 0)
      return -1;

  struct item_t *aux;

  if (pos == 0) {
      // inicio
      aux = lst->primeiro;
  }
  else if (pos == -1) {
      // fim
      aux = lst->ultimo;
  }
  else if (pos > 0 && pos < lst->tam) {
      // meio
      aux = lst->primeiro;
      for (int i = 0; i < pos; i++)
          aux = aux->prox;
  }
  else {
      // qualquer outro pos (>=tam ou < -1) = erro
      return -1;
  }

  *item = aux->valor;
  return lst->tam;
}

int lista_procura(struct lista_t *lst, int valor) {
  
  // valida parametros
  if (!lst || lst->tam == 0) {
    return -1;
  }

  // percorre lista
  struct item_t *aux = lst->primeiro;
  for (int i = 0; i < lst->tam; i++) {
      
    if (aux->valor == valor) {
      return i;  // encontrou na posição i
    }           
    aux = aux->prox;

  }

  // nao encontrou
  return -1;

}

int lista_tamanho (struct lista_t *lst) {

  if(!lst) {
    return -1;
  }

  return lst->tam;

}

void lista_imprime(struct lista_t *lst) {
  
  if (!lst) return;

  struct item_t *aux = lst->primeiro;
  int primeiro = 1;

  while (aux) {
      if (!primeiro) {
          printf(" "); // printa espaço antes de 2º, 3º… elementos
      }
      printf("%d", aux->valor);
      primeiro = 0; // depois de imprimir o 1º, marca que nao eh mais o primeiro
      aux = aux->prox; // avança 
  }

}
