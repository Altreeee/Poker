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
#include "config/card.h"

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
int boxHeight = 7;

char buf1_1[100], buf2_1[100], buf3_1[100]; // 用于存储更新内容的缓冲区
char buf1_2[100], buf2_2[100], buf3_2[100];
char bufPlayer_1[100], bufPlayer_2[100];
char bufPublic_1[100], bufPublic_2[100], bufPublic_3[100]; // 先用3张公共牌，省去后续找最好组合的流程，直接判断5张牌的大小
char bufCommunicateContent[100]; // 通知显示内容
char* content_t = "..."; // 临时显示内容

HAND_CARDS handcard1 = {0}, handcard2 = {0}, handcard3 = {0}; // 3个NPC的初始手牌
HAND_CARDS handcardPlayer = {0}; // 玩家初始手牌
PUBLIC_CARDS publiccards = {0}; // 公共牌


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

// 清理用户输入内容
static void clearInputLine(int x, int y, int width) {
    moveCursor(x, y);
    for (int i = 0; i < width; i++) putchar(' ');
    fflush(stdout);
    moveCursor(x, y); // 光标回到原点
}

// 由schedule.c直接调用，阻塞，只有在得到了用户输入后才会继续运行schedule
// ui_input需要由调用这个函数并使用input内容的模块初始化，如char ui_input[100];
// updateBox(32, 2 + boxHeight + 4, boxWidth, boxHeight, linesCommunicateContent, 1);
void getUserInput(char* ui_input) {
    int x_input = 34;
    int y_input = 2 + boxHeight + 4 + boxHeight - 2;
    moveCursor(x_input, y_input); // 光标放在输入框内部，例如 Box A 第二行左边
    printf(">> ");
    fflush(stdout);

    // ========= 阻塞输入 =========
    fgets(ui_input, sizeof(100 * sizeof(char)), stdin);
    ui_input[strcspn(ui_input, "\n")] = '\0'; // 去掉换行
    clearInputLine(34, 2 + boxHeight + 4 + boxHeight - 2, 5); // 假设输入框宽度5
}

// 处理指令队列
static void processCommands(void) {
    while (head != tail) {
        COMMAND_MSG_TO_TABLE msg = cmdQueue[head];
        head = (head + 1) % CMD_QUEUE_SIZE;

        switch (msg.msgtype) {
            case Npc_data_update:   
                /* npc数据更新逻辑 */ 
                if (msg.msgcontent.npc_information.npc_index == 1){
                    handcard1 = msg.msgcontent.npc_information.hand_cards;
                    sprintf(buf1_1, "HandCard 1: %d, %d", handcard1.cards[0].rank, handcard1.cards[0].suit);
                    sprintf(buf1_2, "HandCard 2: %d, %d", handcard1.cards[1].rank, handcard1.cards[1].suit);
                    const char *linesA[] = { buf1_1, buf1_2 };
                    updateBox(2, 2, boxWidth, boxHeight, linesA, 2);
                }
                else if (msg.msgcontent.npc_information.npc_index == 2){
                    handcard2 = msg.msgcontent.npc_information.hand_cards;
                    sprintf(buf2_1, "HandCard 1: %d, %d", handcard2.cards[0].rank, handcard2.cards[0].suit);
                    sprintf(buf2_2, "HandCard 2: %d, %d", handcard2.cards[1].rank, handcard2.cards[1].suit);
                    const char *linesB[] = { buf2_1, buf2_2 };
                    updateBox(32, 2, boxWidth, boxHeight, linesB, 2);
                }
                else if (msg.msgcontent.npc_information.npc_index == 3){
                    handcard3 = msg.msgcontent.npc_information.hand_cards;
                    sprintf(buf3_1, "HandCard 1: %d, %d", handcard3.cards[0].rank, handcard3.cards[0].suit);
                    sprintf(buf3_2, "HandCard 2: %d, %d", handcard3.cards[1].rank, handcard3.cards[1].suit);
                    const char *linesC[] = { buf3_1, buf3_2 };
                    updateBox(62, 2, boxWidth, boxHeight, linesC, 2);
                }
                else if (msg.msgcontent.npc_information.npc_index == 4){
                    handcardPlayer = msg.msgcontent.npc_information.hand_cards;
                    sprintf(bufPlayer_1, "HandCard 1: %d, %d", handcardPlayer.cards[0].rank, handcardPlayer.cards[0].suit);
                    sprintf(bufPlayer_2, "HandCard 2: %d, %d", handcardPlayer.cards[1].rank, handcardPlayer.cards[1].suit);
                    const char *linesPlayer[] = { bufPlayer_1, bufPlayer_2 };
                    updateBox(62, 2 + boxHeight + 4, boxWidth, boxHeight, linesPlayer, 2);
                }

                break;

            case Content_update:
                /* 显示内容更新逻辑 */
                if (msg.msgcontent.content_information.specific_content){
                    content_t = msg.msgcontent.content_information.specific_content;
                    sprintf(bufCommunicateContent, " - : %s", content_t);
                    const char *linesCommunicateContent[] = { bufCommunicateContent };
                    updateBox(32, 2 + boxHeight + 4, boxWidth, boxHeight, linesCommunicateContent, 1);
                }
                break;

            case Public_cards_update:
                /* 更新公共牌数据 */
                publiccards = msg.msgcontent.public_cards_information.public_cards;
                sprintf(bufPublic_1, "PublicCard 1: %d, %d", publiccards.cards[0].rank, publiccards.cards[0].suit);
                sprintf(bufPublic_2, "PublicCard 2: %d, %d", publiccards.cards[1].rank, publiccards.cards[1].suit);
                sprintf(bufPublic_3, "PublicCard 3: %d, %d", publiccards.cards[2].rank, publiccards.cards[2].suit);
                const char *linesPublic[] = { bufPublic_1, bufPublic_2, bufPublic_3 };
                updateBox(2, 2 + boxHeight + 4, boxWidth, boxHeight, linesPublic, 3);
                break;

            default: break;
        }
    }
}

