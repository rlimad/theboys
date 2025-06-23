// entity_test.c
#include <stdio.h>
#include "entity.h"
#include "conjunto.h"
#include "lista.h"

int main(void) {
    // --- Teste de Hero ---
    struct cjto_t *habs = cjto_cria(10);
    cjto_insere(habs, 2);
    Hero *h = hero_create(1, habs, 5, 10);
    printf("Hero: ID=%d, pac=%d, vel=%d, vivo=%d, exp=%d, base_atual=%d\n",
           h->id, h->paciencia, h->velocidade, h->vivo, h->experiencia, h->base_atual);
    hero_destroy(h);

    // --- Teste de Base ---
    Point p = { .x = 10, .y = 20 };
    Base *b = base_create(2, 3, p);
    printf("Base: ID=%d, lotacao=%d, local=(%d,%d), fila_max=%d, missoes=%d\n",
           b->id, b->lotacao, b->local.x, b->local.y, b->fila_max, b->missoes);
    base_destroy(b);

    // --- Teste de Mission ---
    struct cjto_t *req = cjto_cria(10);
    cjto_insere(req, 4);
    Mission *m = mission_create(3, req, (Point){ .x = 5, .y = 6 });
    printf("Mission: ID=%d, cumprida=%d, tentativas=%d\n",
           m->id, m->cumprida, m->tentativas);
    mission_destroy(m);

    // --- Teste de World ---
    World *w = world_create(1, 1, 1, 10, 2, 100);
    // popula vetores dinamicamente
    w->herois[0]  = hero_create(4, cjto_aleat(2,10), 8, 12);
    w->bases[0]   = base_create(5, 5, (Point){ .x = 1, .y = 2 });
    w->missoes[0] = mission_create(6, cjto_aleat(1,10), (Point){ .x = 3, .y = 4 });
    printf("World: n_herois=%d, n_bases=%d, n_missoes=%d, relogio=%d\n",
           w->n_herois, w->n_bases, w->n_missoes, w->relogio);
    world_destroy(w);

    printf("Todos os testes concluídos com sucesso.\n");
    return 0;
}
