/**
 * 显示ui，在cmd中给每个元素固定位置
 */

#ifndef CARD_TABLE_H
#define CARD_TABLE_H

typedef struct {
    void (*showUI)(void);
    
} CardTable;

void showUI(void);

#endif // CARD_TABLE_H