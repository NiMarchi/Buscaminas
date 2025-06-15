#include <stdio.h>
#include <stdbool.h>
#include <SDL_image.h>
#include "logs.h"
#include "variables.h"

// Carga imagen y devuelve superficie.
SDL_Surface* loadSurface(const char* path) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		fprintf(stderr, "Error al cargar imagen %s: %s\n", path, IMG_GetError());
	}
	return surface;
}

// Crea textura desde superficie.
SDL_Texture* createTexture(SDL_Surface* surface) {
	if (!surface) {
		return NULL;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		fprintf(stderr, "Error al crear textura: %s\n", SDL_GetError());
	}
	return texture;
}

// Inicializa todos los subsistemas SDL necesarios.
bool initializeSDLSystems() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
		return false;
	}
	if (TTF_Init() != 0) {
		fprintf(stderr, "Error al inicializar SDL_ttf: %s\n", TTF_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		fprintf(stderr, "Error al inicializar SDL_image: %s\n", IMG_GetError());
		return false;
	}
	if (Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, "Error al inicializar SDL_mixer: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

// Función principal de inicialización.
bool initializeWindow() {
	logs = fopen("logs.txt", "a");
	if (logs == NULL) {
		perror("Error al intentar abrir el archivo de logs");
		return false;
	}

	if (!initializeSDLSystems()) {
		return false;
	}

	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
		fprintf(stderr, "Error al crear la ventana: %s\n", SDL_GetError());
		return false;
	}

	// Establecer icono de la ventana.
	SDL_Surface* iconSurface = loadSurface(WINDOW_ICON);  // Asegúrate de definir WINDOW_ICON correctamente
	if (iconSurface) {
		SDL_SetWindowIcon(window, iconSurface);
		SDL_FreeSurface(iconSurface);
	} else {
		fprintf(stderr, "Error al cargar el icono de la ventana: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Error al crear el renderer: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Carga fuentes.
	font_main = TTF_OpenFont(FONT_PATH_MAIN, FONT_SIZE);
	font_secondary = TTF_OpenFont(FONT_PATH_SECONDARY, FONT_SIZE);
	if (!font_main || !font_secondary) {
		fprintf(stderr, "Error al cargar fuentes.\n");
		return false;
	}

	// Carga superficies y texturas.
	bgScrollTexture = createTexture(loadSurface(BACKGROUND_WALLPAPER));
	menuButtonTextTexture1 = createTexture(loadSurface(NEW_OK_BUTTON));
	menuButtonTextTexture3 = createTexture(loadSurface(NEW_OK_BUTTON));
	menuButtonTextTexture4 = createTexture(loadSurface(NEW_OK_BUTTON));
	menuButtonTextTexture2 = createTexture(loadSurface(QUIT_BUTTON));
	okButtonTexture = createTexture(loadSurface(NEW_OK_BUTTON));
	mineBoomIconTexture = createTexture(loadSurface(MINE_BOOM_ICON));
	mineDeathIconTexture = createTexture(loadSurface(MINE_DEATH_ICON));
	edgeIconTexture = createTexture(loadSurface(EDGE_ICON));
	flagIconTexture = createTexture(loadSurface(FLAG_ICON));
	coverIconTexture = createTexture(loadSurface(COVER_ICON));

	// Carga música y efectos de sonido.
	backgroundMusicMenu = Mix_LoadMUS(BACKGROUND_MUSIC_MENU);
	backgroundMusicStage = Mix_LoadMUS(BACKGROUND_MUSIC_STAGE);
	soundEffectL = Mix_LoadWAV(SOUND_EFFECT_L);
	soundEffectR = Mix_LoadWAV(SOUND_EFFECT_R);
	soundEffectMenu = Mix_LoadWAV(SOUND_EFFECT_MENU);
	soundEffectMine = Mix_LoadWAV(SOUND_EFFECT_MINE);
	soundEffectVictory = Mix_LoadWAV(SOUND_EFFECT_VICTORY);

	// Inicializa rectángulos.
	bgScrollRect1 = (SDL_Rect){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	bgScrollRect2 = (SDL_Rect){0, -WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT};
	infoRect = (SDL_Rect){0, 0, 250, 20};
	infoPlayer = (SDL_Rect){0, 20, 250, 20};
	infoTimeRect = (SDL_Rect){0, 40, 250, 20};
	infoTrampRect = (SDL_Rect){0, 60, 250, 20};

	saveEventGenericLog("Inicialización de Recursos");

	return true;
}

// Cierra la ventana, libera memoria y finaliza el proceso.
void destroyWindow() {
	saveEventGenericLog("Liberación de Recursos");
	saveBlankLineLog();
	fclose(logs);
	
	SDL_DestroyTexture(bgScrollTexture);
	SDL_DestroyTexture(menuButtonTextTexture1);
	SDL_DestroyTexture(menuButtonTextTexture2);
	SDL_DestroyTexture(okButtonTexture);
	SDL_DestroyTexture(tileTextTexture);
	SDL_DestroyTexture(mineBoomIconTexture);
	SDL_DestroyTexture(mineDeathIconTexture);
	SDL_DestroyTexture(flagIconTexture);
	SDL_DestroyTexture(edgeIconTexture);
	SDL_DestroyTexture(coverIconTexture);

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