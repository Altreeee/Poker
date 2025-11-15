/**
 * npc控制接口
 * 保存npc手牌状态、筹码数
 * 接收荷官的命令，如当前轮下注金额、公共牌等，筹码由荷官管理
 */
#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H
#include "config/interface.h"
#include "config/card.h"




typedef struct {
    char name[10]; // npc名称
    HandCards hand_cards; // 手牌
    int chips; // 筹码数
    IMessageReceiver receiveMessageFromDealer; // 消息接收器

    IMessageReceiver *receiverTarget; // 消息发送器
    void (*sendMessageToDealer)(void *self, const char *msg); // 发送消息给 dealer 的函数指针
} NPC;

/* 构造函数 */
NPC new_npc(const char *name, int chips);

// NPC 接收来自 dealer 的消息处理函数
void on_npc_message(void *self, const MSG* msg);

void sendMessage2Dealer (void *self, const char *msg);


#endif // NPC_CONTROLLER_H