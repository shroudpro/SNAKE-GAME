#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

typedef struct snake {
    int x, y;           // 坐标
    int direction;      // 移动方向  上1  左2  下3  右4
    struct snake* next;
} snake;

struct food {
    int x, y;
} F; // 只需要一个食物

int level;      // 难度等级
int length = 4; // 蛇长度
int score = 0;  // 分数
int speed;      // 不同等级的速度
int n = 0;      // 吃到食物个数

// 函数声明
snake* creat();
void showMain();
void gotoxy(int x, int y);
void c(int k);
void ycgb(int k);
void game();
void draw(snake* head);
void newfood(snake* head);
int direc(char ch);
void key(snake* head, snake* p);
void cls();

int main() {
    srand((unsigned)time(NULL)); // 随机数种子只初始化一次
    ycgb(0);
    showMain();
    game();
    while (1) {
        cls();
        gotoxy(32, 10);
        printf("按回车可重新开始游戏！");
        gotoxy(36, 11);
        printf("按Esc可离开游戏！");
        char ch = _getch();
        while (ch != 13 && ch != 27) {
            ch = _getch();
        }
        if (ch == 13) {
            showMain();
            game();
        }
        else break;
    }
    return 0;
}

int direc(char ch) {
    if (ch == -32) {
        ch = _getch();
        if (ch == 72) return 1;
        if (ch == 80) return 3;
        if (ch == 75) return 2;
        if (ch == 77) return 4;
    }
    return 0;
}

snake* creat() {
    snake* head, * p, * q;
    head = (snake*)malloc(sizeof(snake));
    if (!head) {
        return NULL; // 内存分配失败
    }
    head->x = 28;
    head->y = 10;
    head->direction = 4;
    head->next = NULL;
    p = head;
    for (int i = 0; i < 3; i++) {
        q = (snake*)malloc(sizeof(snake));
        if (!q) {
            // 内存分配失败，清理已分配的内存
            while (head) {
                p = head;
                head = head->next;
                free(p);
            }
            return NULL;
        }
        p->next = q;
        q->next = NULL;
        q->x = p->x + 2;
        q->y = p->y;
        q->direction = p->direction;
        p = q;
    }
    return head;
}

void showMain() {
    int i;
    system("title 贪吃蛇——by shroud");
    system("mode con cols=80 lines=30");
    gotoxy(28, 10);
    c(3);
    printf("欢迎进入贪吃蛇的世界！！");
    gotoxy(0, 0);
    c(253);
    for (i = 0; i < 40; i++) {
        printf("▇");
        Sleep(10);
    }
    for (i = 1; i < 25; i++) {
        gotoxy(78, i);
        printf("▇");
        Sleep(10);
    }
    for (i = 39; i >= 0; i--) {
        gotoxy(2 * i, 25);
        printf("▇");
        Sleep(10);
    }
    for (i = 24; i >= 1; i--) {
        gotoxy(0, i);
        printf("▇");
        Sleep(10);
    }
    c(3);
    gotoxy(30, 11);
    printf("请选择难度（1~6）：");
    scanf("%d", &level);
    switch (level) {
    case 1: speed = 600; break;
    case 2: speed = 400; break;
    case 3: speed = 200; break;
    case 4: speed = 150; break;
    case 5: speed = 100; break;
    case 6: speed = 60; break;
    default: speed = 2333; break;
    }
    // 重置分数、长度、食物数
    score = 0;
    length = 4;
    n = 0;
    cls();
    c(15);
    gotoxy(4, 26);
    printf("等级： %d", level);
    gotoxy(20, 26);
    printf("分数： %d", score);
    gotoxy(4, 27);
    printf("当前长度： %d", length);
    gotoxy(20, 27);
    printf("已吃食物个数： %d", n);
    gotoxy(45, 26);
    printf("移动：↑↓←→");
    gotoxy(45, 27);
    printf("暂停or继续：空格");
    gotoxy(67, 27);
    c(4);
    printf("制作：SHROUD");
}

