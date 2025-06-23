// src/log.c
#include "log.h"

void log_chega(int t, Hero *h, Base *b, bool espera) {
    printf("%4d: CHEGADA   H%d → B%d : %s\n",
           t, h->id, b->id, espera ? "espera" : "desiste");
}

void log_espera(int t, Hero *h, Base *b, int sz) {
    printf("%4d: ESPERA   H%d em B%d : fila=%d\n",
           t, h->id, b->id, sz);
}

void log_desiste(int t, Hero *h, Base *b) {
    printf("%4d: DESISTE  H%d de B%d\n",
           t, h->id, b->id);
}

void log_avisa_inicio(int t, Base *b, int pres, int lot, struct lista_t *fila) {
    printf("%4d: AVISA    B%d : %d/%d ocupação, fila=%d\n",
           t, b->id, pres, lot, lista_tamanho(fila));
}

void log_avisa_admite(int t, Base *b, int hid) {
    printf("%4d: ADMITIR  H%d em B%d\n",
           t, hid, b->id);
}

void log_entra(int t, Hero *h, Base *b, int pres, int lot) {
    printf("%4d: ENTRA    H%d em B%d : %d/%d\n",
           t, h->id, b->id, pres, lot);
}

void log_sai(int t, Hero *h, Base *b, int pres, int lot) {
    printf("%4d: SAI      H%d de B%d : %d/%d\n",
           t, h->id, b->id, pres, lot);
}

void log_viaja(int t, Hero *h, Base *o, Base *d, int dist, int vel, int t2) {
    printf("%4d: VIAJA    H%d de B%d → B%d : dist=%d, vel=%d, chega=%d\n",
           t, h->id, o->id, d->id, dist, vel, t2);
}

void log_morre(int t, Hero *h, Mission *m) {
    printf("%4d: MORRE    H%d na M%d\n",
           t, h->id, m->id);
}

void log_missao_req(int t, Mission *m) {
    printf("%4d: MISSÃO?  M%d @(%d,%d)\n",
           t, m->id, m->local.x, m->local.y);
}

void log_missao_base(int t, Mission *m, Base *b, int dist, struct lista_t *fila) {
    printf("%4d: CHECA    M%d em B%d : dist=%d, fila=%d\n",
           t, m->id, b->id, dist, lista_tamanho(fila));
}

void log_missao_succ(int t, Mission *m, Base *b, int dist, struct lista_t *v) {
    printf("%4d: SUCESSO  M%d em B%d : dist=%d, equipe=",
           t, m->id, b->id, dist);
    // imprime IDs na lista v
    for (int i = 0; i < lista_tamanho(v); i++) {
        int id; lista_consulta(v, &id, i);
        printf("H%d ", id);
    }
    printf("\n");
}

void log_missao_composto(int t, Mission *m, Hero *h) {
    printf("%4d: COMPOSTO M%d com H%d\n",
           t, m->id, h->id);
}

void log_missao_impossivel(int t, Mission *m) {
    printf("%4d: IMPOSSÍVEL M%d : reagendado\n",
           t, m->id);
}

void log_fim(int t) {
    printf("%4d: FIM_SIM  tempo=%d\n", t, t);
}
