/**
 * 调度器
 * 在ui的循环过程中依次唤醒各个模块的处理函数
 */
#include "config/interface.h"
#include "config/card.h"
#include "scheduler.h"
#include "card_table.h"
#include "card_sender.h"
#include "npc_controller.h"
#include <stdlib.h>
#include <string.h>

char ui_input[100]; // 存储从ui传来的用户输入内容
int public_chip = 0; // 公共筹码由scheduler管理

/*
// 向绑定的第index个模块发送消息
static void schedulerSend(void *self, int index, MSG *msg){
    Scheduler *scheduler = (Scheduler *)self;
    int i = index;
    
    if(scheduler->send_target[i] && scheduler->send_target[i]->on_message){
        scheduler->send_target[i]->on_message(scheduler->send_target[i], msg);
    }
    
}

Scheduler new_scheduler(void){
    Scheduler scheduler = {0};
    scheduler.send = schedulerSend;
    return scheduler;
}
*/

// 简易显示
static void sendCommand2Table_sendcontent (char *content) {
    sendCommand2Table(
        Content_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .content_information.specific_content = content
        }
    );
}

// 简易发手牌
static void changePlayerHandcards_simple (int npc_index, int handcard1_rank, int handcard1_suit, int handcard2_rank, int handcard2_suit) {
    changePlayerHandcards(
        npc_index, 
        (HAND_CARDS){
            .cards = {
                {.rank = handcard1_rank, .suit = handcard1_suit},
                {.rank = handcard2_rank, .suit = handcard2_suit}
            }
        }
    );
}

// 简易发公共牌
static void sendCommand2Table_sendpubliccard (int publiccard1_rank, int publiccard1_suit, int publiccard2_rank, int publiccard2_suit, int publiccard3_rank, int publiccard3_suit) {
    sendCommand2Table(
        Public_cards_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .public_cards_information = {
                .public_cards = {
                    .cards = {
                        {.rank = publiccard1_rank, .suit = publiccard1_suit},
                        {.rank = publiccard2_rank, .suit = publiccard2_suit},
                        {.rank = publiccard3_rank, .suit = publiccard3_suit}
                    }
                }
            }
        }
    );
}




GAME_STATE current_game_state = Game_start;

void wakeUpScheduler(void) {
    CARD* card_t; // 临时存储发牌器发来的牌结构体数组指针，注意：后面需要free
    int new_cards_num;
    switch (current_game_state) {
        // 开始
        case Game_start:
            sendCommand2Table_sendcontent("Game Start(Y/N?)");
            current_game_state = get_Game_start_confirm;
            break;

        case get_Game_start_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                
                // 将所有牌+筹码初始化
                changePlayerHandcards_simple(1, 0, 0, 0, 0);
                changePlayerHandcards_simple(2, 0, 0, 0, 0);
                changePlayerHandcards_simple(3, 0, 0, 0, 0);
                changePlayerHandcards_simple(4, 0, 0, 0, 0);

                // 向npc_controller发送消息，让其准备好每个人的筹码
                //（如果上一局npc的筹码仍然足够最低入局数，则继续使用，如果在牌局中归零，或者再开局时低于最低入局数，则生成一个新的npc）
                changePlayerChips(1,0);
                changePlayerChips(2,0);
                changePlayerChips(3,0);
                changePlayerChips(4,0);

                sendCommand2Table_sendpubliccard(0, 0, 0, 0, 0, 0);

                current_game_state = Deal_the_hole_cards;
            } 
            else {
                current_game_state = get_Game_start_confirm;
            }
            break;





        // 发手牌
        case Deal_the_hole_cards:
            /*
                给多个外部模块依次发消息，并等待回信，
                一得到一个回信就立刻调用sendCommand2Table将命令装到card_table的队列中，
                然后接着发送下一个消息并等待回复然后把得到的消息发到card_table模块去更新UI
            */
            sendCommand2Table_sendcontent("Deal hands(Y/N?)");
            current_game_state = get_Deal_the_hole_cards_confirm;
            break;

        case get_Deal_the_hole_cards_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                // 调用发底牌模块处理函数，一次性拿8张牌，每人2张
                new_cards_num = 8;
                card_t = cardsenderProcesser(new_cards_num);
                if (card_t){
                    // 给每个npc新发两张牌，修改npc_controller中保存的npc具体数据，然后由npc_controller发送命令更新数据
                    changePlayerHandcards_simple(1, card_t[0].rank, card_t[0].suit, card_t[1].rank, card_t[1].suit);
                    changePlayerHandcards_simple(2, card_t[2].rank, card_t[2].suit, card_t[3].rank, card_t[3].suit);
                    changePlayerHandcards_simple(3, card_t[4].rank, card_t[4].suit, card_t[5].rank, card_t[5].suit);
                    changePlayerHandcards_simple(4, card_t[6].rank, card_t[6].suit, card_t[7].rank, card_t[7].suit);

                    // 给每个人发10块筹码（测试用）
                    changePlayerChips(1,10);
                    changePlayerChips(2,10);
                    changePlayerChips(3,10);
                    changePlayerChips(4,10);
                    
                    free(card_t);
                }
                current_game_state = Deal_the_flop;
            }
            else {
                current_game_state = get_Deal_the_hole_cards_confirm;
            }
            break;





        // 翻公共牌
        case Deal_the_flop:
            // 翻开3张公共牌
            sendCommand2Table_sendcontent("Deal flops(Y/N?)");
            current_game_state = get_Deal_the_flop_confirm;
            break;

        case get_Deal_the_flop_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                new_cards_num = 3;
                card_t = cardsenderProcesser(new_cards_num);
                int i;
                // 构造临时公共牌结构体
                PUBLIC_CARDS public_cards_t;
                for (i = 0; i < 3; ++i) {
                    public_cards_t.cards[i] = card_t[i];
                }

                if (card_t) {
                    sendCommand2Table(
                        Public_cards_update, 
                        (COMMAND_CONTENT_TO_TABLE){
                            .public_cards_information = {
                                .public_cards = public_cards_t
                            }
                        }
                    );
                }

                current_game_state = Deal_the_turn;
            }
            else {
                current_game_state = get_Deal_the_flop_confirm;
            }
            break;






        case Deal_the_turn:
            sendCommand2Table_sendcontent("Deal_the_flop");
            
            current_game_state = Game_start;
            break;
    }
    // free(card_t);
}