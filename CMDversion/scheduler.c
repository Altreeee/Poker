/**
 * 调度器
 * 在ui的循环过程中依次唤醒各个模块的处理函数
 */
#include "config/interface.h"
#include "scheduler.h"
#include "card_table.h"

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






GAME_STATE current_game_state = Shuffle;

void wakeUpScheduler(void) {
    switch (current_game_state) {
        case Deal_the_hole_cards:
            /*
                给多个外部模块依次发消息，并等待回信，
                一得到一个回信就立刻调用sendCommand2Table将命令装到card_table的队列中，
                然后接着发送下一个消息并等待回复然后把得到的消息发到card_table模块去更新UI
            */
            // 调用发底牌模块处理函数
            if (cardsenderProcesser(2) == Success){
                sendCommand2Table(CHANGE_NPC_CARDS, 1, 2);
            }
            current_game_state = Deal_the_flop;
            break;
        case Deal_the_flop:
            // 调用发翻牌模块处理函数
            if (cardsenderProcesser(2) == Success){
                sendCommand2Table(CHANGE_NPC_CARDS, 2, 2);
            }
            current_game_state = Deal_the_turn;
            break;
        case Deal_the_turn:
            // 调用发转牌模块处理函数
            if (cardsenderProcesser(2) == Success){
                sendCommand2Table(CHANGE_NPC_CARDS, 3, 2);
            }
            current_game_state = Deal_the_hole_cards;
            break;
    }
}