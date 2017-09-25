//{=========================================================================== 
//! @file       Examples\Demo\Tree.cpp
//!
//! @brief      Этюд "Дерево"
//!
//! @date       2010
//! @author     Кирилл Кулешов, 7 класс
//          (C) Кирилл Кулешов, 7 класс, 2010
//}===========================================================================

#include "qtx/libqtx.h"
#define GetAsyncKeyState(x) bool(x)

//----------------------------------------------------------------------------

void DrawTree (double x, double y, double length, double angle);


bool VK_ESCAPE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SHIFT;
inline void clearKeys() {  VK_ESCAPE = VK_LEFT = VK_RIGHT = VK_UP = VK_DOWN = VK_SHIFT = false; }
void keyHandler(int key)
{
    switch(key)
    {
	case Qt::Key_Escape: VK_ESCAPE = true; break;
	case Qt::Key_Left: VK_LEFT = true; break;
	case Qt::Key_Right: VK_RIGHT = true; break;
	case Qt::Key_Up: VK_UP = true; break;
	case Qt::Key_Down: VK_DOWN = true; break;
	case Qt::Key_Shift: VK_SHIFT = true; break;
    }
}


int main ()
    {
    txCreateWindow (800, 600);
    txKeyEvent(keyHandler);
    txBegin();
 
    clearKeys();
    while (!GetAsyncKeyState(VK_ESCAPE))
        {
        txSetFillColor (TX_BLACK);
        txClear();
	
	txBegin(true);
        DrawTree (400, 0, 100, txPI/2);
	txEnd();

        txSleep (100);
        }

    txEnd();
    return txExec();
    }

//----------------------------------------------------------------------------

void DrawTree (double x, double y, double length, double angle)
    {
    if (length <= 0) return;

    double wind = 0;
    int  height = 0;

    if (GetAsyncKeyState (VK_LEFT))  wind   =               +0.2;
    if (GetAsyncKeyState (VK_RIGHT)) wind   =               -0.2;
    if (GetAsyncKeyState (VK_SHIFT)) wind   = random (-0.7, +0.7);
    if (GetAsyncKeyState (VK_UP))    height =               +5;
    if (GetAsyncKeyState (VK_DOWN))  height =               -5;

    double leaves = 0;
    if (height ==  0) leaves = 20;
    if (height ==  5) leaves = 25;
    if (height == -5) leaves = 15;

    txSetColor ((length > leaves)? RGB (115, 65, 10) : RGB (35, 255, 10),
                (int) (pow (length/7, 0.9) + 0.5));

    // Here comes the tree
                
    double x1 = x + length * cos (angle),
           y1 = y + length * sin (angle);

    txLine (x, 600 - y, x1, 600 - y1);

    DrawTree (x1, y1, length - 10 + random (-1, +1) + height, angle + 0.35 + random (-0.2, +0.2) + wind);
    DrawTree (x1, y1, length - 10                           , angle - 0.35 - random (-0.2, +0.2) + wind);
    clearKeys();
    }

