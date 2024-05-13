#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>


void removeHighlightBox(Box* box, WINDOW* mapWin) {
    if (box == NULL) return;
    int x = box->coord.x, y = box->coord.y;
    int yOffset = y*3;
    int xOffset = x*8 + (y%2==0)*4;

    mvwchgat(mapWin, yOffset, xOffset + 4, 1, A_NORMAL, 0, NULL);
    mvwchgat(mapWin, yOffset + 1, xOffset + 2, 5, A_NORMAL, 0, NULL);
    mvwchgat(mapWin, yOffset + 2, xOffset + 1, 7, A_NORMAL, 0, NULL);
    mvwchgat(mapWin, yOffset + 3, xOffset + 2, 5, A_NORMAL, 0, NULL);
    mvwchgat(mapWin, yOffset + 4, xOffset + 4, 1, A_NORMAL, 0, NULL);

    wrefresh(mapWin);
}
void highlightBox(Box* box, WINDOW* mapWin) {
    if (box == NULL) return;
    int x = box->coord.x, y = box->coord.y;
    int yOffset = y*3;
    int xOffset = x*8 + (y%2==0)*4;

    mvwchgat(mapWin, yOffset, xOffset + 4, 1, A_NORMAL, 8, NULL);
    mvwchgat(mapWin, yOffset + 1, xOffset + 2, 5, A_NORMAL, 8, NULL);
    mvwchgat(mapWin, yOffset + 2, xOffset + 1, 7, A_NORMAL, 8, NULL);
    mvwchgat(mapWin, yOffset + 3, xOffset + 2, 5, A_NORMAL, 8, NULL);
    mvwchgat(mapWin, yOffset + 4, xOffset + 4, 1, A_NORMAL, 8, NULL);

    wrefresh(mapWin);
}

void printBox(Box* box, WINDOW* mapWin, int printBorder, int printFishes) {
    if (box == NULL) return;
    int x = box->coord.x, y = box->coord.y;
    int yOffset = y*3;
    int xOffset = x*8 + (y%2==0)*4;
    if (printBorder) {
        mvwaddch(mapWin, yOffset, xOffset + 3, '/');
        mvwaddch(mapWin, yOffset, xOffset + 5, '\\');
        mvwaddch(mapWin, yOffset + 1, xOffset + 1, '/');
        mvwaddch(mapWin, yOffset + 1, xOffset + 7, '\\');
        mvwaddch(mapWin, yOffset + 2, xOffset, '|');
        mvwaddch(mapWin, yOffset + 2, xOffset + 8, '|');
        mvwaddch(mapWin, yOffset + 3, xOffset + 3, ' ');
        mvwaddch(mapWin, yOffset + 3, xOffset + 1, '\\');
        mvwaddch(mapWin, yOffset + 3, xOffset + 7, '/');
        mvwaddch(mapWin, yOffset + 4, xOffset + 3, '\\');
        mvwaddch(mapWin, yOffset + 4, xOffset + 5, '/');
    }
    if (printFishes && box->fishes > 0) {

    }
    if (box->playerId >= 0) {
        wchar_t peng[] = L"🐧";
        attron(COLOR_PAIR(box->playerId + 1));
        mvwprintw(mapWin, yOffset + 2, xOffset + 3, "\U0001f427 a");
        attroff(COLOR_PAIR(box->playerId + 1));
        //mvwprintw(mapWin, yOffset + 2, xOffset + 3, "");
    }
    wrefresh(mapWin);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);
    Map* map = mapBuilder(penguins*nbPlayers, 9, 9);

    initscr();
    start_color();
    use_default_colors();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLUE);



    int mapWinHeight = 5 + (map->width - 1) * 3;
    int mapWinLength = map->length * 8 + 1;

    WINDOW* mapWin = newwin(mapWinHeight, mapWinLength, 0, 0);
    WINDOW* popUp = newwin(3, 100, LINES-1-3, 0);

    refresh();
    //box(mapWin, 0, 0);
    box(popUp, 0, 0);


    for (int i = 0; i < map->nBoxes; ++i) {
        printBox(map->boxes + i, mapWin, 1, 1);
    }

    wrefresh(mapWin);
    wrefresh(popUp);
    noecho();

    // Placement des pingouins
    for (int i = 0; i < penguins; i++) {
            for (Player* p = players; p < players + nbPlayers; p++) {
                mvwprintw(popUp, 1, 1, "C'est au tour de %s de séléctionner l'emplacement de son pingouin", p->name);
                wrefresh(popUp);
                Coord coord = coordBuilder(0, 0);
                Box* box = getBox(map, coord);
                highlightBox(box, mapWin);
                int key;
                do {
                    key = getch();
                    switch (key) {
                        case KEY_UP:
                        case 66: // up
                            coord.y += 1;
                            break;
                        case KEY_DOWN:
                        case 65: // down
                            coord.y -= 1;
                            break;
                        case KEY_RIGHT:
                        case 67: // right
                            coord.x += 1;
                            break;
                        case KEY_LEFT:
                        case 68: // left
                            coord.x -= 1;
                            break;
                        case KEY_ENTER:
                        case 10:
                            break;
                        default:
                            /*
                            mvwprintw(popUp, 1, 1, "key: %d\n", key);
                            wrefresh(popUp);
                             */
                            continue;
                    }
                    Box* newBox = getBox(map, coord);
                    if (newBox == NULL) {
                        coord = box->coord;
                        continue;
                    }
                    removeHighlightBox(box, mapWin);
                    box = newBox;
                    highlightBox(box, mapWin);
                    if ((key == KEY_ENTER || key == 10) && !isSpawnpoint(box)) {
                        mvwprintw(popUp, 1, 1, "%s prenez une autre case, celle-ci n'est pas valide");
                        wrefresh(popUp);
                    }
                } while ((key != KEY_ENTER && key != 10) || !isSpawnpoint(box));
                removeHighlightBox(box, mapWin);
                box->playerId = p - players;
                printBox(box, mapWin, 0, 0);
            }
    }
    wrefresh(popUp);
    wrefresh(mapWin);
    refresh();              // Rafraîchit la fenêtre courante afin de voir le message apparaître
    curs_set(0);
    getch();                // On attend que l'utilisateur appui sur une touche pour quitter
    endwin();


    for (int i = 0; i < map->nBoxes; i++) {
        free(map->boxes[i].fishValues);
        free(map->boxes + i);
    }
    free(map);
    free(players);
}