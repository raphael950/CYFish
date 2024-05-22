#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include "movement.h"
#include "window.h"




int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int nbPlayers = 0;
    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    printf("Penguins: %d\n", penguins);

    initscr();
    curs_set(0);
    noecho();
    start_color();
    use_default_colors();


    Map* map = mapBuilder(penguins*nbPlayers, 9, 9);
    
    
    init_color(1, 600, 800, 1000); // Bleu
    init_color(2, 600, 1000, 600); // Vert
    init_color(3, 1000, 600, 600); // Rouge
    init_color(4, 600, 600, 1000); // Violet
    init_color(5, 1000, 1000, 600); // Jaune
    init_color(6, 600, 1000, 1000); // Cyan

    init_color(7, 500, 700, 900); // Bleu Foncé
    init_color(8, 500, 900, 500); // Vert Foncé
    init_color(9, 900, 500, 500); // Rouge Foncé
    init_color(10, 500, 500, 900); // Violet Foncé
    init_color(11, 900, 900, 500); // Jaune Foncé
    init_color(12, 500, 900, 900); // Cyan Foncé

    init_color(13, 900, 900, 900); // Blanc

    init_pair(1, COLOR_WHITE, 1);
    init_pair(2, COLOR_WHITE, 2);
    init_pair(3, COLOR_WHITE, 3);
    init_pair(4, COLOR_WHITE, 4);
    init_pair(5, COLOR_WHITE, 5);
    init_pair(6, COLOR_WHITE, 6);
    init_pair(7, COLOR_WHITE, 7);
    init_pair(8, COLOR_WHITE, 8);
    init_pair(9, COLOR_WHITE, 9);
    init_pair(10, COLOR_WHITE, 10);
    init_pair(11, COLOR_WHITE, 11);
    init_pair(12, COLOR_WHITE, 12);
    init_pair(13, COLOR_BLACK, 13);

    WINDOW* popUp = getMessageWindow();

    refresh();
    box(popUp, 0, 0);
    wrefresh(popUp);

    for (int i = 0; i < map->nBoxes; ++i) {
        printBox(map->boxes + i, map->mapWin, 1, 1);
    }


    // Placement des pingouins
    for (int i = 0; i < penguins; i++) {
            for (Player* p = players; p < players + nbPlayers; p++) {
                placeOnePenguin(map, p);
            }
    }

    int played = 0;
    while(1) {
        // for each players
        for (Player* p = players; p < players + nbPlayers; p++) {
            int res = turn(p, map);
            if (res == 0) continue;
            played++;
            mvwprintw(popUp, 1, 1, "C'est au tour de %s de jouer", p->name);
            wrefresh(popUp);
            int key;
            do {
                key = getch();
                if (key == KEY_RIGHT || key == KEY_LEFT || key == KEY_UP || key == KEY_DOWN || key == 67 || key == 68 || key == 65 || key == 66) {
                    Box* from = penguinSelection(map, p->playerId, map->boxes, key);
                    if (from == NULL) continue;
                    Box* to = getRelativeBox(map, from->coord, key);
                    movePenguin(from, to, p, map);
                }
            } while (key != KEY_ENTER && key != 10);
        }
        if (played == 0) break; // game finished
    }//

    wrefresh(popUp);
    wrefresh(map->mapWin);
    getch();
    endwin();


    for (int i = 0; i < map->nBoxes; i++) {
        free(map->boxes[i].fishValues);
        free(map->boxes + i);
    }
    free(map);
    free(players);
}