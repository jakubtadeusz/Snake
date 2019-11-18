﻿
#include "pch.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <ctime>
#include <fstream>


const char KEY_UP = 'w';
const char KEY_DOWN = 's';
const char KEY_LEFT = 'a';
const char KEY_RIGHT = 'd';
const char KEY_ACCEPT = 'f';
const char KEY_UP2 = 72;
const char KEY_DOWN2 = 80;
const char KEY_LEFT2 = 75;
const char KEY_RIGHT2 = 77;
const char KEY_ACCEPT2 = 13;

const int QUIT = -1;
const int MAIN_MENU_OPTIONS_AMOUNT = 4;
const int OPTIONS_AMOUNT = 5;

using namespace std;

struct SNAKE
{
	int *x;
	int *y;
};

void fill(char **array);
void show(char **array, int score);
void clear();
bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize);
bool makeMove(int ch, char **map, int *x, int *y, int snakeSize);
void deletePointers(char **map, int *x, int *y);
bool checkCollision(int colX, int colY, int *x, int *y, int snakeSize);
SNAKE createSnake(int snakeSize);
SNAKE extendSnake(SNAKE snake, int *snakeSize);
char **createMap();
bool checkForPoint(int destX, int destY, int x, int y, int *score);
void createApple(int *x, int *y, char **map);
void setTerminalWindow();
int playGame();
void showOptions(int menuCursor);
void runOptions();
void createGame();
void mainMenu();
void saveScore(string nick, int score);
void exit();
int useMenuOption(int i);
void showMenu(int menuCursor);
int manageMenu(int menuCursor);
void runMenu();
void showHighscores();
void mainMenu();
int moveCursorDown(int menuCursor, int length);
int moveCursorUp(int menuCursor, int length);

int width = 60;
int height = 40;
int delay = 20;
bool gamemode = 0;

int main()
{
	mainMenu();
	return 0;
}

void fill(char **array)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			array[j][i] = ' ';
		}
	}
}

void show(char **array, int score)
{
	string toPrint = "Score: ";
	toPrint += to_string(score);
	toPrint += "\n";
	for (int i = -3; i < width + 3; i++)
	{
		toPrint += "!";
	}
	toPrint += "\n";
	for (int i = 0; i < height; i++)
	{
		toPrint += "!!!";
		for (int j = 0; j < width; j++)
		{
			toPrint += array[j][i];
		}
		toPrint += "!!!\n";
	}
	for (int i = -3; i < width + 3; i++)
	{
		toPrint += "!";
	}
	toPrint += "\n";
	cout << toPrint;
}

