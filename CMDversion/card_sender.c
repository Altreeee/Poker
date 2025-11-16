/**
 * 发牌器
 */
#include "config/interface.h"
#include "config/card.h"
#include "card_sender.h"
#include "card_table.h"
#include "dice_roller.h"
#include <stdio.h>
#include <stdlib.h>



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
// 输入需要新发放的牌数，返回动态分配的结构体数组指针
CARD* cardsenderProcesser(int num_cards){
    // 测试用代码
    // printf("send %d cards out", num_cards); 不能再用printf，会影响ui显示

    /*
        返回num_cards对应数量的牌结构体
    */
    // 分配空间
    CARD* cards = malloc(sizeof(CARD)*num_cards);
    if(!cards) return NULL;
    // 装内容
    int i;
    for(i = 0; i < num_cards; i++){
        cards[i].rank = generateRandomNumber(1, 13);
        cards[i].suit = generateRandomNumber(0, 3);
    }

    return cards;
}


/* 
    动态分配的结构体数组指针例子
Item* create_items(size_t count) {
    Item* items = malloc(sizeof(Item) * count);
    if (!items) return NULL; // 分配失败要检查

    for (size_t i = 0; i < count; ++i) {
        items[i].id = i;
        sprintf(items[i].name, "Item_%zu", i);
    }

    return items; // 返回指向第一个元素的指针
}


    使用时
size_t n = 5;
Item* items = create_items(n);
if (items) {
    for (size_t i = 0; i < n; ++i) {
        printf("%d %s\n", items[i].id, items[i].name);
    }
    free(items); // 调用者负责释放
}
*/