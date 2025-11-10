#include "dealer.h"
#include "npc_controller.h"
#include <stddef.h>
#include <stdio.h>

Dealer new_dealer(void) {
    Dealer d = {0};
    d.receiverTarget = NULL;
    d.sendMessage2npc = sendMessage2npc; // 实现发送消息的具体函数

    d.receiveMessageFromNPC.on_message = on_dealer_message; // 初始化消息接收器为空

    return d;
}

void sendMessage2npc(void *self, const char *msg) {
    Dealer *a = (Dealer *)self;
    if (a->receiverTarget && a->receiverTarget->on_message)
        a->receiverTarget->on_message(a->receiverTarget, msg);
}

// dealer 接收来自 NPC 的消息处理函数
void on_dealer_message(void *self, const char *msg) {
    printf("dealer received: %s\n", msg);
}