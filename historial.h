#ifndef HISTORIAL_H
#define HISTORIAL_H

typedef struct {
    char tam[10];
    int minas;
    char nombre[50];
    char tiempo[6];
    char resultado[10];
} Registro;

void guardar_historial(int tam, int minas, const char *nombre, const char *tiempo, const char *resultado);
void cargar_historial_desde_archivo(const char *ruta);

#endif