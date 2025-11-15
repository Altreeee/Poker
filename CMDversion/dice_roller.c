#include "dice_roller.h"
#include <time.h>
#include <stdlib.h>

int rollDice(int numberOfDice, int sidesPerDie) {
    int total = 0;
    // 使用当前时间作为随机数种子
    srand((unsigned int)time(NULL));
    for (int i = 0; i < numberOfDice; i++) {
        total += (rand() % sidesPerDie) + 1; // 掷骰子，结果在1到sidesPerDie之间
    }
    return total;
}

int generateRandomNumber(int min, int max){
    // 使用当前时间作为随机数种子
    srand((unsigned int)time(NULL));
    int res = rand() % (max - min + 1) + min;
    return res;
}
