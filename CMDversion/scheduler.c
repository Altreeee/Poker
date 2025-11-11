/**
 * 调度器
 * 在ui的循环过程中依次唤醒各个模块的处理函数
 */
#include "config/interface.h"

GAME_STATE current_game_state = Shuffle;

void scheduler_run(void) {
    switch (current_game_state) {
        case Shuffle:
            // 调用洗牌模块处理函数
            current_game_state = Deal_the_hole_cards; // 进入下一个状态
            break;
        case Deal_the_hole_cards:
            // 调用发底牌模块处理函数
            break;
        case Deal_the_flop:
            // 调用发翻牌模块处理函数
            break;
        case Deal_the_turn:
            // 调用发转牌模块处理函数
            break;
        case Deal_the_river:
            // 调用发河牌模块处理函数
            break;
        case Showdown:
            // 调用摊牌模块处理函数
            break;
    }
}