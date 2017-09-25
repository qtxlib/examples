//{===========================================================================
//! @file       Examples\Demo\Shaker.cpp
//!
//! @brief      Этюд "Встряхиватель шариков"
//!
//! @date       2009
//! @author     Георгий Шкуратов, 7 класс
//          (C) Георгий Шкуратов, 7 класс, 2009
//}===========================================================================

#include "qtx/libqtx.h"

#define GetAsyncKeyState(x) bool(x)
bool VK_ESCAPE, VK_CONTROL;
void keyHandler(int key) 
{ 
    if(key == Qt::Key_Escape) VK_ESCAPE = true; 
    if(key == Qt::Key_Control) VK_CONTROL = true; 
}
inline void clearKeys() { VK_ESCAPE = false; VK_CONTROL = false; }
inline TXWINDOW txWindow() {   return txGetWindow(); }
inline void GetWindowRect (TXWINDOW w, RECT *r) { txGetWindowRect(w, r);  }
unsigned GetTickCount()
{
        struct timeval tv;
        if(gettimeofday(&tv, NULL) != 0) 
    	    return 0;
        return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}



//#if !defined (_TX_VER) || (_TX_VER < 0x172a0000)
//#error Must use TXLib.h version >= 1.72 to compile this. -- Ded
//#endif

//============================================================================

unsigned long Time = 0;
bool _txExit = false;

POINT WindowSize   = {0,0};

//============================================================================

struct Ball_t
    {
    double   x_;
    double   y_;
    double   vx_;
    double   vy_;
    double   r_;
    COLORREF color_;

    Ball_t();

    void draw (bool shading) const;
    void move (const POINT& d);
    };

//----------------------------------------------------------------------------

Ball_t::Ball_t() :
    x_     (random (100, WindowSize.x - 100)),
    y_     (random (100, WindowSize.y - 100)),
    vx_    (random (  5, 15)),
    vy_    (random (  5, 15)),
    r_     (random ( 10, 30)),
    color_ (RGB (int(random (50, 255)), int(random (50, 255)), int(random (50, 255))))
    {}

//----------------------------------------------------------------------------

void Ball_t::draw (bool shading) const
    {
    if (shading)
        {
        double r = txExtractColor (color_, TX_RED)   / 255.0,
               g = txExtractColor (color_, TX_GREEN) / 255.0,
               b = txExtractColor (color_, TX_BLUE)  / 255.0;

        for (double t = 0; t <= 1; t += 1.0/10)
            {
            double c = pow (t + 0.05, 0.25);
            txColor     (r*c, g*c, b*c);
            txFillColor (r*c, g*c, b*c);
            txCircle (x_, y_, r_ * (1-t) + 0.5);

            if (GetTickCount() > Time) break;
            }
        }
    else
        {
        txSetColor     (color_);
        txSetFillColor (color_);
        txCircle (x_, y_, r_);
        }
    }

//----------------------------------------------------------------------------

#define AY         ( 0.7 )
#define DT         ( 1.0 )
#define ELASTICITY ( 0.6 + random (-0.2, +0.1) )

void Ball_t::move (const POINT& d)
    {
    x_  -= d.x;
    y_  -= d.y;

    y_  += vy_ * DT;
    x_  += vx_ * DT;

    vy_ += AY;

    if (x_ < 0            + r_) { x_ = 0            + r_; vx_ = -vx_ * ELASTICITY + d.x; }
    if (y_ < 0            + r_) { y_ = 0            + r_; vy_ = -vy_ * ELASTICITY + d.y; }
    if (x_ > WindowSize.x - r_) { x_ = WindowSize.x - r_; vx_ = -vx_ * ELASTICITY + d.x; }
    if (y_ > WindowSize.y - r_) { y_ = WindowSize.y - r_; vy_ = -vy_ * ELASTICITY + d.y; }
    }

//============================================================================

POINT TxGetWindowOrg();

POINT TxGetWindowOrg()
    {
    RECT r;
//    r.left = r.top = r.right = r.bottom = 0;// = {0, 0, 0, 0};
    GetWindowRect (txWindow(), &r);
    POINT org = { r.left, r.top };
    return org;
    }

//============================================================================

#define SLEEP_TIME 20


int main()
    {
    srand ((unsigned) time (NULL));

    txCreateWindow (700, 500);
    txKeyEvent(keyHandler); clearKeys();

    WindowSize.x = txGetExtent().X;
    WindowSize.y = txGetExtent().Y;
    POINT org1 = TxGetWindowOrg();

    Ball_t balls[20];

    txBegin();

    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        clearKeys();
        Time = GetTickCount() + SLEEP_TIME;

        txSetFillColor (TX_BLACK);
        if (!GetAsyncKeyState (VK_CONTROL)) txClear();

        txDrawText (0, WindowSize.y * 4/10, WindowSize.x, WindowSize.y * 6/10, 
                    "Move window, shake the balls. [Ctrl] paints, [Esc] exits");

        POINT org0 = org1; org1 = TxGetWindowOrg();
        POINT d = { org1.x - org0.x, org1.y - org0.y };

        for (unsigned i = 0; i < SIZEARR (balls); i++) balls[i].move (d);
        for (unsigned i = 0; i < SIZEARR (balls); i++) balls[i].draw (false);

        txSleep (0);

        char s[100] = ""; sprintf (s, "%+03ld ms free", Time - GetTickCount());
//        SetWindowText (txWindow(), s);

//        while (GetTickCount() < Time) Sleep (0);
	txSleep(SLEEP_TIME);
        }

    txEnd();

    _txExit = true;
    return 0; //txExec();
    }
