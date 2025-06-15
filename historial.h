#ifndef HISTORIAL_H
#define HISTORIAL_H

typedef struct {
    char size[10];
    int mines;
    char name[50];
    char time[6];
    char result[10];
} Register;

void saveHistory(const int size, const int mines, const char *name, const char *time, const char *result);
void loadHistoryFile(const char *route);

#endif