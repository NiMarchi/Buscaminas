#ifndef LOGS_H
#define LOGS_H

void saveEventGenericLog(const char *text);
void saveEventMouseLog(const char *text, const int x, const int y);
void saveBlankLineLog();

#endif