#include <stdio.h>
#include <time.h>
#include "variables.h"

time_t t;
struct tm *info;
char buffer[80];

void saveEventGenericLog(const char *text) {
    time(&t);
    info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info);
    fprintf(logs, "%s: %s\n", buffer, text);
}

void saveEventMouseLog(const char *text, const int x, const int y) {
    time(&t);
    info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info);
    fprintf(logs, "%s: %s. Coordenadas: (x: %d, y: %d)\n", buffer, text, x, y);
}

void saveBlankLineLog() {
    fprintf(logs, "\n");
}