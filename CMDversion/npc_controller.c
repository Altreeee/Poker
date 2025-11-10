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
void on_npc_message(void *self, const char *msg) {
    printf("NPC received: %s\n", msg);
}

void sendMessage2Dealer (void *self, const char *msg) {
    NPC *npc = (NPC *)self;
    if (npc->receiverTarget && npc->receiverTarget->on_message) {
        npc->receiverTarget->on_message(npc, msg);
    }
}

