#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "initField.h"
#include "fillField.h"
#include "printEngine.h"
#include "sumMineField.h"
#include "openSpaceField.h"
#include "game.h"
#include "constants.h"

// Variables globales de SDL.
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

SDL_Color colorTip = COLOR_TIP;
SDL_Color colorTitle = COLOR_TITLE;
SDL_Color colorInfo;
SDL_Color colorForm = COLOR_FORM;
SDL_Color colorButton = COLOR_BUTTON;
SDL_Color colorSelectInput = COLOR_SEL_INPT;
SDL_Color colorAlert = COLOR_ALERT;
SDL_Color colorMenuText = COLOR_MENU_TEXT;

TTF_Font *font_main, *font_secondary;

Mix_Music *backgroundMusicMenu, *backgroundMusicStage;
Mix_Chunk *soundEffectL, *soundEffectR, *soundEffectMine, *soundEffectMenu, *soundEffectVictory;

SDL_Surface *aboutTextSurface, *bgScrollSurface, *menuPresentationSurface, *menuTitlePresentationSurface, *menuButtonTextSurface1,
*menuButtonTextSurface2, *labelSurface, *textInputSurface, *okButtonSurface, *alertTextSurface, *infoTextSurface, *tileTextSurface,
*mineIconSurface, *mineBoomIconSurface, *mineDeathIconSurface, *flagIconSurface, *edgeIconSurface, *coverIconSurface, *finaleTextSurface;

SDL_Texture *aboutTextTexture, *bgScrollTexture, *menuPresentationTexture, *menuTitlePresentationTexture, *menuButtonTextTexture1,
*menuButtonTextTexture2, *labelTexture, *textInputTexture, *okButtonTexture, *alertTextTexture, *infoTextTexture, *tileTextTexture,
*mineIconTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture, *edgeIconTexture, *coverIconTexture, *finaleTextTexture;

SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect, menuButtonRect1,
menuButtonRect2, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect, heightFieldTextboxRect, mineAmountLabelRect,
mineAmountTextboxRect, okButtonRect, infoRect, tileSquareRect;

// Variables globales del juego.
bool game_is_running = false;
bool stage_is_running = false;
bool main_menu_is_running = false;
bool select_menu_is_running = false;
bool clickedL = false;
bool clickedR = false;
bool win = false;
bool lose = false;
bool canInteract = false;
bool showMines = false;
bool resetIJ = true;
bool soundEffectPlayed = false;

int last_frame_time = 0, angle = 0;
int length, option = RESET_OPTION, formField = 0;
int alpha1 = ALPHA_UNSELECTED, alpha2 = ALPHA_UNSELECTED, alpha3 = ALPHA_UNSELECTED, alpha4 = ALPHA_UNSELECTED;
int i, j, counter1 = 1, counter2 = 1, counter3 = 1;
int ij_selected[3];
int xm, ym, xi, xf, yi, yf;
int button_x, button_y, button_w, button_h;
int w, h, m;
int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
int mineRemainingInt;
float delta_time = 0.0f;
char *aux;
char paramInput1[3], paramInput2[3], paramInput3[4], mineRemainingStr[4], mineRemainingConcat[18] = MINE_INGAME_INFO;
field_t *f, *c;

