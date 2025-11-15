#include "dice_roller.h"

int rollDice(int numberOfDice, int sidesPerDie) {
    int total = 0;
    for (int i = 0; i < numberOfDice; i++) {
        total += (rand() % sidesPerDie) + 1; // 掷骰子，结果在1到sidesPerDie之间
    }
    return total;
}

int generateRandomNumber(int min, int max){
    int res = rand() % (max - min + 1) + min;
    return res;
}
