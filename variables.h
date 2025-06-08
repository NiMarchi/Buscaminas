#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "campo.h"

extern FILE *logs;

// Variables para manejar la fecha y hora
extern time_t t;
extern struct tm *info;
extern char buffer[80];

// Variables SDL
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

extern SDL_Color colorTip;
extern SDL_Color colorTitle;
extern SDL_Color colorInfo;
extern SDL_Color colorForm;
extern SDL_Color colorButton;
extern SDL_Color colorSelectInput;
extern SDL_Color colorAlert;
extern SDL_Color colorMenuText;

extern TTF_Font *font_main, *font_secondary;

extern Mix_Music *backgroundMusicMenu, *backgroundMusicStage;
extern Mix_Chunk *soundEffectL, *soundEffectR, *soundEffectMine, *soundEffectMenu, *soundEffectVictory;

extern SDL_Surface *aboutTextSurface, *bgScrollSurface, *menuPresentationSurface, *menuTitlePresentationSurface,
*menuButtonTextSurface1, *menuButtonTextSurface2, *menuButtonTextSurface3, *menuButtonTextSurface4,
*labelSurface, *textInputSurface, *okButtonSurface, *alertTextSurface, *infoTextSurface,
*tileTextSurface, *mineIconSurface, *mineBoomIconSurface, *mineDeathIconSurface, *flagIconSurface,
*edgeIconSurface, *coverIconSurface, *finaleTextSurface;

extern SDL_Texture *aboutTextTexture, *bgScrollTexture, *menuPresentationTexture, *menuTitlePresentationTexture,
*menuButtonTextTexture1, *menuButtonTextTexture2, *menuButtonTextTexture3, *menuButtonTextTexture4,
*labelTexture, *textInputTexture, *okButtonTexture, *alertTextTexture, *infoTextTexture,
*tileTextTexture, *mineIconTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture,
*edgeIconTexture, *coverIconTexture, *finaleTextTexture;

extern SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect,
menuButtonRect1, menuButtonRect2, menuButtonRect3, menuButtonRect4, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect,
heightFieldTextboxRect, mineAmountLabelRect, mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, tileSquareRect;

// Variables del juego
extern bool game_is_running, stage_is_running, main_menu_is_running, select_menu_is_running;
extern bool clickedL, clickedR, win, lose, canInteract, showMines, resetIJ, soundEffectPlayed;

extern int last_frame_time, angle, mineRemainingInt;
extern int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
extern int xm, ym, xi, xf, yi, yf;
extern int counter1, counter2, counter3;
extern int h, m, i, j;
extern int length, formField, option;
extern int alpha1, alpha2, alpha3, alpha4;
extern int ij_selected[3];
extern float delta_time;
extern char *aux;
extern char paramInput1[15], paramInput2[5], paramInput3[5], mineRemainingStr[5], infoPlayerName[25], mineRemainingConcat[20];

extern field_t *f, *c;

#endif