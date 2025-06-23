// src/utils.c
#include "utils.h"

int utils_rand_int(int min, int max) {
    if (max <= min) return min;
    return min + rand() % (max - min + 1);
}

int utils_distancia(Point a, Point b) {
    double dx = (double)a.x - b.x;
    double dy = (double)a.y - b.y;
    return (int)ceil(sqrt(dx*dx + dy*dy));
}
