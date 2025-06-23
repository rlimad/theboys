// src/simulation.c
#include <stdlib.h>
#include "simulation.h"
#include "fprio.h"
#include "conjunto.h"
#include "lista.h"
#include "log.h"
#include "utils.h"
#include "event.h"       

// Protótipos dos handlers de evento
static void handle_chega    (struct fprio_t *fila, World *W, Evento *e);
static void handle_espera   (struct fprio_t *fila, World *W, Evento *e);
static void handle_desiste  (struct fprio_t *fila, World *W, Evento *e);
static void handle_avisa    (struct fprio_t *fila, World *W, Evento *e);
static void handle_entra    (struct fprio_t *fila, World *W, Evento *e);
static void handle_sai      (struct fprio_t *fila, World *W, Evento *e);
static void handle_viaja    (struct fprio_t *fila, World *W, Evento *e);
static void handle_morre    (struct fprio_t *fila, World *W, Evento *e);
static void handle_missao   (struct fprio_t *fila, World *W, Evento *e);
static void handle_fim      (struct fprio_t *fila, World *W, Evento *e);

void simular(World *W) {
    struct fprio_t *fila = fprio_cria();
    if (!fila) return;

    // 1) Agendar CHEGADA inicial
    for (int i = 0; i < W->n_herois; i++) {
        Hero *h = W->herois[i];
        int bidx = utils_rand_int(0, W->n_bases - 1);
        int t0   = utils_rand_int(0, 4320);
        h->base_atual = bidx;
        Evento *ev = criar_evento_chegada(t0, h, W->bases[bidx]);
        fprio_insere(fila, ev, ev->tipo, ev->tempo);
    }
    // 2) Agendar MISSÃO inicial
    for (int i = 0; i < W->n_missoes; i++) {
        Mission *m = W->missoes[i];
        int tm = utils_rand_int(0, W->tamanho_mundo);
        Evento *ev = criar_evento_missao(tm, m);
        fprio_insere(fila, ev, ev->tipo, ev->tempo);
    }
    // 3) Agendar FIM da simulação
    Evento *ef = criar_evento_fim(W->tamanho_mundo);
    fprio_insere(fila, ef, ef->tipo, ef->tempo);

    // Loop principal
    while (fprio_tamanho(fila) > 0) {
        int tipo, tempo;
        Evento *e = fprio_retira(fila, &tipo, &tempo);
        W->relogio = tempo;
        switch (e->tipo) {
            case CHEGADA:        handle_chega(fila, W, e);    break;
            case ESPERA:         handle_espera(fila, W, e);   break;
            case DESISTENCIA:    handle_desiste(fila, W, e);  break;
            case AVISA_PORTARIA: handle_avisa(fila, W, e);    break;
            case ENTRADA:        handle_entra(fila, W, e);    break;
            case SAIDA:          handle_sai(fila, W, e);      break;
            case VIAGEM:         handle_viaja(fila, W, e);    break;
            case MORTE:          handle_morre(fila, W, e);    break;
            case MISSAO:         handle_missao(fila, W, e);   break;
            case FIM_SIMULACAO:  handle_fim(fila, W, e);      return;
        }
        evento_destruir(e);
    }
}

