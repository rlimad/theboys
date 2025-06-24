#include <stdio.h>
#include <stdbool.h>
#include "log.h"
#include "conjunto.h"
#include "lista.h"

// Chegada
void log_chega(int t, Hero *h, Base *b, bool espera) {
    int pres = cjto_card(b->presentes);
    if (espera) {
        printf("%6d: CHEGA HEROI %2d BASE %2d (%2d/%2d) ESPERA\n",
               t, h->id, b->id, pres, b->lotacao);
    } else {
        printf("%6d: CHEGA HEROI %2d BASE %2d (%2d/%2d) DESISTE\n",
               t, h->id, b->id, pres, b->lotacao);
    }
}

// Espera
void log_espera(int t, Hero *h, Base *b, int fila_size) {
    printf("%6d: ESPERA HEROI %2d BASE %2d (%2d)\n",
           t, h->id, b->id, fila_size);
}

// Desistência
void log_desiste(int t, Hero *h, Base *b) {
    printf("%6d: DESIST HEROI %2d BASE %2d\n",
           t, h->id, b->id);
}

// Aviso início
void log_avisa_inicio(int t, Base *b, int pres, int lot, struct lista_t *fila) {
    printf("%6d: AVISA PORTEIRO BASE %2d (%2d/%2d) FILA [",
           t, b->id, pres, lot);
    int sz = lista_tamanho(fila);
    for (int i = 0; i < sz; i++) {
        int hid;
        lista_consulta(fila, &hid, i);
        printf(" %2d", hid);
    }
    printf(" ]\n");
}

// Aviso admite
void log_avisa_admite(int t, Base *b, int hid) {
    printf("%6d: AVISA PORTEIRO BASE %2d ADMITE %2d\n",
           t, b->id, hid);
}

// Entrada
void log_entra(int t, Hero *h, Base *b, int pres, int lot) {
    printf("%6d: ENTRA HEROI %2d BASE %2d (%2d/%2d)\n",
           t, h->id, b->id, pres, lot);
}

// Saída
void log_sai(int t, Hero *h, Base *b, int pres, int lot) {
    printf("%6d: SAI HEROI %2d BASE %2d (%2d/%2d)\n",
           t, h->id, b->id, pres, lot);
}

// Viagem
void log_viaja(int t, Hero *h, Base *orig, Base *dest,
               int dist, int vel, int t_chega) {
    printf("%6d: VIAJA HEROI %2d DE BASE %2d PARA BASE %2d "
           "DIST %d VELOC %d CHEGA %d\n",
           t, h->id, orig->id, dest->id,
           dist, vel, t_chega);
}

// Morte
void log_morre(int t, Hero *h, Mission *m) {
    if(m != NULL) {
    printf("%6d: MORRE HEROI %2d EM MISS %2d\n",
           t, h->id, m->id);
    }
}

// Missão: requisição
void log_missao_req(int t, Mission *m) {
    printf("%6d: MISSAO %2d REQ\n", t, m->id);
}

// DEBUG: análise de base para missão (lista de heróis presentes)
void log_missao_base(int t, Mission *m, Base *b, int dist) {
    printf("%6d: MISSAO %2d BASE %2d DIST %d HEROIS [", t, m->id, b->id, dist);
    int cap = b->presentes->cap;
    for (int hid = 0; hid < cap; hid++) {
        if (cjto_pertence(b->presentes, hid))
            printf(" %2d", hid);
    }
    printf(" ]\n");
}

// DEBUG: mostra habilidades de cada herói
void log_missao_hab(int t, Mission *m, Hero *h) {
    printf("%6d: MISSAO %2d HAB HEROI %2d: [", t, m->id, h->id);
    cjto_imprime(h->habs);
    printf(" ]\n");
}

// DEBUG: mostra união final de habilidades de uma base
void log_missao_uniao(int t, Mission *m, Base *b, struct cjto_t *uni) {
    printf("%6d: MISSAO %2d UNIAO HAB BASE %2d: [", t, m->id, b->id);
    cjto_imprime(uni);
    printf(" ]\n");
}

// Missão: sucesso
void log_missao_succ(int t, Mission *m, Base *b, int dist, struct lista_t *herois) {
    printf("%6d: MISSAO %2d SUCESSO BASE %2d DIST %d [",
           t, m->id, b->id, dist);
    int sz = lista_tamanho(herois);
    for (int i = 0; i < sz; i++) {
        int hid;
        lista_consulta(herois, &hid, i);
        printf(" %2d", hid);
    }
    printf(" ]\n");
}

// Missão: Composto V
void log_missao_composto(int t, Mission *m, Hero *h) {
    printf("%6d: MISSAO %2d COMPOSTO V HERO %2d\n",
           t, m->id, h->id);
}

// Missão: impossível
void log_missao_impossivel(int t, Mission *m) {
    printf("%6d: MISSAO %2d IMPOSSIVEL\n", t, m->id);
}

// Fim da simulação
void log_fim(int t) {
    printf("%6d: FIM_SIMULACAO\n", t);
}
