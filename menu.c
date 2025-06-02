#include "constantes.h"
#include "juego.h"
#include "variables.h"

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