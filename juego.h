#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>
#include "campo.h"

bool checkWin(const field_t *f, const field_t *c);
bool checkLose(const field_t *f, const field_t *c, const int *inpt);
char* substring(char *destination, const char *source, const int beg, const int n);
void setupStage(const int h, const int m);
void waitInterval();
void update();
void render();

#endif