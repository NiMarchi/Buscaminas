#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "impresion.h"
#include "logs.h"
#include "variables.h"
#include "historial.h"
#include "restaurar.h"

// Comprueba si el jugador venció el juego.
bool checkWin(const field_t *f, const field_t *c) {
	for (int i = 1; i < c->x - 1; i++) {
		for (int j = 1; j < c->y - 1; j++) {
			const int cover = c->mat[i][j]; // Valor de la casilla cubierta.
			const int field = f->mat[i][j]; // Valor de la casilla con mina/pista.

			// Si la casilla está cubierta o marcada, debe haber una mina debajo.
			// Si la casilla está descubierta, debe coincidir con el campo inferior.
			if (!((cover == COVER && field == MINE) || (cover == FLAG && field == MINE) || (cover == field))) {
				return false; // Si alguna condición no se cumple, el jugador aún no ha ganado.
			}
		}
	}

	// Si todas las casillas cumplen con las condiciones, el jugador ha ganado.
	return true;
}

// Comprueba si el jugador perdió el juego.
bool checkLose(const field_t *f, const field_t *c, const int *inpt) {
	return f->mat[inpt[0]][inpt[1]] == MINE && c->mat[inpt[0]][inpt[1]] != FLAG && inpt[2] == OPEN_F;
}

// Función para implementar la función de subcadena en C.
char *substring(char *destination, const char *source, const int beg, const int n) {
	strncpy(destination, source + beg, n);
	destination[n] = '\0';
	return destination;
}

// Inicializa los parámetros del juego.
void setupStage(const int h, const int m) {
    tile.x = TILE_SPACING; // Espaciado de mosaicos entre sí.
    tile.y = TILE_SPACING; // Espaciado de mosaicos entre sí.
    tile.w = TILE_SIDE_SIZE; // Tamaño del lado del azulejo.
    tile.h = TILE_SIDE_SIZE; // Tamaño del lado del azulejo.

	if (!restored_game) {
		f = initField(h, h, m); // Asigna el campo inferior, donde se escribirán las minas y las puntas.
		c = initCover(h, h); // Asigna el campo superior, donde el jugador se descubrirá.
		fillFieldEdge(c); // Rellena el campo superior con caracteres de borde.
		fillFieldCover(c); // Llena el campo superior con caracteres de portada.
		fillFieldZero(f); // Rellena el campo inferior con ceros.
		fillFieldEdge(f); // Rellena el campo superior con caracteres de borde.
		fillFieldMine(f); // Llena el campo inferior con minas.
		countMines(f); // Calcula la cantidad de minas y llena los vertederos en los alrededores de (minas).

		startTime = time(NULL); // Inicializa el tiempo para el conteo.
	}

	colorInfo = (SDL_Color){RED_INFO_POSITIVE, GREEN_INFO_POSITIVE, BLUE_INFO_POSITIVE}; // Asigna el color a las letras de los datos del juego (Como la cantidad de minas y nombre del jugador).

	memset(infoPlayerName, 0, sizeof infoPlayerName); // Borra el array que muestra el nombre del jugador.
	strcpy(infoPlayerName, PLAYER_NAME_INFO);
	strcat(infoPlayerName, paramInput1); // Almacena el nombre del jugador para mostrarlo como información.

	saveEventGenericLog("Inicio del Juego");
}

// Espera un intervalo si el jugador ha ganado o perdido antes de volver al menú principal.
void waitInterval() {
	if (win || lose) {
		SDL_Delay(5000); // Espera 5 segundos.
		Mix_HaltMusic(); // Detiene la música.
		main_menu_is_running = true; // Vuelve al menú principal.
		stage_is_running = false; // Finaliza la etapa de juego.
	}
}

