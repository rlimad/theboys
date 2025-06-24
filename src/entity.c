#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "conjunto.h"
#include "lista.h"

// ----- heroi -----
Hero *hero_create(int id, struct cjto_t *habs, int paciencia, int velocidade) {
    Hero *h = malloc(sizeof(Hero));
    if (!h) return NULL;
    h->id = id;
    h->habs = habs;
    h->paciencia = paciencia;
    h->velocidade = velocidade;
    h->experiencia = 0;
    h->base_atual = -1;
    h->vivo = true;
    return h;
}

void hero_destroy(Hero *h) {
    if (!h) return;
    if (h->habs) cjto_destroi(h->habs);
    free(h);
}

// ----- base -----
Base *base_create(int id, int lotacao, Point local) {
    Base *b = malloc(sizeof(Base));
    if (!b) return NULL;
    b->id = id;
    b->lotacao = lotacao;
    b->presentes = cjto_cria(lotacao);
    b->espera = lista_cria();
    b->local = local;
    b->fila_max = 0;
    b->missoes = 0;
    return b;
}

void base_destroy(Base *b) {
    if (!b) return;
    if (b->presentes) cjto_destroi(b->presentes);
    if (b->espera) lista_destroi(b->espera);
    free(b);
}

// ----- missao -----
Mission *mission_create(int id, struct cjto_t *req_habs, Point local) {
    Mission *m = malloc(sizeof(Mission));
    if (!m) return NULL;
    m->id = id;
    m->req_habs = req_habs;
    m->local = local;
    m->cumprida = false;
    m->tentativas = 0;
    return m;
}

void mission_destroy(Mission *m) {
    if (!m) return;
    if (m->req_habs) cjto_destroi(m->req_habs);
    free(m);
}

// ----- mundo -----
World *world_create(int n_herois, int n_bases, int n_missoes,
                    int n_habilidades, int n_compostos_v,
                    int tamanho_mundo, int t_fim) {
    World *w = malloc(sizeof(World));
    if (!w) return NULL;

    w->n_herois = n_herois;
    w->herois = malloc(n_herois * sizeof(Hero *));
    if (!w->herois) { free(w); return NULL; }
    memset(w->herois, 0, n_herois * sizeof(Hero *));

    w->n_bases = n_bases;
    w->bases = malloc(n_bases * sizeof(Base *));
    if (!w->bases) { free(w->herois); free(w); return NULL; }
    memset(w->bases, 0, n_bases * sizeof(Base *));

    w->n_missoes = n_missoes;
    w->missoes = malloc(n_missoes * sizeof(Mission *));
    if (!w->missoes) { free(w->bases); free(w->herois); free(w); return NULL; }
    memset(w->missoes, 0, n_missoes * sizeof(Mission *));

    w->n_habilidades = n_habilidades;
    w->n_compostos_v = n_compostos_v;
    w->tamanho_mundo = tamanho_mundo;
    w->t_fim = t_fim;
    w->relogio = 0;
    return w;
}

void world_destroy(World *w) {
    if (!w) return;
    for (int i = 0; i < w->n_herois; i++)
        if (w->herois[i]) hero_destroy(w->herois[i]);
    free(w->herois);

    for (int i = 0; i < w->n_bases; i++)
        if (w->bases[i]) base_destroy(w->bases[i]);
    free(w->bases);

    for (int i = 0; i < w->n_missoes; i++)
        if (w->missoes[i]) mission_destroy(w->missoes[i]);
    free(w->missoes);

    free(w);
}
