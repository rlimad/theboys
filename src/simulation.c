#include <stdlib.h>
#include <limits.h>
#include "simulation.h"
#include "fprio.h"
#include "conjunto.h"
#include "lista.h"
#include "log.h"
#include "utils.h"
#include "event.h"

// Protótipos dos handlers de evento
static void handle_chega   (struct fprio_t*, World*, Evento*);
static void handle_espera  (struct fprio_t*, World*, Evento*);
static void handle_desiste (struct fprio_t*, World*, Evento*);
static void handle_avisa   (struct fprio_t*, World*, Evento*);
static void handle_entra   (struct fprio_t*, World*, Evento*);
static void handle_sai     (struct fprio_t*, World*, Evento*);
static void handle_viaja   (struct fprio_t*, World*, Evento*);
static void handle_morre   (struct fprio_t*, World*, Evento*);
static void handle_missao  (struct fprio_t*, World*, Evento*);
static void handle_fim     (struct fprio_t*, World*, Evento*);

// Contadores para estatísticas finais
static int eventos_tratados;
static int max_fila_global;
static int compostos_ini;

void simular(World *W) {
    eventos_tratados = 0;
    max_fila_global  = 0;
    compostos_ini    = W->n_compostos_v;

    struct fprio_t *fila = fprio_cria();
    if (!fila) return;

    // 1) Agendar CHEGADA inicial (primeiros 3 dias = 4320min)
    for (int i = 0; i < W->n_herois; i++) {
        Hero *h = W->herois[i];
        int bidx = utils_rand_int(0, W->n_bases - 1);
        int t0   = utils_rand_int(0, 3*24*60);
        h->base_atual = bidx;
        Evento *ev = criar_evento_chegada(t0, h, W->bases[bidx]);
        fprio_insere(fila, ev, ev->tipo, ev->tempo);
    }

    // 2) Agendar MISSÃO inicial **após** o período de chegada (4320 min)
    for (int i = 0; i < W->n_missoes; i++) {
        Mission *m = W->missoes[i];
        int tm = utils_rand_int(4*24*60 + 1, W->t_fim);
        Evento *ev = criar_evento_missao(tm, m);
        fprio_insere(fila, ev, ev->tipo, ev->tempo);
    }

    // 3) Agendar FIM da simulação
    Evento *ef = criar_evento_fim(W->t_fim);
    fprio_insere(fila, ef, ef->tipo, ef->tempo);

    // Loop principal
    while (fprio_tamanho(fila) > 0) {
        int tipo, tempo;
        Evento *e = fprio_retira(fila, &tipo, &tempo);
        if (!e) break;
        eventos_tratados++;
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
            case FIM_SIMULACAO:  handle_fim(fila, W, e);      goto end;
        }

        evento_destruir(e);
    }

end:
    evento_destruir(ef);
}

// ===== Handlers de evento =====

