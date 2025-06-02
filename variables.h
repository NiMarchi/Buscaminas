#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "campo.h"

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

extern SDL_Surface *aboutTextSurface, *bgScrollSurface, *menuPresentationSurface, *menuTitlePresentationSurface, *menuButtonTextSurface1,
*menuButtonTextSurface2, *labelSurface, *textInputSurface, *okButtonSurface, *alertTextSurface, *infoTextSurface, *tileTextSurface,
*mineIconSurface, *mineBoomIconSurface, *mineDeathIconSurface, *flagIconSurface, *edgeIconSurface, *coverIconSurface, *finaleTextSurface;

extern SDL_Texture *aboutTextTexture, *bgScrollTexture, *menuPresentationTexture, *menuTitlePresentationTexture, *menuButtonTextTexture1,
*menuButtonTextTexture2, *labelTexture, *textInputTexture, *okButtonTexture, *alertTextTexture, *infoTextTexture, *tileTextTexture,
*mineIconTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture, *edgeIconTexture, *coverIconTexture, *finaleTextTexture;

extern SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect, menuButtonRect1,
menuButtonRect2, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect, heightFieldTextboxRect, mineAmountLabelRect,
mineAmountTextboxRect, okButtonRect, infoRect, tileSquareRect;

// Variables del juego
extern bool game_is_running, stage_is_running, main_menu_is_running, select_menu_is_running;
extern bool clickedL, clickedR, win, lose, canInteract, showMines, resetIJ, soundEffectPlayed;

extern int last_frame_time, angle, mineRemainingInt;
extern int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
extern int xm, ym, xi, xf, yi, yf;
extern int button_x, button_y, button_w, button_h;
extern int counter1, counter2, counter3;
extern int w, h, m, i, j;
extern int length, formField, option;
extern int alpha1, alpha2, alpha3, alpha4;
extern int ij_selected[3];
extern float delta_time;
extern char *aux;
extern char paramInput1[3], paramInput2[3], paramInput3[4], mineRemainingStr[4], mineRemainingConcat[18];

extern field_t *f, *c;

#endif