#include "qtx/libqtx.h"

#define MAXX 11
#define MAXY 25
#define MAXB 50
#define DELAY 2

int rx, by, rstepx, bstepy, score, rackets, level, field[MAXX][MAXY];
float bx, bstepx;
bool withball, gameover;

void drawRacket(bool mode = true)
{
    txBegin(true);
    txSetColor(mode == false ? TX_BLACK : TX_RED);
    txSetFillColor(mode == false ? TX_BLACK : TX_RED);
    txRectangle(rx, 570, rx + 10, 580);
    txRectangle(rx + 60, 570, rx + 70, 580);
    txSetColor(mode == false ? TX_BLACK : TX_WHITE);
    txLine(rx, 572, rx, 578);
    txLine(rx + 70, 572, rx + 70, 578);
    txSetFillColor(mode == false ? TX_BLACK : TX_GRAY);
    txRectangle(rx+11, 572, rx + 59, 580);
    txEnd();
}

void drawBall(bool mode = true)
{
    txBegin(true);
    txSetFillColor(mode == false ? TX_BLACK : TX_WHITE);
    txSetColor(mode == false ? TX_BLACK : TX_GRAY);
    txCircle(bx, by, 7);
    txEnd();
}

void drawInfo()
{
    char buf[128];
    txBegin(true);
    txSetFillColor(TX_BLACK);
    txSetColor(TX_BLACK);
    txRectangle(550, 450, 790, 590);
    txSelectFont("Lucida", 25, 25, false);
    sprintf(buf, "Раунд: %d", level);
    txSetColor(TX_LIGHTGREEN);
    txTextOut(550, 450, buf);
    txSetColor(TX_GREEN);
    txTextOut(551, 451, buf);

    sprintf(buf, "Ракеток: %d", rackets);
    txSetColor(TX_LIGHTGREEN);
    txTextOut(550, 490, buf);
    txSetColor(TX_GREEN);
    txTextOut(551, 491, buf);
    sprintf(buf, "Счет: %d", score);
    txSetColor(TX_LIGHTGREEN);
    txTextOut(550, 530, buf);
    txSetColor(TX_GREEN);
    txTextOut(551, 531, buf);
    txEnd();
}

void drawBrick(int x, int y)
{
    txBegin(true);
    switch(field[x][y])
    {
	case 1: txSetFillColor(TX_RED); break;
        case 2: txSetFillColor(TX_BLUE); break;
        case 3: txSetFillColor(TX_GRAY); break;
        case 4: txSetFillColor(TX_BROWN); break;
        case 5: txSetFillColor(TX_ORANGE); break;
        case 6: txSetFillColor(TX_GREEN); break;
        case 0: txSetFillColor(TX_BLACK); break;
    }
    txSetColor(TX_BLACK);
    txRectangle(x * 42 + 21, y * 15 + 21, x * 42 + 63, y * 15 + 36);
    if(field[x][y])
    {
	txSetColor(TX_WHITE);
	txLine(x * 42 + 21, y * 15 + 21, x * 42 + 21, y * 15 + 36);
	txLine(x * 42 + 21, y * 15 + 21, x * 42 + 63, y * 15 + 21);
    }
    txEnd();
}

void drawBricks()
{
    int i, c, x, y;
    txBegin(true);
    for(i = 0; i < MAXB; i++)
    {
	do
	{
	    x = random(MAXX);
	    y = random(MAXY);
	} while(field[x][y] || y<5);
	c = random(5) + 1;

	field[x][y] = c;
	drawBrick(x, y);
    }
    txEnd();
}

void getReady()
{
    rx = 230;
    bx = 268;
    by = 563;
    rstepx = 0;
    bstepx = 0;
    bstepy = 0;
    withball =true;
    drawRacket();
    drawBall();
    drawInfo();
}


void crash()
{
    drawBall(false);
    drawRacket(false);
    rackets-=1;
    if(rackets < 0) // game over
    {
	txSelectFont("Lucida", 25, 25, false);
	txSetColor(TX_YELLOW);
	txTextOut(110, 450, "ИГРА ОКОНЧЕНА");
	gameover = true;
	return;
    }
    getReady();
}

