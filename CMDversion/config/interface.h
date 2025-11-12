#ifndef INTERFACE_H
#define INTERFACE_H

/**
 * 通用的回调函数消息模板接口
 */
typedef struct IMessageReceiver {
    void (*on_message)(void *self, MSG *msg);
} IMessageReceiver;

// 统一模块返回类型
typedef enum{
    Success,
    Fail,
} MODULE_PROCESS_RESULT;




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

typedef enum {
    FROM_DEALER,
    FROM_SCHEDULER,
    // 其它来源模块，需要后续添加
} MSG_SOURCE;

typedef struct {
    MSG_SOURCE msg_source; // 消息来源
    union{
        DEALER_COMMAND dealer_command;
        // 根据不同的来源分为不同的命令类型
        GAME_STATE game_state;
    } detail;
} MSG;




/**
 * 其它模块向牌桌发送的消息类型
 */
typedef enum {
    CHANGE_NPC_CARDS, // NPC的手牌
} COMMAND_TYPE_TO_TABLE;

/**
 * 其它模块向牌桌发送消息时统一使用的消息结构体
 */
typedef struct {
    COMMAND_TYPE_TO_TABLE msgtype;  // 消息类型
    int index_player;   // 待更改的玩家序号
    int change; // 具体更改(正负数)
} COMMAND_MSG_TO_TABLE;

#endif