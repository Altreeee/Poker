/**
 * 给npc赋予一个npc灵魂
 */

#include "npc_controller.h"
#include "config/interface.h"


// 外界调用函数，传入存储npc所能获取的全部信息，返回做出的决定
Betting_Decision makeADecision (int current_index, const NPC* npc_information, PUBLIC_CARDS public_cards, int forced_chips) {
    /* 
        npc默认处理方式 
    */
    Betting_Decision final_decision;

    // 只要跟得起就跟
    if (npc_information->chips - forced_chips >= 10) {
        final_decision.decison_type = Call;
        final_decision.raise_num = NOT_RAISE_TYPE;
    } 
    // 跟不起就弃牌
    else {
        final_decision.decison_type = Fold;
        final_decision.raise_num = NOT_RAISE_TYPE;
    }

    return final_decision;
}