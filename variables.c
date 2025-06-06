#include <stdio.h>
#include <time.h>

#include "variables.h"
#include "constantes.h"

FILE *logs;

time_t t;
struct tm *info;
char buffer[80];

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

TTF_Font *font_main = NULL, *font_secondary = NULL;

Mix_Music *backgroundMusicMenu = NULL, *backgroundMusicStage = NULL;
Mix_Chunk *soundEffectL = NULL, *soundEffectR = NULL, *soundEffectMine = NULL, *soundEffectMenu = NULL, *soundEffectVictory = NULL;

SDL_Surface *aboutTextSurface = NULL, *bgScrollSurface = NULL, *menuPresentationSurface = NULL, *menuTitlePresentationSurface = NULL,
*menuButtonTextSurface1 = NULL, *menuButtonTextSurface2 = NULL, *labelSurface = NULL, *textInputSurface = NULL, *okButtonSurface = NULL,
*alertTextSurface = NULL, *infoTextSurface = NULL, *tileTextSurface = NULL, *mineIconSurface = NULL, *mineBoomIconSurface = NULL,
*mineDeathIconSurface = NULL, *flagIconSurface = NULL, *edgeIconSurface = NULL, *coverIconSurface = NULL, *finaleTextSurface = NULL;

SDL_Texture *aboutTextTexture = NULL, *bgScrollTexture = NULL, *menuPresentationTexture = NULL, *menuTitlePresentationTexture = NULL,
*menuButtonTextTexture1 = NULL, *menuButtonTextTexture2 = NULL, *labelTexture = NULL, *textInputTexture = NULL, *okButtonTexture = NULL,
*alertTextTexture = NULL, *infoTextTexture = NULL, *tileTextTexture = NULL, *mineIconTexture = NULL, *mineBoomIconTexture = NULL,
*mineDeathIconTexture = NULL, *flagIconTexture = NULL, *edgeIconTexture = NULL, *coverIconTexture = NULL, *finaleTextTexture = NULL;

SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect, menuButtonRect1,
menuButtonRect2, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect, heightFieldTextboxRect, mineAmountLabelRect,
mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, tileSquareRect;

bool game_is_running = false, stage_is_running = false, main_menu_is_running = false, select_menu_is_running = false;
bool clickedL = false, clickedR = false, win = false, lose = false, canInteract = false, showMines = false, resetIJ = true, soundEffectPlayed = false;

int last_frame_time = 0, angle = 0, mineRemainingInt = 0;
int centerFieldX = 0, centerFieldY = 0, centerFormTextX = 0, centerFormInputX = 0;
int xm = 0, ym = 0, xi = 0, xf = 0, yi = 0, yf = 0;
int button_x = 0, button_y = 0, button_w = 0, button_h = 0;
int counter1 = 1, counter2 = 1, counter3 = 1;
int h = 0, m = 0, i = 0, j = 0;
int length = 0, formField = 0, option = RESET_OPTION;
int alpha1 = ALPHA_UNSELECTED, alpha2 = ALPHA_UNSELECTED, alpha3 = ALPHA_UNSELECTED, alpha4 = ALPHA_UNSELECTED;
int ij_selected[3] = {0};
float delta_time = 0.0f;
char *aux = NULL;
char paramInput1[15] = {0}, paramInput2[5] = {0}, paramInput3[5] = {0}, mineRemainingStr[5] = {0}, infoPlayerName[25] = {0}, mineRemainingConcat[20] = MINE_INGAME_INFO;

field_t *f = NULL, *c = NULL;