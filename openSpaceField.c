#include "openSpaceField.h"
#include "fillField.h"

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