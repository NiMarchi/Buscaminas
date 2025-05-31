#pragma once
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define WIDTH_MIN 2
#define WIDTH_MAX 31
#define HEIGHT_MIN 2
#define HEIGHT_MAX 16
#define MINE_MIN 0

#define ALERT "Parametros Invalidos"

#define SLIDING_SPEED 1

#define BUTTON_X 0
#define BUTTON_Y 0
#define BUTTON_W 450
#define BUTTON_H 100
#define BUTTON_SPACING 150

#define WIDTH_TEXT "Ingrese el ancho del campo: (min 3, max 30)"
#define HEIGHT_TEXT "Ingrese la altura del campo: (min 3, max 15)"
#define MINE_AMOUNT_TEXT "Introduzca el numero de minas: (min 1, max ancho * max alto - 1)"
#define OK_BUTTON_TEXT " OK "

#define MINE_INGAME_INFO "Minas Restantes: "

#define WIDTH_TEXT_Y 80
#define WIDTH_TEXT_W WINDOW_WIDTH / 1.75
#define WIDTH_TEXT_H 50

#define WIDTH_INPUT_Y 140
#define WIDTH_INPUT_W WINDOW_WIDTH / 30
#define WIDTH_INPUT_H 80

#define HEIGHT_TEXT_Y 240
#define HEIGHT_TEXT_W WINDOW_WIDTH / 1.75
#define HEIGHT_TEXT_H 50

#define HEIGHT_INPUT_Y 300
#define HEIGHT_INPUT_W WINDOW_WIDTH / 30
#define HEIGHT_INPUT_H 80

#define MINE_TEXT_Y 400
#define MINE_TEXT_W WINDOW_WIDTH / 1.5
#define MINE_TEXT_H 50

#define MINE_INPUT_Y 460
#define MINE_INPUT_W WINDOW_WIDTH / 30
#define MINE_INPUT_H 80

#define OK_BUTTON_Y 600
#define OK_BUTTON_W WINDOW_WIDTH / 4
#define OK_BUTTON_H 100

#define TILE_SIDE_SIZE 35
#define TILE_SPACING 5

#define COLOR_TIP {192, 192, 192}
#define COLOR_TITLE {255, 255, 255}
#define COLOR_FORM {120, 180, 255}
#define COLOR_BUTTON {0, 0, 0}
#define COLOR_SEL_INPT {255, 255, 255}
#define COLOR_ALERT {0, 255, 255, 192}
#define COLOR_MENU_TEXT {0, 0, 0, 255}

#define RED_INFO_POSITIVE 122
#define GREEN_INFO_POSITIVE 255
#define BLUE_INFO_POSITIVE 133
#define RED_INFO_NEGATIVE 255
#define GREEN_INFO_NEGATIVE 38
#define BLUE_INFO_NEGATIVE 38
#define RED_INFO_NEUTRAL 255
#define GREEN_INFO_NEUTRAL 255
#define BLUE_INFO_NEUTRAL 255

#define FINISH_BANNER_X 0
#define FINISH_BANNER_Y WINDOW_HEIGHT / 3
#define FINISH_BANNER_W WINDOW_WIDTH
#define FINISH_BANNER_H WINDOW_HEIGHT / 3

#define ALERT_BANNER_X 0
#define ALERT_BANNER_Y WINDOW_HEIGHT / 2.5
#define ALERT_BANNER_W WINDOW_WIDTH
#define ALERT_BANNER_H WINDOW_HEIGHT / 5

#define X_FINE_ADJUSTEMENT 2
#define Y_FINE_ADJUSTEMENT 0
#define TEXT_BOX_FINE_ADJUSTMENT 50

#define WINDOW_TITLE "Buscaminas"
#define TITLE "Buscaminas"
#define TITLE_H 20
#define TITLE_X WINDOW_WIDTH / 2
#define TITLE_Y WINDOW_HEIGHT - TITLE_H
#define TITLE_W WINDOW_WIDTH - WINDOW_WIDTH / 2

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define FONT_SIZE 100
#define FONT_PATH_MAIN "../resources/fonts/rexlia-rg.otf"
#define FONT_PATH_SECONDARY "../resources/fonts/rexlia-rg.otf"
#define ALPHA_UNSELECTED 64
#define ALPHA_SELECTED 127
#define RESET_OPTION 0

#define WINDOW_ICON "../resources/icons/window_icon.bmp"
#define MINE_BOOM_ICON "../resources/icons/mine_boom_icon.bmp"
#define MINE_DEATH_ICON "../resources/icons/mine_death_icon.bmp"
#define FLAG_ICON "../resources/icons/flag_icon.bmp"
#define EDGE_ICON "../resources/icons/edge_icon.bmp"
#define COVER_ICON "../resources/icons/cover_icon.bmp"
#define BACKGROUND_WALLPAPER "../resources/icons/background_menu.bmp"
#define NEW_OK_BUTTON "../resources/icons/new_ok_button.bmp"
#define QUIT_BUTTON "../resources/icons/quit_button.bmp"
#define PRESENTATION "../resources/icons/menu_presentation.png"

#define BACKGROUND_MUSIC_MENU "../resources/sounds/bg-menu_space-exploration.mp3"
#define BACKGROUND_MUSIC_STAGE "../resources/sounds/bg-stage_hypnosphere.mp3"
#define SOUND_EFFECT_L "../resources/sounds/effect-left-click_mixkit-select-click-1109.wav"
#define SOUND_EFFECT_R "../resources/sounds/effect-right-click_mixkit-negative-tone-interface-tap-2569.wav"
#define SOUND_EFFECT_MENU "../resources/sounds/effect-menu_mixkit-cool-interface-click-tone-2568.wav"
#define SOUND_EFFECT_MINE "../resources/sounds/effect-defeat_mixkit-arcade-game-explosion-echo-1698.wav"
#define SOUND_EFFECT_VICTORY "../resources/sounds/effect-victory_mixkit-warfare-horn-2289.wav"

#define ERROR_SDL_INIT "Error al inicializar SDL.\n"
#define ERROR_SDL_WINDOW "Error al crear SDL Window.\n"
#define ERROR_SDL_RENDERER "Error al crear SDL Renderer.\n"
#define ERROR_SDL_TTF "Error al inicializar SDL_TTF.\n"
#define ERROR_SDL_AUDIO "Error al inicializar SDL Audio.\n"
#define ERROR_SDL_INIT_PNG "Error al inicializar PNG support.\n"

#define AUTHOR "Buscaminas - v1.0.0.2025"

#define NEW_GAME_TEXT " Nuevo Juego "
#define QUIT_GAME_TEXT " Salir del Juego "

#define WIN_TEXT "Winner"
#define LOSE_TEXT "Game Over"

#endif