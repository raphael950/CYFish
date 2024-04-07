#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOX_LENGTH 9 // Length of chars that form Hexagon
#define BOX_WIDTH 5 // Width of chars that form Hexagon

#define BOX_Y 3 // Always supposed to be > 1 because of map scheme
#define BOX_X 9 // Always supposed to be > 0 and odd because of map scheme TODO: add odd support by fixing saveHexagons() first for loop x

#define SCREEN_WIDTH (BOX_Y * BOX_WIDTH - BOX_Y + 1) // Explained with a map scheme
#define SCREEN_LENGTH (BOX_LENGTH * (BOX_X / 2 + 1) + (BOX_LENGTH - 4) * (BOX_X / 2)) // Explained with a map scheme


#define random(min, max) (rand() % (max-min+1) + min)

#define MIN_FISHES 1
#define MAX_FISHES 3

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    char fishes;
    char fishSlots[MAX_FISHES]; // slots of fishes between 0 and 17, -1 for no fish
    char playerId; // -1 if no player allowed, 0 if no player, else it's player id
    Coordinate absoluteCoords; // cordonnes sur le plan parmis les autres hexagones
} Hexagon;


Hexagon hexagonBuilder(char fishes, char* fishSlots, char playerId) {
    Hexagon box;
    box.fishes = fishes;

    for (int i = 0; i < fishes; ++i) {
        box.fishSlots[i] = fishSlots[i];
    }
    box.playerId = playerId;
    return box;
}


void showBox(Hexagon box) {
    printf("Hexagon: \n");
    printf("- Fishes: %d\n- Slots: [", box.fishes);
    for (int i = 0; i < box.fishes; ++i) {
        printf("%d", box.fishSlots[i]);
        if (i+1 != box.fishes) printf(", ");
    }
    printf("]\n");
    if (box.playerId == -1) {
        printf("This box is out !\n");
    } else if (box.playerId == 0) {
        printf("No player on this box!\n");
    } else {
        printf("Player n°%d is on this box!\n");
    }
}

// On renvoie un Hexagon* pour pouvoir return NULL (aucun hexa dispo)
Hexagon* relativeHexagon(Hexagon map[BOX_Y][BOX_X], Hexagon start, int direction) {

    int x = start.absoluteCoords.x, y = start.absoluteCoords.y;
    int isColumnEven = (x % 2 == 0);

    // On modifie les coordonnes pour obtenir celles du futur hexagone
    switch (direction) {
        case 1:
            y--;
            break;
        case 4:
            y++;
            break;
        case 2:
            if (isColumnEven) y--;
            x++;
            break;
        case 3:
            if (!isColumnEven) y++;
            x++;
            break;
        case 5:
            if (!isColumnEven) y++;
            x--;
            break;
        case 6:
            if (isColumnEven) y--;
            x--;
            break;
        default:
            return NULL;
    }

    if (x < 0 || y < 0) return NULL;
    if (x >= BOX_X || y >= BOX_Y) return NULL;

    // Only for this type of scheme
    if (y == BOX_Y - 1 && !isColumnEven) return NULL;

    return &map[y][x];
}

void fillBlankMap(Hexagon map[BOX_Y][BOX_X]) {
    for (int y = 0; y < BOX_Y; ++y) {
        for (int x = 0; x < BOX_X; ++x) {

            map[y][x] = hexagonBuilder(0, NULL, 0);
            if (y == BOX_Y - 1 && x % 2 == 1) map[y][x].playerId = -1;
        }
    }
}

bool contains(char* tab, int len, char value) {
    for (int i = 0; i < len; ++i) {
        if (tab[i] == value)
            return true;
    }
    return false;
}

void defineRandomFishSlots(Hexagon* box) {
    for (int i = 0; i < box->fishes; ++i) {
        int slot;
        do {
            slot = random(0, 17);
        } while (contains(box->fishSlots, i, slot));
        box->fishSlots[i] = slot;
    }
}

