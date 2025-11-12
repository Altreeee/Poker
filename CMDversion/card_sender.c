/**
 * 发牌器
 */
#include "config/interface.h"
#include "card_sender.h"



/*
// 接收消息后的处理函数
static void on_cardsender_message(void *self, MSG *msg) {
    printf("CardSender received a message.\n");
}

CardSender new_cardsender(void) {
    CardSender cs = {0};
    cs.msg_recive_interface.on_message = on_cardsender_message; // 设置消息接收函数
    return cs;
}
*/

// 开放给外界使用的接口
// 输入需要新发放的牌数，输出对应数量的牌结构体
MODULE_PROCESS_RESULT cardsenderProcesser(int num_cards){
    // 测试用代码
    printf("send %d cards out", num_cards);
    return Success;
}
