//
// Created by Raphaël Drecourt on 21/05/2024.
//

#ifndef CYFISH_WINDOW_H
#define CYFISH_WINDOW_H
#include <ncurses.h>

WINDOW* getScoreWindow();
WINDOW* getMessageWindow();
void clearMessageWindow();
void printMessage(char* message, ...);

#endif //CYFISH_WINDOW_H