void key(snake* head, snake* p) {
    char ch;
    int i, j;
    if (_kbhit()) {
        ch = _getch();
        if (ch == -32) {
            i = direc(ch);
            j = p->direction;
            // 不能直接反向
            if ((i == 1 && j == 3) || (i == 2 && j == 4) || (i == 3 && j == 1) || (i == 4 && j == 2)) {
                // 反向移动，不改变方向
            }
            else if (i != 0) {
                p->direction = i;
            }
        }
        else if (ch == 32) {
            cls();
            gotoxy(36, 10);
            printf("暂停中");
            while (1) {
                if (_kbhit() && _getch() == 32) {
                    gotoxy(36, 10);
                    printf("     ");
                    draw(head);
                    gotoxy(F.x, F.y);
                    c(14);
                    printf("●");
                    break;
                }
            }
        }
    }
}

void cls() {
    gotoxy(2, 1);
    for (int i = 1; i < 26; i++) {
        for (int j = 2; j < 78; j++)
            printf(" ");
        gotoxy(2, i);
    }
}

void newfood(snake* head) {
    snake* p;
    int valid;
    do {
        valid = 1;
        F.x = rand() % 76 + 1;
        if (F.x & 1) F.x++;
        F.y = rand() % 24 + 1;
        p = head;
        while (p) {
            if (p->x == F.x && p->y == F.y) {
                valid = 0;
                break;
            }
            p = p->next;
        }
    } while (!valid);
    gotoxy(F.x, F.y);
    c(14);
    printf("●");
}

void draw(snake* head) {
    snake* p = head;
    if (!p) return; // 防止head为NULL
    if (p->next == NULL) {
        // 只有一个节点，既是头又是尾
        c(12);
        gotoxy(p->x, p->y);
        printf("◆");
        return;
    }
    int isFirst = 1;
    while (p) {
        if (isFirst) {
            c(12);
            gotoxy(p->x, p->y);
            printf("●"); // 蛇头
            isFirst = 0;
        }
        else if (p->next == NULL) {
            c(12);
            gotoxy(p->x, p->y);
            printf("◆"); // 蛇尾
        }
        else {
            c(15);
            gotoxy(p->x, p->y);
            printf("●");
        }
        p = p->next;
    }
}

void c(int k) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ycgb(int k) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = k;
    SetConsoleCursorInfo(hOut, &cci);
}

void game() {
    snake* head, * p, * q, * s;
    head = creat();
    if (!head) {
        cls();
        gotoxy(30, 10);
        printf("初始化失败！");
        Sleep(2500);
        return;
    }
    p = head;
    while (p->next) p = p->next; // p指向初始蛇尾

    draw(head);
    newfood(head);

    while (1) {
        // 生成新节点q
        q = (snake*)malloc(sizeof(snake));
        if (!q) {
            // 内存分配失败处理
            cls();
            gotoxy(30, 10);
            printf("内存分配失败！");
            Sleep(2500);
            return;
        }
        q->direction = p->direction;
        q->next = NULL;
        switch (p->direction) {
        case 1: q->x = p->x;     q->y = p->y - 1; break; // 上
        case 2: q->x = p->x - 2; q->y = p->y;     break; // 左
        case 3: q->x = p->x;     q->y = p->y + 1; break; // 下
        case 4: q->x = p->x + 2; q->y = p->y;     break; // 右
        }

        // 判断是否吃到食物
        if (q->x == F.x && q->y == F.y) {
            score += 10;
            length++;
            n++;
            p->next = q;
            p = q;
            draw(head);
            newfood(head);
            // 更新分数等显示
            gotoxy(20, 26); printf("分数： %d", score);
            gotoxy(4, 27);  printf("当前长度： %d", length);
            gotoxy(20, 27); printf("已吃食物个数： %d", n);
        }
        else {
            // 没吃到食物，正常移动，删除头节点
            p->next = q;
            p = q;
            // 删除头节点
            s = head;
            head = head->next;
            gotoxy(s->x, s->y); printf(" ");
            free(s);
            draw(head);
        }

        Sleep(speed);
        key(head, p);

        // 撞墙
        if (q->x < 2 || q->x > 76 || q->y < 1 || q->y > 24) {
            cls();
            gotoxy(30, 10);
            printf("您的分数为%d", score);
            Sleep(2500);
            // 清理内存
            while (head) {
                s = head;
                head = head->next;
                free(s);
            }
            return;
        }

        // 撞自己
        s = head;
        while (s && s != p) {
            if (q->x == s->x && q->y == s->y) {
                cls();
                gotoxy(30, 10);
                printf("您的分数为%d", score);
                Sleep(2500);
                // 清理内存
                while (head) {
                    s = head;
                    head = head->next;
                    free(s);
                }
                return;
            }
            s = s->next;
        }
    }
}