// Inicializa la ventana.
bool initialize_window(void) {
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

	// Inicializa el renderizador SDL.
	renderer = SDL_CreateRenderer(window, -1, 0);

	// Si no puede crear el renderizador, aparecerá un mensaje de error.
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
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
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

// Inicializa los parámetros del menú.
void setup_main_menu() {
	// Establece 1 tamaño de imagen de presentación.
	menuPresentationRect1.w = 128;
	menuPresentationRect1.x = WINDOW_WIDTH / 16;
	menuPresentationRect1.h = 128;
	menuPresentationRect1.y = WINDOW_HEIGHT / 4 - menuPresentationRect1.h / 2;

	// Establece el tamaño de la imagen de presentación en 2.
	menuPresentationRect2.w = 128;
	menuPresentationRect2.x = WINDOW_WIDTH / 1.2;
	menuPresentationRect2.h = 128;
	menuPresentationRect2.y = WINDOW_HEIGHT / 4 - menuPresentationRect1.h / 2;

	// Establece el tamaño del título de la presentación.
	menuTitlePresentationRect.w = 750;
	menuTitlePresentationRect.x = WINDOW_WIDTH / 5;
	menuTitlePresentationRect.h = 128;
	menuTitlePresentationRect.y = WINDOW_HEIGHT / 4 - menuTitlePresentationRect.h / 2;

	// Establece el tamaño del botón.
	button_x = BUTTON_X;
	button_y = BUTTON_Y;
	button_w = BUTTON_W;
	button_h = BUTTON_H;

	// Nueva posición del botón del juego.
	menuButtonRect1.x = button_x + ((WINDOW_WIDTH / 2) - button_w / 2);
	menuButtonRect1.y = button_y + ((WINDOW_HEIGHT / 2) - button_h / 3);
	menuButtonRect1.w = button_w;
	menuButtonRect1.h = button_h;

	// Posición del botón para salir del juego.
	menuButtonRect2.x = menuButtonRect1.x;
	menuButtonRect2.y = menuButtonRect1.y + BUTTON_SPACING;
	menuButtonRect2.w = button_w;
	menuButtonRect2.h = button_h;
}

// Inicializa los parámetros del juego.
void setup_stage(const int w, const int h, const int m) {
	tile.x = TILE_SPACING; // Espaciado de mosaicos entre sí.
	tile.y = TILE_SPACING; // Espaciado de mosaicos entre sí.
	tile.w = TILE_SIDE_SIZE; // Tamaño del lado del azulejo.
	tile.h = TILE_SIDE_SIZE; // Tamaño del lado del azulejo.

	f = initField(w, h, m); // Asigna el campo inferior, donde se escribirán las minas y las puntas.
	c = initCover(w, h); // Asigna el campo superior, donde el jugador se descubrirá.
	fillFieldEdge(c); // Rellena el campo superior con caracteres de borde.
	fillFieldCover(c); // Llena el campo superior con caracteres de portada.
	fillFieldZero(f); // Rellena el campo inferior con ceros.
	fillFieldEdge(f); // Rellena el campo superior con caracteres de borde.
	fillFieldMine(f); // Llena el campo inferior con minas.
	countMines(f); // Calcula la cantidad de minas y llena los vertederos en los alrededores de (minas).
}

// Cuando finaliza un juego, espera 3 segundos y detiene la música antes de volver al menú principal.
void wait_interval() {
	if (win || lose) {
		SDL_Delay(5000);
		main_menu_is_running = true;
		stage_is_running = false;
		Mix_HaltMusic();
	}
}

// Procesar las entradas del usuario mediante el mouse/teclado en el menú.
void process_input() {
	SDL_StartTextInput();
	while (SDL_PollEvent(&event)) {
		// Si el menú principal está ejecutándose...
		if (main_menu_is_running && !select_menu_is_running && !stage_is_running) {
			showMines = false;
			resetIJ = true;
			win = false;
			lose = false;
			ij_selected[0] = 0;
			ij_selected[1] = 0;
			ij_selected[2] = 0;
			counter1 = counter2 = counter3 = formField = 0;
			memset(paramInput1, 0, sizeof paramInput1);
			memset(paramInput2, 0, sizeof paramInput2);
			memset(paramInput3, 0, sizeof paramInput3);

			// Si la música del menú de fondo no se reproduce, comienza a reproducirse.
			if (!Mix_PlayingMusic()) {
				Mix_PlayMusic(backgroundMusicMenu, -1);
			}

			// ... Esperar un evento de entrada.
			switch (event.type) {
				// Si se hace clic en el botón X de la ventana, se cierra.
				case SDL_QUIT:
					game_is_running = false;
					main_menu_is_running = false;
					select_menu_is_running = false;
					stage_is_running = false;
					break;
				// Si se presiona una tecla...
				case SDL_KEYDOWN:
					// ... Y es la tecla Escape, para salir del juego.
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						game_is_running = false;
						main_menu_is_running = false;
						select_menu_is_running = false;
						stage_is_running = false;
					}
					// ... Y es la flecha hacia abajo, vaya al botón de abajo.
					if (event.key.keysym.sym == SDLK_DOWN && option < 1) {
						option++;
					}
					// ... Y es la flecha hacia arriba, vaya al botón de arriba.
					if (event.key.keysym.sym == SDLK_UP && option > 0) {
						option--;
					}
					// ... Y es el Retorno, selecciona ...
					if (event.key.keysym.sym == SDLK_RETURN) {
						// ... El nuevo botón de juego.
						if (option == 0) {
							option = RESET_OPTION;
							main_menu_is_running = false;
							select_menu_is_running = true;
							stage_is_running = false;
						} else if (option == 1) {
							// ... El botón para salir del juego.
							game_is_running = false;
							main_menu_is_running = false;
							select_menu_is_running = false;
							stage_is_running = false;
						}
					}
					break;
				// Si se presiona el botón izquierdo del ratón, se activa su variable.
				case SDL_MOUSEBUTTONDOWN:
					clickedL = event.button.button == SDL_BUTTON_LEFT;
					break;
				// Si se suelta el botón izquierdo del ratón, se desactiva su variable.
				case SDL_MOUSEBUTTONUP:
					clickedL = !event.button.button == SDL_BUTTON_LEFT;
					break;
				default:
					break;
			}
		} else if (!main_menu_is_running && select_menu_is_running && !stage_is_running) {
			// Si el menú de selección está en ejecución...
			switch (event.type) {
				// Si se hace clic en el botón X de la ventana, se cierra.
				case SDL_QUIT:
					game_is_running = false;
					main_menu_is_running = false;
					select_menu_is_running = false;
					stage_is_running = false;
					break;
					// Si se presiona una tecla...
				case SDL_KEYDOWN:
					// ... Y es la tecla Escape, regresa al menú principal.
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						option = RESET_OPTION;
						main_menu_is_running = true;
						select_menu_is_running = false;
						stage_is_running = false;
					}
					// ... Y es la flecha hacia abajo, vaya a la opción de abajo.
					if (event.key.keysym.sym == SDLK_DOWN && option < 3) {
						formField++;
						option++;
					}
					// ... Y es la flecha hacia arriba, ve a la opción de arriba.
					if (event.key.keysym.sym == SDLK_UP && option > 0) {
						formField--;
						option--;
					}
					if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput1) > 0 && (formField == 0 || option == 0)) {
						substring(paramInput1, paramInput1, 0, strlen(paramInput1) - 1);
						counter1 -= TEXT_BOX_FINE_ADJUSTMENT;
					} else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput2) > 0 && (formField == 1 || option == 1)) {
						substring(paramInput2, paramInput2, 0, strlen(paramInput2) - 1);
						counter2 -= TEXT_BOX_FINE_ADJUSTMENT;
					} else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput3) > 0 && (formField == 2 || option == 2)) {
						substring(paramInput3, paramInput3, 0, strlen(paramInput3) - 1);
						counter3 -= TEXT_BOX_FINE_ADJUSTMENT;
					}
					break;
				case SDL_TEXTINPUT:
					if (event.text.text[0] != '0' && event.text.text[0] != '1' && event.text.text[0] != '2' && event.text.text[0] != '3' && event.text.text[0] != '4' && event.text.text[0] != '5' && event.text.text[0] != '6' && event.text.text[0] != '7' && event.text.text[0] != '8' && event.text.text[0] != '9') {

					} else {
						if (paramInput1[1] == '\0' && (formField == 0 || option == 0)) {
							SDL_SetTextInputRect(&widthFieldLabelRect);
							strcat(paramInput1, event.text.text);
							counter1 += TEXT_BOX_FINE_ADJUSTMENT;
						} else if (paramInput2[1] == '\0' && (formField == 1 || option == 1)) {
							SDL_SetTextInputRect(&widthFieldLabelRect);
							strcat(paramInput2, event.text.text);
							counter2 += TEXT_BOX_FINE_ADJUSTMENT;
						} else if (paramInput3[2] == '\0' && (formField == 2 || option == 2)) {
							SDL_SetTextInputRect(&widthFieldLabelRect);
							strcat(paramInput3, event.text.text);
							counter3 += TEXT_BOX_FINE_ADJUSTMENT;
						}
					}
					break;
					// Si se presiona el botón izquierdo o derecho del ratón, se activan sus variables.
				case SDL_MOUSEBUTTONDOWN:
					clickedL = event.button.button == SDL_BUTTON_LEFT;
					break;
					// Si se suelta el botón izquierdo o derecho del ratón, se desactivan sus variables.
				case SDL_MOUSEBUTTONUP:
					clickedL = !event.button.button == SDL_BUTTON_LEFT;
					break;
				default:
					break;
			}
		} else if (!main_menu_is_running && !select_menu_is_running && stage_is_running) {
			// Si el escenario está en marcha...
			// Si no se reproduce música de fondo del escenario, comienza a reproducirse.
			if (!Mix_PlayingMusic()) {
				Mix_PlayMusic(backgroundMusicStage, -1);
			}

			// ... Esperar un evento de entrada.
			switch (event.type) {
				// Si se hace clic en el botón X de la ventana, se cierra.
				case SDL_QUIT:
					game_is_running = false;
					main_menu_is_running = false;
					select_menu_is_running = false;
					stage_is_running = false;
					break;
				// Si se presiona una tecla...
				case SDL_KEYDOWN:
					// ... Y es la tecla Escape, regresa al menú principal.
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						Mix_HaltMusic(); // Stops playing stage music.
						main_menu_is_running = true;
						select_menu_is_running = false;
						stage_is_running = false;
					}
					break;
				// Si se presiona el botón izquierdo o derecho del ratón, se activan sus variables.
				case SDL_MOUSEBUTTONDOWN:
					clickedL = event.button.button == SDL_BUTTON_LEFT;
					clickedR = event.button.button == SDL_BUTTON_RIGHT;
					break;
				// Si se suelta el botón izquierdo o derecho del ratón, se desactivan sus variables.
				case SDL_MOUSEBUTTONUP:
					clickedL = !event.button.button == SDL_BUTTON_LEFT;
					clickedR = !event.button.button == SDL_BUTTON_RIGHT;
					break;
				default:
					break;
			}
		}
	}
	SDL_StopTextInput();
	SDL_GetMouseState(&xm, &ym);
}

