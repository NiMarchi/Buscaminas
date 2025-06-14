#include <stdio.h>
#include <string.h>

#include "historial.h"
#include "constantes.h"
#include "variables.h"

void guardar_historial(const int tam, const int minas, const char *nombre, const char *tiempo, const char *resultado) {
    Registro registros[MAX_REGISTROS];
    int total = 0;
    FILE *archivo = fopen("historial.txt", "r");

    // Leer registros existentes del archivo.
    if (archivo) {
        char linea[MAX_LINEA];
        while (fgets(linea, sizeof(linea), archivo) && total < MAX_REGISTROS) {
            sscanf(linea, "Tamaño: %9[^,], Minas: %d, Jugador: %49[^,], Tiempo: %5s, Resultado: %9[^\n]",
                   registros[total].tam,
                   &registros[total].minas,
                   registros[total].nombre,
                   registros[total].tiempo,
                   registros[total].resultado);
            total++;
        }
        fclose(archivo);
    }

    // Eliminar el registro más antiguo si hay 10.
    if (total == MAX_REGISTROS) {
        for (int i = 1; i < MAX_REGISTROS; i++) {
            registros[i - 1] = registros[i];
        }
        total--;
    }

    // Agregar nuevo registro.
    snprintf(registros[total].tam, sizeof(registros[total].tam), "%dx%d", tam, tam); // Tamaño como "10x10"
    registros[total].minas = minas;
    strncpy(registros[total].nombre, nombre, sizeof(registros[total].nombre) - 1);
    registros[total].nombre[sizeof(registros[total].nombre) - 1] = '\0';
    strncpy(registros[total].tiempo, tiempo, sizeof(registros[total].tiempo) - 1);
    registros[total].tiempo[sizeof(registros[total].tiempo) - 1] = '\0';
    strncpy(registros[total].resultado, resultado, sizeof(registros[total].resultado) - 1);
    registros[total].resultado[sizeof(registros[total].resultado) - 1] = '\0';
    total++;

    // Escribir todos los registros actualizados.
    archivo = fopen("historial.txt", "w");
    if (archivo) {
        for (int i = 0; i < total; i++) {
            fprintf(archivo, "Tamaño: %s, Minas: %d, Jugador: %s, Tiempo: %s, Resultado: %s\n",
                    registros[i].tam,
                    registros[i].minas,
                    registros[i].nombre,
                    registros[i].tiempo,
                    registros[i].resultado);
        }
        fclose(archivo);
    } else {
        perror("No se pudo abrir el archivo para escribir");
    }
}

void cargar_historial_desde_archivo(const char *ruta) {
    FILE *file = fopen(ruta, "r");
    if (!file) {
        perror("No se pudo abrir historial.txt");
        return;
    }

    historial_count = 0;
    while (fgets(historial_lineas[historial_count], MAX_LINE_LENGTH, file) && historial_count < MAX_HISTORIAL_ENTRIES) {
        const size_t len = strlen(historial_lineas[historial_count]);
        if (len > 0 && historial_lineas[historial_count][len - 1] == '\n') {
            historial_lineas[historial_count][len - 1] = '\0';
        }
        historial_count++;
    }
    fclose(file);
}