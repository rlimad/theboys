// evento_test.c
#include <stdio.h>
#include "event.h"
#include "fprio.h"

// Mapeia o enum para string para facilitar a leitura
static const char* tipo_str(TipoEvento t) {
    switch (t) {
        case CHEGADA:        return "CHEGADA";
        case ESPERA:         return "ESPERA";
        case DESISTENCIA:    return "DESISTENCIA";
        case AVISA_PORTARIA: return "AVISA_PORTARIA";
        case ENTRADA:        return "ENTRADA";
        case SAIDA:          return "SAIDA";
        case VIAGEM:         return "VIAGEM";
        case MORTE:          return "MORTE";
        case MISSAO:         return "MISSAO";
        case FIM_SIMULACAO:  return "FIM_SIMULACAO";
        default:             return "???";
    }
}

int main(void) {
    // Cria fila de prioridades
    struct fprio_t *fila = fprio_cria();
    if (!fila) {
        fprintf(stderr, "Erro ao criar fila\n");
        return 1;
    }

    // Cria alguns eventos (passamos NULL para herói/base/missão, pois só testamos order)
    Evento *e1 = criar_evento_chegada(10, NULL, NULL);
    Evento *e2 = criar_evento_espera(5,  NULL, NULL);
    Evento *e3 = criar_evento_missao(10, NULL);
    Evento *e4 = criar_evento_viagem(5,  NULL, NULL);

    // Insere na fila: o campo 'tipo' e 'tempo' viram, respectivamente, tipo e prioridade
    fprio_insere(fila, e1, CHEGADA,       10);
    fprio_insere(fila, e2, ESPERA,         5);
    fprio_insere(fila, e3, MISSAO,        10);
    fprio_insere(fila, e4, VIAGEM,         5);

    // Mostra a fila antes de retirar
    printf("Fila (tipo prioridade): ");
    fprio_imprime(fila);
    printf("\n\n");

    // Retira tudo em ordem
    int tipo, prio;
    while (fprio_tamanho(fila) > 0) {
        Evento *e = fprio_retira(fila, &tipo, &prio);
        printf("Retirado -> tempo=%2d, tipo=%s\n", prio, tipo_str(tipo));
        evento_destruir(e);
    }

    // Limpa a fila (já vazia)
    fprio_destroi(fila);
    return 0;
}
