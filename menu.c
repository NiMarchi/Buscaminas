#include <stdio.h>

#include "constantes.h"
#include "juego.h"
#include "variables.h"

void setup_main_menu() {
	// Constantes de tamaño.
	const int icon_size = 128;
	const int title_width = 750;
	const int title_height = 128;

	// Imagen de presentación izquierda.
	menuPresentationRect1.w = icon_size;
	menuPresentationRect1.h = icon_size;
	menuPresentationRect1.x = WINDOW_WIDTH / 16;
	menuPresentationRect1.y = WINDOW_HEIGHT / 4 - icon_size / 2;

	// Imagen de presentación derecha.
	menuPresentationRect2.w = icon_size;
	menuPresentationRect2.h = icon_size;
	menuPresentationRect2.x = WINDOW_WIDTH / 1.2;
	menuPresentationRect2.y = menuPresentationRect1.y;

	// Título de presentación.
	menuTitlePresentationRect.w = title_width;
	menuTitlePresentationRect.h = title_height;
	menuTitlePresentationRect.x = WINDOW_WIDTH / 5;
	menuTitlePresentationRect.y = WINDOW_HEIGHT / 4 - title_height / 2;

	// Botón (Centralizado).
	button_x = BUTTON_X;
	button_y = BUTTON_Y;
	button_w = BUTTON_W;
	button_h = BUTTON_H;

	// Botón "Nuevo juego".
	menuButtonRect1.w = button_w;
	menuButtonRect1.h = button_h;
	menuButtonRect1.x = button_x + ((WINDOW_WIDTH - button_w) / 2);
	menuButtonRect1.y = button_y + ((WINDOW_HEIGHT / 2) - button_h / 3);

	// Botón "Salir".
	menuButtonRect2 = menuButtonRect1;
	menuButtonRect2.y += BUTTON_SPACING;
}

// Función auxiliar para cerrar el juego y salir de todos los estados.
void handle_quit() {
	game_is_running = false;
	main_menu_is_running = false;
	select_menu_is_running = false;
	stage_is_running = false;
}

// Función auxiliar para manejar el estado de los botones del mouse.
void handle_mouse_button(const SDL_Event *event, const bool pressed) {
	if (event->button.button == SDL_BUTTON_LEFT) {
		clickedL = pressed;
	}
	if (event->button.button == SDL_BUTTON_RIGHT) {
		clickedR = pressed;
	}
}

