//
// Created by Raphaël Drecourt on 21/05/2024.
//

#include "window.h"

WINDOW* getScoreWindow() {
    static WINDOW* scoreWin = NULL;
    if (scoreWin == NULL) {
        // place above message window
        WINDOW* messageWin = getMessageWindow();
        int y, x;
        getmaxyx(messageWin, y, x);
        scoreWin = newwin(3, 100, y-3-1, 0);
    }
    return scoreWin;
}

WINDOW* getMessageWindow() {
    static WINDOW* messageWin = NULL;
    if (messageWin == NULL) {
        messageWin = newwin(3, 100, LINES-1-3, 0);
    }
    return messageWin;
}

void printMessage(char* message, ...) {
    WINDOW* messageWin = getMessageWindow();
    wclear(messageWin);
    box(messageWin, 0, 0);
    va_list args;
    va_start(args, message);
    wmove(messageWin, 1, 1);
    vw_printw(messageWin, message, args);
    va_end(args);
    wrefresh(messageWin);
}