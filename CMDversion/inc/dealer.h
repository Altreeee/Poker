/**
 * 荷官
 * 负责向所有npc/玩家发布当前进行状态
 * 向npc/玩家发送命令
 * 从npc/玩家接收各自的操作指令
 * 向牌桌发送洗牌命令
 * 对每个人分数进行结算并发布结果、更改所有玩家各自筹码数
 * 控制整局游戏的进程
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