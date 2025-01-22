#include "player.h"
#include "map.h"
#include "penguin.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>


void printCentered(int row, const char *text) {
    int x = 0, width = 0;
    getmaxyx(stdscr, width, x);  // Get the size of the window
    row += (width-7)/2;
    mvprintw(row, (x - strlen(text)) / 2, "%s", text);  // Print text centered
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int nbPlayers = 0;

    initscr();
    start_color();

    curs_set(0);
    use_default_colors();

    char* title[9] = {
        "  _______ ___ ___     _______ ___ _______ ___ ___ ",
        " |   _   |   Y   |   |   _   |   |   _   |   Y   |",
        " |.  1___|   1   |   |.  1___|.  |   1___|.  1   |",
        " |.  |___ \\_   _/    |.  __) |.  |____   |.  _   |",
        " |:  1   | |:  |     |:  |   |:  |:  1   |:  |   |",
        " |::.. . | |::.|     |::.|   |::.|::.. . |::.|:. |",
        " `-------' `---'     `---'   `---`-------`--- ---'",
        "                                                  ",
        "Press any key to start..."
    };

    int start_row = 0;

    clear();  // Clear screen

    for (int i = 0; i < 9; i++) {
        printCentered(i, title[i]);
    }

    refresh();
    getch();
    clear();
    refresh();

    Player* players = askPlayers(&nbPlayers);
    int penguins = nbPenguin(nbPlayers);
    noecho();


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
    init_pair(14, -1, -1);


    refresh();

    for (int i = 0; i < map->nBoxes; ++i) {
        printBox(map->boxes + i, map->mapWin, 1, 1);
    }


    // Placement des pingouins
    for (int i = 0; i < penguins; i++) {
            for (Player* p = players; p < players + nbPlayers; p++) {
                placeOnePenguin(map, p);
            }
    }

    int played;
    do {
        played = 0;
        for (Player* p = players; p < players + nbPlayers; p++) {
            if (turn(p, map)) played++;
        }
    } while(played != 0);

    // print end game and scores
    clear();
    printCentered(0, "Fin de la partie");
    // winner:

    int maxScore = 0;
    Player* winner = NULL;
    for (Player* p = players; p < players + nbPlayers; p++) {
        if (p->score > maxScore) {
            maxScore = p->score;
            winner = p;
        }
    }
    printCentered(1, "Le gagnant est:");
    printCentered(2, winner->name);

    refresh();


    getch();
    endwin();

    free(map);
    free(players);
}