//
//  main.cpp
//  贪吃蛇
//
//  Created by mac on 2024/10/12.
//

#include <stdio.h>
#include <iostream>
#include <random>
#include <ncurses.h>
#include <ctime>
#include <chrono>
#include <iomanip>


using namespace std;
// 原始界面

const int HEIGHT=10,WIDTH=10;
int state[HEIGHT][WIDTH]={
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};
// 展示界面
void show(int a[HEIGHT][WIDTH]) {
    clear();
    for (int x = 0; x < HEIGHT; ++x) {
        for (int y = 0; y < WIDTH; ++y) {
            switch (a[x][y]) {
                case 1:
                    mvprintw(x, y * 2, "#");
                    break;
                case 3:
                    mvprintw(x, y * 2, "A");
                    break;
                case 2:
                    mvprintw(x, y * 2, "E");
                    break;
                case 0:
                    mvprintw(x, y * 2, " ");
                    break;
                case 4:
                    mvprintw(x, y * 2, "*");
                    break;
            }
        }
    }
    refresh();
}
// 随机数
int suijishu(int a, int b){
    //random_device 是一个类，用于生成高质量的非确定性随机数种子，创造一个类的对象rd
    std::random_device rd;
    // 使用该随机设备对象生成种子，初始化Mersenne Twister引擎
    std::mt19937 gen(rd());
    // 设置一个从1到100范围内均匀分布的整数分布器
    std::uniform_int_distribution<> dis(a,b);
    // 输出随机数
    return dis(gen);
}
// 随机生成蛋的位置
void dan(){
    int a,b;
    bool begin=true;
    while (begin) {
        a=suijishu(1, 8);
        b=suijishu(1, 8);
        if (state[a][b]==0) {
            state[a][b]=4;
            begin=false;
        }

    }
}
// 随机生成头的位置
struct tou1{
    int  x;
    int  y;
    tou1(): x(0),y(0){};
};

tou1 t(){
    int a,b;
    tou1 t1;
    bool begin=true;
    while (begin) {
        a=suijishu(1, 8);
        b=suijishu(1, 8);
        if (state[a][b]==0) {
            state[a][b]=3;
            t1.x=a;
            t1.y=b;
            begin=false;
        }
    }
 
    
    return t1;
}

// 将se容器的内容更新到state上去
void show_se(vector<tou1> se0){
    
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            if (state[x][y] == 2 || state[x][y] == 3) {
                state[x][y] = 0;
            }
        }
    }
    
    
    long int t=0;
    t=se0.size();
    if (t) {
        for (int i=0; i<t; ++i) {
            if(i==0){
                state[se0[i].x][se0[i].y]=3;// 头
            }else state[se0[i].x][se0[i].y]=2; //身体
        }
    }
}


void jishi_time(int n){
    // 获取当前时间点
    auto start_time = std::chrono::system_clock::now();
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start_time.time_since_epoch()).count();

    while (true) {
        auto current_time = std::chrono::system_clock::now();
        auto current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
        if ((current_ms - start_ms) >= n) {
            break;
        }
    }
}



int moveSnake(vector<tou1>& snake, int directionX, int directionY, int key) {
    long int t = snake.size();
    int new_x = snake[0].x + directionX;
    int new_y = snake[0].y + directionY;

    if (new_x >= 0 && new_x < HEIGHT && new_y >= 0 && new_y < WIDTH) {
        if (state[new_x][new_y] == 0) {
            for (long int i = t - 1; i > 0; --i) {
                snake[i] = snake[i - 1];
            }
            snake[0].x = new_x;
            snake[0].y = new_y;
            show_se(snake);
            show(state);
            printw("\n 1 GO next step \n");
        } else if (state[new_x][new_y] == 4) {
            tou1 new_tou;
            new_tou.x=new_x;
            new_tou.y=new_y;
            snake.insert(snake.begin(), new_tou);
            show_se(snake);
            dan();
            show(state);
            printw("\n 2 next step \n");
        } else {
            clear();
            printw("\n Hit the wall!!! Game ending soon.\n");
            jishi_time(3000);
            return 'q';
        }
    } else {
        clear();
        printw("\n Hit the wall!!! Game ending soon.\n");
        jishi_time(3000);
        return 'q';
    }
    printw("Snake length: %ld\n", snake.size());
    return getch();
}



int main() {
    // 初始化屏幕
    initscr();           // 初始化 ncurses
    cbreak();            // 启用逐字符输入模式
    noecho();            // 禁用字符回显
    keypad(stdscr, TRUE); // 启用特殊键的处理
    refresh();
    //g++ -o main main.cpp -I/opt/homebrew/Cellar/ncurses/6.5/include -L/opt/homebrew/Cellar/ncurses/6.5/lib -lncurses -std=c++11
    //./main
    dan();
    tou1 t0=t();
    int ch=97;
    // 生成蛇神的容器，记录每个位置的坐标
    vector<tou1> snake;
    //生成蛇头
    snake.insert(snake.begin(), t0);
    
    // 展示界面
    show(state);
    printw("\n Begin: 1. If you hit the wall or your body, the game will end.\n");
    printw("\n This Weekend:");
    printw("\n - Reading a book to enhance your knowledge and improve your life.");
    printw("\n - Learning a musical instrument to cultivate your interests and develop a new skill.");
    printw("\n - Going on a hike to enjoy nature and get some exercise.");
    printw("\n - Relaxing at a cafe with a cup of coffee and some good music.");
    printw("\n Enjoy your weekend!");
    ch=getch();
    
    while (ch != 'q') {
        switch (ch) {
            case KEY_UP:
                ch = moveSnake(snake, -1, 0, ch);
                break;
            case KEY_DOWN:
                ch = moveSnake(snake, 1, 0, ch);
                break;
            case KEY_LEFT:
                ch = moveSnake(snake, 0, -1, ch);
                break;
            case KEY_RIGHT:
                ch = moveSnake(snake, 0, 1, ch);
                break;
            default:
                break;
        }
        refresh();
    }

   endwin();
    return 0;
}
