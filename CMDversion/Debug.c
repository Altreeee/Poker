/**
 * 测试各模块功能，编译时代替main.c使用
 */


// 测试card_table模块
#include "card_table.h"
#include "config/interface.h"
int main(){
    initUI();
    startContinuousRunUI();
    return 0;
}