// Procesar estados de objetos del juego en el juego.
void update() {
	// Dormir la ejecución hasta que alcancemos el tiempo de cuadro objetivo en milisegundos.
	const int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	// Solo llama al retraso si somos demasiado rápidos para procesar este cuadro.
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	// Obtenga un factor de tiempo delta convertido a segundos para usarlo para actualizar mis objetos.
	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	//TODO: Here is where we can update the state of our objects

	// Si los campos superiores e inferiores aún están asignados, el jugador puede abrir/marcar fichas y luego el juego verifica si ganó o perdió.
	if (f != NULL && c != NULL) {
		if (canInteract) {
			openField(f, c, ij_selected[0], ij_selected[1], ij_selected[2], &mineRemainingInt);
		}

		sprintf(mineRemainingStr, "%d", mineRemainingInt); // Transformar la cantidad de mina en una cadena, para su posterior exhibición.
		mineRemainingConcat[17] = '\0'; // Restablece la cadena para evitar anexiones infinitas.
		strcat(mineRemainingConcat, mineRemainingStr); // Concatena el número de minas restantes con el texto de información fijo.

		win = checkWin(f, c);
		lose = checkLose(f, c, ij_selected);
		canInteract = canInteract = false;
	}

	if (mineRemainingInt < 0) {
		colorInfo.r = RED_INFO_NEGATIVE;
		colorInfo.g = GREEN_INFO_NEGATIVE;
		colorInfo.b = BLUE_INFO_NEGATIVE;
	} else if(mineRemainingInt > 0) {
		colorInfo.r = RED_INFO_POSITIVE;
		colorInfo.g = GREEN_INFO_POSITIVE;
		colorInfo.b = BLUE_INFO_POSITIVE;
	} else {
		colorInfo.r = RED_INFO_NEUTRAL;
		colorInfo.g = GREEN_INFO_NEUTRAL;
		colorInfo.b = BLUE_INFO_NEUTRAL;
	}

	// Si el tamaño del fondo de pantalla izquierdo llega al lado más a la derecha, se reinicia al inicio.
	if (bgScrollRect1.y > WINDOW_HEIGHT) {
		bgScrollRect1.y = 0;
	}
	if(bgScrollRect2.y > 0) {
		bgScrollRect2.y = -WINDOW_HEIGHT;
	}
	bgScrollRect1.y += SLIDING_SPEED; // Diapositiva de fondo 1 hacia abajo.
	bgScrollRect2.y += SLIDING_SPEED; // Diapositiva de fondo 2 hacia abajo.

	// Cambia el ángulo de presentación.
	if (angle > 360) {
		angle = 0;
	} else {
		angle++;
	}
}

