#ifndef INTERFACE_H
#define INTERFACE_H



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
    // 显示提示信息，由于ui是队列式处理任务，但是getUserInput却是阻塞式，因此为了让显示一定早于输入，要让显示在输入之前的状态
    Game_start,
    get_Game_start_confirm, // 将获取模式转换确认单独作为一个状态
    Shuffle,             // 洗牌
    Deal_the_hole_cards, // 发底牌, 每人发两张手牌
    get_Deal_the_hole_cards_confirm,

    Deal_the_first_publiccard,       // 发翻牌，翻开三张公共牌
    Deal_the_second_publiccard,
    Deal_the_third_publiccard,
    get_Deal_first_publiccard_confirm,
    get_Deal_second_publiccard_confirm,
    get_Deal_third_publiccard_confirm,

    Draw_for_the_big_blind, // 抽大盲注id
    collect_bigblind_chip, // 扣除大盲注筹码


    First_betting_round, // 第一轮下注
    get_First_betting_round_confirm,

    NPC1_Decision_post,
    NPC1_Decision,
    NPC2_Decision_post,
    NPC2_Decision,
    NPC3_Decision_post,
    NPC3_Decision,
    Player_Decision_post,
    Player_Decision,

    Showdown,            // 摊牌，开始比牌大小

    NPC1_in,
    NPC2_in,
    NPC3_in,
    Player_in,

    send_Hand_card_NPC1,
    send_Hand_card_NPC2,
    send_Hand_card_NPC3,
    send_Hand_card_player,
} GAME_STATE;

/**
 * 消息类型(上面的总和)
 */
typedef enum {
    Dealer_command,
    Game_state,
} MSG_TYPE;

typedef enum {
    From_dealer,
    From_scheduler,
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
 * 临时NPC结构体（测试card_table用）
 */
#include "card.h"

typedef struct {
    int npc_index; //1、2、3
    HAND_CARDS hand_cards;
    int chips;
} NPC_INFORMATION;

/**
 * 公共牌结构体
 */
typedef struct {
    PUBLIC_CARDS public_cards;
} PUBLIC_CARDS_INFORMATION;



typedef struct {
    char* specific_content; // 要显示的具体内容
} COMMUNICATE_INFORMATION;

/**
 * 其它模块向牌桌发送的消息类型
 */
typedef enum {
    Npc_data_update, // 更新npc数据
    Content_update,  // 更新通知显示数据
    Public_cards_update, // 更新公共牌数据
    Public_chips_update, // 更新公共筹码数据
} COMMAND_TYPE_TO_TABLE;

typedef union {
    NPC_INFORMATION npc_information; // 如果消息类型是Npc_data_update，直接传输新的NPC结构体过去
    COMMUNICATE_INFORMATION content_information; // 向ui传输显示通知的内容
    PUBLIC_CARDS_INFORMATION public_cards_information; // 如果消息类型是Public_cards_update，直接传新的公共牌过去
    int public_chips; // 如果消息类型是Public_chips_update，直接传新的公共筹码数
}  COMMAND_CONTENT_TO_TABLE;

/**
 * 其它模块向牌桌发送消息时统一使用的消息结构体
 */
typedef struct {
    COMMAND_TYPE_TO_TABLE msgtype;  // 消息类型
    COMMAND_CONTENT_TO_TABLE msgcontent; // 消息内容（几种不同结构体，更新不同部分的内容）
} COMMAND_MSG_TO_TABLE;





/**
 * 通用的回调函数消息模板接口
 */
typedef struct IMessageReceiver {
    void (*on_message)(void *self, MSG *msg);
} IMessageReceiver;


/**
 * 决定，弃牌（放弃本局）、跟注（跟上大盲注金额）、加注（增加下注额）
 */
typedef enum {
    Fold, // 弃牌
    Call, // 跟注
    Raise, // 加注
} Decision_Type;

#define NOT_RAISE_TYPE -1

typedef struct {
    Decision_Type decison_type;
    int raise_num; // 如果不是加注，则为-1, 使用时首先检测raise_num != NOT_RAISE_TYPE
} Betting_Decision;











#endif