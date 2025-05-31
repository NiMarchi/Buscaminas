#include "sumMineField.h"
#include "fillField.h"

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