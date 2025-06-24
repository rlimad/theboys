#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include "conjunto.h"   // declara struct cjto_t
#include "lista.h"      // declara struct lista_t

// Estrutura para representar coordenadas no plano
typedef struct {
    int x;
    int y;
} Point;

// Herói (H)
typedef struct {
    int               id;            // identifica unicamente o herói
    struct cjto_t    *habs;          // conjunto de habilidades (IDs)
    int               paciencia;     // paciência do herói
    int               velocidade;    // velocidade de deslocamento (m/min)
    int               experiencia;   // número de missões já realizadas
    int               base_atual;    // ID da base onde se encontra atualmente
    bool              vivo;          // status (true = vivo, false = morto)
} Hero;

// Base (B)
typedef struct {
    int               id;            // identifica unicamente a base
    int               lotacao;       // capacidade máxima de heróis
    struct cjto_t    *presentes;     // conjunto de IDs de heróis presentes
    struct lista_t   *espera;        // fila de espera (IDs de heróis)
    Point             local;         // coordenadas da base
    int               fila_max;      // tamanho máximo atingido pela fila de espera
    int               missoes;       // número de missões cumpridas pela base
} Base;

// Missão (M)
typedef struct {
    int               id;            // identifica unicamente a missão
    struct cjto_t    *req_habs;      // conjunto de habilidades requeridas
    Point             local;         // coordenadas da missão
    bool              cumprida;      // indica se a missão foi cumprida
    int               tentativas;    // número de tentativas realizadas
} Mission;

// Mundo (W)
typedef struct {
    // Quantidades estáticas
    int               n_herois;
    Hero            **herois;        // vetor de ponteiros para heróis
    int               n_bases;
    Base            **bases;         // vetor de ponteiros para bases
    int               n_missoes;
    Mission         **missoes;       // vetor de ponteiros para missões
    int               n_habilidades; // número total de habilidades possíveis
    int               n_compostos_v; // número total de Compostos V disponíveis
    int               tamanho_mundo; // dimensão máxima do plano (coordenadas)
    int               t_fim;         // instante de fim da simulação (minutos)
    int               relogio;       // tempo atual da simulação (minutos)
} World;

// ===== prototipos de criação e destruição =====

Hero *hero_create(int id, struct cjto_t *habs, int paciencia, int velocidade);

void  hero_destroy(Hero *h);

Base *base_create(int id, int lotacao, Point local);

void  base_destroy(Base *b);
 
Mission *mission_create(int id, struct cjto_t *req_habs, Point local);

void    mission_destroy(Mission *m);

World *world_create(int n_herois,
                    int n_bases,
                    int n_missoes,
                    int n_habilidades,
                    int n_compostos_v,
                    int tamanho_mundo,
                    int t_fim);

void   world_destroy(World *w);

#endif 
