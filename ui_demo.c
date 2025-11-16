/**
 * linux: gcc ui_demo.c -o ui_demo
 * windows: gcc ui_demo.c -o ui_demo.exe
 * 
 * ChatGPT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 平台检测宏，Linux 用 usleep，Windows 用 Sleep
#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

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

// -------------------------
// 绘制方框与更新内容
// -------------------------
void drawBox(int x, int y, int width, int height, const char *title) {
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

void updateBox(int x, int y, int width, int height, const char *lines[], int lineCount) {
    int i, j;
    for (i = 0; i < height - 2 && i < lineCount; i++) {
        moveCursor(x + 1, y + 1 + i);
        printf("%-*s", width - 2, lines[i]);
    }
    fflush(stdout);
}

// -------------------------
// 主程序逻辑
// -------------------------
int main() {
    clearScreen();

    int boxWidth = 28;
    int boxHeight = 7;

    // 三个方框的初始位置
    drawBox(2,  2, boxWidth, boxHeight, "Box A");
    drawBox(32, 2, boxWidth, boxHeight, "Box B");
    drawBox(62, 2, boxWidth, boxHeight, "Box C");

    // 玩家位置
    drawBox(62, 2 + boxHeight + 4, boxWidth, boxHeight, "ME");

    // 公共牌位置
    drawBox(2, 2 + boxHeight + 4, boxWidth, boxHeight, "Public Card");

    // 沟通+输入栏
    drawBox(32, 2 + boxHeight + 4, boxWidth, boxHeight, "Public Card");

    int counter = 0;
    char buf1[100], buf2[100], buf3[100];

    while (1) {
        sprintf(buf1, "Temp: %d C", counter); // 把“Temp: counter C”写入buffer1当中
        const char *linesA[] = { buf1, "Running..." };

        sprintf(buf2, "Speed: %d km/h", counter % 120);
        const char *linesB[] = { buf2, "OK" };

        sprintf(buf3, "Data: %d", counter * counter);
        const char *linesC[] = { buf3, "Active" };

        updateBox(2, 2, boxWidth, boxHeight, linesA, 2);
        updateBox(32, 2, boxWidth, boxHeight, linesB, 2);
        updateBox(62, 2, boxWidth, boxHeight, linesC, 2);

        SLEEP_MS(300);
        counter++;
    }

    return 0;
}
