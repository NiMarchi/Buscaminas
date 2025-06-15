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

SDL_Color colorTip = COLOR_TIP, colorTitle = COLOR_TITLE, colorForm = COLOR_FORM, colorButton = COLOR_BUTTON, colorSelectInput = COLOR_SEL_INPT,
colorAlert = COLOR_ALERT, colorMenuText = COLOR_MENU_TEXT, colorInfo;

TTF_Font *font_main = NULL, *font_secondary = NULL;

Mix_Music *backgroundMusicMenu = NULL, *backgroundMusicStage = NULL;
Mix_Chunk *soundEffectL = NULL, *soundEffectR = NULL, *soundEffectMine = NULL, *soundEffectMenu = NULL, *soundEffectVictory = NULL;

SDL_Surface *tileTextSurface = NULL;

SDL_Texture *bgScrollTexture = NULL, *okButtonTexture = NULL, *coverIconTexture = NULL,
*menuButtonTextTexture1 = NULL, *menuButtonTextTexture2 = NULL, *menuButtonTextTexture3 = NULL, *menuButtonTextTexture4 = NULL,
*tileTextTexture = NULL, *mineBoomIconTexture = NULL, *mineDeathIconTexture = NULL, *flagIconTexture = NULL, *edgeIconTexture = NULL;

SDL_Rect bgScrollRect1, bgScrollRect2, menuPresentationRect1, menuPresentationRect2, menuTitlePresentationRect,
menuButtonRect1, menuButtonRect2, menuButtonRect3, menuButtonRect4, widthFieldLabelRect, widthFieldTextboxRect, heightFieldLabelRect,
heightFieldTextboxRect, mineAmountLabelRect, mineAmountTextboxRect, okButtonRect, infoRect, infoPlayer, infoTimeRect, infoTrampRect, tileSquareRect;

bool game_is_running = false, stage_is_running = false, main_menu_is_running = true, select_menu_is_running = false, history_menu_is_running = false;
bool clickedL = false, clickedR = false, win = false, lose = false, canInteract = false, showMines = false, resetIJ = true, soundEffectPlayed = false, restored_game = false;

int last_frame_time = 0, angle = 0, mineRemainingInt = 0;
int centerFieldX = 0, centerFieldY = 0, centerFormTextX = 0, centerFormInputX = 0;
int xm = 0, ym = 0, xi = 0, xf = 0, yi = 0, yf = 0;
int counter1 = 1, counter2 = 1, counter3 = 1;
int h = 0, m = 0, i = 0, j = 0;
int length = 0, formField = 0, option = 0;
int alpha1 = ALPHA_UNSELECTED, alpha2 = ALPHA_UNSELECTED, alpha3 = ALPHA_UNSELECTED, alpha4 = ALPHA_UNSELECTED;
int ij_selected[3] = {0};
char *aux = NULL;
char paramInput1[15] = {0}, paramInput2[5] = {0}, paramInput3[5] = {0}, mineRemainingStr[5] = {0}, elapsedTime[5] = {0}, infoPlayerName[25] = {0}, infoTime[25] = TIME_INFO, mineRemainingConcat[25] = MINE_INGAME_INFO;

char historyLines[MAX_HISTORIAL_ENTRIES][MAX_LINE_LENGTH];
int historyCount = 0;

time_t startTime, currentTime;

field_t *f = NULL, *c = NULL;