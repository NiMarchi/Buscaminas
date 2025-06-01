#include <stdlib.h>
#include <time.h>

#include "campo.h"

struct tile tile;

// Asigna el campo inferior, donde se escribirán las minas y las puntas.
field_t* initField(const int x, const int y, const int mines) {
    field_t *fieldShow = malloc(sizeof(field_t));
    fieldShow->x = x + 2;
    fieldShow->y = y + 2;
    fieldShow->m = mines;
    fieldShow->mat = (int**)malloc((fieldShow->x) * sizeof(int*));
    for (int i = 0; i < fieldShow->x; i++) {
        fieldShow->mat[i] = (int*)malloc((fieldShow->y) * sizeof(int));
    }
    return fieldShow;
}

// Asigna el campo superior, donde el jugador se descubrirá.
field_t* initCover(const int x, const int y) {
    field_t *fieldHide = malloc(sizeof(field_t));
    fieldHide->x = x + 2;
    fieldHide->y = y + 2;
    fieldHide->m = 0;
    fieldHide->mat = (int**)malloc((fieldHide->x) * sizeof(int*));
    for (int i = 0; i < fieldHide->x; i++) {
        fieldHide->mat[i] = (int*)malloc((fieldHide->y) * sizeof(int));
    }
    return fieldHide;
}

// Calcula la cantidad de minas y coloca pistas en las casillas vecinas a las minas.
void countMines(const field_t *f) {
    for (int i = 0; i < f->x; i++) {
        for (int j = 0; j < f->y; j++) {
            // Si la casilla inferior no está en el borde y no es una mina, entonces se incrementa el contador de minas de cada casilla adyacente a ella.
            if (f->mat[i][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j] != MINE) {
                if (f->mat[i][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j + 1] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j - 1] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i + 1][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i - 1][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i + 1][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j + 1] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i - 1][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j - 1] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i - 1][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j + 1] == MINE) {
                    f->mat[i][j]++;
                }
                if (f->mat[i + 1][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j - 1] == MINE) {
                    f->mat[i][j]++;
                }
            }
        }
    }
}

// Rellena el campo con ceros.
void fillFieldZero(const field_t *f) {
    for (int i = 0; i < f->x; i++) {
        for (int j = 0; j < f->y; j++) {
            f->mat[i][j] = BLANK;
        }
    }
}

// Llena el campo con caracteres de borde.
void fillFieldEdge(const field_t *f) {
    for (int i = 0; i < f->x; i++) {
        f->mat[i][0] = EDGE_T_B;
        f->mat[i][f->y - 1] = EDGE_T_B;
    }
    for (int j = 0; j < f->y; j++) {
        f->mat[0][j] = EDGE_L_R;
        f->mat[f->x - 1][j] = EDGE_L_R;
    }
}

// Llena el campo con minas.
void fillFieldMine(const field_t *f) {
    srand(time(NULL));
    for (int i = 0; i < f->m; i++) {
        const int x = rand() % f->x; // Coloca minas aleatoriamente en x.
        const int y = rand() % f->y; // Coloca minas aleatoriamente en y.

        // Coloca minas en posiciones que no sean bordes, que no tengan ya una mina y que no sean la casilla donde el jugador hizo su primer movimiento.
        if (f->mat[x][y] != MINE && f->mat[x][y] != EDGE_T_B && f->mat[x][y] != EDGE_L_R) {
            f->mat[x][y] = MINE;
        } else {
            i--;
        }
    }
}

// Cubre el campo con caracteres ocultos.
void fillFieldCover(const field_t *f) {
    for (int i = 1; i < f->x - 1; i++) {
        for (int j = 1; j < f->y - 1; j++) {
            f->mat[i][j] = COVER;
        }
    }
}

// Función auxiliar para llenado por inundación.
void openFieldUtil(field_t *f, field_t *c, const int x, const int y, const int old) {
    if (x < 0 || x >= c->x || y < 0 || y >= c->y) {
        return;
    }
    if (c->mat[x][y] == FLAG) {
        return;
    }
    if (c->mat[x][y] != old) {
        return;
    }
    if (c->mat[x][y] == f->mat[x][y]) {
        return;
    }
    if (f->mat[x][y] == MINE) {
        c->mat[x][y] = f->mat[x][y];
        return;
    }
    if (f->mat[x][y] != 0) {
        c->mat[x][y] = f->mat[x][y];
        return;
    }

    c->mat[x][y] = f->mat[x][y];

    // Recursión para relleno de inundaciones.
    openFieldUtil(f, c, x + 1, y, old);
    openFieldUtil(f, c, x - 1, y, old);
    openFieldUtil(f, c, x, y + 1, old);
    openFieldUtil(f, c, x, y - 1, old);
}

// Abre la ranura y todas las ranuras vacías (cero) adyacentes a ella (relleno de inundación).
void openField(field_t *f, field_t *c, const int x, const int y, const int flag, int *flagQty) {
    // Si el jugador se abre y la ranura superior es igual a la cubierta.
    if (flag == 1 && c->mat[x][y] == COVER) {
        const int old = c->mat[x][y];
        if (old == f->mat[x][y]) {
            return;
        }
        openFieldUtil(f, c, x, y, old);
    } else if (flag == 2 && c->mat[x][y] != FLAG && c->mat[x][y] != f->mat[x][y]) {
        // De lo contrario, si el jugador marca la ranura superior y no está marcada, marca.
        c->mat[x][y] = FLAG;
        (*flagQty)--;
    } else if (flag == 2 && c->mat[x][y] != COVER && c->mat[x][y] != FLAG && c->mat[x][y] != MINE) {
        // De lo contrario, si el jugador marca la ranura superior, y no está cubierta Y no está marcada Y no es mía, no hagas nada.
        c->mat[x][y] = c->mat[x][y];
    } else if (flag == 2 && c->mat[x][y] == FLAG) {
        // De lo contrario, si el jugador marca la ranura superior, y ya está marcada, la cubre nuevamente.
        c->mat[x][y] = COVER;
        (*flagQty)++;
    }
}