#ifndef CARD_SENDER_H
#define CARD_SENDER_H

#include "config/interface.h"
#include "config/card.h"

/*
typedef struct CardSender {
    IMessageReceiver msg_recive_interface; // 实现消息接收接口
} CardSender;

CardSender new_cardsender(void);
*/


Card* cardsenderProcesser(int num_cards);

#endif // CARD_SENDER_H