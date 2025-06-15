#include <string.h>
#include "historial.h"
#include "variables.h"

void saveHistory(const int size, const int mines, const char *name, const char *time, const char *result) {
    Register registers[MAX_REGISTERS];
    int total = 0;
    FILE *file = fopen("historial.txt", "r");

    // Leer registros existentes del archivo.
    if (file) {
        char line[MAX_LINE];
        while (fgets(line, sizeof(line), file) && total < MAX_REGISTERS) {
            sscanf(line, "Tamaño: %9[^,], Minas: %d, Jugador: %49[^,], Tiempo: %5s, Resultado: %9[^\n]",
               registers[total].size,
               &registers[total].mines,
               registers[total].name,
               registers[total].time,
               registers[total].result
            );
            total++;
        }
        fclose(file);
    }

    // Eliminar el registro más antiguo si hay 10.
    if (total == MAX_REGISTERS) {
        for (int i = 1; i < MAX_REGISTERS; i++) {
            registers[i - 1] = registers[i];
        }
        total--;
    }

    // Agregar nuevo registro.
    snprintf(registers[total].size, sizeof(registers[total].size), "%dx%d", size, size);
    registers[total].mines = mines;
    strncpy(registers[total].name, name, sizeof(registers[total].name) - 1);
    registers[total].name[sizeof(registers[total].name) - 1] = '\0';
    strncpy(registers[total].time, time, sizeof(registers[total].time) - 1);
    registers[total].time[sizeof(registers[total].time) - 1] = '\0';
    strncpy(registers[total].result, result, sizeof(registers[total].result) - 1);
    registers[total].result[sizeof(registers[total].result) - 1] = '\0';
    total++;

    // Escribir todos los registros actualizados.
    file = fopen("historial.txt", "w");
    if (file) {
        for (int i = 0; i < total; i++) {
            fprintf(file, "Tamaño: %s, Minas: %d, Jugador: %s, Tiempo: %s, Resultado: %s\n",
                registers[i].size,
                registers[i].mines,
                registers[i].name,
                registers[i].time,
                registers[i].result
            );
        }
        fclose(file);
    } else {
        perror("No se pudo abrir el archivo para escribir");
    }
}

void loadHistoryFile(const char *route) {
    FILE *file = fopen(route, "r");
    if (!file) {
        perror("No se pudo abrir historial.txt");
        return;
    }

    historyCount = 0;
    while (fgets(historyLines[historyCount], MAX_LINE_LENGTH, file) && historyCount < MAX_HISTORIAL_ENTRIES) {
        const size_t len = strlen(historyLines[historyCount]);
        if (len > 0 && historyLines[historyCount][len - 1] == '\n') {
            historyLines[historyCount][len - 1] = '\0';
        }
        historyCount++;
    }
    fclose(file);
}