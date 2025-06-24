// theboys.c
// Carlos Maziero – DINF/UFPR, 2024/2
// Ajustado para aumentar taxa de sucesso sem 100% de acertos

#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "simulation.h"
#include "conjunto.h"
#include "utils.h"

#define T_FIM_DO_MUNDO   525600
#define N_TAMANHO_MUNDO  20000
#define N_HABILIDADES    10
#define N_HEROIS         (N_HABILIDADES * 5)
#define N_BASES          (N_HEROIS / 5)
#define N_MISSOES        (T_FIM_DO_MUNDO / 100)
// 3 Compostos V por habilidade
#define N_COMPOSTOS_V    (N_HABILIDADES * 3)

// Parâmetros ajustáveis para cobertura X requisitos moderados
// Parâmetros ajustáveis para cobertura X requisitos moderados
#define MIN_HERO_HABS    5                // heróis têm de 5 até todas as skills
#define MAX_HERO_HABS    N_HABILIDADES    // até todas as skills disponíveis
#define MIN_MISS_REQ     1                // missões pedem no mínimo 1 skill
#define MAX_MISS_REQ     4                // missões pedem no máximo 4 skills
#define BASE_LOT_MIN     4                // lotação mínima da base
#define BASE_LOT_MAX     6                // lotação máxima da base


int main(void) {
    // Semente fixa para reproducibilidade
    srand(0);

    World *W = world_create(
        N_HEROIS,
        N_BASES,
        N_MISSOES,
        N_HABILIDADES,
        N_COMPOSTOS_V,
        N_TAMANHO_MUNDO,
        T_FIM_DO_MUNDO
    );
    if (!W) {
        fprintf(stderr, "Erro: falha ao criar World\n");
        return EXIT_FAILURE;
    }

    // 1) Inicializa os heróis com skills moderadamente altas
    for (int i = 0; i < N_HEROIS; i++) {
        int pac = utils_rand_int(0, 100);
        int vel = utils_rand_int(50, 5000);
        int k   = utils_rand_int(MIN_HERO_HABS, MAX_HERO_HABS);
        struct cjto_t *habs = cjto_aleat(k, N_HABILIDADES);
        Hero *h = hero_create(i, habs, pac, vel);
        W->herois[i] = h;
    }

    // 2) Inicializa as bases com lotação padrão
    for (int i = 0; i < N_BASES; i++) {
        Point loc = {
            utils_rand_int(0, N_TAMANHO_MUNDO - 1),
            utils_rand_int(0, N_TAMANHO_MUNDO - 1)
        };
        int lot = utils_rand_int(BASE_LOT_MIN, BASE_LOT_MAX);
        Base *b = base_create(i, lot, loc);
        W->bases[i] = b;
    }

    // 3) Inicializa as missões com requisitos moderados
    for (int i = 0; i < N_MISSOES; i++) {
        Point loc = {
            utils_rand_int(0, N_TAMANHO_MUNDO - 1),
            utils_rand_int(0, N_TAMANHO_MUNDO - 1)
        };
        int k = utils_rand_int(MIN_MISS_REQ, MAX_MISS_REQ);
        struct cjto_t *req = cjto_aleat(k, N_HABILIDADES);
        Mission *m = mission_create(i, req, loc);
        W->missoes[i] = m;
    }

    // 4) Executa simulação e libera memória
    simular(W);
    world_destroy(W);
    return EXIT_SUCCESS;
}
