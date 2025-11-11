/**
 * 显示ui，在cmd中给每个元素固定位置
 */
#include "card_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config/interface.h"



// 指令消息队列（循环缓冲区）
#define CMD_QUEUE_SIZE 16
static COMMAND_MSG_TO_TABLE cmdQueue[CMD_QUEUE_SIZE];
static int head = 0, tail = 0;

void initUI(void);
void startContinuousRunUI(void);

static void processCommands(void);

static void clearScreen();
static void moveCursor(int x, int y);
static void drawBox(int x, int y, int width, int height, const char *title);
static void updateBox(int x, int y, int width, int height, const char *lines[], int lineCount);

static int boxWidth = 28;
static int boxHeight = 9;
static char buf1[100], buf2[100], buf3[100]; // 3个用于存储更新内容的缓冲区


// 通用接口函数(给其它模块调用)
/*
发送时：
NPC_Info npc = { "Dealer Bob" };
Message msg = { MSG_NPC_NAME, &npc };
sendCommand(&msg);
*/
void sendCommand2Table(COMMAND_TYPE_TO_TABLE cmd, int param) {
    // 入队
    cmdQueue[tail] = (COMMAND_MSG_TO_TABLE){cmd, param};
    tail = (tail + 1) % CMD_QUEUE_SIZE;
}

// 处理指令队列
static void processCommands(void) {
    while (head != tail) {
        COMMAND_MSG_TO_TABLE msg = cmdQueue[head];
        head = (head + 1) % CMD_QUEUE_SIZE;

        switch (msg.msgtype) {
            case NPC_NAME:   /* ...逻辑... */ break;
            case NPC_CHIPS:  /* ...逻辑... */ break;
            case NPC_CARDS:    /* ...逻辑... */ break;
            default: break;
        }
    }
}

void initUI(void){
    clearScreen();
    // 画出三个方框的初始位置
    drawBox(2,  2, boxWidth, boxHeight, "Box A");
    drawBox(32, 2, boxWidth, boxHeight, "Box B");
    drawBox(62, 2, boxWidth, boxHeight, "Box C");
}

void startContinuousRunUI(void){
    while (1) {
        // 调度器依次唤醒其它模块
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