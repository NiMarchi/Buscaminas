#include <stdlib.h>
#include <time.h>

#include "campo.h"

struct tile tile;

// Reserva memoria para una matriz (x filas, y columnas) y la inicializa a cero.
static int **allocMatrix(const int rows, const int cols) {
    int **mat = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        mat[i] = calloc(cols, sizeof(int)); // calloc = inicializa a 0.
    }
    return mat;
}

// Inicializa el campo (Con bordes +2).
field_t *initField(const int x, const int y, const int mines) {
    field_t *field = malloc(sizeof(field_t));
    field->x = x + 2;
    field->y = y + 2;
    field->m = mines;
    field->mat = allocMatrix(field->x, field->y);
    return field;
}

// Inicializa el campo de cobertura (Sin minas).
field_t *initCover(const int x, const int y) {
    return initField(x, y, 0);
}

// Llena bordes con constantes específicas.
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

// Llena el campo con ceros (BLANK).
void fillFieldZero(const field_t *f) {
    for (int i = 0; i < f->x; i++) {
        for (int j = 0; j < f->y; j++) {
            f->mat[i][j] = BLANK;
        }
    }
}

// Llena el campo con minas, evitando bordes y duplicados.
void fillFieldMine(const field_t *f) {
    srand(time(NULL));
    int placed = 0;
    while (placed < f->m) {
        const int x = 1 + rand() % (f->x - 2);
        const int y = 1 + rand() % (f->y - 2);
        if (f->mat[x][y] != MINE) {
            f->mat[x][y] = MINE;
            placed++;
        }
    }
}

// Cuenta minas adyacentes y coloca pistas en casillas.
void countMines(const field_t *f) {
    for (int i = 1; i < f->x - 1; i++) {
        for (int j = 1; j < f->y - 1; j++) {
            if (f->mat[i][j] == MINE || f->mat[i][j] == EDGE_T_B || f->mat[i][j] == EDGE_L_R) {
                continue;
            }
            int count = 0;
            for (int k = 0; k < 8; k++) {
                // Desplazamientos para los 8 vecinos.
                const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
                const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

                const int nx = i + dx[k];
                const int ny = j + dy[k];
                if (f->mat[nx][ny] == MINE) {
                    count++;
                }
            }
            f->mat[i][j] = count;
        }
    }
}

// Cubre el campo con COVER (Solo zona interior).
void fillFieldCover(const field_t *f) {
    for (int i = 1; i < f->x - 1; i++) {
        for (int j = 1; j < f->y - 1; j++) {
            f->mat[i][j] = COVER;
        }
    }
}

// Función auxiliar para abrir casillas vacías recursivamente (Relleno por inundación).
void openFieldUtil(field_t *f, field_t *c, const int x, const int y, const int old) {
    if (x < 1 || x >= c->x - 1 || y < 1 || y >= c->y - 1) {
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

    openFieldUtil(f, c, x + 1, y, old);
    openFieldUtil(f, c, x - 1, y, old);
    openFieldUtil(f, c, x, y + 1, old);
    openFieldUtil(f, c, x, y - 1, old);
}

// Abre una casilla y expande si es necesario; marca/desmarca banderas.
void openField(field_t *f, field_t *c, const int x, const int y, const int flag, int *flagQty) {
    if (flag == 1 && c->mat[x][y] == COVER) {
        const int old = c->mat[x][y];
        if (old == f->mat[x][y]) {
            return;
        }
        openFieldUtil(f, c, x, y, old);
    } else if (flag == 2) {
        if (c->mat[x][y] != FLAG && c->mat[x][y] != f->mat[x][y]) {
            c->mat[x][y] = FLAG;
            (*flagQty)--;
        } else if (c->mat[x][y] == FLAG) {
            c->mat[x][y] = COVER;
            (*flagQty)++;
        }
    }
}