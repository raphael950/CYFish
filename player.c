#include "player.h"
#include <stdio.h>
#include <stdlib.h>


Player* askPlayers(int* nbPlayers) {
    while (nbPlayers == NULL || *nbPlayers < 2 || *nbPlayers > 6) {
        printf("Combien de joueur ? ");
        scanf("%d", nbPlayers);
    }
    Player* players = malloc(sizeof(Player)**nbPlayers);
    for (int i = 0; i < *nbPlayers; i++) {
        players[i] = playerBuilder();
    }
    return players;
}

Player playerBuilder() {
    Player player;
    printf("Nom du joueur: ");
    scanf("%s", player.name);
    player.score = 0;
    return player;
}