static void handle_chega(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    int nafila = lista_tamanho(b->espera);
    int npres  = cjto_card(b->presentes);
    bool entra = (npres < b->lotacao && nafila == 0)
              || (h->paciencia > 10 * nafila);
    log_chega(W->relogio, h, b, entra);
    Evento *ev = entra
      ? criar_evento_entrada(W->relogio, h, b)
      : criar_evento_espera(W->relogio, h, b);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_espera(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    lista_insere(b->espera, h->id, -1);
    int fsz = lista_tamanho(b->espera);
    if (fsz > b->fila_max) b->fila_max = fsz;
    log_espera(W->relogio, h, b, fsz);
    Evento *ev = criar_evento_avisa_portaria(W->relogio, b);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_desiste(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    log_desiste(W->relogio, h, b);
    int dest = utils_rand_int(0, W->n_bases - 1);
    Evento *ev = criar_evento_viagem(W->relogio, h, W->bases[dest]);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_avisa(struct fprio_t *fila, World *W, Evento *e) {
    Base *b = e->base;
    int npres = cjto_card(b->presentes);
    int lot   = b->lotacao;
    log_avisa_inicio(W->relogio, b, npres, lot, b->espera);
    while (npres < lot && lista_tamanho(b->espera) > 0) {
        int hid;
        lista_retira(b->espera, &hid, 0);
        Hero *h = W->herois[hid];
        cjto_insere(b->presentes, hid);
        log_avisa_admite(W->relogio, b, hid);
        Evento *ent = criar_evento_entrada(W->relogio, h, b);
        fprio_insere(fila, ent, ent->tipo, ent->tempo);
        npres++;
    }
}

static void handle_entra(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    int npres = cjto_card(b->presentes);
    log_entra(W->relogio, h, b, npres, b->lotacao);
    int tpb = 15 + h->paciencia * utils_rand_int(1, 20);
    // corrige nome da fábrica
    Evento *ev = criar_evento_saida(W->relogio + tpb, h, b);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_sai(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    cjto_retira(b->presentes, h->id);
    int npres = cjto_card(b->presentes);
    log_sai(W->relogio, h, b, npres, b->lotacao);
    int dest = utils_rand_int(0, W->n_bases - 1);
    Evento *via = criar_evento_viagem(W->relogio, h, W->bases[dest]);
    fprio_insere(fila, via, via->tipo, via->tempo);
    Evento *av = criar_evento_avisa_portaria(W->relogio, b);
    fprio_insere(fila, av, av->tipo, av->tempo);
}

static void handle_viaja(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *dest = e->base;
    Base *orig = W->bases[h->base_atual];
    int dist = utils_distancia(orig->local, dest->local);
    int dur  = dist / h->velocidade;
    log_viaja(W->relogio, h, orig, dest, dist, h->velocidade, W->relogio + dur);
    h->base_atual = dest->id;
    Evento *cev = criar_evento_chegada(W->relogio + dur, h, dest);
    fprio_insere(fila, cev, cev->tipo, cev->tempo);
}

static void handle_morre(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Mission *m = e->missao;
    Base *b = W->bases[h->base_atual];
    if (cjto_pertence(b->presentes, h->id)) {
        cjto_retira(b->presentes, h->id);
        Evento *av = criar_evento_avisa_portaria(W->relogio, b);
        fprio_insere(fila, av, av->tipo, av->tempo);
    }
    h->vivo = false;
    log_morre(W->relogio, h, m);
}

static void handle_missao(struct fprio_t *fila, World *W, Evento *e) {
    Mission *m = e->missao;
    m->tentativas++;
    log_missao_req(W->relogio, m);

    int best = -1, best_dist = 0;
    // encontra base apta e mais próxima
    for (int i = 0; i < W->n_bases; i++) {
        Base *b = W->bases[i];
        if (cjto_card(b->presentes) == 0) continue;
        int d = utils_distancia(b->local, m->local);
        if (cjto_contem(b->presentes, m->req_habs) == 1) {
            if (best < 0 || d < best_dist) {
                best = i;
                best_dist = d;
            }
        }
    }

    if (best >= 0) {
        Base *b = W->bases[best];
        m->cumprida = true;
        b->missoes++;
        struct lista_t *v = lista_cria();
        for (int hid = 0; hid < W->n_herois; hid++) {
            if (cjto_pertence(b->presentes, hid) == 1) {
                Hero *h = W->herois[hid];
                h->experiencia++;
                lista_insere(v, hid, -1);
            }
        }
        log_missao_succ(W->relogio, m, b, best_dist, v);
        lista_destroi(v);
    }
    else if (W->n_compostos_v > 0) {
        W->n_compostos_v--;
        int top = -1, maxexp = -1;
        for (int i = 0; i < W->n_herois; i++) {
            Hero *h = W->herois[i];
            if (h->vivo && h->experiencia > maxexp) {
                maxexp = h->experiencia;
                top = i;
            }
        }
        Hero *h = W->herois[top];
        m->cumprida = true;
        log_missao_composto(W->relogio, m, h);
    }
    else {
        log_missao_impossivel(W->relogio, m);
        Evento *retry = criar_evento_missao(W->relogio + 24*60, m);
        fprio_insere(fila, retry, retry->tipo, retry->tempo);
    }
}

static void handle_fim(struct fprio_t *fila, World *W, Evento *e) {
    (void)e;
    log_fim(W->relogio);
    fprio_destroi(fila);
}
