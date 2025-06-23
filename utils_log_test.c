// utils_log_test.c
#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "log.h"
#include "entity.h"  // para Point, Hero e Base

int main(void) {
    // Testa util_distancia
    Point a = {0, 0}, b = {3, 4};
    printf("dist = %d\n", utils_distancia(a, b));  // espera 5

    // Testa log_chega
    Hero dummy = {
        .id = 7,
        .habs = NULL,
        .paciencia = 0,
        .velocidade = 0,
        .experiencia = 0,
        .base_atual = 0,
        .vivo = true
    };
    Hero *h = &dummy;

    Base base = {
        .id = 2,
        .lotacao = 1,
        .presentes = NULL,
        .espera = NULL,
        .local = {0,0},
        .fila_max = 0,
        .missoes = 0
    };
    Base *B = &base;

    log_chega(10, h, B, true);
    return 0;
}
