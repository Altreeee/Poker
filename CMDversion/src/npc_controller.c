#include "config/config.h"
#include "npc_controller.h"
#include "card_table.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    const int npc_index; // 1\2\3:npc, 4: player
    HAND_CARDS handcards;
    int chips;
} NPC;

// 每一个具体的数据应当存储在对应的对象结构体中
static NPC npc1 = {.npc_index = 1};
static NPC npc2 = {.npc_index = 2};
static NPC npc3 = {.npc_index = 3};

static NPC player = {.npc_index = 4};



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
    if (npc_index == 1) {
        if (!equal_handcards(new_handcards, npc1.handcards)) {
            npc1.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc1);
        }
    } 
    else if (npc_index == 2) {
        if (!equal_handcards(new_handcards, npc2.handcards)) {
            npc2.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc2);
        }
    } 
    else if (npc_index == 3) {
        if (!equal_handcards(new_handcards, npc3.handcards)) {
            npc3.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc3);
        }
    } 
    else if (npc_index == 4) {
        if (!equal_handcards(new_handcards, player.handcards)) {
            player.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(player);
        }
    } 

}

void changePlayerChips (int npc_index, int new_chips) {
    if (npc_index == 1) {
        if (npc1.chips != new_chips) {
            npc1.chips = new_chips;
            sendCommand2Table_NPC_data_update(npc1);
        }
    } 
    else if (npc_index == 2) {
        if (npc2.chips != new_chips) {
            npc2.chips = new_chips;
            sendCommand2Table_NPC_data_update(npc2);
        }
    } 
    else if (npc_index == 3) {
        if (npc3.chips != new_chips) {
            npc3.chips = new_chips;
            sendCommand2Table_NPC_data_update(npc3);
        }
    } 
    else if (npc_index == 4) {
        if (player.chips != new_chips) {
            player.chips = new_chips;
            sendCommand2Table_NPC_data_update(player);
        }
    } 
}
