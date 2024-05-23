#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "window.h"

WINDOW* getScoreWin() {
    static WINDOW* scoreWin = NULL;
    if (scoreWin == NULL) {
        scoreWin = newwin(10, 20, 0, 0);
    }
    return scoreWin;
}

Player* askPlayers(int* nbPlayers) {
    while (nbPlayers == NULL || *nbPlayers < 2 || *nbPlayers > 6) {
        printf("Combien de joueurs ? ");
        scanf("%d", nbPlayers);
    }
    Player* players = malloc(sizeof(Player)**nbPlayers);
    for (int i = 0; i < *nbPlayers; i++) {
        players[i] = playerBuilder(i);
        // TODO: Check if name already used.
    }
    return players;
}

Player playerBuilder(int playerId) {
    Player player;
    player.playerId = playerId;
    printf("Nom du joueur: ");
    scanf("%s", player.name);
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

int canPlay(Player* player, Map* map) {
    // foreach hexa where player is, check if a movement is possible
    for (Box* box = map->boxes; box < map->boxes + map->nBoxes; box++) {
        if (box-> playerId == player->playerId && !isStuck(map, box))
            return 1;
    }
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

void movePenguin(Box* from, Box* to, Player* player, Map* map) {
    if (from == NULL || to == NULL) return;
    if (from->playerId != player->playerId) return;
    if (to->playerId != -1) return;

    int score = 0;
    for (int i = 0; i < from->fishes; ++i) {
        score += from->fishValues[i];
    }
    player->score += score;
    // TODO:
    // - Check if the player is stuck

    to->playerId = player->playerId;
    from->playerId = -1;
    printBox(from, map->mapWin, 0, 0);
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

    int direction = 0;
    highlightBox(possibleMoves[direction], map->mapWin, player->playerId + 7);
    if (nPossibleMoves > 1) {
        int key;
        do {
            printMessage("%s sélection direction (%d/%d)", player->name, direction+1, nPossibleMoves);
            key = getch();
            if ((key == KEY_LEFT || key == 68) && (direction - 1) >= 0) {
                removeHighlightBox(possibleMoves[direction], map->mapWin);
                direction--;
            } else if ((key == KEY_RIGHT || key == 67) && (direction + 1) < nPossibleMoves) {
                removeHighlightBox(possibleMoves[direction], map->mapWin);
                direction++;
            } else continue;
            highlightBox(possibleMoves[direction], map->mapWin, player->playerId + 7);
        } while (key != KEY_ENTER && key != 10);
    }



int moveId = 0;




    free(penguins);
    return 1;
}

void showScores(Player* players, int nbPlayers, WINDOW* scoreWin) {

}