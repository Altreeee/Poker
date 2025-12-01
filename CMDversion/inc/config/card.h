#ifndef CARD_H
#define CARD_H

typedef enum {
    Spades,     // 黑桃
    Clubs,      // 梅花
    Hearts,     // 红心
    Diamonds,   // 方块
    OUT, // 已弃牌
} SUIT;

typedef struct {
    int rank; // 牌面值 A1 2-10 J11 Q12 K13
    SUIT suit; // 花色
} CARD; // 单张牌

typedef struct {
    CARD cards[2]; // 手牌包含两张牌
} HAND_CARDS; // 手牌

typedef struct {
    CARD cards[3]; // 公共牌包含三张牌
} PUBLIC_CARDS; // 公共牌

#endif // CARD_H
