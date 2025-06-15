#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "campo.h"
#include "constantes.h"

extern FILE *logs;

extern time_t t;
extern struct tm *info;
extern char buffer[80];

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

extern SDL_Color colorTip, colorTitle, colorForm, colorButton, colorSelectInput,
colorAlert, colorMenuText, colorInfo;

extern TTF_Font *font_main, *font_secondary;

extern Mix_Music *backgroundMusicMenu, *backgroundMusicStage;
extern Mix_Chunk *soundEffectL, *soundEffectR, *soundEffectMine, *soundEffectMenu, *soundEffectVictory;

extern SDL_Surface *tileTextSurface;

extern SDL_Texture *bgScrollTexture, *okButtonTexture, *coverIconTexture,
*menuButtonTextTexture1, *menuButtonTextTexture2, *menuButtonTextTexture3, *menuButtonTextTexture4,
*tileTextTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture, *edgeIconTexture;

extern SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect,
menuButtonRect1, menuButtonRect2, menuButtonRect3, menuButtonRect4, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect,
heightFieldTextboxRect, mineAmountLabelRect, mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, infoTimeRect, infoTrampRect, tileSquareRect;

extern bool game_is_running, stage_is_running, main_menu_is_running, select_menu_is_running, history_menu_is_running;
extern bool clickedL, clickedR, win, lose, canInteract, showMines, resetIJ, soundEffectPlayed, restored_game;

extern int last_frame_time, angle, mineRemainingInt;
extern int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
extern int xm, ym, xi, xf, yi, yf;
extern int counter1, counter2, counter3;
extern int h, m, i, j;
extern int length, formField, option;
extern int alpha1, alpha2, alpha3, alpha4;
extern int ij_selected[3];
extern char *aux;
extern char paramInput1[15], paramInput2[5], paramInput3[5], mineRemainingStr[5], elapsedTime[5], infoPlayerName[25], infoTime[25], mineRemainingConcat[25];

extern time_t startTime, currentTime;

extern char historyLines[MAX_HISTORIAL_ENTRIES][MAX_LINE_LENGTH];
extern int historyCount;

extern field_t *f, *c;

#endif