void clear()
{
	COORD cur = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

bool moveSnake(char **map, int *x, int *y, int x_dest, int y_dest, int snakePosition, int snakeSize)
{
	int x_old = x[snakePosition];
	int y_old = y[snakePosition];
	map[x_old][y_old] = ' ';
	x[snakePosition] = x_dest;
	y[snakePosition] = y_dest;
	map[x[snakePosition]][y[snakePosition]] = '#';

	if (snakePosition != snakeSize)
	{
		moveSnake(map, x, y, x_old, y_old, snakePosition + 1, snakeSize);
	}

	return 1;
}

bool makeMove(int ch, char **map, int *x, int *y, int snakeSize)
{
	switch (ch)
	{
	case KEY_LEFT:
		if (x[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, width - 1, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] - 1, y[0], 0, snakeSize);
		break;
	case KEY_RIGHT:
		if (x[0] == width - 1)
		{
			return gamemode ? moveSnake(map, x, y, 0, y[0], 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0] + 1, y[0], 0, snakeSize);
		break;
	case KEY_UP:
		if (y[0] == 0)
		{
			return gamemode ? moveSnake(map, x, y, x[0], height - 1, 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0], y[0] - 1, 0, snakeSize);
		break;
	case KEY_DOWN:
		if (y[0] == height - 1)
		{
			return gamemode ? moveSnake(map, x, y, x[0], 0, 0, snakeSize) : 0;
		}
		else moveSnake(map, x, y, x[0], y[0] + 1, 0, snakeSize);
		break;
	case 27: return 0;
	}
	clear();
	return 1;
}

void deletePointers(char **map, int *x, int *y)
{
	for (int i = 0; i < width; i++)
	{
		delete[] map[i];
	}
	delete[] map;
	delete[] x;
	delete[] y;
}

bool checkCollision(int colX, int colY, int *x, int *y, int snakeSize)
{
	for (int i = 1; i < snakeSize; i++)
	{
		if (colX == x[i] && colY == y[i])
		{
			return 0;
		}
	}
	return 1;
}

SNAKE createSnake(int snakeSize)
{
	int *x = new int[snakeSize];
	int *y = new int[snakeSize];
	srand(time(NULL));
	SNAKE snake;
	for (int i = 0; i < snakeSize; i++)
	{
		x[i] = 3;
		y[i] = 5;
	}
	snake.x = x;
	snake.y = y;
	return snake;
}

SNAKE extendSnake(SNAKE snake, int *snakeSize)
{
	SNAKE newSnake;
	(*snakeSize)++;
	newSnake.x = new int[*snakeSize];
	newSnake.y = new int[*snakeSize];
	for (int i = 0; i < (*snakeSize) - 1; i++)
	{
		newSnake.x[i] = snake.x[i];
		newSnake.y[i] = snake.y[i];
	}
	newSnake.x[*snakeSize - 1] = snake.x[*snakeSize - 2];
	newSnake.y[*snakeSize - 1] = snake.y[*snakeSize - 2];
	return newSnake;
}

char **createMap()
{
	char **map = new char*[width];
	for (int i = 0; i < width; i++)
	{
		map[i] = new char[height];
	}
	return map;
}

bool checkForPoint(int destX, int destY, int x, int y, int *score)
{
	if (destX == x && destY == y)
	{
		*score += 1000;
		return 1;
	}
	return 0;
}

void createApple(int *x, int *y, char **map)
{
	int celX = rand() % height;
	int celY = rand() % width;
	while (map[celX][celY] != ' ')
	{
		celX = rand() % height;
		celY = rand() % width;
	}
	map[celX][celY] = '0';
	*x = celX;
	*y = celY;
}

void setTerminalWindow()
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	MoveWindow(console, r.left, r.top, width * 10 + 4, (height + 4) * 18, TRUE);
}

int playGame()
{
	setTerminalWindow();
	int score = 0;
	char **map = createMap();
	fill(map);
	int snakeSize = 3;
	SNAKE snake = createSnake(snakeSize);
	snake = extendSnake(snake, &snakeSize);
	int *x = snake.x;
	int *y = snake.y;
	bool stan = 1;
	char direction = 'd';
	int appleX;
	int appleY;
	createApple(&appleX, &appleY, map);
	while (stan)
	{
		clear();
		show(map, score);
		if (_kbhit())
		{
			char key = _getch();
			switch (direction)
			{
			case KEY_RIGHT: if (key == KEY_LEFT || key == KEY_LEFT2 || tolower(key) == KEY_LEFT) key = KEY_RIGHT; break;
			case KEY_LEFT: if (key == KEY_RIGHT || key == KEY_RIGHT2 || tolower(key) == KEY_RIGHT) key = KEY_LEFT; break;
			case KEY_UP: if (key == KEY_DOWN || key == KEY_DOWN2 || tolower(key) == KEY_DOWN) key = KEY_UP; break;
			case KEY_DOWN: if (key == KEY_UP || key == KEY_UP2 || tolower(key) == KEY_UP) key = KEY_DOWN; break;
			}
			if (key == KEY_LEFT2 || key == KEY_DOWN2 || key == KEY_RIGHT2 || key == KEY_UP2 || key == 27)
			{
				direction = key;
			}
			switch (key)
			{
			case KEY_LEFT2: direction = KEY_LEFT; break;
			case KEY_RIGHT2: direction = KEY_RIGHT; break;
			case KEY_UP2: direction = KEY_UP; break;
			case KEY_DOWN2: direction = KEY_DOWN; break;
			}
			key = tolower(key);
			if (key == KEY_LEFT || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_UP || key == 27)
			{
				direction = key;
			}
		}
		stan = makeMove(direction, map, snake.x, snake.y, snakeSize - 1) && checkCollision(snake.x[0], snake.y[0], snake.x, snake.y, snakeSize);
		if (checkForPoint(snake.x[0], snake.y[0], appleX, appleY, &score))
		{
			createApple(&appleX, &appleY, map); snake = extendSnake(snake, &snakeSize);
		}
		Sleep(delay);
		score++;
	}
	deletePointers(map, snake.x, snake.y);
	system("cls");
	return score;
}

