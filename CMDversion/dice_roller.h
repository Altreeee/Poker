/**
 * 掷骰器，用于给npc的随机行为返回结果，
 * 通过npc性格的初始设置、当前筹码数和观察场内其它玩家的行为来决定骰子的数量和决策区间
 */
#ifndef DICE_ROLLER_H
#define DICE_ROLLER_H


/**
 * 掷骰器
 * 入参：骰子数量，骰子面数
 * 出参：骰子结果总和
 */
int rollDice(int numberOfDice, int sidesPerDie);

/**
 * 生成从min到max之间的随机数（包含min和max）
 */
int generateRandomNumber(int min, int max);


#endif // DICE_ROLLER_H