static void handle_chega(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;

    Base *b = e->base;
    int nafila = lista_tamanho(b->espera);
    int npres  = cjto_card(b->presentes);

    Evento *ev;

    // Regra clara: se há vaga e ninguém na fila, entra direto
    if (npres < b->lotacao && nafila == 0) {
        log_chega(W->relogio, h, b, true);
        ev = criar_evento_entrada(W->relogio, h, b);
    }

    // Caso contrário, ele pode esperar se tiver paciência suficiente
    else if (h->paciencia > 10 * nafila) {
        log_chega(W->relogio, h, b, true);
        ev = criar_evento_espera(W->relogio, h, b);
    }

    // Senão, desiste
    else {
        log_chega(W->relogio, h, b, false);
        ev = criar_evento_desistencia(W->relogio, h, b);
    }

    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_espera(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;
    Base *b = e->base;
    lista_insere(b->espera, h->id, -1);
    int fsz = lista_tamanho(b->espera);
    if (fsz > b->fila_max)     b->fila_max = fsz;
    if (fsz > max_fila_global) max_fila_global = fsz;
    log_espera(W->relogio, h, b, fsz);
    Evento *ev = criar_evento_avisa_portaria(W->relogio, b);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_desiste(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;
    Base *b = e->base;
    log_desiste(W->relogio, h, b);
    int dest;
    do {
        dest = utils_rand_int(0, W->n_bases - 1);
    } while (dest == h->base_atual);
    Evento *ev = criar_evento_viagem(W->relogio, h, W->bases[dest]);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_avisa(struct fprio_t *fila, World *W, Evento *e) {
    Base *b = e->base;
    int pres = cjto_card(b->presentes);
    log_avisa_inicio(W->relogio, b, pres, b->lotacao, b->espera);
    while (pres < b->lotacao && lista_tamanho(b->espera) > 0) {
        int hid;
        lista_retira(b->espera, &hid, 0);
        Hero *h = W->herois[hid];
        if (!h->vivo) continue;
        h->base_atual = b->id;
        log_avisa_admite(W->relogio, b, hid);
        Evento *ent = criar_evento_entrada(W->relogio, h, b);
        fprio_insere(fila, ent, ent->tipo, ent->tempo);
        pres++;
    }
}

static void handle_entra(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    Base *b = e->base;
    cjto_insere(b->presentes, h->id);
    if (!h->vivo) return;
    int pres = cjto_card(b->presentes);
    log_entra(W->relogio, h, b, pres, b->lotacao);
    int base_tempo = utils_rand_int(100, 200);  
    int tpb = 200 + h->paciencia * base_tempo;
    Evento *ev = criar_evento_saida(W->relogio + tpb, h, b);
    fprio_insere(fila, ev, ev->tipo, ev->tempo);
}

static void handle_sai(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;
    Base *b = e->base;
    cjto_retira(b->presentes, h->id);
    int pres = cjto_card(b->presentes);
    log_sai(W->relogio, h, b, pres, b->lotacao);
    int dest;
    do {
        dest = utils_rand_int(0, W->n_bases - 1);
    } while (dest == h->base_atual);
    Evento *via = criar_evento_viagem(W->relogio, h, W->bases[dest]);
    fprio_insere(fila, via, via->tipo, via->tempo);
    Evento *av = criar_evento_avisa_portaria(W->relogio, b);
    fprio_insere(fila, av, av->tipo, av->tempo);
}

static void handle_viaja(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;
    Base *orig = W->bases[h->base_atual];
    Base *dest = e->base;
    int dist = utils_distancia(orig->local, dest->local);
    int dur  = dist / h->velocidade;
    log_viaja(W->relogio, h, orig, dest, dist, h->velocidade, W->relogio + dur);
    h->base_atual = dest->id;
    Evento *cev = criar_evento_chegada(W->relogio + dur, h, dest);
    fprio_insere(fila, cev, cev->tipo, cev->tempo);
}

static void handle_missao(struct fprio_t *fila, World *W, Evento *e) {
    int T = W->relogio;
    Mission *m = e->missao;
    m->tentativas++;
    log_missao_req(T, m);

    // 1) Tenta encontrar base com habilidades cobertas
    int best = -1, best_dist = 0;
    for (int i = 0; i < W->n_bases; i++) {
        Base *b = W->bases[i];
        int pres = cjto_card(b->presentes);
        if (pres == 0) continue;

        int d = utils_distancia(b->local, m->local);

        // DEBUG: mostra cada base analisada
        log_missao_base(T, m, b, d);

        // monta união das habilidades presentes
        struct cjto_t *uni = cjto_cria(W->n_habilidades);
        for (int hid = 0; hid < W->n_herois; hid++) {
            if (cjto_pertence(b->presentes, hid)) {
                Hero *h2 = W->herois[hid];
                // DEBUG: habilidades de cada herói
                log_missao_hab(T, m, h2);
                struct cjto_t *habs = h2->habs;
                for (int s = 0; s < W->n_habilidades; s++) {
                    if (cjto_pertence(habs, s))
                        cjto_insere(uni, s);
                }
            }
        }

        // DEBUG: união final
        log_missao_uniao(T, m, b, uni);

        if (cjto_contem(uni, m->req_habs)) {
            if (best < 0 || d < best_dist) {
                best = i;
                best_dist = d;
            }
        }
        cjto_destroi(uni);
    }

    // 2) Se achou base, missão cumprida normalmente
    if (best >= 0) {
        Base *b = W->bases[best];
        m->cumprida = true;
        b->missoes++;  // incrementa estatística de missões da base
        struct lista_t *v = lista_cria();
        for (int hid = 0; hid < W->n_herois; hid++) {
            if (cjto_pertence(b->presentes, hid)) {
                W->herois[hid]->experiencia++;
                lista_insere(v, hid, -1);
            }
        }
        log_missao_succ(T, m, b, best_dist, v);
        lista_destroi(v);
    }
    // 3) Se não achou, e for instante múltiplo de 2500, usa Composto V
    else if (W->n_compostos_v > 0 && (T % 2500) == 0) {
        W->n_compostos_v--;
        int top = -1, maxexp = -1;
        for (int i = 0; i < W->n_herois; i++) {
            Hero *h = W->herois[i];
            if (h->vivo && h->experiencia > maxexp) {
                maxexp = h->experiencia;
                top = i;
            }
        }
        if (top >= 0) {
            Hero *h2 = W->herois[top];
            m->cumprida = true;
            h2->experiencia++;
            // conta também no base_atual do herói ressuscitado
            Base *b2 = W->bases[h2->base_atual];
            b2->missoes++;
            log_missao_composto(T, m, h2);

            // Agendar morte
            Evento *kill = criar_evento_morte(T, h2, m);
            fprio_insere(fila, kill, kill->tipo, kill->tempo);
        }
    }
    // 4) Caso contrário, impossível: reagenda para próximo múltiplo de 2500
    else if (!m->cumprida && m->tentativas < 100) {
        log_missao_impossivel(T, m);
        Evento *retry = criar_evento_missao(T + 24*60, m);
        fprio_insere(fila, retry, retry->tipo, retry->tempo);
    }
}

// heroi morreu apos usar Composto V — remove da base e libera vaga
static void handle_morre(struct fprio_t *fila, World *W, Evento *e) {
    Hero *h = e->heroi;
    if (!h->vivo) return;
    h->vivo = false;
    // retira presença e avisa liberação de vaga
    Base *b = W->bases[h->base_atual];
    if (cjto_pertence(b->presentes, h->id)) {
        cjto_retira(b->presentes, h->id);
        Evento *av = criar_evento_avisa_portaria(W->relogio, b);
        fprio_insere(fila, av, av->tipo, av->tempo);
    }
    log_morre(W->relogio, h, e->missao);
    printf("DEBUG: Heroi %d morreu em T=%d\n", h->id, W->relogio);
}

static void handle_fim(struct fprio_t *fila, World *W, Evento *e) {
    (void)e;
    (void)fila;
    int total = W->n_missoes;
    int succ = 0, imp = 0, somaTent = 0;
    int minTent = INT_MAX, maxTent = 0;
    for (int i = 0; i < total; i++) {
        Mission *m = W->missoes[i];
        somaTent += m->tentativas;
        minTent = m->tentativas < minTent ? m->tentativas : minTent;
        maxTent = m->tentativas > maxTent ? m->tentativas : maxTent;
        if (m->cumprida) succ++; else imp++;
    }
    double mediaTent = total ? (double)somaTent / total : 0.0;
    double taxaSuc   = total ? (double)succ / total * 100.0 : 0.0;
    int compostosUsados = (W->n_habilidades * 3) - W->n_compostos_v;
    double taxaMort = W->n_herois ? (double)compostosUsados / W->n_herois * 100.0 : 0.0;

    // HERÓIS
    printf("HEROIS:\n");
    for (int i = 0; i < W->n_herois; i++) {
        Hero *h = W->herois[i];
        printf("HEROI %2d %s PAC %4d VEL %4d EXP %4d HABS [",
               h->id, h->vivo ? "VIVO " : "MORTO", h->paciencia, h->velocidade, h->experiencia);
        for (int s = 0; s < W->n_habilidades; s++) {
            if (cjto_pertence(h->habs, s)) printf(" %d", s);
        }
        printf(" ]\n");
    }

    // BASES
    printf("\nBASES:\n");
    for (int i = 0; i < W->n_bases; i++) {
        Base *b = W->bases[i];
        printf("BASE %2d LOT %2d MAIOR FILA %2d MISSOES %d\n",
               b->id, b->lotacao, b->fila_max, b->missoes);
    }

    // ESTATÍSTICAS
    printf("\nEVENTOS TRATADOS: %d\n", eventos_tratados);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", succ, total, taxaSuc);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", minTent, maxTent, mediaTent);
    printf("TAXA MORTALIDADE: %.1f%%\n", taxaMort);
}

