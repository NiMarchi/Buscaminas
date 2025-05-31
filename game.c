#include <stdbool.h>
#include "game.h"
#include "initField.h"
#include "fillField.h"

// Comprueba si el jugador venció el juego.
bool checkWin(const field_t *f, const field_t *c) {
	// Si cada casilla superior está cubierta Y la inferior es una mina, O si la casilla superior tiene una bandera Y la inferior es una mina, O si la casilla superior es igual a la inferior, CONTINÚA. Si no, el jugador aún no ha ganado.
	for (int i = 1; i < c->x - 1; i++) {
		for (int j = 1; j < c->y - 1; j++) {
			if ((c->mat[i][j] == COVER && f->mat[i][j] == MINE) || (c->mat[i][j] == FLAG && f->mat[i][j] == MINE) || (c->mat[i][j] == f->mat[i][j])) {
				continue;
			}
			return false;
		}
	}
	return true;
}

// Comprueba si el jugador perdió el juego.
bool checkLose(const field_t *f, const field_t *c, const int *inpt) {
	// Si la casilla inferior es una mina Y la casilla superior no es una bandera Y el jugador eligió 'abrir' como movimiento, el jugador pierde.
	if (f->mat[inpt[0]][inpt[1]] == MINE && c->mat[inpt[0]][inpt[1]] != FLAG && inpt[2] == OPEN_F) {
		return true;
	}
	return false;
}

// Función para implementar la función de subcadena en C.
char *substring(char *destination, const char *source, const int beg, int n) {
	// Extrae n caracteres de la cadena fuente comenzando desde el índice beg y los copia en la cadena de destino.
	while (n > 0) {
		*destination = *(source + beg);
		destination++;
		source++;
		n--;
	}

	// Cadena de destino de terminación nula.
	*destination = '\0';

	// Devolver la cadena de destino.
	return destination;
}