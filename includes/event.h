#ifndef EVENTO_H
#define EVENTO_H

#include "entity.h"   

// eventos
typedef enum {
    CHEGADA,        // herói chega à base
    ESPERA,         // herói entra na fila de espera
    DESISTENCIA,    // herói desiste da espera
    AVISA_PORTARIA, // base avisa que há vaga
    ENTRADA,        // herói entra na base
    SAIDA,          // herói sai da base
    VIAGEM,         // herói viaja para outra base
    MORTE,          // herói morre na base
    MISSAO,         // uma missão é disparada
    FIM_SIMULACAO   // fim da simulação
} TipoEvento;

// Definição completa do evento
typedef struct evento_t {
    int          tempo;   // instante em que ocorre
    TipoEvento   tipo;    // qual ação executar
    Hero        *heroi;   // herói envolvido (se aplicável)
    Base        *base;    // base envolvida (origem/destino)
    Mission     *missao;  // missão envolvida (se aplicável)
} Evento;

// Fábricas de eventos
Evento* criar_evento_chegada(int tempo, Hero* heroi, Base* base);
Evento* criar_evento_espera(int tempo, Hero* heroi, Base* base);
Evento* criar_evento_desistencia(int tempo, Hero* heroi, Base* base);
Evento* criar_evento_avisa_portaria(int tempo, Base* base);
Evento* criar_evento_entrada(int tempo, Hero* heroi, Base* base);
Evento* criar_evento_saida(int tempo, Hero* heroi, Base* base);
Evento* criar_evento_viagem(int tempo, Hero* heroi, Base* base_destino);
Evento* criar_evento_morte(int tempo, Hero* heroi, Mission* missao);
Evento* criar_evento_missao(int tempo, Mission* missao);
Evento* criar_evento_fim(int tempo);

// Comparador para fila de prioridade
int evento_comparar(const void* a, const void* b);

// Libera um Evento alocado
void evento_destruir(Evento* e);

#endif 