// Procesar la representación de objetos en el juego.
void render() {
	// Si el menú principal está ejecutándose.
	if (main_menu_is_running && !select_menu_is_running && !stage_is_running) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Imagen de presentación 1.
		SDL_RenderCopyEx(renderer, menuPresentationTexture, NULL, &menuPresentationRect1, angle, NULL, SDL_FLIP_NONE);

		// Imagen de presentación 2.
		SDL_RenderCopyEx(renderer, menuPresentationTexture, NULL, &menuPresentationRect2, -angle, NULL, SDL_FLIP_NONE);

		// Título de la presentación.
		printTextLine(renderer, menuTitlePresentationSurface, menuTitlePresentationTexture, font_main, colorTitle, menuTitlePresentationRect, TITLE, 0, 0, 0, 0);

		// Botones de menú.
		SDL_RenderCopy(renderer, menuButtonTextTexture1, NULL, &menuButtonRect1);
		SDL_RenderCopy(renderer, menuButtonTextTexture2, NULL, &menuButtonRect2);

		// Imprime el título.
		printTitle(renderer, aboutTextSurface, aboutTextTexture, font_main, colorTitle);

		// Si el mouse está sobre el botón de nuevo juego, se resalta y, si se hace clic, pasa a la pantalla de selección de tamaño/minas.
		if (xm >= menuButtonRect1.x && xm <= menuButtonRect1.x + menuButtonRect1.w && ym >= menuButtonRect1.y && ym <= menuButtonRect1.y + menuButtonRect1.h) {
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			option = RESET_OPTION;
			if (clickedL) {
				main_menu_is_running = false;
				select_menu_is_running = true;
				clickedL = false;
			}
		} else if (xm >= menuButtonRect2.x && xm <= menuButtonRect2.x + menuButtonRect2.w && ym >= menuButtonRect2.y && ym <= menuButtonRect2.y + menuButtonRect2.h) {
			// De lo contrario, si el mouse está sobre el botón para salir del juego, se resalta y, si se hace clic, sale del juego.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			option = 1;
			if (clickedL) {
				game_is_running = false;
				main_menu_is_running = false;
				select_menu_is_running = false;
				stage_is_running = false;
				clickedL = false;
			}
		} else {
			soundEffectPlayed = false; // Si el mouse sale de cualquier área de botones, establece el efecto de sonido para que se reproduzca nuevamente al pasar el mouse sobre él.
		}

		// Aspectos destacados del nuevo botón de juego.
		if (option == 0) {
			printTextLine(renderer, menuButtonTextSurface1, menuButtonTextTexture1, font_main, colorMenuText, menuButtonRect1, NEW_GAME_TEXT, 0, 0, 0, 0);
			printTextLine(renderer, menuButtonTextSurface1, menuButtonTextTexture1, font_main, colorMenuText, menuButtonRect2, QUIT_GAME_TEXT, 0, 0, 0, 127);
		} else if (option == 1) {
			// Aspectos destacados del botón para salir del juego.
			printTextLine(renderer, menuButtonTextSurface1, menuButtonTextTexture1, font_main, colorMenuText, menuButtonRect1, NEW_GAME_TEXT, 0, 0, 0, 127);
			printTextLine(renderer, menuButtonTextSurface1, menuButtonTextTexture1, font_main, colorMenuText, menuButtonRect2, QUIT_GAME_TEXT, 0, 0, 0, 0);
		}

		SDL_RenderPresent(renderer);
	} else if (!main_menu_is_running && select_menu_is_running && !stage_is_running) {
		// Si el menú de selección está en ejecución.
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Imprime el título.
		printTitle(renderer, aboutTextSurface, aboutTextTexture, font_main, colorTitle);

		widthFieldLabelRect.y = WIDTH_TEXT_Y;
		widthFieldLabelRect.w = WIDTH_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (widthFieldLabelRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		widthFieldLabelRect.h = WIDTH_TEXT_H;
		widthFieldLabelRect.x = centerFormTextX;

		widthFieldTextboxRect.y = WIDTH_INPUT_Y;
		widthFieldTextboxRect.w = WIDTH_INPUT_W + counter1;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (widthFieldTextboxRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		widthFieldTextboxRect.h = WIDTH_INPUT_H;
		widthFieldTextboxRect.x = centerFormInputX;
		printTextLine(renderer, textInputSurface, textInputTexture, font_main, colorForm, widthFieldTextboxRect, paramInput1, 0, 0, 0, 255);

		heightFieldLabelRect.y = HEIGHT_TEXT_Y;
		heightFieldLabelRect.w = HEIGHT_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (heightFieldLabelRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		heightFieldLabelRect.h = HEIGHT_TEXT_H;
		heightFieldLabelRect.x = centerFormTextX;

		heightFieldTextboxRect.y = HEIGHT_INPUT_Y;
		heightFieldTextboxRect.w = HEIGHT_INPUT_W + counter2;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (heightFieldTextboxRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		heightFieldTextboxRect.h = HEIGHT_INPUT_H;
		heightFieldTextboxRect.x = centerFormInputX;
		printTextLine(renderer, textInputSurface, textInputTexture, font_main, colorForm, heightFieldTextboxRect, paramInput2, 0, 0, 0, 255);

		mineAmountLabelRect.y = MINE_TEXT_Y;
		mineAmountLabelRect.w = MINE_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (mineAmountLabelRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		mineAmountLabelRect.h = MINE_TEXT_H;
		mineAmountLabelRect.x = centerFormTextX;

		mineAmountTextboxRect.y = MINE_INPUT_Y;
		mineAmountTextboxRect.w = MINE_INPUT_W + counter3;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (mineAmountTextboxRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		mineAmountTextboxRect.h = MINE_INPUT_H;
		mineAmountTextboxRect.x = centerFormInputX;
		printTextLine(renderer, textInputSurface, textInputTexture, font_main, colorForm, mineAmountTextboxRect, paramInput3, 0, 0, 0, 255);

		okButtonRect.y = OK_BUTTON_Y;
		okButtonRect.w = OK_BUTTON_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (okButtonRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		okButtonRect.h = OK_BUTTON_H;
		okButtonRect.x = centerFormTextX;

		// Dibuja cada etiqueta de texto.
		printTextLine(renderer, labelSurface, labelTexture, font_main, colorForm, widthFieldLabelRect, WIDTH_TEXT, 0, 0, 0, 0);
		printTextLine(renderer, labelSurface, labelTexture, font_main, colorForm, heightFieldLabelRect, HEIGHT_TEXT, 0, 0, 0, 0);
		printTextLine(renderer, labelSurface, labelTexture, font_main, colorForm, mineAmountLabelRect, MINE_AMOUNT_TEXT, 0, 0, 0, 0);

		// Dibuja el botón Aceptar.
		SDL_RenderCopy(renderer, okButtonTexture, NULL, &okButtonRect);
		printTextLine(renderer, okButtonSurface, okButtonTexture, font_main, colorButton, okButtonRect, OK_BUTTON_TEXT, 0, 0, 0, alpha4);

		// Dibuja cada cuadro de texto para la entrada.
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha1);
		SDL_RenderFillRect(renderer, &widthFieldTextboxRect);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha2);
		SDL_RenderFillRect(renderer, &heightFieldTextboxRect);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha3);
		SDL_RenderFillRect(renderer, &mineAmountTextboxRect);

		// Si el mouse está sobre el cuadro de texto de ancho y se detecta un clic, se selecciona ese cuadro para la entrada.
		if (xm >= widthFieldTextboxRect.x && xm <= widthFieldTextboxRect.x + widthFieldTextboxRect.w && ym >= widthFieldTextboxRect.y && ym <= widthFieldTextboxRect.y + widthFieldTextboxRect.h) {
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del cuadro de texto.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL) {
				formField = 0;
				option = RESET_OPTION;
				clickedL = false;
			}
		} else if (xm >= heightFieldTextboxRect.x && xm <= heightFieldTextboxRect.x + heightFieldTextboxRect.w && ym >= heightFieldTextboxRect.y && ym <= heightFieldTextboxRect.y + heightFieldTextboxRect.h) {
			// Si el mouse está sobre el cuadro de texto de altura y se detecta un clic, selecciona ese cuadro para la entrada.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del cuadro de texto.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL) {
				formField = 1;
				option = 1;
				clickedL = false;
			}
		} else if (xm >= mineAmountTextboxRect.x && xm <= mineAmountTextboxRect.x + mineAmountTextboxRect.w && ym >= mineAmountTextboxRect.y && ym <= mineAmountTextboxRect.y + mineAmountTextboxRect.h) {
			// Si el mouse está sobre el cuadro de texto de la mina y se detecta un clic, se selecciona ese cuadro para la entrada.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del cuadro de texto.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL) {
				formField = 2;
				option = 2;
				clickedL = false;
			}
		} else if (xm >= okButtonRect.x && xm <= okButtonRect.x + okButtonRect.w && ym >= okButtonRect.y && ym <= okButtonRect.y + okButtonRect.h) {
			// Si el ratón está sobre el botón Aceptar y se detecta un clic, se inicia una nueva etapa.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL) {
				formField = 3;
				option = 3;
				clickedL = false;
			}
		} else {
			soundEffectPlayed = false; // Si el mouse sale de cualquier área de cuadro de texto o botón, establece el efecto de sonido para que se reproduzca nuevamente en caso de colisión.
		}

		// Resalta el cuadro de texto de ancho..
		if (formField == 0 || option == 0) {
			alpha1 = ALPHA_SELECTED;
			alpha2 = ALPHA_UNSELECTED;
			alpha3 = ALPHA_UNSELECTED;
			alpha4 = ALPHA_UNSELECTED;
		}
		// Resalta el cuadro de texto de altura.
		if (formField == 1 || option == 1) {
			alpha1 = ALPHA_UNSELECTED;
			alpha2 = ALPHA_SELECTED;
			alpha3 = ALPHA_UNSELECTED;
			alpha4 = ALPHA_UNSELECTED;
		}
		// Resalta el cuadro de texto de la mina.
		if (formField == 2 || option == 2) {
			alpha1 = ALPHA_UNSELECTED;
			alpha2 = ALPHA_UNSELECTED;
			alpha3 = ALPHA_SELECTED;
			alpha4 = ALPHA_UNSELECTED;
		}
		// Agarra el ancho, la altura y la cantidad de minas y comienza una nueva etapa.
		if (option == 3) {
			alpha1 = ALPHA_UNSELECTED;
			alpha2 = ALPHA_UNSELECTED;
			alpha3 = ALPHA_UNSELECTED;
			alpha4 = ALPHA_SELECTED;

			if (formField == 3) {
				// Convierte cada cadena en un entero.
				w = strtol(paramInput1, NULL, 10);
				h = strtol(paramInput2, NULL, 10);
				m = strtol(paramInput3, NULL, 10);
				if (w > WIDTH_MIN && w < WIDTH_MAX && h > HEIGHT_MIN && h < HEIGHT_MAX && m > MINE_MIN && m < (w * h)) {
					mineRemainingInt = m;
					select_menu_is_running = false;
					stage_is_running = true;
					option = RESET_OPTION;
					Mix_HaltMusic(); // Detiene la música del menú de fondo.
				} else {
					printAlert(renderer, alertTextSurface, alertTextTexture, font_main, colorAlert); // Si los parámetros del campo están fuera de los límites, se muestra una alerta de banner.
				}
			}
		}

		SDL_RenderPresent(renderer);
	} else if (!main_menu_is_running && !select_menu_is_running && stage_is_running) {
		// Si el juego se está ejecutando.
		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_SetTextureBlendMode(bgScrollTexture, SDL_BLENDMODE_BLEND); // Establece el modo de fusión para la transparencia alfa en las texturas.
		SDL_SetTextureAlphaMod(bgScrollTexture, 64); // Establece la transparencia alfa.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Imprime el título.
		printTitle(renderer, aboutTextSurface, aboutTextTexture, font_main, colorTitle);

		// Imprime la cantidad de mina restante.
		printTextLine(renderer, infoTextSurface, infoTextTexture, font_secondary, colorInfo, infoRect, mineRemainingConcat, 0, 0, 0, 0);

		i = 0;
		j = 0;

		// Centraliza el campo en el centro de la ventana.
		centerFieldX = ((WINDOW_WIDTH / 2) - (c->x * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);
		centerFieldY = ((WINDOW_HEIGHT / 2) - (c->y * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);

		//TODO: Here is where we can start drawing ou game objects

		for (i = 0; i < c->x; i++) {
			for (j = 0; j < c->y; j++) {
				xi = (tile.x + tile.w) * i + centerFieldX + X_FINE_ADJUSTEMENT;
				yi = (tile.y + tile.h) * j + centerFieldY + Y_FINE_ADJUSTEMENT;
				xf = (xi + tile.w);
				yf = (yi + tile.h);

				tileSquareRect.x = xi;
				tileSquareRect.y = yi;
				tileSquareRect.w = tile.w;
				tileSquareRect.h = tile.h;

				if (xm >= xi && xm <= xf && ym >= yi && ym <= yf && c->mat[i][j] != EDGE_L_R && c->mat[i][j] != EDGE_T_B) {
					if (clickedL) {
						// Si aún no se ha abierto la tapa, reproduce el sonido de clic izquierdo.
						if (c->mat[i][j] != f->mat[i][j]) {
							Mix_PlayChannel(-1, soundEffectL, 0);
						}
						canInteract = true;
						ij_selected[0] = i;
						ij_selected[1] = j;
						ij_selected[2] = OPEN_F;
					}
					if (clickedR) {
						// Si aún no se ha abierto la cubierta, reproduce el sonido de clic derecho.
						if (c->mat[i][j] != f->mat[i][j]) {
							Mix_PlayChannel(-1, soundEffectR, 0);
						}
						canInteract = true;
						ij_selected[0] = i;
						ij_selected[1] = j;
						ij_selected[2] = FLAG_F;
					}
					clickedL = false;
					clickedR = false;
				}

				if (c->mat[i][j] == MINE) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);
					f->mat[i][j] = MINE_TRIG;

					// Muestra un icono personalizado para las minas.
					SDL_RenderCopy(renderer, mineDeathIconTexture, NULL, &tileSquareRect);
					showMines = true;
					if (resetIJ == true) {
						i = j = 0;
						resetIJ = false;
						break;
					}
				} else if (c->mat[i][j] == EDGE_L_R || c->mat[i][j] == EDGE_T_B) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Muestra un icono personalizado para los bordes.
					SDL_RenderCopy(renderer, edgeIconTexture, NULL, &tileSquareRect);
				} else if (c->mat[i][j] == FLAG) {
					SDL_SetRenderDrawColor(renderer, 127, 255, 0, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Muestra un icono personalizado para las banderas.
					SDL_RenderCopy(renderer, flagIconTexture, NULL, &tileSquareRect);
				} else if (c->mat[i][j] == COVER) {
					SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Muestra un icono personalizado para la portada.
					SDL_RenderCopy(renderer, coverIconTexture, NULL, &tileSquareRect);
				} else if (c->mat[i][j] == 0) {
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);
				} else {
					length = snprintf(NULL, 0, "%d", c->mat[i][j]);
					aux = malloc(length + 1);
					snprintf(aux, length + 1, "%d", c->mat[i][j]);

					SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					tileTextSurface = TTF_RenderText_Blended(font_main, aux, colorTip);
					tileTextTexture = SDL_CreateTextureFromSurface(renderer, tileTextSurface);
					SDL_RenderCopy(renderer, tileTextTexture, NULL, &tileSquareRect);
					SDL_FreeSurface(tileTextSurface);
					SDL_DestroyTexture(tileTextTexture);
				}

				// Si el jugador muere, muestra todas las minas ocultas.
				if (f->mat[i][j] == MINE && showMines) {
					// Muestra un icono personalizado para las minas.
					SDL_RenderCopy(renderer, mineBoomIconTexture, NULL, &tileSquareRect);
				}
			}
		}

		// Si el jugador gana, muestra una bandera de victoria y desasigna los campos superiores e inferiores.
		if (win) {
			Mix_PlayChannel(-1, soundEffectVictory, 0); // Reproduce un sonido de victoria.
			printFinish(renderer, finaleTextSurface, finaleTextTexture, font_main, colorTip, true);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
			stage_is_running = false;
		}
		// Si el jugador gana, muestra un cartel de derrota y desasigna los campos superiores e inferiores.
		if (lose) {
			Mix_PlayChannel(-1, soundEffectMine, 0); // Reproduce un sonido de derrota.
			printFinish(renderer, finaleTextSurface, finaleTextTexture, font_main, colorTip, false);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
			stage_is_running = false;
		}

		SDL_RenderPresent(renderer);
	}
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

// Función principal.
int main(int argc, char* argv[]) {
	// Si initialize_window está inicializado, game_is_running es igual a verdadero.
	game_is_running = initialize_window();
	main_menu_is_running = game_is_running;

	while (game_is_running) {
		setup_main_menu();

		while (main_menu_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		while (select_menu_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse/teclado en el menú.
			update(); // Procesar estados de objetos del juego en el menú.
			render(); // Procesar la representación de objetos en el menú.
		}

		if(stage_is_running) {
			setup_stage(w, h, m); // Inicializa los parámetros del campo minado.
		}

		while (stage_is_running) {
			process_input(); // Procesar las entradas del usuario mediante el mouse y el teclado en el escenario.
			update(); // Procesar los estados de los objetos del juego en el escenario.
			render(); // Procesar la representación de objetos en etapa.
			wait_interval(); // Cuando finaliza un juego, espera 3 segundos y detiene la música antes de volver al menú principal.
		}
	}

	destroy_window(); // Cierra la ventana y finaliza el proceso.
	return 0;
}