// Función principal para procesar la entrada del usuario.
void process_input() {
	// Inicia la entrada de texto (Para SDL_TEXTINPUT).
	SDL_StartTextInput();

	while (SDL_PollEvent(&event)) {
		// Determinar en qué estado del juego estamos.
		const bool in_main_menu = main_menu_is_running && !select_menu_is_running && !stage_is_running;
		const bool in_select_menu = !main_menu_is_running && select_menu_is_running && !stage_is_running;
		const bool in_stage = !main_menu_is_running && !select_menu_is_running && stage_is_running;

		// Estado: Menú principal.
		if (in_main_menu) {
			// Reiniciar variables del juego.
			showMines = false;
			resetIJ = true;
			win = lose = false;
			ij_selected[0] = ij_selected[1] = ij_selected[2] = 0;
			counter1 = counter2 = counter3 = formField = 0;

			// Limpiar los campos de entrada.
			memset(paramInput1, 0, sizeof paramInput1);
			memset(paramInput2, 0, sizeof paramInput2);
			memset(paramInput3, 0, sizeof paramInput3);

			// Reproducir música del menú si no está sonando.
			if (!Mix_PlayingMusic()) {
				Mix_PlayMusic(backgroundMusicMenu, -1);
			}
		}

		// Procesar eventos.
		switch (event.type) {
			// Evento: Cierre de ventana.
			case SDL_QUIT:
				handle_quit();
				break;

			// Evento: Tecla presionada.
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					// Tecla ESCAPE.
					case SDLK_ESCAPE:
						if (in_stage) {
							Mix_HaltMusic(); // Detener música del escenario.
							main_menu_is_running = true;
							select_menu_is_running = false;
							stage_is_running = false;

							// Reproducir música del menú.
							if (!Mix_PlayingMusic()) {
								Mix_PlayMusic(backgroundMusicMenu, -1);
							}
						} else if (in_select_menu) {
							option = RESET_OPTION;
							main_menu_is_running = true;
							select_menu_is_running = false;
						} else {
							handle_quit();
						}
						break;

					// Flecha hacia abajo.
					case SDLK_DOWN:
						if (option < (in_main_menu ? 1 : 3)) {
							option++;
							if (in_select_menu) {
								formField++;
							}
						}
						break;

					// Flecha hacia arriba.
					case SDLK_UP:
						if (option > 0) {
							option--;
							if (in_select_menu) {
								formField--;
							}
						}
						break;

					// Tecla ENTER.
					case SDLK_RETURN:
						if (in_main_menu) {
							if (option == 0) {
								option = RESET_OPTION;
								main_menu_is_running = false;
								select_menu_is_running = true;
							} else if (option == 1) {
								handle_quit();
							}
						}
						break;

					// Tecla BACKSPACE (Borrar caracteres).
					case SDLK_BACKSPACE:
						if (in_select_menu) {
							if ((formField == 0 || option == 0) && strlen(paramInput1) > 0) {
								substring(paramInput1, paramInput1, 0, (int)strlen(paramInput1) - 1);
								counter1 -= TEXT_BOX_FINE_ADJUSTMENT;
							} else if ((formField == 1 || option == 1) && strlen(paramInput2) > 0) {
								substring(paramInput2, paramInput2, 0, (int)strlen(paramInput2) - 1);
								counter2 -= TEXT_BOX_FINE_ADJUSTMENT;
							} else if ((formField == 2 || option == 2) && strlen(paramInput3) > 0) {
								substring(paramInput3, paramInput3, 0, (int)strlen(paramInput3) - 1);
								counter3 -= TEXT_BOX_FINE_ADJUSTMENT;
							}
						}
						break;
					default:
						break;
				}
				break;

			// Evento: Entrada de texto.
			case SDL_TEXTINPUT:
				if (in_select_menu) {
					const char c = event.text.text[0];
					if (c != ' ' && (formField == 0 || option == 0) && paramInput1[9] == '\0') {
						SDL_SetTextInputRect(&widthFieldLabelRect);
						strcat(paramInput1, event.text.text);
						counter1 += TEXT_BOX_FINE_ADJUSTMENT;
					}
					// Solo admite numeros como entrada.
					if (c >= '0' && c <= '9') {
						 if ((formField == 1 || option == 1) && paramInput2[1] == '\0') {
							SDL_SetTextInputRect(&widthFieldLabelRect);
							strcat(paramInput2, event.text.text);
							counter2 += TEXT_BOX_FINE_ADJUSTMENT;
						 } else if ((formField == 2 || option == 2) && paramInput3[2] == '\0') {
							SDL_SetTextInputRect(&widthFieldLabelRect);
							strcat(paramInput3, event.text.text);
							counter3 += TEXT_BOX_FINE_ADJUSTMENT;
						}
					}
				}
				break;

			// Evento: Botón del mouse presionado.
			case SDL_MOUSEBUTTONDOWN:
				handle_mouse_button(&event, true);
				break;

			// Evento: Botón del mouse soltado.
			case SDL_MOUSEBUTTONUP:
				handle_mouse_button(&event, false);
				break;

			default:
				break;
		}

		// Estado: Escenario (stage), reproducir música si no suena.
		if (in_stage && !Mix_PlayingMusic()) {
			Mix_PlayMusic(backgroundMusicStage, -1);
		}
	}

	// Detener entrada de texto y capturar posición actual del mouse.
	SDL_StopTextInput();
	SDL_GetMouseState(&xm, &ym);
}