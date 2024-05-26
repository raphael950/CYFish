#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include <string.h>

WINDOW* getScoreWin() {
    static WINDOW* scoreWin = NULL;
    if (scoreWin == NULL) {
        scoreWin = newwin(10, 20, 0, 0);
    }
    return scoreWin;
}

Player* askPlayers(int* nbPlayers) {
    int res = 0;
    while (res != 1 || nbPlayers == NULL || *nbPlayers < 2 || *nbPlayers > 6) {
        printw("Combien de joueurs ? ");
        refresh();
        res = scanw("%d", nbPlayers);
    }
    Player* players = malloc(sizeof(Player)**nbPlayers);
    for (int i = 0; i < *nbPlayers; i++) {
        players[i] = playerBuilder(i);
    }
    return players;
}

Player playerBuilder(int playerId) {
    Player player;
    player.playerId = playerId;
    printw("Nom du joueur %d: ", playerId + 1);
    refresh();
    int res = 0;
    do {
        res = wgetnstr(stdscr, player.name, 20);
    } while (res != 1 || strlen(player.name) == 0 || strlen(player.name) > 20);
    player.score = 0;
    return player;
}

Box** getContactBoxes(Map* map, Coord coord, int freeOnly, int* nBoxes) {
    Box** boxes = malloc(sizeof(Box*)*6);
    int i = 0;
    for (int j = 0; j < 6; j++) {
        Box* box = getRelativeBox(map, coord, j);
        if (box != NULL && !box->isMelt && (!freeOnly || box->playerId ==-1)) boxes[i++] = box;
    }
    *nBoxes = i;
    return boxes;
}

int isStuck(Map* map, Box* box) {
    int nBoxes;
    Box** boxes = getContactBoxes(map, box->coord, 1, &nBoxes);
    free(boxes);
    if (nBoxes == 0) return 1;
    return 0;
}

Box** getPlayablePenguins(Map* map, int playerId, int* nAlivePenguins) {
    Box** boxes = malloc(sizeof(Box*)*map->nBoxes);
    int i = 0;
    for (Box* box = map->boxes; box < map->boxes + map->nBoxes; box++) {
        if (box->playerId == playerId && !box->isMelt && !isStuck(map, box)) boxes[i++] = box;
    }
    *nAlivePenguins = i;
    return boxes;
}
void updateScore(Player* player, Map* map) {
    static WINDOW* scoreWin = NULL;
    if (scoreWin == NULL) {
        WINDOW* mapWin = map->mapWin;
        int x, y;
        getmaxyx(mapWin, y, x);
        scoreWin = newwin(y, 20, 0, x);
    }
    wattron(scoreWin, COLOR_PAIR(player->playerId + 1));
    mvwprintw(scoreWin, player->playerId, 0, "%s: %d", player->name, player->score);
    wattroff(scoreWin, COLOR_PAIR(player->playerId + 1));
    wrefresh(scoreWin);
}

void movePenguin(Box* from, Box* to, Player* player, Map* map) {
    if (from == NULL || to == NULL) return;
    if (from->playerId != player->playerId) return;
    if (to->playerId != -1) return;

    // Update score
    int score = 0;
    for (int i = 0; i < from->fishes; ++i) {
        score += from->fishValues[i];
    }
    player->score += score;
    updateScore(player, map);

    // Move penguin
    to->playerId = player->playerId;
    meltBox(from, map);
    printBox(to, map->mapWin, 0, 0);
}

int turn(Player* player, Map* map) {
    int nPenguins;
    Box** penguins = getPlayablePenguins(map, player->playerId, &nPenguins);
    if (nPenguins == 0) return 0;

    printMessage("C'est au tour de %s de jouer", player->name);
    int pengId = 0;

    if (nPenguins > 1) {
        int key;
        highlightBox(penguins[pengId], map->mapWin, player->playerId + 7);
        do {
            printMessage("%s sélection pingouin (%d/%d)", player->name, pengId+1, nPenguins);
            key = getch();
            if ((key == KEY_LEFT || key == 68) && (pengId - 1) >= 0) {
                highlightBox(penguins[pengId], map->mapWin, player->playerId + 1);
                pengId--;
            } else if ((key == KEY_RIGHT || key == 67) && (pengId + 1) < nPenguins) {
                highlightBox(penguins[pengId], map->mapWin, player->playerId + 1);
                pengId++;
            } else continue;
            highlightBox(penguins[pengId], map->mapWin, player->playerId + 7);
        } while (key != KEY_ENTER && key != 10);
        highlightBox(penguins[pengId], map->mapWin, player->playerId + 1);
    }

    int nPossibleMoves = 0;
    Box** possibleMoves = getContactBoxes(map, penguins[pengId]->coord, 1, &nPossibleMoves);

    if (nPossibleMoves == 0) return 0;

    int contactBoxId = 0;
    highlightBox(possibleMoves[contactBoxId], map->mapWin, player->playerId + 7);
    if (nPossibleMoves > 1) {
        int key;
        do {
            printMessage("%s sélection direction (%d/%d)", player->name, contactBoxId+1, nPossibleMoves);
            key = getch();
            if ((key == KEY_LEFT || key == 68) && (contactBoxId - 1) >= 0) {
                removeHighlightBox(possibleMoves[contactBoxId], map->mapWin);
                contactBoxId--;
            } else if ((key == KEY_RIGHT || key == 67) && (contactBoxId + 1) < nPossibleMoves) {
                removeHighlightBox(possibleMoves[contactBoxId], map->mapWin);
                contactBoxId++;
            } else continue;
            highlightBox(possibleMoves[contactBoxId], map->mapWin, player->playerId + 7);
        } while (key != KEY_ENTER && key != 10);
    }

    int direction = 0;
    // set direction to the direction of the selected box
    for (int i = 0; i < 6; i++) {
        if (possibleMoves[contactBoxId] == getRelativeBox(map, penguins[pengId]->coord, i)) {
            direction = i;
            break;
        }
    }

    // choice of steps
    int steps = 1;
    int maxSteps = getAvailableSteps(map, penguins[pengId]->coord, direction);

    if (maxSteps == 0) exit(1);

    int key;
    Box* to = getDistancedRelativeBox(map, penguins[pengId]->coord, direction, steps);
    do {
        printMessage("%s déplacement de %d case sur %d", player->name, steps, maxSteps);
        key = getch();
        if (key >= 48 && key <= 57) {
            steps = key - 48;
        }
        if (steps > maxSteps) steps = maxSteps;
        removeHighlightBox(to, map->mapWin);
        to = getDistancedRelativeBox(map, penguins[pengId]->coord, direction, steps);
        highlightBox(to, map->mapWin, player->playerId + 7);
    } while (key != KEY_ENTER && key != 10);

    if (to == NULL) exit(1);
    movePenguin(penguins[pengId], to, player, map);

    free(penguins);
    free(possibleMoves);
    return 1;
}

