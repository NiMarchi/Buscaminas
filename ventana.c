#include <stdio.h>
#include <stdbool.h>

#include <SDL_image.h>

#include "juego.h"
#include "constantes.h"
#include "variables.h"

// Carga imagen y devuelve superficie.
SDL_Surface* load_surface(const char* path) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		fprintf(stderr, "Error al cargar imagen %s: %s\n", path, IMG_GetError());
	}
	return surface;
}

// Crea textura desde superficie.
SDL_Texture* create_texture(SDL_Surface* surface) {
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
bool initialize_sdl_systems() {
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
bool initialize_window() {
	logs = fopen("logs.txt", "a");
	if (logs == NULL) {
		perror("Error al intentar abrir el archivo de logs");
		return false;
	}

	if (!initialize_sdl_systems()) {
		return false;
	}

	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
		fprintf(stderr, "Error al crear la ventana: %s\n", SDL_GetError());
		return false;
	}

	// Establecer icono de la ventana.
	SDL_Surface* iconSurface = load_surface(WINDOW_ICON);  // Asegúrate de definir WINDOW_ICON correctamente
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
	bgScrollSurface = load_surface(BACKGROUND_WALLPAPER);
	bgScrollTexture = create_texture(bgScrollSurface);

	menuPresentationSurface = load_surface(PRESENTATION);
	menuPresentationTexture = create_texture(menuPresentationSurface);

	menuButtonTextSurface1 = load_surface(NEW_OK_BUTTON);
	menuButtonTextTexture1 = create_texture(menuButtonTextSurface1);

	menuButtonTextSurface3 = load_surface(NEW_OK_BUTTON);
	menuButtonTextTexture3 = create_texture(menuButtonTextSurface3);

	menuButtonTextSurface4 = load_surface(NEW_OK_BUTTON);
	menuButtonTextTexture4 = create_texture(menuButtonTextSurface4);

	menuButtonTextSurface2 = load_surface(QUIT_BUTTON);
	menuButtonTextTexture2 = create_texture(menuButtonTextSurface2);

	okButtonSurface = load_surface(NEW_OK_BUTTON);
	okButtonTexture = create_texture(okButtonSurface);

	mineBoomIconSurface = load_surface(MINE_BOOM_ICON);
	mineBoomIconTexture = create_texture(mineBoomIconSurface);

	mineDeathIconSurface = load_surface(MINE_DEATH_ICON);
	mineDeathIconTexture = create_texture(mineDeathIconSurface);

	edgeIconSurface = load_surface(EDGE_ICON);
	edgeIconTexture = create_texture(edgeIconSurface);

	flagIconSurface = load_surface(FLAG_ICON);
	flagIconTexture = create_texture(flagIconSurface);

	coverIconSurface = load_surface(COVER_ICON);
	coverIconTexture = create_texture(coverIconSurface);

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
void destroy_window() {
	saveEventGenericLog("Liberación de Recursos");
	saveBlankLineLog();
	fclose(logs);

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

	SDL_DestroyTexture(aboutTextTexture);
	SDL_DestroyTexture(bgScrollTexture);
	SDL_DestroyTexture(menuPresentationTexture);
	SDL_DestroyTexture(menuTitlePresentationTexture);
	SDL_DestroyTexture(menuButtonTextTexture1);
	SDL_DestroyTexture(menuButtonTextTexture2);
	SDL_DestroyTexture(labelTexture);
	SDL_DestroyTexture(textInputTexture);
	SDL_DestroyTexture(okButtonTexture);
	SDL_DestroyTexture(alertTextTexture);
	SDL_DestroyTexture(infoTextTexture);
	SDL_DestroyTexture(tileTextTexture);
	SDL_DestroyTexture(mineIconTexture);
	SDL_DestroyTexture(mineBoomIconTexture);
	SDL_DestroyTexture(mineDeathIconTexture);
	SDL_DestroyTexture(flagIconTexture);
	SDL_DestroyTexture(edgeIconTexture);
	SDL_DestroyTexture(coverIconTexture);
	SDL_DestroyTexture(finaleTextTexture);

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