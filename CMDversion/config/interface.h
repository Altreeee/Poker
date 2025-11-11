#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * 通用的回调函数消息模板接口
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
    dealer_COMMAND,
    game_STATE,
} MSG_TYPE;

typedef struct {
    MSG_TYPE type;
    union{
        DEALER_COMMAND dealer_command;
        GAME_STATE game_state;
    } detail;
} MSG;




/**
 * 其它模块向牌桌发送的消息类型
 */
typedef enum {
    NPC_NAME, // NPC的名字
    NPC_CHIPS, // NPC的筹码数量
    NPC_CARDS, // NPC的手牌
} COMMAND_TYPE_TO_TABLE;

/**
 * 其它模块向牌桌发送消息时统一使用的消息结构体
 */
typedef struct {
    COMMAND_TYPE_TO_TABLE msgtype;  // 消息类型
    void *payload;      // 指向数据的通用指针
} COMMAND_MSG_TO_TABLE;

#endif