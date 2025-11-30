/**
 * npc控制接口
 * 保存npc手牌状态、筹码数
 * 接收荷官的命令，如当前轮下注金额、公共牌等，筹码由荷官管理
 */
#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H
#include "config/interface.h"
#include "config/card.h"


void changePlayerHandcards (int npc_index, HAND_CARDS new_handcards);
void changePlayerChips (int npc_index, int change_chip);
void initNPC_chip (int npc_index);
Betting_Decision ask_decision (int current_index, PUBLIC_CARDS public_cards);


typedef struct {
    const int npc_index; // 1\2\3:npc, 4: player
    HAND_CARDS handcards;
    int chips;
} NPC;

#endif // NPC_CONTROLLER_H