void initUI(void){
    clearScreen();
    // 画出三个NPC方框的初始位置
    drawBox(2,  2, boxWidth, boxHeight, "NPC 1");
    drawBox(32, 2, boxWidth, boxHeight, "NPC 2");
    drawBox(62, 2, boxWidth, boxHeight, "NPC 3");
    // 玩家位置
    drawBox(62, 2 + boxHeight + 4, boxWidth, boxHeight, "U");
    // 公共牌位置
    drawBox(2, 2 + boxHeight + 4, boxWidth, boxHeight, "Public Card");
    // 沟通+输入栏
    drawBox(32, 2 + boxHeight + 4, boxWidth, boxHeight, "Words2U");

    /*
        初始化npc部分ui显示
    */
    sprintf(buf1_1, "HandCard 1: %d, %d", handcard1.cards[0].rank, handcard1.cards[0].suit); // NPC 1的手牌1（现在先把花色用数字代替（还没知道怎么直接把枚举类型的名字写出来））
    sprintf(buf1_2, "HandCard 2: %d, %d", handcard1.cards[1].rank, handcard1.cards[1].suit);
    const char *linesA[] = { buf1_1, buf1_2 };
    sprintf(buf2_1, "HandCard 1: %d, %d", handcard2.cards[0].rank, handcard2.cards[0].suit);
    sprintf(buf2_2, "HandCard 2: %d, %d", handcard2.cards[1].rank, handcard2.cards[1].suit);
    const char *linesB[] = { buf2_1, buf2_2 };
    sprintf(buf3_1, "HandCard 1: %d, %d", handcard3.cards[0].rank, handcard3.cards[0].suit);
    sprintf(buf3_2, "HandCard 2: %d, %d", handcard3.cards[1].rank, handcard3.cards[1].suit);
    const char *linesC[] = { buf3_1, buf3_2 };

    updateBox(2, 2, boxWidth, boxHeight, linesA, 2);
    updateBox(32, 2, boxWidth, boxHeight, linesB, 2);
    updateBox(62, 2, boxWidth, boxHeight, linesC, 2);

    /*
        初始化玩家部分ui显示
    */
    sprintf(bufPlayer_1, "HandCard 1: %d, %d", handcardPlayer.cards[0].rank, handcardPlayer.cards[0].suit);
    sprintf(bufPlayer_2, "HandCard 2: %d, %d", handcardPlayer.cards[1].rank, handcardPlayer.cards[1].suit);
    const char *linesPlayer[] = { bufPlayer_1, bufPlayer_2 };
    updateBox(62, 2 + boxHeight + 4, boxWidth, boxHeight, linesPlayer, 2);

    /*
        初始化公共牌部分ui显示
    */
    sprintf(bufPublic_1, "PublicCard 1: %d, %d", publiccards.cards[0].rank, publiccards.cards[0].suit);
    sprintf(bufPublic_2, "PublicCard 2: %d, %d", publiccards.cards[1].rank, publiccards.cards[1].suit);
    sprintf(bufPublic_3, "PublicCard 3: %d, %d", publiccards.cards[2].rank, publiccards.cards[2].suit);
    const char *linesPublic[] = { bufPublic_1, bufPublic_2, bufPublic_3 };
    updateBox(2, 2 + boxHeight + 4, boxWidth, boxHeight, linesPublic, 3);

    /*
        初始化沟通栏部分ui显示
    */
    sprintf(bufCommunicateContent, " - : %s", content_t);
    const char *linesCommunicateContent[] = { bufCommunicateContent };
    updateBox(32, 2 + boxHeight + 4, boxWidth, boxHeight, linesCommunicateContent, 1);
}

void startContinuousRunUI(void){
    while (1) {
        // 调度器依次唤醒其它模块
        wakeUpScheduler();
        processCommands();
        SLEEP_MS(300);
    }
}

// -------------------------
// 光标移动与清屏
// -------------------------
#ifdef _WIN32
void clearScreen() {
    system("cls");
}

void moveCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
#else
void clearScreen() {
    printf("\033[2J");
    fflush(stdout);
}

void moveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}
#endif



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
    int i;
    for (i = 0; i < height - 2 && i < lineCount; i++) {
        moveCursor(x + 1, y + 1 + i);
        printf("%-*s", width - 2, lines[i]);
    }
    fflush(stdout);
}