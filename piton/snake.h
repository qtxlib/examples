#define AMAXX 35
#define AMAXY 31

int GameA[AMAXX][AMAXY];

struct TStack // элемент змейки
{
    int x, y; // координаты
    int view;  // вид элемента
    int number; // номер элемента по порядку
    HDC pixmap;
    TStack *next, *prev; // связи с соседями
    
};

typedef TStack *TElem;

class TSnake
{
    public:
	TSnake(int, int, int, int, int);
	~TSnake();
	void Release();
	void Draw();
	void DrawElement(TStack*);
	int Move(int, int);
	void Add();
	void Remove();
	TStack *GetByNumber(int);
	TStack *GetByCoord(int, int);
	int SetDirection(int);
	void SetView(int);
//    private:
	TStack *first, *last;
	int direction, length, color;
	HDC iSnake, iBg;
	
};

TSnake::TSnake(int sDirection, int sLength, int sX, int sY, int sColor)
{
    int i;
    TStack *beg, *tek, *tek2;
    if(sDirection == 3) direction = 1; else direction = sDirection;
    length = sLength;
    color = sColor;
    first = last = NULL;
    if((sX < 1) || (sY < 1) || (sX > AMAXX) || (sY > AMAXY) ||
	((sX + sLength) > AMAXX) || !sLength)
    {
	printf("Error creating snake\n");
	return;
    }
    iSnake = txLoadImage("images/snake.png");
    iBg = txLoadImage("images/bg.jpg");
    beg = new TStack;
    beg -> x = sX + sLength;
    beg -> y = sY;
    beg -> view = 0;
    beg -> number = 1;
    beg -> next = NULL;
    beg -> prev = NULL;
    beg -> pixmap = iSnake;
    
    first = beg;
    tek = beg;
    if(sLength - 1 < 1) return;
    
    for(i = 1; i <= (sLength - 1); i++)
    {
	tek2 = new TStack;
	tek2 -> pixmap = iSnake;
	tek2 -> x = tek -> x-1;
	tek2 -> y = tek -> y;
	tek2 -> view = 0;
	tek2 -> next = NULL;
	tek2 -> prev = tek;
	tek2 -> number = tek -> number+1;
	
	tek -> next = tek2;
	tek = tek2;
	last = tek2;
    }
    
}

TSnake::~TSnake()
{
    TStack *tek = first, *tek2;

    while(tek != NULL)
    {
	tek2 = tek -> next;
	delete tek -> pixmap;
	delete tek;
	tek = tek2;
    }
}

void TSnake::Release()
{
    TStack *tek = first;
    
    if(tek == NULL) return;
    GameA[tek -> x][tek -> y] = 1;
    while(tek -> next != NULL)
    {
	tek = tek -> next;
	GameA[tek -> x][tek -> y] = 1;
    }
    GameA[tek -> x][tek -> y] = 0;
}

TStack *TSnake::GetByNumber(int number)
{
    TStack *tek = first;
    
    while(tek != NULL)
    {
	if(tek -> number == number) return tek;
	tek = tek -> next;
    }
    return NULL;
}

TStack *TSnake::GetByCoord(int x, int y)
{
    TStack *tek = first;

    while(tek != NULL)
    {
	if(tek -> x == x && tek -> y == y) return tek;
	tek = tek -> next;
    }
    return NULL;
}

int TSnake::Move(int newx, int newy)
{
    TStack *tek = last, *tek2;
    int result = 0;
    
    if(first == NULL) return 0;
    if(newx > AMAXX) return -1; //newx = 1;
    if(newx < 1) return -1; //newx = AMAXX;
    if(newy > AMAXY) return -1; //newy = 1;
    if(newy < 1) return -1; //newy = AMAXY;
    Release();
    if(last -> x > 0)
	txDrawDC(last -> x * 15 + 15, last -> y * 15 + 55, iBg);
    
    if(GameA[newx][newy] > 0) result = GameA[newx][newy];
    while(tek -> prev != NULL)
    {
	tek2 = tek -> prev;
	tek -> x = tek2 -> x;
	tek -> y = tek2 -> y;
	tek -> view = tek2 -> view;
	tek = tek2;
    }
    first -> x = newx;
    first -> y = newy;
    return result;
}

int TSnake::SetDirection(int sDirection)
{
    if(sDirection == 1 && direction == 3) return 0;
    if(sDirection == 3 && direction == 1) return 0;
    if(sDirection == 2 && direction == 4) return 0;
    if(sDirection == 4 && direction == 2) return 0;
    direction = sDirection;
    return 1;
}

void TSnake::Add()
{
    TStack *tek = new TStack;
    tek -> pixmap = iSnake;;
    tek -> x = 0;
    tek -> y = 0;
    tek -> next = NULL;
    tek -> prev = last;
    tek -> view = last -> view;
    tek -> number = last -> number+1;
    last -> next = tek;
    last = tek;
    length++;
}

void TSnake::Remove()
{
    TStack *tek = last;
    if(tek == first) return;
    last = tek -> prev;
    last -> next = NULL;
    delete tek -> pixmap;
    delete tek;
    length--;
}

void TSnake::SetView(int sView)
{
    TStack *tek = first;
    while(tek != NULL)
    {
	tek -> view = sView;
	tek = tek -> next;
    }
}

void TSnake::DrawElement(TStack *elem)
{
    if(elem -> view == 0)
    {
	GameA[elem -> x][elem -> y] = 1;
	if( elem -> x > 0)
	txDrawDC(elem -> x * 15 + 15, elem -> y * 15 + 55, elem -> pixmap);
    }
}

void TSnake::Draw()
{
    TStack *tek = first;
    txBegin(true);
    while(tek != NULL)
    {
	DrawElement(tek);
	tek = tek -> next;
    }
    txEnd();
}
