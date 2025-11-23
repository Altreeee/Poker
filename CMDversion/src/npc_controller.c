#include "config/config.h"
#include "npc_controller.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    HAND_CARDS handcards;
    int chips;
} NPC;

// 每一个具体的数据应当存储在对应的对象结构体中
static NPC npc1, npc2, npc3;
static NPC player;

// 向UI更新新的NPC数据
static void sendCommand2Table_NPC_data_update (int npc_index, NPC update_target) {
    sendCommand2Table(
        Npc_data_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .npc_information = {
                .npc_index = npc_index,
                .hand_cards = update_target.handcards
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
            sendCommand2Table_NPC_data_update(npc_index, npc1);
        }
    } 
    else if (npc_index == 2) {
        if (!equal_handcards(new_handcards, npc2.handcards)) {
            npc1.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc_index, npc2);
        }
    } 
    else if (npc_index == 3) {
        if (!equal_handcards(new_handcards, npc3.handcards)) {
            npc1.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc_index, npc3);
        }
    } 
    else if (npc_index == 4) {
        if (!equal_handcards(new_handcards, player.handcards)) {
            npc1.handcards = new_handcards;
            sendCommand2Table_NPC_data_update(npc_index, player);
        }
    } 

}