string getNick()
{
	string nick;
	cout << "Podaj imie:\n";
	cin >> nick;
	system("cls");
	return nick;
}

void createGame()
{
	clear();
	saveScore(getNick(), playGame());
}

void exit()
{
	system("cls");
}

int setIntValue()
{
	int value;
	int buff;
	system("cls");
	cout << "Podaj wartosc: \n";
	cin >> buff;
	while (cin.fail())
	{
		cin.clear();
		system("cls");
		while (cin.get() != '\n') continue;
		cout << "Prosze podac prawidlowa wartosc: \n";
		cin >> buff;
	} 
	system("cls");
	value = buff;
	return value;
}

int speedOption(int menuCursor)
{
	switch (menuCursor)
	{
	case 0: return 80; break;
	case 1: return 40; break;
	case 2:	return 20; break;
	}
	return 40;
}

int manageSpeedMenu(int menuCursor)
{
	char key = _getch();
	switch (key)
	{
	case KEY_DOWN2: return moveCursorDown(menuCursor, OPTIONS_AMOUNT);
	case KEY_UP2: return moveCursorUp(menuCursor, OPTIONS_AMOUNT);
	case KEY_ACCEPT2: return -speedOption(menuCursor);	
	}
	key = tolower(key);
	switch (key)
	{
	case KEY_DOWN: return moveCursorDown(menuCursor, OPTIONS_AMOUNT);
	case KEY_UP: return moveCursorUp(menuCursor, OPTIONS_AMOUNT);
	case KEY_ACCEPT: return -speedOption(menuCursor);
	}
	return menuCursor;
}

void showSpeedMenu(int menuCursor)
{
	string choices[3] = { "Wolno   ", "Standardowo   ", "Szybko   "};
	string toPrint = "Wybierz predkosc rozgrywki\n";
	toPrint = "\n";
	for (int i = 0; i < 3; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	cout << toPrint;
}

int setGameSpeed()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "";
	do
	{
		showSpeedMenu(menuCursor);
		menuCursor = manageSpeedMenu(menuCursor);
		clear();
	} while (menuCursor >= 0);
	return -menuCursor;
}

bool changeGamemode()
{
	return !gamemode;
}



int changeOption(int menuCursor)
{
	switch (menuCursor)
	{
	case 0: width = setIntValue(); break;
	case 1: height = setIntValue(); break;
	case 2:	delay = setGameSpeed(); break;
	case 3: gamemode = changeGamemode(); break;
	case 4: exit(); return QUIT;
	}
	return 0;
}

int manageOptions(int menuCursor)
{
	char key = _getch();
	switch (key)
	{
	case KEY_DOWN2: return moveCursorDown(menuCursor, OPTIONS_AMOUNT);
	case KEY_UP2: return moveCursorUp(menuCursor, OPTIONS_AMOUNT);
	case KEY_ACCEPT2: return changeOption(menuCursor);
	}
	key = tolower(key);
	switch (key)
	{
	case KEY_DOWN: return moveCursorDown(menuCursor, OPTIONS_AMOUNT);
	case KEY_UP: return moveCursorUp(menuCursor, OPTIONS_AMOUNT);
	case KEY_ACCEPT: return changeOption(menuCursor);
	}
	return menuCursor;
}

