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
#include "dice_roller.h"
#include <stdio.h>

char ui_input[100]; // 存储从ui传来的用户输入内容
int public_chip = 0; // 公共筹码由scheduler管理
PUBLIC_CARDS public_cards = {0}; // 公共牌由scheduler管理

int id_chosen = 0; // 大盲注id号，初始化为0，使用值应为1-4

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

// 简易更改公共牌
static void sendCommand2Table_sendpubliccard () {
    sendCommand2Table(
        Public_cards_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .public_cards_information = {
                .public_cards = public_cards
            }
        }
    );
}

// 简易更改公共筹码数
static void updatePublicChips () {
    sendCommand2Table(
        Public_chips_update, 
        (COMMAND_CONTENT_TO_TABLE){
            .public_chips = public_chip
        }
    );
}


GAME_STATE current_game_state = Game_start;

void wakeUpScheduler(void) {
    CARD* card_t; // 临时存储发牌器发来的牌结构体数组指针，注意：后面需要free
    int new_cards_num;
    switch (current_game_state) {
        /*
            开始
        */
        case Game_start:
            sendCommand2Table_sendcontent("Game Start(Y/N?)");
            current_game_state = get_Game_start_confirm;
            break;

        case get_Game_start_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                
                // 将所有牌初始化
                changePlayerHandcards_simple(1, 0, 0, 0, 0);
                changePlayerHandcards_simple(2, 0, 0, 0, 0);
                changePlayerHandcards_simple(3, 0, 0, 0, 0);
                changePlayerHandcards_simple(4, 0, 0, 0, 0);

                // 向npc_controller发送消息，让其准备好每个人的筹码
                //（如果上一局npc的筹码仍然足够最低入局数，则继续使用，如果在牌局中归零，或者再开局时低于最低入局数，则生成一个新的npc）
                changePlayerChips(1,-9999);
                changePlayerChips(2,-9999);
                changePlayerChips(3,-9999);
                changePlayerChips(4,-9999);

                memset(&public_cards, 0, sizeof(public_cards)); // 将公共牌清零
                sendCommand2Table_sendpubliccard();

                id_chosen = 0; // 初始化大盲注id

                current_game_state = NPC1_in;
            } 
            else {
                current_game_state = get_Game_start_confirm;
            }
            break;

        


        /*
            NPC入场
        */
        case NPC1_in:
            initNPC_chip (1);
            current_game_state = NPC2_in;
            break;
        case NPC2_in:
            initNPC_chip (2);
            current_game_state = NPC3_in;
            break;
        case NPC3_in:
            initNPC_chip (3);
            current_game_state = Player_in;
            break;
        case Player_in:
            initNPC_chip (4);
            current_game_state = Deal_the_hole_cards;
            break;





        /*
            发手牌
        */
        case Deal_the_hole_cards:
            /*
                给多个外部模块依次发消息，并等待回信，
                一得到一个回信就立刻调用sendCommand2Table将命令装到card_table的队列中，
                然后接着发送下一个消息并等待回复然后把得到的消息发到card_table模块去更新UI
            */
            sendCommand2Table_sendcontent("Deal hands?");
            current_game_state = get_Deal_the_hole_cards_confirm;
            break;

        case get_Deal_the_hole_cards_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                current_game_state = send_Hand_card_NPC1;
            }
            else {
                current_game_state = get_Deal_the_hole_cards_confirm;
            }
            break;

        case send_Hand_card_NPC1:
            new_cards_num = 2;
            card_t = cardsenderProcesser(new_cards_num);
            if (card_t){
                changePlayerHandcards_simple(1, card_t[0].rank, card_t[0].suit, card_t[1].rank, card_t[1].suit);
                free(card_t);
            }
            current_game_state = send_Hand_card_NPC2;
            break;
        case send_Hand_card_NPC2:
            new_cards_num = 2;
            card_t = cardsenderProcesser(new_cards_num);
            if (card_t){
                changePlayerHandcards_simple(2, card_t[0].rank, card_t[0].suit, card_t[1].rank, card_t[1].suit);
                free(card_t);
            }
            current_game_state = send_Hand_card_NPC3;
            break;
        case send_Hand_card_NPC3:
            new_cards_num = 2;
            card_t = cardsenderProcesser(new_cards_num);
            if (card_t){
                changePlayerHandcards_simple(3, card_t[0].rank, card_t[0].suit, card_t[1].rank, card_t[1].suit);
                free(card_t);
            }
            current_game_state = send_Hand_card_player;
            break;
        case send_Hand_card_player:
            new_cards_num = 2;
            card_t = cardsenderProcesser(new_cards_num);
            if (card_t){
                changePlayerHandcards_simple(4, card_t[0].rank, card_t[0].suit, card_t[1].rank, card_t[1].suit);
                free(card_t);
            }
            current_game_state = Draw_for_the_big_blind;
            break;






        /*
            抽大盲注id
        */
        case Draw_for_the_big_blind:
            id_chosen = generateRandomNumber(1,4);
            char str[100];
            if (id_chosen != 4) {
                sprintf(str, "big blind is NPC%d", id_chosen);
            }
            else {
                sprintf(str, "big blind is you");
            }
            sendCommand2Table_sendcontent(str);
            current_game_state = collect_bigblind_chip;
            break;
        // 扣大盲注筹码
        case collect_bigblind_chip:
            changePlayerChips(id_chosen, -10);
            public_chip += 10;
            current_game_state = First_betting_round;
            updatePublicChips();
            break;

        
        // 第一轮下注
        case First_betting_round:
            sendCommand2Table_sendcontent("Start first betting round");
            current_game_state = get_First_betting_round_confirm;
            break;
        case get_First_betting_round_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                /* 依次询问玩家决定：弃牌（放弃本局）、跟注（跟上大盲注金额）、加注（增加下注额） */
                int i;
                int current_index;
                for (i = 0; i < 4; ++i) {
                    current_index = (id_chosen + i - 1)%4 + 1;
                    Betting_Decision begging_decision;
                    begging_decision = ask_decision (current_index, public_cards);
                    if (begging_decision.decison_type == Fold) {

                    } else if (begging_decision.decison_type == Call) {

                    } else {
                        // Raise 加注

                    }
                }
                current_game_state = Deal_the_first_publiccard;
            }
            else {
                current_game_state = get_First_betting_round_confirm;
            }
            break;





        /*
            翻公共牌
        */
        // 翻开第一张张公共牌
        case Deal_the_first_publiccard:
            sendCommand2Table_sendcontent("Deal 1st flops?");
            current_game_state = get_Deal_first_publiccard_confirm;
            break;

        case get_Deal_first_publiccard_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                new_cards_num = 1;
                card_t = cardsenderProcesser(new_cards_num);
                
                public_cards.cards[0] = card_t[0];

                sendCommand2Table_sendpubliccard();
                
                current_game_state = Deal_the_second_publiccard;
            }
            else {
                current_game_state = get_Deal_first_publiccard_confirm;
            }
            break;
        
        case Deal_the_second_publiccard:
            // 翻开第二张张公共牌
            sendCommand2Table_sendcontent("Deal 2nd flops?");
            current_game_state = get_Deal_second_publiccard_confirm;
            break;

        case get_Deal_second_publiccard_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                new_cards_num = 1;
                card_t = cardsenderProcesser(new_cards_num);
                
                public_cards.cards[1] = card_t[0];

                sendCommand2Table_sendpubliccard();
                

                current_game_state = Deal_the_third_publiccard;
            }
            else {
                current_game_state = get_Deal_second_publiccard_confirm;
            }
            break;

        case Deal_the_third_publiccard:
            // 翻开第一张张公共牌
            sendCommand2Table_sendcontent("Deal 3rd flops?");
            current_game_state = get_Deal_third_publiccard_confirm;
            break;

        case get_Deal_third_publiccard_confirm:
            getUserInput(ui_input);
            if (strcmp(ui_input, "Y") || strcmp(ui_input, "y")) {
                new_cards_num = 1;
                card_t = cardsenderProcesser(new_cards_num);
                
                public_cards.cards[2] = card_t[0];

                sendCommand2Table_sendpubliccard();
                

                current_game_state = Game_start;
            }
            else {
                current_game_state = get_Deal_third_publiccard_confirm;
            }
            break;





        
    }
    // free(card_t);
}