#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>

#include "campo.h"

bool checkWin(const field_t *f, const field_t *c);
bool checkLose(const field_t *f, const field_t *c, const int *inpt);
char* substring(char *destination, const char *source, int beg, int n);
void setup_stage(int h, int m);
void wait_interval();
void update();
void render();
void saveEventGenericLog(const char *text);
void saveEventMouseLog(const char *text, int x, int y);
void saveBlankLineLog();

#endif