// Actualiza el estado del juego por cada frame.
void update() {
    // Calcula el tiempo restante hasta alcanzar el frame objetivo.
    const int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    // Si el frame se ejecutó más rápido de lo esperado, espera para mantener una tasa de FPS estable.
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    // Calcula el tiempo transcurrido (delta) entre frames en segundos.
    last_frame_time = SDL_GetTicks();

    // Si el juego está activo y los campos están cargados.
    if (f && c) {
        if (canInteract) {
            openField(f, c, ij_selected[0], ij_selected[1], ij_selected[2], &mineRemainingInt);
        }

        // Actualiza el contador de minas restantes como cadena.
        sprintf(mineRemainingStr, "%d", mineRemainingInt);
        mineRemainingConcat[20] = '\0'; // Reset para evitar basura.
        strcat(mineRemainingConcat, mineRemainingStr);

    	// Calcula tiempo y muestra contador.
    	currentTime = time(NULL);
    	const int tiempo_transcurrido = (int)difftime(currentTime, startTime);
    	const int minutos = tiempo_transcurrido / 60;
    	const int segundos = tiempo_transcurrido % 60;

    	sprintf(elapsedTime, "%02d:%02d", minutos, segundos);
    	infoTime[17] = '\0'; // Reset para evitar basura.
    	strcat(infoTime, elapsedTime);

        // Verifica si el jugador ganó o perdió.
        win = checkWin(f, c);
        lose = checkLose(f, c, ij_selected);
        canInteract = false;
    }

    // Actualiza el desplazamiento del fondo para la animación vertical.
    if (bgScrollRect1.y > WINDOW_HEIGHT) {
    	bgScrollRect1.y = 0;
    }
    if (bgScrollRect2.y > 0) {
    	bgScrollRect2.y = -WINDOW_HEIGHT;
    }
    bgScrollRect1.y += SLIDING_SPEED;
    bgScrollRect2.y += SLIDING_SPEED;

    // Rota el ángulo del renderizado (Decorativo).
    angle = (angle > 360) ? 0 : angle + 1;
}

