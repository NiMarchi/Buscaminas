#ifndef HISTORIAL_H
#define HISTORIAL_H

#define MAX_REGISTROS 10
#define MAX_LINEA 150

#define MAX_HISTORIAL_ENTRIES 10
#define MAX_LINE_LENGTH 256

extern char historial_lineas[MAX_HISTORIAL_ENTRIES][MAX_LINE_LENGTH];
extern int historial_count;

void guardar_historial(int tam, int minas, const char *nombre, const char *tiempo, const char *resultado);
void cargar_historial_desde_archivo(const char *ruta);

#endif