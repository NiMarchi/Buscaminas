#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "constantes.h"
#include "impresion.h"

// Función auxiliar para renderizar texto sobre un rectángulo.
void renderTextBox(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color color, const SDL_Rect *rect, const char *text, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, rect);

	SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
	if (!surface) return;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_FreeSurface(surface);
		return;
	}

	SDL_RenderCopy(renderer, texture, NULL, rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

// Imprime una línea de alerta.
void printAlert(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color color) {
	const SDL_Rect alert_banner = {
		ALERT_BANNER_X, ALERT_BANNER_Y,
		ALERT_BANNER_W, ALERT_BANNER_H
	};
	renderTextBox(renderer, font, color, &alert_banner, ALERT, 255, 0, 0, 127);
}

// Imprime una línea de victoria/derrota.
void printFinish(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color color, const bool win) {
	const SDL_Rect finish_banner = {
		FINISH_BANNER_X, FINISH_BANNER_Y,
		FINISH_BANNER_W, FINISH_BANNER_H
	};

	if (win) {
		renderTextBox(renderer, font, color, &finish_banner, WIN_TEXT, 0, 255, 0, 127);
	} else {
		renderTextBox(renderer, font, color, &finish_banner, LOSE_TEXT, 255, 0, 0, 127);
	}
}

// Imprime título con versión.
void printTitle(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color color) {
	const SDL_Rect title_banner = {
		TITLE_X, TITLE_Y,
		TITLE_W, TITLE_H
	};
	renderTextBox(renderer, font, color, &title_banner, AUTHOR, 0, 0, 0, 0);
}

// Imprime una línea de texto genérica.
void printTextLine(SDL_Renderer *renderer, TTF_Font *font, const SDL_Color color, const SDL_Rect rect, const char *text, const int r, const int g, const int b, const int a) {
	renderTextBox(renderer, font, color, &rect, text, r, g, b, a);
}