void keyHandler(int key)
{
    if(gameover) return;
    switch(key)
    {
	case Qt::Key_Left: rstepx = -1; if(withball) bstepx = -1; break;
	case Qt::Key_Right: rstepx = 1; if(withball) bstepx = 1; break;
	case Qt::Key_Space: 
	    if(withball)
	    { 
		withball = false;
		bstepy = -1;
		if(!bstepx) bstepx = 0.3;
	    }
	    break;
    }
    
}

void timerHandler(void *p)
{
    int i, j, x, y;
    bool victory;
    if(gameover) return;
    if((rstepx < 0 && rx < 22) || (rstepx > 0 && rx > 409)) rstepx = 0;
    if(rstepx)
    {
	drawRacket(false);
	rx = rx + rstepx;
	drawRacket(true);
    }
    if(bstepx < 0 && bx < 30) bstepx = -bstepx;
    if(bstepx > 0 && bx > 470) bstepx = -bstepx;
    if(bstepy < 0 && by < 30) bstepy = -bstepy;
    if(bstepy > 0 && by > 563 && bx > rx && bx < rx + 70) bstepy = -bstepy;
    if(bstepy > 0 && by > 585) { crash(); return; }
    if(bstepx > 0 && bx > 450 && withball) bstepx = 0;
    if(bstepx < 0 && bx < 60 && withball) bstepx = 0;
    victory = true;
    for(i = 0; i < MAXX; i++)
	for(j = 0; j < MAXY; j++)
	    if(field[i][j])
	    {
		victory = false;
		x = i * 42 + 21;
		y = j * 15 + 21;
		if((bstepy < 0 && ((bx > x && bx < x + 42) || (bx-10 > x && bx-10 < x + 42) || (bx+10 > x && bx+10 < x + 42)) && by - 10 == y + 15) || // from bottom
		(bstepy > 0 && ((bx > x && bx < x + 42) || (bx-10 > x && bx-10 < x + 42) || (bx+10 > x && bx+10 < x + 42)) && by + 10 == y )) // from bottom
		{
		    field[i][j] = 0;
		    drawBrick(i,  j);
		    bstepy = - bstepy;
		    score += 10;
		    drawInfo();
		}
		if((bstepx > 0 && ((by > y && by < y + 15) || (by-10 > y && by-10 < y + 15) || (by+10 > y && by+10 < y + 15)) && bx + 10 == x) || // from left
		(bstepx < 0 && ((by > y && by < y + 15) || (by-10 > y && by-10 < y + 15) || (by+10 > y && by+10 < y + 15)) && bx - 10 == x + 42 )) // from right
		{
		    field[i][j] = 0;
		    drawBrick(i,  j);
		    bstepx = - bstepx;
		    score += 10;
		    drawInfo();
		}
	    }
    
    if(victory)
    {
	txSelectFont("Lucida", 48, 48, false);
	txSetColor(TX_YELLOW);
	txTextOut(40, 250, "П О Б Е Д А !");
	gameover = true;
	return;
    }
    if(bstepx || bstepy)
    { 
	drawBall(false);
	bx += bstepx;
	by += bstepy;
	drawBall(true);
    }
    UNUSED(p);
}

void arInit()
{
    int i, j;
    srand(time(NULL));
    for(i = 0; i < MAXX; i++)
	for(j = 0; j < MAXY; j++)
	    field[i][j] = 0;
    txCreateWindow(800, 600, "ARKANOID");
    txSetFillColor(TX_BLACK);
    txRectangle(5, 5, 795, 595);
    txSetFillColor(TX_GRAY);
    txSetColor(TX_GRAY);
    txRectangle(5, 5, 20, 595);
    txRectangle(5, 5, 500, 20);
    txRectangle(484, 5, 500, 595);
    txSelectFont("Lucida", 36, 36, true);
    txSetColor(TX_BROWN);
    txTextOut(503, 40, "ARKANOID");
    score = 0;
    level = 1;
    rackets = 3;
    gameover = false;
    drawBricks();
    txKeyEvent(keyHandler);
    txTimerEvent(timerHandler, NULL, DELAY);

}

int main()
{
    arInit();
    getReady();
    return txExec();
}