#ifndef CAMPO_H
#define CAMPO_H

typedef struct field {
    int **mat;
    int x;
    int y;
    int m;
} field_t;

typedef struct tile {
    int x;
    int y;
    int w;
    int h;
} tile_t;

field_t* initField(int x, int y, int mines);
field_t* initCover (int x, int y);
void countMines(const field_t *f);
void fillFieldZero(const field_t *f);
void fillFieldEdge(const field_t *f);
void fillFieldMine(const field_t *f);
void fillFieldCover(const field_t *f);
void openFieldUtil(field_t *f, field_t *c, int x, int y, int old);
void openField(field_t *f, field_t *c, int x, int y, int flag, int *flagQty);

extern tile_t tile;

#endif