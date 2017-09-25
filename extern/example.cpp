#define EXTERN_QTX 1

#include "qtx/libqtx.h"



long count;
char buf[512];
bool state = true, rstate = true;
int id = 1;

void keyHandler(int key)
{
   if(key == Qt::Key_Backspace)
   {
	printf("back\n");
    txSetColor(TX_BLUE);
    txSetFillColor(TX_GRAY);
    txCircle(150,150, 45);
	
    }
}

void mouseHandler(int x, int y, int b, bool dbclick)
{
    const char *input;
    if(b == Qt::LeftButton)
	printf("Left\n");
    if(b == Qt::RightButton)
	printf("Right\n");
    if(b == Qt::MiddleButton)
    {
	printf("Middle\n");
	input = txInputBox("This is a message", "This is a caption", "input here");
	txMessageBox(input, "Заголовок", 0);
    }
    if(dbclick)
    {
	txMessageBox("Это какой-то текст в окне сообщения", "Заголовок", 0);
	printf("Double\n");
    }
    UNUSED(x);
    UNUSED(y);
}


void idleHandler(void *p)
{
    UNUSED(p);
//    printf("%d\n", count++);
}

void clicked()
{
    printf("CLICK 1\n");
}
void clicked1()
{
    printf("CLICK 2\n");
    printf("Text:%s\n", buf);
    printf("Bool:%s\n", state == true ? "true" : "false");
}

void clicked2()
{
    printf("Text changed\n");
}

void toggled()
{
    printf("Radio toggled %d\n", id);
}

int main()
{

    // ListBox, ComboBox, CheckBox, RadioButton
    
    strcpy(buf, "HELLO");
    count = 0;
//    init("Пример");
    TXWINDOW w = txCreateWindow (800, 600, "My first prog with qtxlib");
    txSetColor(TX_BLACK);
    txLine(400,0, 400, 600);
    txLine(0,310, 800, 310);

    txKeyEvent(keyHandler);
    txMouseEvent(mouseHandler);
    txTimerEvent(idleHandler, NULL, 500);
    TXWINDOW w1 = txCreateWindow (200, 100, "2nd");
    txSetWindow(w);
    
    txSetColor(TX_BLUE);
    txSetFillColor(TX_WHITE);
    txCircle(50,50, 45);
    txSetFillColor(TX_GREEN);
    
    txFloodFill(55, 55, TX_BLUE);
    txSetWindow(w1);
    txSetColor(TX_BLUE);
    txSetFillColor(TX_BLUE);
    txCircle(50, 50, 50);

    txNotifyIcon(0, "My application", "Hello");
//    txPlaySound("notify.wav");

    txSetWindow(w);
//    qtx -> label -> decorate();
    txButton(100,100,50, 50, "BUT1", clicked);
    txButton(100,200,50, 50, "BUT2", clicked1);
    txLabel(100, 245, 100, 30, "Enter your name:");
    txEdit(100, 270, 100, 30, buf, clicked2);
    txCheckBox(100, 320, 100, 30, "Checkbox", &state, clicked1);
//    txRadioButton(100, 340, 100, 30, "Radio button", &rstate, toggled);
    txRadioGroup(100, 370, 100, 90, "Radio group", &id, toggled, NULL, "First", "Second", "Third", NULL);
    const char *list[4] = { "First", "Second", "Third", NULL };
    txComboBox(100, 450, 100, 30, "Combo box", &id, toggled, NULL, (char**)list);
    txListBox(100, 480, 100, 100, "List box", &id, toggled, NULL, (char**)list);
    return txExec();
}



