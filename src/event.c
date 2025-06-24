#include <stdlib.h>
#include "event.h"

// Cria um evento de chegada   (tempo, herói, base)
Evento* criar_evento_chegada(int tempo, Hero* heroi, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = CHEGADA;
    e->heroi   = heroi;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de espera    (tempo, herói, base)
Evento* criar_evento_espera(int tempo, Hero* heroi, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = ESPERA;
    e->heroi   = heroi;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de desistência (tempo, herói, base)
Evento* criar_evento_desistencia(int tempo, Hero* heroi, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = DESISTENCIA;
    e->heroi   = heroi;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de aviso à portaria (tempo, base)
Evento* criar_evento_avisa_portaria(int tempo, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = AVISA_PORTARIA;
    e->heroi   = NULL;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de entrada na base   (tempo, herói, base)
Evento* criar_evento_entrada(int tempo, Hero* heroi, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = ENTRADA;
    e->heroi   = heroi;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de saída da base     (tempo, herói, base)
Evento* criar_evento_saida(int tempo, Hero* heroi, Base* base) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = SAIDA;
    e->heroi   = heroi;
    e->base    = base;
    e->missao  = NULL;
    return e;
}

// Cria um evento de viagem            (tempo, herói, base_destino)
Evento* criar_evento_viagem(int tempo, Hero* heroi, Base* base_destino) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = VIAGEM;
    e->heroi   = heroi;
    e->base    = base_destino;
    e->missao  = NULL;
    return e;
}

// Cria um evento de morte             (tempo, herói, base)
Evento* criar_evento_morte(int tempo, Hero* heroi, Mission* missao) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = MORTE;
    e->heroi   = heroi;
    e->base    = NULL;
    e->missao  = missao;
    return e;
}

// Cria um evento de missão            (tempo, missão)
Evento* criar_evento_missao(int tempo, Mission* missao) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = MISSAO;
    e->heroi   = NULL;
    e->base    = NULL;
    e->missao  = missao;
    return e;
}

// Cria um evento de fim de simulação  (tempo)
Evento* criar_evento_fim(int tempo) {
    Evento* e = malloc(sizeof *e);
    if (!e) return NULL;
    e->tempo   = tempo;
    e->tipo    = FIM_SIMULACAO;
    e->heroi   = NULL;
    e->base    = NULL;
    e->missao  = NULL;
    return e;
}

// Compara dois eventos para ordenação em fila de prioridade
int evento_comparar(const void* a, const void* b) {
    const Evento* e1 = a;
    const Evento* e2 = b;
    if (e1->tempo < e2->tempo) return -1;
    if (e1->tempo > e2->tempo) return  1;
    return e1->tipo - e2->tipo;
}

// Libera o evento da memória
void evento_destruir(Evento *e) {
    if (!e) return;
    free(e);
}

