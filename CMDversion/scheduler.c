/**
 * 调度器
 * 在ui的循环过程中依次唤醒各个模块的处理函数
 */
#include "config/interface.h"
#include "config/card.h"
#include "scheduler.h"
#include "card_table.h"
#include "card_sender.h"
#include <stdlib.h>
#include <string.h>

char ui_input[100]; // 存储从ui传来的用户输入内容

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
static void sendCommand2Table_simple (char *content) {
    sendCommand2Table(
        Content_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .content_information.specific_content = content
        }
    );
}




GAME_STATE current_game_state = Game_start;

void wakeUpScheduler(void) {
    CARD* card_t; // 临时存储发牌器发来的牌结构体数组指针，注意：后面需要free
    int new_cards_num;
    switch (current_game_state) {
        case Game_start:
            sendCommand2Table_simple("Game Start(Y/N?)");
            current_game_state = get_Game_start_confirm;
            break;

        case get_Game_start_confirm:
            getUserInput(ui_input);
            // if(ui_input == "Y" || ui_input == "y") {
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                current_game_state = Deal_the_hole_cards;
            } 
            else {
                current_game_state = Game_start;
            }
            break;

        case Deal_the_hole_cards:
            /*
                给多个外部模块依次发消息，并等待回信，
                一得到一个回信就立刻调用sendCommand2Table将命令装到card_table的队列中，
                然后接着发送下一个消息并等待回复然后把得到的消息发到card_table模块去更新UI
            */
            sendCommand2Table_simple("Deal_the_hole_cards");

            // 调用发底牌模块处理函数，一次性拿8张牌，每人2张
            new_cards_num = 8;
            card_t = cardsenderProcesser(new_cards_num);
            if (card_t){
                // 给每个npc新发两张牌（这里应该去修改npc_controller中保存的npc具体数据，
                //      然后由npc_controller发送命令更新数据，但是测试阶段跳过这部分，直接向ui发送修改）
                //      直接发送一个新NPC结构体
                sendCommand2Table(
                    Npc_data_update, 
                    (COMMAND_CONTENT_TO_TABLE){
                        .npc_information = {
                            .npc_index = 1,
                            .hand_cards = {
                                .cards = {
                                    {.rank = card_t[0].rank, .suit = card_t[0].suit},
                                    {.rank = card_t[1].rank, .suit = card_t[1].suit}
                                }
                            }
                        }
                    }
                );
                sendCommand2Table(
                    Npc_data_update, 
                    (COMMAND_CONTENT_TO_TABLE){
                        .npc_information = {
                            .npc_index = 2, // 给下一个npc发牌
                            .hand_cards = {
                                .cards = {
                                    {.rank = card_t[2].rank, .suit = card_t[2].suit},
                                    {.rank = card_t[3].rank, .suit = card_t[3].suit}
                                }
                            }
                        }
                    }
                );
                sendCommand2Table(
                    Npc_data_update, 
                    (COMMAND_CONTENT_TO_TABLE){
                        .npc_information = {
                            .npc_index = 3, // 给下一个npc发牌
                            .hand_cards = {
                                .cards = {
                                    {.rank = card_t[4].rank, .suit = card_t[4].suit},
                                    {.rank = card_t[5].rank, .suit = card_t[5].suit}
                                }
                            }
                        }
                    }
                );
                sendCommand2Table(
                    Npc_data_update, 
                    (COMMAND_CONTENT_TO_TABLE){
                        .npc_information = {
                            .npc_index = 4, // 给玩家发牌
                            .hand_cards = {
                                .cards = {
                                    {.rank = card_t[6].rank, .suit = card_t[6].suit},
                                    {.rank = card_t[7].rank, .suit = card_t[7].suit}
                                }
                            }
                        }
                    }
                );
                free(card_t);
            }
            
            current_game_state = Deal_the_flop;
            break;

        case Deal_the_flop:
            // 翻开3张公共牌
            sendCommand2Table_simple("Deal_the_flop");


            current_game_state = Deal_the_turn;
            break;

        case Deal_the_turn:
            sendCommand2Table_simple("Deal_the_flop");
            
            current_game_state = Game_start;
            break;
    }
    // free(card_t);
}