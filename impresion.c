#include <stdbool.h>

#include "constantes.h"
#include "impresion.h"

// Función auxiliar para renderizar texto sobre un rectángulo.
void renderTextBox(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color, const SDL_Rect *rect, const char *texto, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
	SDL_SetRenderDrawColor(rend, r, g, b, a);
	SDL_RenderFillRect(rend, rect);

	SDL_Surface *superficie = TTF_RenderText_Blended(fuente, texto, color);
	if (!superficie) return;

	SDL_Texture *textura = SDL_CreateTextureFromSurface(rend, superficie);
	if (!textura) {
		SDL_FreeSurface(superficie);
		return;
	}

	SDL_RenderCopy(rend, textura, NULL, rect);
	SDL_FreeSurface(superficie);
	SDL_DestroyTexture(textura);
}

// Imprime una línea de alerta.
void imprimirAlerta(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color) {
	const SDL_Rect alerta_banner = {
		ALERTA_BANNER_X, ALERTA_BANNER_Y,
		ALERTA_BANNER_W, ALERTA_BANNER_H
	};
	renderTextBox(rend, fuente, color, &alerta_banner, ALERT, 255, 0, 0, 127);
}

// Imprime una línea de victoria/derrota.
void imprimirFinalizar(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color, const bool victoria) {
	const SDL_Rect finalizar_banner = {
		FINALIZAR_BANNER_X, FINALIZAR_BANNER_Y,
		FINALIZAR_BANNER_W, FINALIZAR_BANNER_H
	};

	if (victoria) {
		renderTextBox(rend, fuente, color, &finalizar_banner, VICTORIA_TEXTO, 0, 255, 0, 127);
	} else {
		renderTextBox(rend, fuente, color, &finalizar_banner, DERROTA_TEXTO, 255, 0, 0, 127);
	}
}

// Imprime título con versión.
void imprimirTitulo(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color) {
	const SDL_Rect titulo_banner = {
		TITULO_X, TITULO_Y,
		TITULO_W, TITLE_H
	};
	renderTextBox(rend, fuente, color, &titulo_banner, AUTOR, 0, 0, 0, 0);
}

// Imprime una línea de texto genérica.
void imprimirLineaTexto(SDL_Renderer *rend, TTF_Font *fuente, const SDL_Color color, const SDL_Rect rect, const char *texto, const int r, const int g, const int b, const int a) {
	renderTextBox(rend, fuente, color, &rect, texto, r, g, b, a);
}