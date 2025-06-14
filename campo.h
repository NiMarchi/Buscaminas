#ifndef CAMPO_H
#define CAMPO_H

#define OPEN_F 1
#define FLAG_F 2
#define MINE 20
#define MINE_TRIG 21
#define BLANK 0
#define COVER 10
#define FLAG 50
#define EDGE_T_B 100
#define EDGE_L_R 101

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