#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "campo.h"
#include "constantes.h"

extern FILE *logs;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

extern TTF_Font *font_main, *font_secondary;

extern Mix_Music *backgroundMusicMenu, *backgroundMusicStage;
extern Mix_Chunk *soundEffectL, *soundEffectR, *soundEffectMine, *soundEffectMenu, *soundEffectVictory;

extern SDL_Surface *tileTextSurface;

extern SDL_Texture *bgScrollTexture, *okButtonTexture, *coverIconTexture, *menuButtonTextTexture1, *menuButtonTextTexture2, *menuButtonTextTexture3,
*menuButtonTextTexture4, *tileTextTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture, *edgeIconTexture;

extern SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect,
menuButtonRect1, menuButtonRect2, menuButtonRect3, menuButtonRect4, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect,
heightFieldTextboxRect, mineAmountLabelRect, mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, infoTimeRect, infoTrampRect, tileSquareRect;

extern bool gameRunning, stageRunning, mainMenuRunning, selectMenuRunning, historyMenuRunning;
extern bool clickedL, clickedR, win, lose, showMines, resetIJ;

extern int mineRemainingInt, formField, option, historyCount;
extern int xm, ym, h, m, ijSelected[3], tileSideSize;
extern int counter1, counter2, counter3;

extern char paramInput1[15], paramInput2[5], paramInput3[5], elapsedTime[5], historyLines[MAX_HISTORIAL_ENTRIES][MAX_LINE_LENGTH];

extern time_t startTime, currentTime;

extern field_t *f, *c;

#endif