#include <stdio.h>
#include <time.h>
#include "variables.h"
#include "constantes.h"

FILE *logs;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

TTF_Font *font_main = NULL, *font_secondary = NULL;

Mix_Music *backgroundMusicMenu = NULL, *backgroundMusicStage = NULL;
Mix_Chunk *soundEffectL = NULL, *soundEffectR = NULL, *soundEffectMine = NULL, *soundEffectMenu = NULL, *soundEffectVictory = NULL;

SDL_Surface *tileTextSurface = NULL;

SDL_Texture *bgScrollTexture = NULL, *okButtonTexture = NULL, *coverIconTexture = NULL, *menuButtonTextTexture1 = NULL, *menuButtonTextTexture2 = NULL, *menuButtonTextTexture3 = NULL,
*menuButtonTextTexture4 = NULL, *tileTextTexture = NULL, *mineBoomIconTexture = NULL, *mineDeathIconTexture = NULL, *flagIconTexture = NULL, *edgeIconTexture = NULL;

SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect,
menuButtonRect1, menuButtonRect2, menuButtonRect3, menuButtonRect4, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect,
heightFieldTextboxRect, mineAmountLabelRect, mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, infoTimeRect, infoTrampRect, tileSquareRect;

bool gameRunning = false, stageRunning = false, mainMenuRunning = true, selectMenuRunning = false, historyMenuRunning = false;
bool clickedL = false, clickedR = false, win = false, lose = false, showMines = false, resetIJ = true;

int mineRemainingInt = 0, formField = 0, option = 0, historyCount = 0;
int xm = 0, ym = 0, h = 0, m = 0, ijSelected[3] = {0};
int counter1 = 1, counter2 = 1, counter3 = 1;

char paramInput1[15] = {0}, paramInput2[5] = {0}, paramInput3[5] = {0}, elapsedTime[5] = {0}, historyLines[MAX_HISTORIAL_ENTRIES][MAX_LINE_LENGTH];

time_t startTime, currentTime;

field_t *f = NULL, *c = NULL;