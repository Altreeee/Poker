#include "dealer.h"
#include "npc_controller.h"



int main() {
    // 创建 dealer 和 npc 实例
    Dealer dealer = new_dealer();
    NPC npc = new_npc("Alice", 100);

    /**
     * 消息处理部分
     */
    // dealer和npc双向绑定
    dealer.receiverTarget = &npc.receiveMessageFromDealer;
    npc.receiverTarget = &dealer.receiveMessageFromNPC;
    // 测试消息互发
    dealer.sendMessage2npc(&dealer, "Hello NPC!");
    npc.sendMessageToDealer(&npc, "Hello Dealer!");
}