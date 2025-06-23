// includes/log.h
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdbool.h>
#include "entity.h"

// Chegada: informa se vai esperar ou desistir
void log_chega(int t, Hero *h, Base *b, bool espera);

// Espera: tamanho atual da fila
void log_espera(int t, Hero *h, Base *b, int fila_size);

// Desistência: herói desiste da fila
void log_desiste(int t, Hero *h, Base *b);

// Aviso à portaria (início): quantos estão e lotação
void log_avisa_inicio(int t, Base *b, int presentes, int lotacao, struct lista_t *fila);

// Aviso à portaria (admissão de um herói)
void log_avisa_admite(int t, Base *b, int hero_id);

// Entrada: quantos ficam presentes após entrada
void log_entra(int t, Hero *h, Base *b, int presentes, int lotacao);

// Saída: quantos ficam após saída
void log_sai(int t, Hero *h, Base *b, int presentes, int lotacao);

// Viagem: origem→destino, distância, velocidade e chegada prevista
void log_viaja(int t, Hero *h, Base *origem, Base *dest, int dist, int vel, int t_chegada);

// Morte: herói morre durante missão
void log_morre(int t, Hero *h, Mission *m);

// Requisição de missão: ponto de partida
void log_missao_req(int t, Mission *m);

// Análise de base para missão (debug)
void log_missao_base(int t, Mission *m, Base *b, int dist, struct lista_t *fila);

// Missão bem-sucedida: base, distância e lista de heróis
void log_missao_succ(int t, Mission *m, Base *b, int dist, struct lista_t *herois);

// Uso de Composto V: herói resurrect
void log_missao_composto(int t, Mission *m, Hero *h);

// Missão impossível: reagendada
void log_missao_impossivel(int t, Mission *m);

// Fim da simulação
void log_fim(int t);

#endif // LOG_H
