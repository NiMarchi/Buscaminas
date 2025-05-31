#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"

bool checkWin(const field_t *f, const field_t *c);
bool checkLose(const field_t *f, const field_t *c, const int *inpt);
void printFinish(SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, SDL_Color color, bool win);
void printTitle(SDL_Renderer *renderer, SDL_Surface *titleTextSurface, SDL_Texture *titleTextTexture, TTF_Font *font, SDL_Color color);
char* substring(char *destination, const char *source, int beg, int n);

#endif