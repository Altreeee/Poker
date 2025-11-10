#include "config/config.h"
#include "npc_controller.h"
#include <string.h>
#include <stdio.h>


NPC new_npc(const char *name, int chips) {
    NPC p = {0}; // 所有内容置 0
    strcpy(p.name, name);
    p.chips = chips;
    p.receiveMessageFromDealer.on_message = on_npc_message; // 从 dealer 接收消息的消息接收器

    p.sendMessageToDealer = sendMessage2Dealer; // 发送消息给 dealer 的函数指针
    
    return p;
}

// NPC 接收来自 dealer 的消息处理函数
void on_npc_message(void *self, const MSG msg) {
    //printf("NPC received: %s\n", msg);
    switch(msg) {
        case DEALER_COMMAND:
            switch(msg) {
                case Place_bets_now:
                    // 等待用户或npc下注
                    // 将下注信息发送给 dealer
                    break;
            }
            break;
        case GAME_STATE:
            switch(msg) {
                // 洗牌
                case Shuffle:
                    printf("Shuffle cards now!\n");
                    break;
                // 发底牌
                case Deal_the_hole_cards:
                    // 等待荷官发来底牌的信息
                    break;
                case Deal_the_flop:
                    // 等待荷官发来新翻开的牌的信息
                    break;
                case Deal_the_turn:
                    break;
                case Deal_the_river:
                    break;
                case Showdown:
                    // 等待荷官发来结算信息
                    break;
            }
            break;
    }
}

void sendMessage2Dealer (void *self, const char *msg) {
    NPC *npc = (NPC *)self;
    if (npc->receiverTarget && npc->receiverTarget->on_message) {
        npc->receiverTarget->on_message(npc, msg);
    }
}

