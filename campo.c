#include <stdlib.h>
#include <time.h>

#include "campo.h"
#include "constantes.h"

struct azulejo azulejo;

// Reserva memoria para una matriz (x filas, y columnas) y la inicializa a cero.
static int **matrizAsignacion(const int f, const int c) {
    int **mat = malloc(f * sizeof(int *));
    for (int i = 0; i < f; i++) {
        mat[i] = calloc(c, sizeof(int));
    }
    return mat;
}

// Inicializa el campo (Con bordes +2).
campo_t *iniciarCampo(const int x, const int y, const int minas) {
    campo_t *campo = malloc(sizeof(campo_t));
    campo->x = x + 2;
    campo->y = y + 2;
    campo->m = minas;
    campo->mat = matrizAsignacion(campo->x, campo->y);
    return campo;
}

// Inicializa el campo de cobertura (Sin minas).
campo_t *iniciarCobertura(const int x, const int y) {
    return iniciarCampo(x, y, 0);
}

// Llena bordes con constantes específicas.
void rellenarBordeCampo(const campo_t *f) {
    for (int i = 0; i < f->x; i++) {
        f->mat[i][0] = BORDE_T_B;
        f->mat[i][f->y - 1] = BORDE_T_B;
    }
    for (int j = 0; j < f->y; j++) {
        f->mat[0][j] = BORDE_L_R;
        f->mat[f->x - 1][j] = BORDE_L_R;
    }
}

// Llena el campo con ceros (Blanco).
void rellenarCampoCero(const campo_t *f) {
    for (int i = 0; i < f->x; i++) {
        for (int j = 0; j < f->y; j++) {
            f->mat[i][j] = BLANCO;
        }
    }
}

// Llena el campo con minas, evitando bordes y duplicados.
void rellenarCampoMinas(const campo_t *f) {
    srand(time(NULL));
    int cant = 0;
    while (cant < f->m) {
        const int x = 1 + rand() % (f->x - 2);
        const int y = 1 + rand() % (f->y - 2);
        if (f->mat[x][y] != MINA) {
            f->mat[x][y] = MINA;
            cant++;
        }
    }
}

// Cuenta minas adyacentes y coloca pistas en casillas.
void contarMinas(const campo_t *f) {
    for (int i = 1; i < f->x - 1; i++) {
        for (int j = 1; j < f->y - 1; j++) {
            if (f->mat[i][j] == MINA || f->mat[i][j] == BORDE_T_B || f->mat[i][j] == BORDE_L_R) {
                continue;
            }
            int cant = 0;
            for (int k = 0; k < 8; k++) {
                // Desplazamientos para los 8 vecinos.
                const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
                const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

                const int nx = i + dx[k];
                const int ny = j + dy[k];
                if (f->mat[nx][ny] == MINA) {
                    cant++;
                }
            }
            f->mat[i][j] = cant;
        }
    }
}

// Cubre el campo con COVER (Solo zona interior).
void rellenarCampoCobertura(const campo_t *f) {
    for (int i = 1; i < f->x - 1; i++) {
        for (int j = 1; j < f->y - 1; j++) {
            f->mat[i][j] = CUBRIR;
        }
    }
}

// Función auxiliar para abrir casillas vacías recursivamente (Relleno por inundación).
void abrirCampoUtil(campo_t *f, campo_t *c, const int x, const int y, const int viejo) {
    if (x < 1 || x >= c->x - 1 || y < 1 || y >= c->y - 1) {
        return;
    }
    if (c->mat[x][y] == BANDERA) {
        return;
    }
    if (c->mat[x][y] != viejo) {
        return;
    }
    if (c->mat[x][y] == f->mat[x][y]) {
        return;
    }
    if (f->mat[x][y] == MINA) {
        c->mat[x][y] = f->mat[x][y];
        return;
    }
    if (f->mat[x][y] != 0) {
        c->mat[x][y] = f->mat[x][y];
        return;
    }

    c->mat[x][y] = f->mat[x][y];

    abrirCampoUtil(f, c, x + 1, y, viejo);
    abrirCampoUtil(f, c, x - 1, y, viejo);
    abrirCampoUtil(f, c, x, y + 1, viejo);
    abrirCampoUtil(f, c, x, y - 1, viejo);
}

// Abre una casilla y expande si es necesario; marca/desmarca banderas.
void abrirCampo(campo_t *f, campo_t *c, const int x, const int y, const int band, int *bandCant) {
    if (band == 1 && c->mat[x][y] == CUBRIR) {
        const int viejo = c->mat[x][y];
        if (viejo == f->mat[x][y]) {
            return;
        }
        abrirCampoUtil(f, c, x, y, viejo);
    } else if (band == 2) {
        if (*bandCant > 0 && c->mat[x][y] != BANDERA && c->mat[x][y] != f->mat[x][y]) {
            c->mat[x][y] = BANDERA;
            (*bandCant)--;
        } else if (c->mat[x][y] == BANDERA) {
            c->mat[x][y] = CUBRIR;
            (*bandCant)++;
        }
    }
}