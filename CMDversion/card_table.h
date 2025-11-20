/**
 * 显示ui，在cmd中给每个元素固定位置
 */

#ifndef CARD_TABLE_H
#define CARD_TABLE_H
#include "config/interface.h"

/**
 * global functions
 */
// 通用接口函数
void sendCommand2Table(COMMAND_TYPE_TO_TABLE msgtype, COMMAND_CONTENT_TO_TABLE msgcontent);
void getUserInput(char* ui_input);

void initUI(void);
void startContinuousRunUI(void);



#endif // CARD_TABLE_H