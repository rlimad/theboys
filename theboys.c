// theboys.c
#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "simulation.h"
#include "conjunto.h"

int main(void) {
    // --- Parâmetros fixos do cenário de teste ---
    int n_habs         = 3;
    int n_herois       = 1;
    int n_bases        = 1;
    int n_missoes      = 1;
    int n_compostos_v  = 0;
    int tamanho_mundo  = 4320;

    // Cria o mundo
    World *W = world_create(n_herois,
                            n_bases,
                            n_missoes,
                            n_habs,
                            n_compostos_v,
                            tamanho_mundo);
    if (!W) {
        fprintf(stderr, "Erro: falha ao criar World\n");
        return 1;
    }

    // --- Cria 1 herói manualmente ---
    // Herói 0: paciência 5, velocidade 10, base 0, habilidades {0,1}
    struct cjto_t *habs = cjto_cria(n_habs);
    cjto_insere(habs, 0);
    cjto_insere(habs, 1);
    Hero *h = hero_create(0, habs, 5, 10);
    h->base_atual = 0;
    W->herois[0] = h;

    // --- Cria 1 base manualmente ---
    // Base 0: lotação 1, posição (0,0)
    W->bases[0] = base_create(0, 1, (Point){ .x = 0, .y = 0 });

    // --- Cria 1 missão manualmente ---
    // Missão 0: requer {0,1}, posição (10,10)
    struct cjto_t *req = cjto_cria(n_habs);
    cjto_insere(req, 0);
    cjto_insere(req, 1);
    W->missoes[0] = mission_create(0, req, (Point){ .x = 10, .y = 10 });

    // Executa a simulação
    simular(W);

    // Limpa tudo
    world_destroy(W);
    return 0;
}
