#include "qtx/libqtx.h"
#include "snake.h"

#define MAX_DIGIT 5

int godmode = 0, delay = 220, gameover;
TSnake *snake;
HDC digit[MAX_DIGIT];

void keyHandler(int key)
{
    switch (key)
    {
    case Qt::Key_Left:
        snake -> SetDirection(3);
        break;
    case Qt::Key_Right:
        snake -> SetDirection(1);
        break;
    case Qt::Key_Up:
        snake -> SetDirection(4);
        break;
    case Qt::Key_Down:
        snake -> SetDirection(2);
        break;
    case Qt::Key_Space:
	snake -> SetDirection(0);
//	txSetColor(TX_YELLOW);
//	txSelectFont("Times", 36, 36, 1);
//        txTextOut(190,250, "П А У З А");
        break;
    case Qt::Key_Escape:
	gameover = 1;
        break;
    }

}

void generateDigits()
{
    int i, n, x, y;
    HDC iRabbit = txLoadImage("images/krolik.png");
    for(i = 0; i < MAX_DIGIT; i++)
    {
    if(digit[i] != NULL) continue;
        for(n = rand(); n > 10; n/=2+1);
    if(n > 9) n = 1;
    do {
        for(x = rand(); x > AMAXX; x/=2+1);
        if(x >= AMAXX) x = 1;

        for(y = rand(); y > AMAXY; y/=2+1);
        if( y>= AMAXY) y = 1;
        } while(GameA[x][y]);
        GameA[x][y] = 100 + i;
        digit[i] = iRabbit;
	txDrawDC(x * 15 + 15, y * 15 + 55, iRabbit);
    }
}

void showInfo()
{   
    char t[64];
    sprintf(t, "Счет: %d", snake -> length);
    txBegin(true);
    txSetFillColor(TX_WHITE);
    txRectangle(0,0, 575, 35);
    txSelectFont("Times", 18, 18, 1);
    txSetColor(TX_BLACK);
    txTextOut(0,10, t);
    txEnd();
}

void finish()
{   
    txSetColor(TX_BLACK);
    txSelectFont("Times", 36, 36, 1);
    txTextOut(120,250, "Игра окончена!");
}


void makeWalls()
{
    int i;
    HDC iWall = txLoadImage("images/wall.jpg");
    txBegin(true);
    for(i = 0; i < 22; i++)
    {
        txDrawDC(i * 25 + 25, 40, iWall);
        txDrawDC(i * 25 + 25, 540, iWall);
    }
    for(i = 0; i < 21; i++)
    {
        txDrawDC(0, i * 25 + 40, iWall);
        txDrawDC(550, i * 25 + 40, iWall);
    }
    txEnd();
}


void timerHandler(void *p)
{
    int result = 0;
    if(gameover) return;
    switch (snake -> direction) 
    {
    case 1: // right
        result = snake -> Move(snake -> first -> x + 1, snake -> first -> y);
        break;
    case 2: // down
        result = snake -> Move(snake -> first -> x, snake -> first -> y + 1);
        break;
    case 3: // left
        result = snake -> Move(snake -> first -> x - 1, snake -> first -> y);
        break;
    case 4: // up
        result = snake -> Move(snake -> first -> x, snake -> first -> y - 1);
        break;
    case 5:
        break;
    }
//    printf("%d\n", result);
//    if(result != -1)
//	snake -> Draw();

    if(result >= 100) // скушали бонус - result равен индексу digit
    {
	int index = GameA[snake -> first -> x][snake -> first -> y] - 100;
//	printf("This is digit\n");
	digit[index] = NULL;
	for(int i = 0; i < 5; i++) snake -> Add();
	showInfo();
	generateDigits();
    }
    else
    if(result == -1 && !godmode) // наткнулись на стену
    {
	gameover = 1;
    }
    else
    if(result && !godmode) // натолкнулись на препятствие или на самого себя
    {
	gameover = 1;
    }
    if(gameover)
    {
	finish();
	return;
    }
    if(result != -1)
	snake -> Draw();
    UNUSED(p);
}

void ptInit()
{
    int i, j;
    txCreateWindow(575, 565, "Игра ПИТОН");
    srand(time(NULL));
    gameover = 0;
    for(i = 0; i < AMAXY; i++)
	for(j = 0; j < AMAXX; j++)
    	    GameA[j][i] = 0;
    for(i = 0; i < MAX_DIGIT; digit[i++] = NULL);
    HDC iBg = txLoadImage("images/bg.jpg");
    txBegin(true);
    for(i = 0; i < AMAXX; i++)
	for(j = 0; j < AMAXY; j++)
	    txDrawDC(i * 15 + 25, j * 16 + 65, iBg);
    makeWalls();
    txEnd();
    snake = new TSnake(0, 5, 10, 10, 255);
    snake -> Draw();
    showInfo();
    generateDigits();
    txTimerEvent(timerHandler, NULL, delay);
    txKeyEvent(keyHandler);
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    for(int i = 1; i < argc; i++)
        if(!strcmp(argv[i], "-g"))
        {
	    godmode = 1;
	    printf("Godmode activated\n");
        }	
        else
        if(!strncmp(argv[i], "-d=", 6))
        {
	    delay = atoi(argv[i]+7);
    	    printf("Delay = %d\n", delay);
        }
    ptInit();
    return txExec();
}

