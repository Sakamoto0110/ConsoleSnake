#include <iostream>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <time.h>
#include <conio.h>

using namespace std;
void gotoxy(int x, int y);
void ShowConsoleCursor(bool showFlag);
void bounds();

int frameWidth = 70;
int frameHeight = 30;
class Food{

private:

public:
    int x;
    int y;

    Food(){
        x = 2+rand()%frameWidth-2;
        y = 2+rand()%frameHeight-2;
    }
    void render(){
        gotoxy(x,y);
        cout << "@";

    }
};


class Player{

private:


public:
    int x = 14;
    int y = 7;
    int tailSize = 4;
    int* tailX;
    int* tailY;
    int oldX = 0;
    int oldY = 0;

    int score = 0;

    bool right = false;
    bool left = false;
    bool top = false;
    bool down = true;
    bool isDead = false;
    int imortal = 5;
    char code = 'o';

    Player(){
        tailX = new int[tailSize];
        tailY = new int[tailSize];
        tailX[0] = x;
        tailX[1] = x;
        tailX[2] = x;
        tailX[3] = x;
        tailY[0] = y;
        tailY[1] = y;
        tailY[2] = y;
        tailY[3] = y;
        moveTo(1+rand()%3);

    }

    void eat(){
        tailSize++;
        int* tempx = new int[tailSize];
        int* tempy = new int[tailSize];
        for(int c = 0; c < tailSize; c++){
            tempx[c] = tailX[c];
            tempy[c] = tailY[c];
        }
        tailX = tempx;
        tailY = tempy;
    }


    void moveTo(int dir){
        top = false;
        down = false;
        right = false;
        left = false;
        switch(dir)
        {
        case 1:
            //top
            top = true;
            break;
        case 2:
            //down
            down = true;
            break;
        case 3:
            //left
            left = true;
            break;
        case 4:
            //right
            right = true;
            break;
        }
    }

    void update(){
        if(imortal > 0){
            imortal--;
        }else{
            for(int c = 0; c < tailSize; c++){
                if(tailX[c] == x && tailY[c] == y){
                    isDead = true;
                }
            }
        }
        for(int c = tailSize-1; c > 0; c--){
            tailX[c] = tailX[c-1];
            tailY[c] = tailY[c-1];
        }
        tailX[0] = x;
        tailY[0] = y;
        oldX = tailX[tailSize-1];
        oldY = tailY[tailSize-1];
        if(right || left){
            if(left && x-1 <= 0){
                isDead = true;
            }
            if(right && x+1 >= frameWidth-1){
                isDead = true;
            }
            x+=right?1:-1;
        }
        if(top || down){
            if(top && y-1 <= 0){
                isDead = true;
            }
            if(down && y+1 >= frameHeight){
                isDead = true;
            }
            y+=down?1:-1;
        }

    }

    void render(){
        //system("COLOR 07");
        for(int c = 0; c < tailSize; c++){
            gotoxy(tailX[c],tailY[c]);
            cout << code;
        }
        gotoxy(oldX,oldY);
        cout << " ";
        gotoxy(x,y);
        cout << code;
    }



};


class Game{
protected:

public:
    Food food = Food();
    Player player = Player();
    void update(){
        if(player.isDead){
            system("cls");
            bounds();
            player = Player();
            food = Food();
        }
        if(player.x == food.x && player.y == food.y){
            player.eat();
            player.score+=10;
            food = Food();
        }
        player.update();

    }

    void render(){
        player.render();
        food.render();
        gotoxy(0,frameHeight+1);
        cout << "Score: " << player.score;
    }

};


bool isRunning = true;
int main()
{
    HWND console = GetConsoleWindow();
    SetConsoleTitle("Snake! By Sakamoto");
    SetWindowPos(
                 console,
                 HWND_TOP,
                 0,
                 0,
                 600,
                 600,
                 SWP_NOMOVE
                 );
    Game game = Game();
    ShowConsoleCursor(false);
    clock_t stime = clock();
    int time = 0;
    int old_time = 0;
    int FPS = 10;
    bounds();
    while(isRunning)
    {
        time = (clock()-stime)/(CLOCKS_PER_SEC/FPS);
        if(time != old_time)
        {
            old_time = time;
            game.update();
            game.render();
            if(kbhit())
            {
                int key = getch();
                switch(key)
                {
                case 'w':
                    game.player.moveTo(1);
                    break;
                case 'a':
                    game.player.moveTo(3);
                    break;
                case 's':
                    game.player.moveTo(2);
                    break;
                case 'd':
                    game.player.moveTo(4);
                    break;

                }
            }
        }
    }
    return 0;
}

void bounds(){
    for(int c = 0; c < frameWidth ; c++){
        gotoxy(c,0);
        cout << '#';
    }
    for(int c = 0; c < frameHeight; c++){
        gotoxy(0,c);
        cout << '#';
    }
    for(int c = 0; c < frameWidth ; c++){
        gotoxy(c,frameHeight);
        cout << '#';
    }
    for(int c = 0; c < frameHeight; c++){
        gotoxy(frameWidth-1,c);
        cout << '#';
    }
}


void gotoxy(int x, int y){
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
