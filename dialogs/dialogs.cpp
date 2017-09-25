#include "qtx/libqtx.h"

long count;
char buf[512];
char buf1[1024];
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
	txMessageBox("Нажата левая кнопка мыши.");
    if(b == Qt::RightButton)
	txMessageBox("Нажата правая кнопка мыши.");
    if(b == Qt::MiddleButton)
	txMessageBox("Нажата средняя кнопка мыши.");
    if(dbclick)
    {
	txMessageBox("Это был двойной щелчек.");
    }
    UNUSED(x);
    UNUSED(y);
    UNUSED(input);
}


void idleHandler(void *p)
{
    UNUSED(p);
//    printf("%d\n", count++);
}

void clicked()
{
    txMessageBox("Нажата правая кнопка ABOUT.");
}
void clicked1()
{
    printf("Radio button state changed: :%s\n", state == true ? "true" : "false");
}

void clickedTE()
{
    printf("Text editor text changed: %s\n", buf1);
}

void clicked2()
{
    printf("Edit line changed: %s\n", buf);
}

void toggled()
{
    printf("Radio toggled: item #%d\n", id);
}

void toggledl()
{
//    char s[256];
    printf("List box item #%d selected\n", id);
//    txMessageBox(s);
    
}

void clickedb1()
{
    txCreateWindow (800, 600, "Пример примитивной графики");
    txBegin(true);
    txSetColor (TX_WHITE);
    txSetFillColor (TX_BLACK);
    txRectangle (10, 10, 790, 590);

    txSetColor (TX_LIGHTCYAN);
    txEllipse (200, 150, 600, 450);
    txEllipse (245, 150, 555, 450);
    txEllipse (290, 150, 510, 450);
    txEllipse (330, 150, 470, 450);
    txEllipse (375, 150, 425, 450);
    txEllipse (200, 150, 600, 450);
    txEllipse (200, 190, 600, 410);
    txEllipse (200, 230, 600, 370);
    txEllipse (200, 270, 600, 330);
    txLine    (200, 300, 600, 300);

    txSetColor (TX_LIGHTGREEN);
    txSelectFont ("Times New Roman", 60);
    txSetTextAlign (TA_CENTER);
    txTextOut (400, 480, "Hello, world!");

    txSetColor (TX_YELLOW);
    txSetFillColor (TX_YELLOW);
    txLine   (385, 135, 385, 120);
    txLine   (385, 135, 375, 150);
    txLine   (385, 135, 395, 150);
    txLine   (385, 125, 375, 135);
    txLine   (385, 125, 400, 120);
    txCircle (385, 115, 6);

    txSetFillColor (TX_TRANSPARENT);
    txLine (400, 75, 400, 150);
    txRectangle (400, 75, 450, 115);
    txSelectFont ("Times New Roman", 20);
    txTextOut (425, 85, "C++");
    txEnd();
}


void clickedb2()
{
    const char *input;
    input = txInputBox("Это InputBox", "А это заголовок", "вводите здесь");
    txMessageBox(input, "Вы ввели значение", 0);
}

void clickedch()
{
    printf("CheckBox state changed: :%s\n", state == true ? "true" : "false");
}

int main()
{

    // ListBox, ComboBox, CheckBox, RadioButton
    
    strcpy(buf, "HELLO");
    count = 0;
//    init("Пример");
    TXWINDOW w = txCreateWindow (800, 600, "Демонстрация qtxlib. Диалоги.");

    txKeyEvent(keyHandler);
    txMouseEvent(mouseHandler);
    txTimerEvent(idleHandler, NULL, 500);
    TXWINDOW w1 = txCreateWindow (70, 400, 240, 230, "2-ое окно");
    txSetWindow(w);
    txSetColor(TX_BLUE);
    txSetFillColor(TX_WHITE);
    txCircle(50,50, 45);
    txSetFillColor(TX_GREEN);
    txFloodFill(55, 55, TX_BLUE);
    txSetColor(TX_RED);
    txTextOut(100, 20, "Легко совмещать графику и элементы управления в едином пространстве");
    txSetColor(TX_BLUE);
    txTextOut(140, 50, "Рисовать и строить диалоговые окна просто как никогда");
    txSetColor(TX_GREEN);
    txTextOut(120, 80, "Элементарно просто обрабатывать события таймера/мыши/клавиатуры");
    txSetColor(TX_BROWN);
    txTextOut(140, 110, "Работайте параллельно со множеством окон");
    txSetColor(TX_BLACK);
    txTextOut(80, 140, "Свойства и методы всех элементов управления доступны через интерфейс Qt");
    txSetColor(TX_ORANGE);
    txTextOut(100, 170, "Реализованы все основные элементы управления диалоговых окон");
    txSetColor(TX_RED);
    txTextOut(230, 200, "Щелкайте кнопками мыши в пределах окна");
    txSetColor(TX_BLUE);
    txTextOut(230, 510, "Щелкайте по элементам управления");
    txTextOut(230, 530, "Уведомления о событиях выводятся в консоль.");
    txTextOut(230, 550, "Смотрите туда :)");
    
    txSetWindow(w1);
//    txSetColor(TX_BLUE);
//    txSetFillColor(TX_BLUE);
//    txCircle(50, 50, 50);
    txButton(170,10,50, 30, "About", clicked);
    txCheckBox(150, 180, 100, 30, "Checkbox", &state, clicked1);
    HDC qt = txLoadImage("qt.png");
    txDrawDC(10, 10, qt);

//    txPlaySound("notify.wav");

    txSetWindow(w);
    txNotifyIcon(0, "Уведомление в системном трее", "Привет");

//    qtx -> label -> decorate();
    txButton(10,100,90, 30, "Новое окно", clickedb1);
    txButton(100,200,90, 50, "Кнопка 2", clickedb2);
    txLabel(100, 245, 100, 30, "Строка ввода:");
    txEdit(100, 270, 100, 30, buf, clicked2);
    txLabel(400, 245, 200, 30, "Редактор многострочного текста:");
    txTextEdit(400, 270, 300, 200, buf1, clickedTE);
    txCheckBox(100, 320, 100, 30, "Checkbox", &state, clickedch);
//    txRadioButton(100, 340, 100, 30, "Radio button", &rstate, toggled);
    txRadioGroup(100, 350, 100, 90, "Radio group", &id, toggled, NULL, "First", "Second", "Third", NULL);
    const char *list[4] = { "First", "Second", "Third", NULL };
    txLabel(100, 430, 100, 30, "ComboBox:");
    txComboBox(170, 430, 100, 30, "Combo box", &id, toggled, NULL, (char**)list);
    txLabel(100, 465, 100, 30, "ListBox:");
    txListBox(100, 490, 100, 100, "List box", &id, toggledl, NULL, (char**)list);
    
//    QTxDialog window;
//    window.setWindowTitle(QString::fromUtf8("MainWindow"));
//    window.resize(450,300);
//    window.decorate();
    return txExec();
}