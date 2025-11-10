/**
 * 荷官
 */
#ifndef DEALER_H
#define DEALER_H
#include "config/interface.h"


typedef struct {
    IMessageReceiver *receiverTarget; // 发送目标 npc/玩家
    void (*sendMessage2npc)(void *self, const char *msg); // 发送消息给 npc/玩家的方法

    IMessageReceiver receiveMessageFromNPC; // 消息接收器
} Dealer;

Dealer new_dealer(void);

void sendMessage2npc(void *self, const char *msg);

void on_dealer_message(void *self, const char *msg);


#endif // DEALER_H