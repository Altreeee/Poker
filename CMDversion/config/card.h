#ifndef CARD_SENDER_H
#define CARD_SENDER_H

typedef enum {
    Spades,     // 黑桃
    Clubs,      // 梅花
    Hearts,     // 红心
    Diamonds,   // 方块
} Suit;

typedef struct {
    int rank; // 牌面值 A1 2-10 J11 Q12 K13
    Suit suit; // 花色
} Card; // 单张牌

typedef struct {
    Card cards[2]; // 手牌包含两张牌
} HandCards; // 手牌

#endif // CARD_SENDER_H
