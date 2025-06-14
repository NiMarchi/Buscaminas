#ifndef IMPRESION_H
#define IMPRESION_H

#include <SDL_ttf.h>

void imprimirAlerta(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color);
void imprimirFinalizar(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color, const bool victoria);
void imprimirTitulo(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color);
void imprimirLineaTexto(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color, const SDL_Rect rect, const char *texto, const int r, const int g, const int b, const int a);

#endif