#include "qtx/libqtx.h"

#define MAX_LENGTH 15

char display[MAX_LENGTH], operand1[MAX_LENGTH], operation;
bool hasDot, op1Taken, calculated, error;
QLineEdit *cDisplay;

void clear()
{
    cDisplay -> setText("0");
    strcpy(display, "0");
    strcpy(operand1, "0");
    operation = 0;
    hasDot = false;
    calculated = false;
    error = false;
    op1Taken = false;
}

void calc()
{
    int i;
    double result = 0, operand2 = atof(display);
    bool dot = false;
//    printf("%s %c %s = \n", operand1, operation, display);
    switch(operation)
    {
	case '+':
	    result = atof(operand1) + operand2;
	    break;
	case '-':
	    result = atof(operand1) - operand2;
	    break;
	case '*':
	    result = atof(operand1) * operand2;
	    break;
	case '/':
	    if(operand2 == 0)
	    {
		clear();
		error = true;
		cDisplay -> setText("Деление на 0");
		return;
	    }
	    else
		result = atof(operand1) / operand2;
	    break;
    }
    if(result - (long)result == 0) 
	sprintf(display, "%ld", (long)result);
    else
	sprintf(display, "%15.6f", result);
    for(i = 0; display[i]; i++) if(display[i] == ',') dot = true;
    for(i--; i > 0 && dot; ) if(display[i] == '0') i--; else break;
    if(dot && display[i+1] == '0') display[i+1] = 0;
    cDisplay -> setText(display);
    strcpy(operand1, "0");
    calculated = true;
}


bool process(const char *s)
{
    int len = cDisplay -> text().length();
    bool isLengthValid = len < MAX_LENGTH;

    if(error)
    {
	cDisplay -> clear();
	error = false;
    }
    if(s[0] == '?')
    {
	txMessageBox("Программа калькулятор с использование QTXLib.<br>(C) Е.Гаевский, А.Легалов, 2016.", "О программе");
	return false;
    }
    strcpy(display, cDisplay -> text().toStdString().c_str());
    if(s[0] == 'C') { clear(); return false;  }
    if(s[0] == '=') { calc(); return false; }
    if(s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
	{ 
	    operation = s[0];
	    strcpy(operand1, cDisplay -> text().toStdString().c_str());
	    return false; 
	}

    if(s[0] == ' ')
    {
	if(!(operand1[0] == '0' && !operand1[1]) && !op1Taken) { strcpy(operand1, "0"); operation = 0; } // operation cancelled
	if(display[0] == '0' && !hasDot) return false;
	if(display[len - 1] == ',') hasDot = false;
	display[len - 1] = 0; 
	cDisplay -> setText(display);
	if(!display[0]) { display[0] = '0';  cDisplay -> setText("0");  }
	return false;
    }
    if((!(operand1[0] == '0' && !operand1[1]) && !op1Taken) || calculated)
    {
	hasDot = false;
	op1Taken = true;
	calculated = false;
	cDisplay -> clear();
	len = cDisplay -> text().length();
	isLengthValid = len < MAX_LENGTH;
    }
    if(s[0] == '0' && display[0] == '0' && !hasDot) return false;
    if(s[0] == ',' && hasDot) return false;
    if(s[0] == ',') hasDot = true;
    if(display[0] == '0' && s[0] != ',' && !hasDot) cDisplay -> clear();
    if(isLengthValid)
	cDisplay -> setText(cDisplay -> text() + s);
    return isLengthValid;
}


void clicked_7() { process("7"); }
void clicked_8() { process("8"); }
void clicked_9() { process("9"); }
void clicked_4() { process("4"); }
void clicked_5() { process("5"); }
void clicked_6() { process("6"); }
void clicked_3() { process("3"); }
void clicked_2() { process("2"); }
void clicked_1() { process("1"); }
void clicked_0() { process("0"); }
void clicked_dot() { process(","); }
void clicked_equ() { process("="); }
void clicked_backspace() { process(" "); }
void clicked_clear() { process("C"); }
void clicked_plus() { process("+"); }
void clicked_minus() { process("-"); }
void clicked_mul() { process("*");  }
void clicked_div() { process("/"); }
void clicked_about() { process("?"); }


void keyHandler(int key)
{
    switch (key)
    {
	case Qt::Key_7: clicked_7(); break;
	case Qt::Key_8: clicked_8(); break;
	case Qt::Key_9: clicked_9(); break;
	case Qt::Key_4: clicked_4(); break;
	case Qt::Key_5: clicked_5(); break;
	case Qt::Key_6: clicked_6(); break;
	case Qt::Key_3: clicked_3(); break;
	case Qt::Key_2: clicked_2(); break;
	case Qt::Key_1: clicked_1(); break;
	case Qt::Key_0: clicked_0(); break;
	case Qt::Key_Backspace: clicked_backspace(); break;
	case Qt::Key_Plus: clicked_plus(); break;
	case Qt::Key_Minus: clicked_minus(); break;
	case Qt::Key_Enter: clicked_equ(); break;
	case Qt::Key_Return: clicked_equ(); break;
	case Qt::Key_Escape: clicked_clear(); break;
	case Qt::Key_Asterisk: clicked_mul(); break;
	case Qt::Key_Slash: clicked_div(); break;
	case Qt::Key_Period: clicked_dot(); break;
	case Qt::Key_Comma: clicked_dot(); break;
    }

}

int main()
{ 
    txCreateWindow(230, 220, "Калькулятор");
    txKeyEvent(keyHandler);
    cDisplay = txEdit(5, 5, 220, 30, display, NULL, NULL);
    cDisplay -> setReadOnly(true);
    cDisplay -> setAlignment(Qt::AlignRight);
    clear();
    txSetColor(TX_WHITE);
    txRectangle(5, 40, 225, 41);
    txButton(5, 50, 35, 35, "7", clicked_7, NULL);
    txButton(45, 50, 35, 35, "8", clicked_8, NULL);
    txButton(85, 50, 35, 35, "9", clicked_9, NULL);
    txButton(5, 90, 35, 35, "4", clicked_4, NULL);
    txButton(45, 90, 35, 35, "5", clicked_5, NULL);
    txButton(85, 90, 35, 35, "6", clicked_6, NULL);
    txButton(5, 130, 35, 35, "3", clicked_3, NULL);
    txButton(45, 130, 35, 35, "2", clicked_2, NULL);
    txButton(85, 130, 35, 35, "1", clicked_1, NULL);
    txButton(5, 170, 35, 35, "0", clicked_0, NULL);
    txButton(45, 170, 35, 35, ".", clicked_dot, NULL);
    txButton(85, 170, 35, 35, "=", clicked_equ, NULL);
    txButton(145, 50, 35, 35, "<-", clicked_backspace, NULL);
    txButton(185, 50, 35, 35, "C", clicked_clear, NULL);
    txButton(145, 90, 35, 35, "+", clicked_plus, NULL);
    txButton(185, 90, 35, 35, "-", clicked_minus, NULL);
    txButton(145, 130, 35, 35, "X", clicked_mul, NULL);
    txButton(185, 130, 35, 35, "/", clicked_div, NULL);
    txButton(185, 170, 35, 35, "?", clicked_about, NULL);
    return txExec();
}

