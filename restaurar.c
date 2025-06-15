#include <string.h>
#include "restaurar.h"
#include "variables.h"

int saveGame(const field_t *f, const field_t *c, const char *player, const char *elapsedTime, const int mineRemainingInt) {
    FILE *file = fopen("savegame.dat", "wb");
    if (!file) return 0;

    SaveGameData data;
    data.width = f->x - 2;
    data.height = f->y - 2;
    data.mines = f->m;
    data.mineRemainingInt = mineRemainingInt;
    currentTime = time(NULL);
    data.elapsedSeconds = difftime(currentTime, startTime);
    strncpy(data.player, player, MAX_NAME_LENGTH);
    strncpy(data.elapsedTime, elapsedTime, sizeof(data.elapsedTime));

    for (int i = 0; i < f->x; i++) {
        for (int j = 0; j < f->y; j++) {
            data.f_matrix[i][j] = f->mat[i][j];
            data.c_matrix[i][j] = c->mat[i][j];
        }
    }

    fwrite(&data, sizeof(SaveGameData), 1, file);
    fclose(file);
    return 1;
}

int loadGame(field_t **f, field_t **c, char *player, char *elapsedTime, int *mineRemainingInt, int *h, int *m) {
    FILE *file = fopen("savegame.dat", "rb");
    if (!file) return 0;

    SaveGameData data;
    fread(&data, sizeof(SaveGameData), 1, file);
    fclose(file);

    *h = data.width;
    *m = data.mines;
    startTime = time(NULL) - data.elapsedSeconds;
    *mineRemainingInt = data.mineRemainingInt;
    strncpy(player, data.player, MAX_NAME_LENGTH);
    strncpy(elapsedTime, data.elapsedTime, 16);

    *f = initField(data.width, data.width, data.mines);
    *c = initCover(data.width, data.width);

    for (int i = 0; i < (*f)->x; i++) {
        for (int j = 0; j < (*f)->y; j++) {
            (*f)->mat[i][j] = data.f_matrix[i][j];
            (*c)->mat[i][j] = data.c_matrix[i][j];
        }
    }

    return 1;
}

int deleteSaveGame() {
    return remove("savegame.dat") == 0;
}