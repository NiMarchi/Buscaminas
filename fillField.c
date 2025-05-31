#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fillField.h"
#include "initField.h"

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