#include<iostream>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<windows.h>

using namespace std;

#define MAX_LENGTH  1000

//Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen(){
	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hconsole, &csbi);
	
	consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point{
	int xcoord;
	int ycoord;
	
	Point(){
	}
	
	Point(int x, int y){
		xcoord = x;
		ycoord = y;
	}
};

class Snake{
	
	int length;
	char direction;
	
public:
	Point body[MAX_LENGTH];
	
	Snake(int x, int y){
		length = 1;
		body[0] = Point(x,y);
		direction = DIR_RIGHT;
	}
	
	int getlength(){
		return length;
	}
	
	void changeDirection(char newDirection){
		if (newDirection == DIR_UP && direction != DIR_DOWN)
		{
			direction = newDirection;	
		}
		else if (newDirection == DIR_DOWN && direction != DIR_UP)
		{
			direction = newDirection;
		}
		else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
		{
			direction = newDirection;
		}
		else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
		{
			direction = newDirection;
		}
	}
	
	bool move(Point food){
		
		for (int i = length-1; i>0; i--)
		{
			body[i] = body[i-1];
		}
		
		switch(direction)
		{
			int val;
			case DIR_UP:
				val = body[0].ycoord;
				body[0].ycoord = val-1;
				break;
			case DIR_DOWN:
				val = body[0].ycoord;
				body[0].ycoord = val+1;
				break;
			case DIR_LEFT:
				val = body[0].xcoord;
				body[0].xcoord = val-1;
				break;
			case DIR_RIGHT:
				val = body[0].xcoord;
				body[0].xcoord = val+1;
				break;
		}
		
		//if Hits Boundaries, Wrap around screen
		if (body[0].xcoord < 0)
		{
    		body[0].xcoord = consoleWidth - 1;
    	}
		else if (body[0].xcoord >= consoleWidth)
		{
    		body[0].xcoord = 0;
    	}

		if (body[0].ycoord < 0)
		{
    		body[0].ycoord = consoleHeight - 1;
    	}
		else if (body[0].ycoord >= consoleHeight)
		{
    		body[0].ycoord = 0;
    	}

		
		//Snake bites itself
		for(int i=1; i<length; i++)
		{
			if(body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord)
			{
				return false;
			}
		}
		
		//Snake eats food
		if (food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord)
		{
			body[length] = Point(body[length-1].xcoord, body[length-1].ycoord);
			length++;
		}
		
		return true;
	}
	
};

class Board{
	
	Snake *snake;//Dynamic memory allocation
	Point food;
	int score;
	
	const char SNAKE_BODY = 'O';
	const char FOOD = 'o';
		
public:
	Board(){
		spawnfood();
		snake = new Snake(10,10);
		score = 0;
	}
	
	~Board(){
		delete snake;
	}
	
	int getscore(){
		return score;
	}
	
	void spawnfood(){
		int x = rand() % consoleWidth;
		int y = rand() % consoleHeight;
		food = Point(x, y);
	}
	
	void displayScore(){
		gotoxy_win(consoleWidth/2,0);
		cout<<"Current Score: "<<score;
	}
	
	void gotoxy_win(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	
	void draw(){
		system("cls");
		displayScore();
		for(int i=0; i<snake->getlength(); i++)
		{
			gotoxy_win(snake->body[i].xcoord, snake->body[i].ycoord);
			cout<<SNAKE_BODY;	
		}
		gotoxy_win(food.xcoord, food.ycoord);
		cout<<FOOD;
	}
	
	bool update()
	{
		bool isAlive = snake->move(food);
		if (isAlive == false)
		{
			return false;
		}
		
		if (food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord)
		{
			score++;
			spawnfood();
		}
	
		return true;
	}
	
	void getInput(){
		if(kbhit())
		{
			int key = getch();
			if (key == 'w' || key == 'W')
			{
				snake->changeDirection(DIR_UP);
			}
			else if (key == 'a' || key == 'A')
			{
				snake->changeDirection(DIR_LEFT);
			}
			else if (key == 's' || key == 'S')
			{
				snake->changeDirection(DIR_DOWN);
			}
			else if (key == 'd' || key == 'D')
			{
				snake->changeDirection(DIR_RIGHT);
			}
		}
	}
	
	
};

int main(){
	srand(time(0));
	initScreen();
	
	Board *board = new Board();
	
	while(board->update())
	{
		board->getInput();
		board->draw();
		Sleep(50);
	}
	
	cout<<"Game Over"<<endl;
	
	return 0;
}