// Procesar la representación de objetos en el juego.
void render() {
	// Si el menú principal está ejecutándose.
	if (main_menu_is_running && !select_menu_is_running && !stage_is_running && !history_menu_is_running) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Título de la presentación.
		printTextLine(renderer, font_main, colorTitle, menuTitlePresentationRect, TITLE, 0, 0, 0, 0);

		// Botones de menú.
		SDL_RenderCopy(renderer, menuButtonTextTexture1, NULL, &menuButtonRect1); // Nuevo juego
		SDL_RenderCopy(renderer, menuButtonTextTexture2, NULL, &menuButtonRect2); // Salir
		SDL_RenderCopy(renderer, menuButtonTextTexture3, NULL, &menuButtonRect3); // Restaurar
		SDL_RenderCopy(renderer, menuButtonTextTexture4, NULL, &menuButtonRect4); // Historial

		// Imprime el título.
		printTitle(renderer, font_main, colorTitle);

		// Si el mouse está sobre el botón de nuevo juego, se resalta y, si se hace clic, pasa a la pantalla de selección de tamaño/minas.
		if (xm >= menuButtonRect1.x && xm <= menuButtonRect1.x + menuButtonRect1.w && ym >= menuButtonRect1.y && ym <= menuButtonRect1.y + menuButtonRect1.h) {
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			option = 0;
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
		} else if (xm >= menuButtonRect3.x && xm <= menuButtonRect3.x + menuButtonRect3.w && ym >= menuButtonRect3.y && ym <= menuButtonRect3.y + menuButtonRect3.h) {
			// De lo contrario, si el mouse está sobre el botón restaurar, se resalta y, si se hace clic, sale del juego.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			option = 4;
			if (clickedL) {
				if (loadGame(&f, &c, paramInput1, elapsedTime, &mineRemainingInt, &h, &m)) {
					main_menu_is_running = false;
					select_menu_is_running = false;
					stage_is_running = true;
					restored_game = true;
					option = 0;
					Mix_HaltMusic();
				}
				clickedL = false;
			}
		} else if (xm >= menuButtonRect4.x && xm <= menuButtonRect4.x + menuButtonRect4.w && ym >= menuButtonRect4.y && ym <= menuButtonRect4.y + menuButtonRect4.h) {
			// De lo contrario, si el mouse está sobre el botón historial, se resalta y, si se hace clic, sale del juego.
			// Si aún no se ha reproducido el efecto de sonido, se reproduce una vez si el mouse permanece dentro del área del botón.
			if (!soundEffectPlayed) {
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			option = 5;
			if (clickedL) {
				main_menu_is_running = false;
				history_menu_is_running = true;
				loadHistoryFile("historial.txt");
				clickedL = false;
			}
		} else {
			soundEffectPlayed = false; // Si el mouse sale de cualquier área de botones, establece el efecto de sonido para que se reproduzca nuevamente al pasar el mouse sobre él.
		}

		// Dibuja texto resaltado según opción.
		printTextLine(renderer, font_main, colorMenuText, menuButtonRect1, NEW_GAME_TEXT, 0, 0, 0, (option == 0) ? 0 : 127);
		printTextLine(renderer, font_main, colorMenuText, menuButtonRect2, QUIT_GAME_TEXT, 0, 0, 0, (option == 1) ? 0 : 127);
		printTextLine(renderer, font_main, colorMenuText, menuButtonRect3, RESTORE_TEXT, 0, 0, 0, (option == 4) ? 0 : 127);
		printTextLine(renderer, font_main, colorMenuText, menuButtonRect4, RECORD_TEXT, 0, 0, 0, (option == 5) ? 0 : 127);

		SDL_RenderPresent(renderer);
	} else if (!main_menu_is_running && select_menu_is_running && !stage_is_running && !history_menu_is_running) {
		// Si el menú de selección está en ejecución.
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Imprime el título.
		printTitle(renderer, font_main, colorTitle);

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
		printTextLine(renderer, font_main, colorForm, widthFieldTextboxRect, paramInput1, 0, 0, 0, 255);

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
		printTextLine(renderer, font_main, colorForm, heightFieldTextboxRect, paramInput2, 0, 0, 0, 255);

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
		printTextLine(renderer, font_main, colorForm, mineAmountTextboxRect, paramInput3, 0, 0, 0, 255);

		okButtonRect.y = OK_BUTTON_Y;
		okButtonRect.w = OK_BUTTON_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (okButtonRect.w) / 2); // Centraliza el texto/formulario/botón en el centro del ancho de la ventana.
		okButtonRect.h = OK_BUTTON_H;
		okButtonRect.x = centerFormTextX;

		// Dibuja cada etiqueta de texto.
		printTextLine(renderer, font_main, colorForm, widthFieldLabelRect, PLAYER_NAME, 0, 0, 0, 0);
		printTextLine(renderer, font_main, colorForm, heightFieldLabelRect, FIELD_SIZE, 0, 0, 0, 0);
		printTextLine(renderer, font_main, colorForm, mineAmountLabelRect, MINE_AMOUNT_TEXT, 0, 0, 0, 0);

		// Dibuja el botón Aceptar.
		SDL_RenderCopy(renderer, okButtonTexture, NULL, &okButtonRect);
		printTextLine(renderer, font_main, colorButton, okButtonRect, OK_BUTTON_TEXT, 0, 0, 0, alpha4);

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
				option = 0;
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

		// Resalta el cuadro de texto de ancho.
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
				h = strtol(paramInput2, NULL, 10);
				m = strtol(paramInput3, NULL, 10);
				if (h >= FIELD_SIZE_MIN && h <= FIELD_SIZE_MAX && m >= MINE_MIN && m <= floor((double)(h * h) / 2)) {
					mineRemainingInt = m;
					select_menu_is_running = false;
					stage_is_running = true;
					option = 0;
					Mix_HaltMusic(); // Detiene la música del menú de fondo.
				} else {
					printAlert(renderer, font_main, colorAlert); // Si los parámetros del campo están fuera de los límites, se muestra una alerta de banner.
				}
			}
		}

		SDL_RenderPresent(renderer);
	} else if (!main_menu_is_running && !select_menu_is_running && stage_is_running && !history_menu_is_running) {
		// Si el juego se está ejecutando.
		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
		SDL_RenderClear(renderer);

		// Animación de fondo.
		SDL_SetTextureBlendMode(bgScrollTexture, SDL_BLENDMODE_BLEND); // Establece el modo de fusión para la transparencia alfa en las texturas.
		SDL_SetTextureAlphaMod(bgScrollTexture, 64); // Establece la transparencia alfa.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Imprime el título.
		printTitle(renderer, font_main, colorTitle);

		// Imprime la cantidad de mina restante.
		printTextLine(renderer, font_secondary, colorInfo, infoRect, mineRemainingConcat, 0, 0, 0, 0);

		// Imprime el nombre del jugador.
		printTextLine(renderer, font_secondary, colorInfo, infoPlayer, infoPlayerName, 0, 0, 0, 0);

		// Imprime el tiempo de juego.
		printTextLine(renderer, font_secondary, colorInfo, infoTimeRect, infoTime, 0, 0, 0, 0);

		// Imprime el tiempo de juego.
		printTextLine(renderer, font_secondary, colorInfo, infoTrampRect, TRAMP_INFO, 0, 0, 0, 0);

		i = 0;
		j = 0;

		// Centraliza el campo en el centro de la ventana.
		centerFieldX = ((WINDOW_WIDTH / 2) - (c->x * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);
		centerFieldY = ((WINDOW_HEIGHT / 2) - (c->y * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);

		// Dibuja los objetos de juego.
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
						saveEventMouseLog("Mouse Click Izquierdo", i, j);
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
						saveEventMouseLog("Mouse Click Derecho", i, j);
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

		// Si el jugador gana, muestra una bandera de victoria y desvincula los campos superiores e inferiores.
		if (win) {
			saveEventGenericLog("Victoria");
			Mix_PlayChannel(-1, soundEffectVictory, 0); // Reproduce un sonido de victoria.
			printFinish(renderer, font_main, colorTip, true);
			free(f); f = NULL;
			free(c); c = NULL;
			stage_is_running = false;
			saveEventGenericLog("Fin del Juego");
			saveHistory(h, m, paramInput1, elapsedTime, "Victoria");
			deleteSaveGame();
			restored_game = false;
		}
		// Si el jugador gana, muestra un cartel de derrota y desvincula los campos superiores e inferiores.
		if (lose) {
			saveEventGenericLog("Derrota");
			Mix_PlayChannel(-1, soundEffectMine, 0); // Reproduce un sonido de derrota.
			printFinish(renderer, font_main, colorTip, false);
			free(f); f = NULL;
			free(c); c = NULL;
			stage_is_running = false;
			saveEventGenericLog("Fin del Juego");
			saveHistory(h, m, paramInput1, elapsedTime, "Derrota");
			deleteSaveGame();
			restored_game = false;
		}

		SDL_RenderPresent(renderer);
	} else if (!main_menu_is_running && !select_menu_is_running && !stage_is_running && history_menu_is_running) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Fondo.
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);

		// Título centrado arriba.
		const SDL_Rect historyTitleRect = { (WINDOW_WIDTH - 600) / 2, 50, 600, 80 };
		printTextLine(renderer, font_main, colorTitle, historyTitleRect, "Historial de Partidas", 0, 0, 0, 0);

		if (historyCount == 0) {
			const SDL_Rect noRecordRect = {
				(WINDOW_WIDTH - 400) / 2,
				WINDOW_HEIGHT / 2 - 25,
				400,
				50
			};
			printTextLine(renderer, font_main, colorAlert, noRecordRect, "Sin Registros", 0, 0, 0, 0);
		}

		// Mostrar las entradas del historial.
		for (int i = 0; i < historyCount; i++) {
			const int startY = 150;
			SDL_Rect entryRect = {
				(WINDOW_WIDTH - 800) / 2,
				startY + i * 60,
				800,
				50
			};

			// Color según victoria o derrota.
			if (strstr(historyLines[i], "Victoria")) {
				SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Verde
			} else {
				SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255); // Rojo
			}
			SDL_RenderFillRect(renderer, &entryRect);

			// Texto por encima del rectángulo.
			printTextLine(renderer, font_secondary, colorInfo, entryRect, historyLines[i], 0, 0, 0, 0);
		}

		SDL_RenderPresent(renderer);
	}
}