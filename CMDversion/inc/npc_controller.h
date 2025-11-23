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
void changePlayerChips (int npc_index, int new_chips);



#endif // NPC_CONTROLLER_H