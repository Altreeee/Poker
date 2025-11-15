/**
 * 显示ui，在cmd中给每个元素固定位置
 */
#include "card_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config/interface.h"
#include "scheduler.h"

// 平台检测宏，Linux 用 usleep，Windows 用 Sleep
#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

// 指令消息队列（循环缓冲区）
#define CMD_QUEUE_SIZE 16
static COMMAND_MSG_TO_TABLE cmdQueue[CMD_QUEUE_SIZE];
static int head = 0, tail = 0;


static void processCommands(void);

static void clearScreen();
static void moveCursor(int x, int y);
static void drawBox(int x, int y, int width, int height, const char *title);
static void updateBox(int x, int y, int width, int height, const char *lines[], int lineCount);

int boxWidth = 28;
int boxHeight = 9;
char buf1_1[100], buf2_1[100], buf3_1[100]; // 用于存储更新内容的缓冲区
char buf1_2[100], buf2_2[100], buf3_2[100];
HandCards handcard1, handcard2, handcard3 = {0}; // 3个NPC的初始手牌


// 通用接口函数(给其它模块调用)
/*
发送时：
NPC_Info npc = { "Dealer Bob" };
Message msg = { MSG_NPC_NAME, &npc };
sendCommand(&msg);
*/
void sendCommand2Table(COMMAND_TYPE_TO_TABLE msgtype, COMMAND_CONTENT_TO_TABLE msgcontent) {
    // 入队
    cmdQueue[tail] = (COMMAND_MSG_TO_TABLE){msgtype, msgcontent};
    tail = (tail + 1) % CMD_QUEUE_SIZE;
}

// 处理指令队列
static void processCommands(void) {
    while (head != tail) {
        COMMAND_MSG_TO_TABLE msg = cmdQueue[head];
        head = (head + 1) % CMD_QUEUE_SIZE;

        switch (msg.msgtype) {
            case NpcDataUpdate:   
                /* ...逻辑... */ 
                if(msg.msgcontent.npc_information.NpcIndex == 1){
                    handcard1 = msg.msgcontent.npc_information.HandCards;
                    sprintf(buf1_1, "HandCard 1: %d, %d", handcard1.cards[0].rank, handcard1.cards[0].suit);
                    sprintf(buf1_2, "HandCard 2: %d, %d", handcard1.cards[1].rank, handcard1.cards[1].suit);
                    char *linesA[] = { buf1_1, buf1_2 };
                    updateBox(2, 2, boxWidth, boxHeight, linesA, 2);
                }
                else if (msg.msgcontent.npc_information.NpcIndex == 2){
                    handcard2 = msg.msgcontent.npc_information.HandCards;
                    sprintf(buf2_1, "HandCard 1: %d, %d", handcard2.cards[0].rank, handcard2.cards[0].suit);
                    sprintf(buf2_2, "HandCard 2: %d, %d", handcard2.cards[1].rank, handcard2.cards[1].suit);
                    char *linesB[] = { buf2_1, buf2_2 };
                    updateBox(32, 2, boxWidth, boxHeight, linesB, 2);
                }
                else if (msg.msgcontent.npc_information.NpcIndex == 3){
                    handcard3 = msg.msgcontent.npc_information.HandCards;
                    sprintf(buf3_1, "HandCard 1: %d, %d", handcard3.cards[0].rank, handcard3.cards[0].suit);
                    sprintf(buf3_2, "HandCard 2: %d, %d", handcard3.cards[1].rank, handcard3.cards[1].suit);
                    char *linesC[] = { buf3_1, buf3_2 };
                    updateBox(62, 2, boxWidth, boxHeight, linesC, 2);
                }

                break;
            
            default: break;
        }
    }
}

void initUI(void){
    clearScreen();
    // 画出三个方框的初始位置
    drawBox(2,  2, boxWidth, boxHeight, "NPC 1");
    drawBox(32, 2, boxWidth, boxHeight, "NPC 2");
    drawBox(62, 2, boxWidth, boxHeight, "NPC 3");

    sprintf(buf1_1, "HandCard 1: %d, %d", handcard1.cards[0].rank, handcard1.cards[0].suit); // NPC 1的手牌1（现在先把花色用数字代替（还没知道怎么直接把枚举类型的名字写出来））
    sprintf(buf1_2, "HandCard 2: %d, %d", handcard1.cards[1].rank, handcard1.cards[1].suit);
    char *linesA[] = { buf1_1, buf1_2 };
    sprintf(buf2_1, "HandCard 1: %d, %d", handcard2.cards[0].rank, handcard2.cards[0].suit);
    sprintf(buf2_2, "HandCard 2: %d, %d", handcard2.cards[1].rank, handcard2.cards[1].suit);
    char *linesB[] = { buf2_1, buf2_2 };
    sprintf(buf3_1, "HandCard 1: %d, %d", handcard3.cards[0].rank, handcard3.cards[0].suit);
    sprintf(buf3_2, "HandCard 2: %d, %d", handcard3.cards[1].rank, handcard3.cards[1].suit);
    char *linesC[] = { buf3_1, buf3_2 };

    updateBox(2, 2, boxWidth, boxHeight, linesA, 2);
    updateBox(32, 2, boxWidth, boxHeight, linesB, 2);
    updateBox(62, 2, boxWidth, boxHeight, linesC, 2);
}

void startContinuousRunUI(void){
    while (1) {
        // 调度器依次唤醒其它模块
        wakeUpScheduler();
        processCommands();
        SLEEP_MS(300);
    }
}

static void clearScreen() {
    printf("\033[2J");
    fflush(stdout);
}

static void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

static void drawBox(int x, int y, int width, int height, const char *title) {
    int i, j;
    moveCursor(x, y);
    printf("+");
    for (i = 0; i < width - 2; i++) printf("-");
    printf("+");

    for (i = 1; i < height - 1; i++) {
        moveCursor(x, y + i);
        printf("|");
        for (j = 0; j < width - 2; j++) printf(" ");
        printf("|");
    }

    moveCursor(x, y + height - 1);
    printf("+");
    for (i = 0; i < width - 2; i++) printf("-");
    printf("+");

    if (title) {
        moveCursor(x + 2, y);
        printf("[%s]", title);
    }
    fflush(stdout);
}

static void updateBox(int x, int y, int width, int height, const char *lines[], int lineCount) {
    int i, j;
    for (i = 0; i < height - 2 && i < lineCount; i++) {
        moveCursor(x + 1, y + 1 + i);
        printf("%-*s", width - 2, lines[i]);
    }
    fflush(stdout);
}