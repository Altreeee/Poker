/**
 * 测试各模块功能，编译时代替main.c使用
 */


// 测试card_table模块
#include "card_table.h"
#include "config/interface.h"
#include "card_sender.h"
#include "scheduler.h"
#include <time.h>
#include <stdlib.h>


int main(){
    /*
    //创建实例
    CardSender cardsender = new_cardsender();
    Scheduler scheduler = new_scheduler();

    // 将scheduler的第0个send_target设置为指向cardsender的消息接收接口
    scheduler.send_target[0] = &cardsender.msg_recive_interface;
    */
    
    // 使用当前时间作为随机数种子
    srand((unsigned int)time(NULL)); // 注意不要多次重复初始化随机数种子，只在main函数中做一次

    initUI();
    startContinuousRunUI();
    return 0;
}