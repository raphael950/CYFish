#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "movement.h"
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

int isStuck(Map* map, Box* box) {
    for (int i = 0; i < 6; i++) {
        Box* relativeBox = getRelativeBox(map, box->coord, i);
        if (relativeBox != NULL && relativeBox->playerId == -1) {
            return 0;
        }
    }
    return 1;
}

int canPlay(Player* player, Map* map) {
    // foreach hexa where player is, check if a movement is possible
    for (Box* box = map->boxes; box < map->boxes + map->nBoxes; box++) {
        if (box-> playerId == player->playerId && !isStuck(map, box))
            return 1;
    }
    return 0;
}

Box** getAlivePenguins(Map* map, int playerId, int* nAlivePenguins) {
    Box** boxes = malloc(sizeof(Box*)*map->nBoxes);
    int i = 0;
    for (Box* box = map->boxes; box < map->boxes + map->nBoxes; box++) {
        if (isStuck(map, box)) box->isDead = 1;
        if (box->playerId == playerId && !box->isDead) boxes[i++] = box;
    }
    *nAlivePenguins = i;
    return boxes;
}

Box* penguinSelection(Map* map, int playerId, Box* from, int key) {
    if (map == NULL || from == NULL) return NULL;

    if (key == KEY_RIGHT || key == 67) {
        for (Box* box = from + 1; box < (map->boxes + map->nBoxes); box++) {
            if (box->playerId == from->playerId && !box->isDead) {
                return box;
            }
        }
        // return first box of player from the begining
        //if (from == (map->boxes - 1)) return NULL; // no box found
        //return penguinSelection(map, playerId, map->boxes - 1, key);

    }
    if (key == KEY_LEFT || key == 68) {
        for (Box* box = from-1; box >= map->boxes; box--) {
            if (box->playerId == from->playerId && !box->isDead) {
                return box;
            }
        }
        // return last box of player from the end
        //if (from == (map->nBoxes + map->boxes)) return NULL; // no box found
        //return penguinSelection(map, playerId, map->boxes + map->nBoxes, key);
    }
    // bad key
    return from;
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
    Box** penguins = getAlivePenguins(map, player->playerId, &nPenguins);
    if (nPenguins == 0) return 0;

    Box* selectedBox = penguins[0];
    WINDOW* popUp = getMessageWindow();
    printMessage("C'est au tour de %s de jouer", player->name);


    /*for (int i = 1; i < nPenguins; ++i) {
        // remove higlight of all boxes
        removeHighlightBox(penguins[i], map->mapWin);
    }*/

    int selectedPengIndex = 1;
    if (nPenguins > 1) {
        int key;
        do {
            printMessage("%s sélection pingouin (%d/%d)", player->name, selectedPengIndex, nPenguins);
            key = getch();
            highlightBox(selectedBox, map->mapWin, player->playerId + 1);
            selectedBox = penguinSelection(map, player->playerId, selectedBox, key);
            highlightBox(selectedBox, map->mapWin, player->playerId + 7);
            for (int i = 0; i < nPenguins; ++i) {
                if (penguins[i] == selectedBox) {
                    selectedPengIndex = i + 1;
                    break;
                }
            }
        } while (key != KEY_ENTER && key != 10);
    }

    Direction direction;
    int distance;

    Box* destination;
    do {
        printMessage("%s entrez la direction de 1 pour le Nord-Est à 6 pour le Nord-Ouest: ");
        direction = (getch() - 1);
        printMessage("%s entrez le nombre de pas");
        distance = getch();
        destination = getDistancedRelativeBox(map, selectedBox->coord, direction, distance);
    } while (destination == NULL || destination->playerId != -1);

    movePenguin(selectedBox, destination, player, map);
    free(penguins);
    return 1;
}

void showScores(Player* players, int nbPlayers, WINDOW* scoreWin) {

}