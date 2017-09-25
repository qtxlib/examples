//{===========================================================================
//! @file       Examples\Tennis\Tennis.cpp
//!
//! @brief      Пример использования функций txLoadImage(), txDeleteDC(),
//!             txBitBlt(), txTransparentBlt(), txAlphaBlend().
//!
//!             Библиотека Тупого Художника (The Dumb Artist Library, TX Library, TXLib).
//!
//!             $Version: 00172a, Revision: 99 $
//!             $Date: 2012-12-24 00:14:48 +0400 $
//!             $Copyright: (C) Ded (Ilya Dedinsky, http://txlib.ru) <mail@txlib.ru> $
//              $Copyright: (C) Ded (Ilya Dedinsky, http://txlib.ru) <mail@txlib.ru> $
//}===========================================================================


// this is the small piece of code my code
#include "qtx/libqtx.h"
#define VK_ESCAPE Qt::Key_Escape
#define VK_UP Qt::Key_Up
#define VK_DOWN Qt::Key_Down
int mouseX = 0, mouseY = 0, lastkey = 0;
int txMouseX() { return mouseX; }
int txMouseY() { return mouseY; }
void clearev() { lastkey = 0; }
bool GetAsyncKeyState(int k) { return k == lastkey; }
void mouseHandler(int x, int y, int buttons, bool dbclick) {  mouseX = x; mouseY = y; UNUSED(buttons); UNUSED(dbclick); }
void keyHandler(int key) { lastkey = key; }
// end of my code. It's very small, isn't it? :)



//#include "TXLib.h"

int main()
    {
    int    mode  = 3;
    double alpha = 1;
    txCreateWindow (800, 600);

    // Загружаем изображения один раз, в начале программы
    
    HDC table  = txLoadImage ("Resources/Images/table.bmp");  // Пример загрузки картинки
    HDC racket = txLoadImage ("Resources/Images/racket.bmp"); // Пример загрузки картинки

    txSetFillColor (TX_RED);


    txKeyEvent(keyHandler); // added
    txMouseEvent(mouseHandler); // added
    
    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        clearev(); // added
        txSleep (20);
        int x = txMouseX(), y = txMouseY();

        txBitBlt (txDC(), 0, 0, 800, 600, table, 0, 0);

        if (GetAsyncKeyState (Qt::Key_1))     mode   = 1;
        if (GetAsyncKeyState (Qt::Key_2))     mode   = 2;
        if (GetAsyncKeyState (Qt::Key_3))     mode   = 3;

        if (GetAsyncKeyState (VK_UP))   alpha += 0.05;
        if (GetAsyncKeyState (VK_DOWN)) alpha -= 0.05;
        if (alpha < 0) alpha = 0;
        if (alpha > 1) alpha = 1;

        // Копирование на экран разными способами

        switch (mode)
            {
            case 1: txBitBlt         (txDC(), x-200/2, y-150/2, 200, 150, racket, 0, 0);        break; // Пример копирования
            case 2: txTransparentBlt (txDC(), x-200/2, y-150/2, 200, 150, racket, 0, 0);        break; // Пример копирования с прозрачным цветом
            case 3: txAlphaBlend     (txDC(), x-200/2, y-150/2, 200, 150, racket, 0, 0, alpha); break; // Пример копирования с полупрозрачностью
            default: break;
            }

        }

    // Удаляем ненужные DC, освобождая системные ресурсы 

    txDeleteDC (table);
    txDeleteDC (racket);

    return 0;
    }



