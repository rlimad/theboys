// includes/utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include "entity.h"

// Retorna um inteiro aleatório no intervalo [min, max]
int utils_rand_int(int min, int max);

// Calcula a distância Euclidiana (arredondada para cima) entre dois pontos
int utils_distancia(Point a, Point b);

#endif // UTILS_H
