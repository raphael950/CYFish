#include "penguin.h"

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

