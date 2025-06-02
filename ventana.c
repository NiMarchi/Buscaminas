#include <stdio.h>
#include <stdbool.h>

#include <SDL_image.h>

#include "constantes.h"
#include "variables.h"

// Inicializa la ventana.
bool initialize_window() {
	// Inicializa SDL, si no puede, emite un mensaje de error.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, ERROR_SDL_INIT);
		return false;
	}

	// Crear ventana con esos parámetros.
	window = SDL_CreateWindow(
		WINDOW_TITLE, // Título en la barra de título.
		SDL_WINDOWPOS_CENTERED, // Posición de la pantalla X.
		SDL_WINDOWPOS_CENTERED, // Posición de la pantalla Y.
		WINDOW_WIDTH, // Ancho de ventana.
		WINDOW_HEIGHT, // Altura de la ventana.
		SDL_WINDOW_ALLOW_HIGHDPI // Bandera para soporte HiDPI.
	);

	// Si no puede crear la ventana, aparecerá un mensaje de error.
	if (!window) {
		fprintf(stderr, ERROR_SDL_WINDOW);
		return false;
	}

	// Inicializa el renderizado SDL.
	renderer = SDL_CreateRenderer(window, -1, 0);

	// Si no puede crear el renderizado, aparecerá un mensaje de error.
	if (!renderer) {
		fprintf(stderr, ERROR_SDL_RENDERER);
		return false;
	}

	// Inicializa font_main a través de SDL_TTF.
	if (TTF_Init() != 0) {
		fprintf(stderr, ERROR_SDL_TTF);
		return false;
	}
	font_main = TTF_OpenFont(FONT_PATH_MAIN, FONT_SIZE);
	font_secondary = TTF_OpenFont(FONT_PATH_SECONDARY, FONT_SIZE);

	// Establece el canal alfa para la fusión.
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Cargar soporte para formatos de imagen PNG.
	const int flags = IMG_INIT_PNG;
	const int started = IMG_Init(flags);
	if ((started & flags) != flags) {
		fprintf(stderr, ERROR_SDL_INIT_PNG);
		return false;
	}

	// Establece íconos e imágenes para los activos.
	bgScrollSurface = IMG_Load(BACKGROUND_WALLPAPER);
	menuPresentationSurface = IMG_Load(PRESENTATION);
	menuButtonTextSurface1 = IMG_Load(NEW_OK_BUTTON);
	menuButtonTextSurface2 = IMG_Load(QUIT_BUTTON);
	okButtonSurface = IMG_Load(NEW_OK_BUTTON);
	mineBoomIconSurface = IMG_Load(MINE_BOOM_ICON);
	mineDeathIconSurface = IMG_Load(MINE_DEATH_ICON);
	edgeIconSurface = IMG_Load(EDGE_ICON);
	flagIconSurface = IMG_Load(FLAG_ICON);
	coverIconSurface = IMG_Load(COVER_ICON);

	bgScrollTexture = SDL_CreateTextureFromSurface(renderer, bgScrollSurface);
	menuPresentationTexture = SDL_CreateTextureFromSurface(renderer, menuPresentationSurface);
	menuButtonTextTexture1 = SDL_CreateTextureFromSurface(renderer, menuButtonTextSurface1);
	menuButtonTextTexture2 = SDL_CreateTextureFromSurface(renderer, menuButtonTextSurface2);
	okButtonTexture = SDL_CreateTextureFromSurface(renderer, okButtonSurface);
	mineDeathIconTexture = SDL_CreateTextureFromSurface(renderer, mineDeathIconSurface);
	edgeIconTexture = SDL_CreateTextureFromSurface(renderer, edgeIconSurface);
	flagIconTexture = SDL_CreateTextureFromSurface(renderer, flagIconSurface);
	coverIconTexture = SDL_CreateTextureFromSurface(renderer, coverIconSurface);
	mineBoomIconTexture = SDL_CreateTextureFromSurface(renderer, mineBoomIconSurface);

	// Si no puede crear audio, aparecerá un mensaje de error.
	if (Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, ERROR_SDL_AUDIO);
		return false;
	}
	backgroundMusicMenu = Mix_LoadMUS(BACKGROUND_MUSIC_MENU); // Loads background music in menus.
	backgroundMusicStage = Mix_LoadMUS(BACKGROUND_MUSIC_STAGE); // Loads background music in stage.
	soundEffectL = Mix_LoadWAV(SOUND_EFFECT_L); // Loads sound effect for left mouse click.
	soundEffectR = Mix_LoadWAV(SOUND_EFFECT_R); // Loads sound effect for right mouse click.
	soundEffectMenu = Mix_LoadWAV(SOUND_EFFECT_MENU); // Loads sound effect for menu hover/click.
	soundEffectMine = Mix_LoadWAV(SOUND_EFFECT_MINE); // Loads sound effect for defeat.
	soundEffectVictory = Mix_LoadWAV(SOUND_EFFECT_VICTORY); // Loads sound effect for victory.

	// Inicializa las coordenadas de la animación de fondo.
	bgScrollRect1.x = 0;
	bgScrollRect1.y = 0;
	bgScrollRect1.w = WINDOW_WIDTH;
	bgScrollRect1.h = WINDOW_HEIGHT;
	bgScrollRect2.x = 0;
	bgScrollRect2.y = -WINDOW_HEIGHT;
	bgScrollRect2.w = WINDOW_WIDTH;
	bgScrollRect2.h = WINDOW_HEIGHT;

	// Inicializa las coordenadas de información de cantidad de minas en el juego.
	infoRect.x = 0;
	infoRect.y = 0;
	infoRect.w = 200;
	infoRect.h = 20;

	// Devuelve verdadero si la función inicializó correctamente.
	return true;
}

// Cierra la ventana, libera memoria y finaliza el proceso.
void destroy_window() {
	SDL_FreeSurface(aboutTextSurface);
	SDL_FreeSurface(bgScrollSurface);
	SDL_FreeSurface(menuPresentationSurface);
	SDL_FreeSurface(menuButtonTextSurface1);
	SDL_FreeSurface(menuButtonTextSurface2);
	SDL_FreeSurface(labelSurface);
	SDL_FreeSurface(textInputSurface);
	SDL_FreeSurface(okButtonSurface);
	SDL_FreeSurface(alertTextSurface);
	SDL_FreeSurface(mineIconSurface);
	SDL_FreeSurface(mineBoomIconSurface);
	SDL_FreeSurface(mineDeathIconSurface);
	SDL_FreeSurface(flagIconSurface);
	SDL_FreeSurface(edgeIconSurface);
	SDL_FreeSurface(coverIconSurface);
	SDL_FreeSurface(finaleTextSurface);

	TTF_CloseFont(font_main);
	TTF_CloseFont(font_secondary);
	TTF_Quit();

	Mix_FreeMusic(backgroundMusicMenu);
	Mix_FreeMusic(backgroundMusicStage);
	Mix_FreeChunk(soundEffectL);
	Mix_FreeChunk(soundEffectR);
	Mix_FreeChunk(soundEffectMenu);
	Mix_FreeChunk(soundEffectMine);
	Mix_FreeChunk(soundEffectVictory);
	Mix_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}