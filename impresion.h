#ifndef IMPRESION_H
#define IMPRESION_H

#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>

void printAlert(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
void printFinish(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, bool win);
void printTitle(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
void printTextLine(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect rect, const char *text, int r, int g, int b, int a);

#endif