string getGameSpeed()
{
	switch (delay)
	{
	case 80: return "Predkosc gry: wolna    "; break;
	case 40: return "Predkosc gry: standardowa    "; break;
	}
	return "Predkosc gry: szybka    ";
}

void showOptions(int menuCursor)
{
	string choices[OPTIONS_AMOUNT] = { "Szerokosc planszy:   " + to_string(width) + "    ", "Wysokosc planszy:   " + to_string(height) + "    ", " ", gamemode ? "Sciany wylaczone    " : "Sciany wlaczone    ", "Cofnij   " };
	choices[2] = getGameSpeed();
	string toPrint = "";
	toPrint = "\n";
	for (int i = 0; i < OPTIONS_AMOUNT; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	cout << toPrint;
}

void runOptions()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "";
	do
	{
		showOptions(menuCursor);
		menuCursor = manageOptions(menuCursor);
		clear();
	} while (menuCursor >= 0);
}

int useMenuOption(int i)
{
	switch (i)
	{
	case 0: createGame(); break;
	case 1: showHighscores(); break;
	case 2: runOptions(); break;
	case 3: exit(); return QUIT; break;
	}
	return 0;
}

void showMenu(int menuCursor)
{
	string choices[MAIN_MENU_OPTIONS_AMOUNT] = { "Rozpocznij Gre   ", "Najlepsze wyniki   ", "Opcje   ", "Wyjscie   " };
	string toPrint = "";
	toPrint = "\n";
	for (int i = 0; i < MAIN_MENU_OPTIONS_AMOUNT; i++)
	{
		toPrint += "  ";
		if (i == menuCursor)
		{
			toPrint += " >";
		}
		toPrint += choices[i];
		toPrint += "\n";
	}
	cout << toPrint;
}

int moveCursorDown(int menuCursor, int length)
{
	menuCursor == (length - 1) ? menuCursor = 0 : menuCursor++;
	return menuCursor;
}

int moveCursorUp(int menuCursor, int length)
{
	menuCursor == 0 ? menuCursor = (length - 1) : menuCursor--;
	return menuCursor;
}

int manageMenu(int menuCursor)
{

	char key = _getch();
	switch (key)
	{
	case KEY_DOWN2: return moveCursorDown(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case KEY_UP2: return moveCursorUp(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case KEY_ACCEPT2: return useMenuOption(menuCursor);
	}
	key = tolower(key);
	switch (key)
	{
	case KEY_DOWN: return moveCursorDown(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case KEY_UP: return moveCursorUp(menuCursor, MAIN_MENU_OPTIONS_AMOUNT);
	case KEY_ACCEPT: return useMenuOption(menuCursor);
	}
	return menuCursor;
}

void runMenu()
{
	system("cls");
	int menuCursor = 0;
	string toPrint = "";
	do
	{
		showMenu(menuCursor);
		menuCursor = manageMenu(menuCursor);
		clear();
	} while (menuCursor >= 0);
}

void saveScore(string nick, int score)
{
	fstream file;
	file.open("highscores.txt");
	int i = 0;
	string sNick;
	int sScore;
	string toSave = "";
	while (file >> sNick && file >> sScore)
	{
		if (score >= sScore)
		{
			toSave += nick + "\n" + to_string(score) + "\n";
			score = sScore;
			nick = sNick;
		}
		else
		{
			toSave += sNick + "\n" + to_string(sScore) + "\n";
		}
		i++;
	}
	if (i < 10)
	{
		toSave += nick + "\n" + to_string(score);
	}
	file.close();
	file.open("highscores.txt", fstream::out);
	file << toSave;
}

void showHighscores()
{
	system("cls");
	ifstream file;
	file.open("highscores.txt");
	int i = 1;
	string nick;
	int score;
	while (file >> nick && file >> score && i <= 10)
	{
		cout << i << " " << nick << "   " << score << endl;
		i++;
	}
	file.close();
	cout << endl << "   >Nacisnij przycisk";
	_getch();
	system("cls");
}

void mainMenu()
{
	setTerminalWindow();
	runMenu();
}