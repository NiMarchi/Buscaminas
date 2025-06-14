#ifndef RESTAURAR_H
#define RESTAURAR_H

#include "campo.h"
#include "constantes.h"

#define MAX_NAME_LENGTH 32

typedef struct {
    int width;
    int height;
    int mines;
    int mineRemainingInt;
    int elapsedSeconds;
    int c_matrix[FIELD_SIZE_MAX + 2][FIELD_SIZE_MAX + 2];
    int f_matrix[FIELD_SIZE_MAX + 2][FIELD_SIZE_MAX + 2];
    char player[MAX_NAME_LENGTH];
    char elapsedTime[16];
} SaveGameData;

int guardar_partida(const field_t *f, const field_t *c, const char *player, const char *elapsedTime, int mineRemainingInt);
int cargar_partida(field_t **f, field_t **c, char *player, char *elapsedTime, int *mineRemainingInt, int *h, int *m);
int eliminar_partida_guardada();

#endif