void generateFishes(Hexagon map[BOX_Y][BOX_X], int minMonoPinguins) {
    int monoPinguinCount = 0;
    for (int y = 0; y < BOX_Y; ++y) {
        for (int x = 0; x < BOX_X; ++x) {

            // Check if Hexagon is in the map
            if(map[y][x].playerId != -1) {
                char randomFishNumber = random(1, 3);
                map[y][x].fishes = randomFishNumber;
                if (randomFishNumber == 1)
                    monoPinguinCount++;
            }
        }
    }

    // Remove fishes if they are too many, disturbing pinguins spawn
    while (monoPinguinCount < minMonoPinguins) {
        int x = rand() % BOX_Y, y = rand() % BOX_X;
        if (map[y][x].fishes > 1) {
            map[y][x].fishes = 1;
            monoPinguinCount++;
        }
    }

    // Random fish slots
    for (int y = 0; y < BOX_Y; ++y) {
        for (int x = 0; x < BOX_X; ++x) {
            // Check if Hexagon is in the map
            if(map[y][x].playerId != -1) {
                defineRandomFishSlots(&map[y][x]);
            }
        }
    }
}

void fillBlankScreen(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {
    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            screen[y][x] = ' ';
        }
    }
}


void showScreen(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {
    for (int y = 0; y < SCREEN_WIDTH; ++y) {
        for (int x = 0; x < SCREEN_LENGTH; ++x) {
            char letter = screen[y][x];

            if (letter == 'F') {
                printf("🐟");
            } else printf("%c", letter);
        }
        printf("\n");
    }
}



char penguinsNumber(char playerNumber) {
    switch (playerNumber) {
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

void saveOneHexagon(char screen[SCREEN_WIDTH][SCREEN_LENGTH], int xOffset, int yOffset) {

    if (xOffset + BOX_LENGTH > SCREEN_LENGTH || yOffset + BOX_WIDTH > SCREEN_WIDTH) return;

    int x, y;
    for (y = yOffset; y < yOffset+5; y+=4) {
        for (x = xOffset + 2; x < xOffset + 7; ++x)
            screen[y][x] = '_';
    }
    screen[1 + yOffset][1 + xOffset] = '/';
    screen[2 + yOffset][0 + xOffset] = '/';
    screen[3 + yOffset][8 + xOffset] = '/';
    screen[4 + yOffset][7 + xOffset] = '/';

    screen[1 + yOffset][7 + xOffset] = '\\';
    screen[2 + yOffset][8 + xOffset] = '\\';
    screen[3 + yOffset][0 + xOffset] = '\\';
    screen[4 + yOffset][1 + xOffset] = '\\';
}

void saveHexagons(char screen[SCREEN_WIDTH][SCREEN_LENGTH]) {

    for (int y = 0; y < SCREEN_WIDTH; y+=BOX_WIDTH/2) {
        for (int x = 0; x < SCREEN_LENGTH; x+=7) {
            if (y % 4 == 0 && x % 14 == 0 || y % 4 != 0 && x % 14 != 0) {
                saveOneHexagon(screen, x, y);
            }
        }
    }

}

void saveFishes(char screen[SCREEN_WIDTH][SCREEN_LENGTH], Hexagon map[BOX_Y][BOX_X]) {
    int xBox=0, yBox=0;
    for (int y = 0; y < SCREEN_WIDTH; y+=BOX_WIDTH/2) {
        for (int x = 0; x < SCREEN_LENGTH; x+=7) {

            if (y % 4 == 0 && x % 14 == 0 || y % 4 != 0 && x % 14 != 0) {
                if (xBox < BOX_X && yBox < BOX_Y) {
                    Hexagon box = map[yBox][xBox];
                    for (int i = 0; i < box.fishes; ++i) {
                        char slot = box.fishSlots[i];
                        if (slot < 5) screen[y+1][x+slot+2] = 'F';
                        else if (slot < 8) screen[y+2][x+slot+1-5] = 'F';
                        else if (slot < 11) screen[y+2][x+slot+1+1-5] = 'F';
                        else screen[y+3][x+slot+1-11] = 'F';
                    }
                    xBox+=2;
                }
            }
        }
        if (y % 4 == 0) {
            xBox = 1;
        } else {
            xBox = 0;
            yBox += 1;
        }
    }
}

int askPlayers(int min, int max) {
    int players, res = -1;
    do {
        printf("Combien de joueurs joueront à la partie ?");
        res = scanf("%d", &players);
        printf("\n");
    } while (res < 1 || players < min || players > max);
    return players;
}



int main() {
    srand(time(NULL));

    int players = askPlayers(2, 6);


    Hexagon map[BOX_Y][BOX_X];
    char screen[SCREEN_WIDTH][SCREEN_LENGTH] = {0};

    fillBlankScreen(screen);
    fillBlankMap(map);

    generateFishes(map, penguinsNumber(players) * players);

    saveHexagons(screen);
    showScreen(screen);

    saveFishes(screen, map);
    showScreen(screen);

    scanf("%d");

    return 0;
}
