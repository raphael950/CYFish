#include <ncurses.h>
#include "penguin.h"
#include "window.h"

int nbPenguin(int nbPlayer) {
    switch (nbPlayer) {
        case 2:
            return 4;
        case 3:
            return 3;
        case 4:
            return 2;
        default:
            return 1;
    }
}

void placeOnePenguin(Map* map, Player* player) {

    WINDOW* mapWin = map->mapWin;

    printMessage("C'est au tour de %s de séléctionner l'emplacement de son pingouin", player->name);
    Coord coord = coordBuilder(0, 0);
    Box* box = getBox(map, coord, 1);
    highlightBox(box, mapWin, player->playerId + 1);
    int key;
    do {
        key = getch();
        if (!boxSelection(key, map, &coord)) continue;

        Box* newBox = getBox(map, coord, 1);
        if (newBox == NULL) {
            coord = box->coord;
            continue;
        }
        printBox(box, mapWin, 0, 0);
        box = newBox;
        highlightBox(box, mapWin, player->playerId + 1);
        if ((key == KEY_ENTER || key == 10) && !isSpawnpoint(box)) {
            printMessage("%s prenez une autre case, celle-ci n'est pas valide", player->name);
        }
    } while ((key != KEY_ENTER && key != 10) || !isSpawnpoint(box));
    //removeHighlightBox(box, mapWin);
    box->playerId = player->playerId;
    printBox(box, mapWin, 0, 0);
}