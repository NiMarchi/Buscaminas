#ifndef CAMPO_H
#define CAMPO_H

typedef struct campo {
    int **mat;
    int x;
    int y;
    int m;
} campo_t;

typedef struct azulejo {
    int x;
    int y;
    int w;
    int h;
} azulejo_t;

campo_t* iniciarCampo(int x, int y, int minas);
campo_t* iniciarCobertura(int x, int y);
void contarMinas(const campo_t *f);
void rellenarCampoCero(const campo_t *f);
void rellenarBordeCampo(const campo_t *f);
void rellenarCampoMinas(const campo_t *f);
void rellenarCampoCobertura(const campo_t *f);
void abrirCampoUtil(campo_t *f, campo_t *c, int x, int y, int viejo);
void abrirCampo(campo_t *f, campo_t *c, int x, int y, int band, int *bandCant);

extern azulejo_t azulejo;

#endif