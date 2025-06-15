#ifndef IMPRESION_H
#define IMPRESION_H

#include <SDL_ttf.h>

void printAlert(SDL_Renderer *rend, TTF_Font *font, const SDL_Color color);
void printFinish(SDL_Renderer *rend, TTF_Font *font, const SDL_Color color, const bool win);
void printTitle(SDL_Renderer *rend, TTF_Font *font, const SDL_Color color);
void printTextLine(SDL_Renderer *rend, TTF_Font *font, const SDL_Color color, const SDL_Rect rect, const char *text, const int r, const int g, const int b, const int a);

#endif