#ifndef _INITFIELD_H_
#define _INITFIELD_H_

typedef struct field {
    int **mat;
    int x;
    int y;
    int m;
} field_t;

// Sólo declaración externa de la variable global
struct tile {
    int x;
    int y;
    int w;
    int h;
};

field_t* initField(int x, int y, int mines);
field_t* initCover(int x, int y);

extern struct tile tile;

#endif