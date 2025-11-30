#include "config/config.h"
#include "npc_controller.h"
#include "card_table.h"
#include <string.h>
#include <stdio.h>
#include "dice_roller.h"
#include "npc_configsoul.h"

#define numberOfNPCs 4 // 总人数（NPC+玩家）




// 每一个具体的数据应当存储在对应的对象结构体中
static NPC npc1 = {.npc_index = 1};
static NPC npc2 = {.npc_index = 2};
static NPC npc3 = {.npc_index = 3};

static NPC player = {.npc_index = 4};

NPC *all_players[] = {&npc1, &npc2, &npc3, &player};

// 通过npc_index找到对应的对象
static NPC* findPlayerIndex (NPC* all_players[], int npc_index) { // 这里NPC* all_players[]会被退化为指针，也就是NPC **all_players
    int i;
    for (i = 0; i < numberOfNPCs; ++i) {
        if (all_players[i]->npc_index == npc_index) {
            return all_players[i];
        }
    }
    return NULL;
}

// 向UI更新新的NPC数据
static void sendCommand2Table_NPC_data_update (NPC update_target) {
    sendCommand2Table(
        Npc_data_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .npc_information = {
                .npc_index = update_target.npc_index,
                .hand_cards = update_target.handcards,
                .chips = update_target.chips
            }
        }
    );
}

// 对比HAND_CARDS结构体
static int equal_handcards(HAND_CARDS handcards_new, HAND_CARDS handcards_old) {
    return (
        handcards_new.cards[0].rank == handcards_old.cards[0].rank && 
        handcards_new.cards[0].suit == handcards_old.cards[0].suit &&
        handcards_new.cards[1].rank == handcards_old.cards[1].rank && 
        handcards_new.cards[1].suit == handcards_old.cards[1].suit
    );
}

// 接口函数供外界修改具体对象的数据
void changePlayerHandcards (int npc_index, HAND_CARDS new_handcards) {
    
    NPC* targetNPC = findPlayerIndex(all_players, npc_index);
    if (targetNPC != NULL) {
        if (!equal_handcards(new_handcards, targetNPC->handcards)) {
            targetNPC->handcards = new_handcards;
            sendCommand2Table_NPC_data_update(*targetNPC);
        }
    }

}

// change_chip 代表更改筹码的方向，正数代表加筹码，负数代表扣除筹码
void changePlayerChips (int npc_index, int change_chip) {

    NPC* targetNPC = findPlayerIndex(all_players, npc_index);
    if (targetNPC != NULL) {
        targetNPC->chips += change_chip;
        if (targetNPC->chips <= 0) {
            targetNPC->chips = 0;
            /*
                To DO: 筹码归零后要通知schedule，做出局准备
            */
        }
        sendCommand2Table_NPC_data_update(*targetNPC);
    }

}


void initNPC_chip (int npc_index) {
    int init_chip = generateRandomNumber(10, 1000);
    NPC* targetNPC = findPlayerIndex(all_players, npc_index);
    if (targetNPC != NULL) {
        targetNPC->chips = init_chip;
        sendCommand2Table_NPC_data_update(*targetNPC);
    }
}


// 询问对应id的玩家下注决策
Betting_Decision ask_decision (int current_index, PUBLIC_CARDS public_cards) {
    // 准备传入给对应npc的所有牌局信息
    Betting_Decision begging_decision;
    begging_decision = makeADecision(current_index, all_players, public_cards);
    return begging_decision;
}