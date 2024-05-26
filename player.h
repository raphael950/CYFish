#ifndef PLAYER_H

#define PLAYER_H

#include "map.h"

typedef struct {
	char name[20];
	int score;
    int playerId;
} Player;

Player* askPlayers(int* nbPlayers);
Player playerBuilder(int playerId);
Box* penguinSelection(Map* map, int playerId, Box* from, int key);
int turn(Player* player, Map* map);
void movePenguin(Box* from, Box* to, Player* player, Map* map);

#endif
