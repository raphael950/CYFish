#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
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

    init_color(7, 400, 600, 800); // Bleu Foncé
    init_color(8, 400, 800, 400); // Vert Foncé
    init_color(9, 800, 400, 400); // Rouge Foncé
    init_color(10, 400, 400, 800); // Violet Foncé
    init_color(11, 800, 800, 400); // Jaune Foncé
    init_color(12, 400, 800, 800); // Cyan Foncé

    init_color(13, 800, 800, 800); // Blanc

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
    do {
        for (Player* p = players; p < players + nbPlayers; p++) {
            int res = turn(p, map);
            if (res == 0) continue;
            played++;
        }
    } while(played != 0);

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