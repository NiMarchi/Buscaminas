#pragma once
#ifndef _PRINTFIELD_H_
#define _PRINTFIELD_H_

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

void printAlert(SDL_Renderer *renderer, SDL_Surface *alertTextSurface, SDL_Texture *alertTextTexture, TTF_Font *font, SDL_Color color);
void printFinish(SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, SDL_Color color, bool win);
void printTitle(SDL_Renderer *renderer, SDL_Surface *aboutTextSurface, SDL_Texture *aboutTextTexture, TTF_Font *font, SDL_Color color);
void printTextLine(SDL_Renderer *renderer, SDL_Surface *formTextSurface, SDL_Texture *formTextTexture, TTF_Font *font, SDL_Color color, SDL_Rect rect, char *text, int r, int g, int b, int a);

#endif