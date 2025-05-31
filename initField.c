#include <stdlib.h>
#include "initField.h"

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