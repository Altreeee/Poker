#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * 通用的消息模板接口
 */
typedef struct IMessageReceiver {
    void (*on_message)(void *self, const char *msg);
} IMessageReceiver;

/**
 * 从荷官处发给npc的命令枚举
 */
typedef enum {
    Place_bets_now, // 开始下注
} DEALER_COMMAND;

/**
 * 当前进行状态
 */
typedef enum {
    Shuffle,             // 洗牌
    Deal_the_hole_cards, // 发底牌, 每人发两张手牌
    Deal_the_flop,       // 发翻牌，翻开三张公共牌
    Deal_the_turn,       // 发转牌，翻开第四张公共牌
    Deal_the_river,      // 发河牌，翻开第五张公共牌
    Showdown,            // 摊牌，开始比牌大小
} GAME_STATE;

/**
 * 消息类型(上面的总和)
 */
typedef enum {
    DEALER_COMMAND,
    GAME_STATE,
} MSG;

#endif