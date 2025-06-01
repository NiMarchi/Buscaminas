#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "constantes.h"
#include "impresion.h"
#include "juego.h"

// Imprime una línea de alerta.
void printAlert(SDL_Renderer *renderer, SDL_Surface *alertTextSurface, SDL_Texture *alertTextTexture, TTF_Font *font, const SDL_Color color) {
	const int length = snprintf(NULL, 0, "%s", ALERT);
	char* aux = malloc(length + 1);
	const SDL_Rect alert_banner = {
		(int)ALERT_BANNER_X,
		(int)ALERT_BANNER_Y,
		(int)ALERT_BANNER_W,
		(int)ALERT_BANNER_H
	};

	snprintf(aux, length + 1, "%s", ALERT);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);

	SDL_RenderFillRect(renderer, &alert_banner);

	alertTextSurface = TTF_RenderText_Blended(font, aux, color);
	alertTextTexture = SDL_CreateTextureFromSurface(renderer, alertTextSurface);
	SDL_RenderCopy(renderer, alertTextTexture, NULL, &alert_banner);
	SDL_FreeSurface(alertTextSurface);
	SDL_DestroyTexture(alertTextTexture);
}

// Imprime una línea de victoria/derrota.
void printFinish(SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, const SDL_Color color, const bool win) {
	char *aux;
	int length;
	const SDL_Rect finish_banner = {
		(int)FINISH_BANNER_X,
		(int)FINISH_BANNER_Y,
		(int)FINISH_BANNER_W,
		(int)FINISH_BANNER_H
	};

	if (win) {
		length = snprintf(NULL, 0, "%s", WIN_TEXT);
		aux = malloc(length + 1);
		snprintf(aux, length + 1, "%s", WIN_TEXT);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 127);
	} else {
		length = snprintf(NULL, 0, "%s", LOSE_TEXT);
		aux = malloc(length + 1);
		snprintf(aux, length + 1, "%s", LOSE_TEXT);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
	}

	SDL_RenderFillRect(renderer, &finish_banner);

	finaleTextSurface = TTF_RenderText_Blended(font, aux, color);
	finaleTextTexture = SDL_CreateTextureFromSurface(renderer, finaleTextSurface);
	SDL_RenderCopy(renderer, finaleTextTexture, NULL, &finish_banner);
	SDL_FreeSurface(finaleTextSurface);
	SDL_DestroyTexture(finaleTextTexture);
}

// Imprime título con versión.
void printTitle(SDL_Renderer *renderer, SDL_Surface *aboutTextSurface, SDL_Texture *aboutTextTexture, TTF_Font *font, const SDL_Color color) {
	const int length = snprintf(NULL, 0, "%s", AUTHOR);
	char *aux = malloc(length + 1);
	const SDL_Rect title_banner = {
		(int)TITLE_X,
		(int)TITLE_Y,
		(int)TITLE_W,
		(int)TITLE_H
	};

	snprintf(aux, length + 1, "%s", AUTHOR);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &title_banner);

	aboutTextSurface = TTF_RenderText_Blended(font, aux, color);
	aboutTextTexture = SDL_CreateTextureFromSurface(renderer, aboutTextSurface);
	SDL_RenderCopy(renderer, aboutTextTexture, NULL, &title_banner);
	SDL_FreeSurface(aboutTextSurface);
	SDL_DestroyTexture(aboutTextTexture);
}

void printTextLine(SDL_Renderer *renderer, SDL_Surface *formTextSurface, SDL_Texture *formTextTexture, TTF_Font *font, const SDL_Color color, const SDL_Rect rect, char *text, const int r, const int g, const int b, const int a) {
	const int length = snprintf(NULL, 0, "%s", text);
	char *aux = malloc(length + 1);

	snprintf(aux, length + 1, "%s", text);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);

	formTextSurface = TTF_RenderText_Blended(font, aux, color);
	formTextTexture = SDL_CreateTextureFromSurface(renderer, formTextSurface);
	SDL_RenderCopy(renderer, formTextTexture, NULL, &rect);
	SDL_FreeSurface(formTextSurface);
	SDL_DestroyTexture(formTextTexture);
}