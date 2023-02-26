#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
#include<map>
#include<fstream>
#include<string>
#include<string.h>
#define _WIN32_WINNT 0x0502 
#include<Windows.h>
#include<stdlib.h>
#include<io.h>
#include<conio.h>
#include<exception>
#include<chrono>



using namespace std;
enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };

enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, esc = 27, space = 32 };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//получает дискриптор активного окна
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
void fontsize(int a, int b) {
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = a;
	lpConsoleCurrentFontEx->dwFontSize.Y = b;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void SetColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetCursor(int x, int y)
{
	COORD myCoords = { x,y };//инициализируем передаваемыми значениями объект координат
	SetConsoleCursorPosition(hStdOut, myCoords);
}
const int GlobalSizeAriaX = 150;// 104 - поле, 46 иформационное пространство
const int GlobalSizeAriaY = 52;// поле 26х26 кубиков
const int SizeTank = 6;
const int GlobalSizeBattle = 26 * 3;
const int otst = 6;


//структура формирования номера объекта
struct Number
{
	int Type = 0;// 0 пусто, 1 танк, 2 препятствие, 3 снаряд
	int Svoy = 0;//1 - свой, 2 - чужой, 3 - препятствие
	int Num = 0;//№ объекта
};
Number masAria[GlobalSizeBattle][GlobalSizeBattle] = {};


//функция преобразования моего направление(1-4) в enum Direction
int SwichDirection(int temp)
{
	int dir = 0;
	switch (temp)
	{
	case 1:
		dir = Left;
		break;
	case 2:
		dir = Right;
		break;
	case 3:
		dir = Up;
		break;
	case 4:
		dir = Down;
		break;
	}
	return dir;
}
//массив построения танка
void MasTankView(int mas[SizeTank][SizeTank], int Direction)
{
	if (Direction == 1)
	{
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				mas[i][j] = 0;
				mas[0][j] = 3;
				mas[SizeTank - 1][j] = 3;
				if ((i > 0 && i < SizeTank - 1) && (j > 0 && j < SizeTank - 1))
				{
					mas[i][j] = 1;
				}
				if ((j >= 0 && j < SizeTank - 2) && (i == 2 || i == 3))
				{
					mas[i][j] = 2;
				}
			}
		}
	}
	else if (Direction == 2)
	{
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				mas[i][j] = 0;
				mas[0][j] = 3;
				mas[SizeTank - 1][j] = 3;
				if ((i > 0 && i < SizeTank - 1) && (j > 0 && j < SizeTank - 1))
				{
					mas[i][j] = 1;
				}
				if ((j > 1 && j < SizeTank) && (i == 2 || i == 3))
				{
					mas[i][j] = 2;
				}
			}
		}
	}
	else if (Direction == 3)
	{
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				mas[i][j] = 0;
				mas[j][0] = 3;
				mas[j][SizeTank - 1] = 3;
				if ((i > 0 && i < SizeTank - 1) && (j > 0 && j < SizeTank - 1))
				{
					mas[i][j] = 1;
				}
				if ((i >= 0 && i < SizeTank - 2) && (j == 2 || j == 3))
				{
					mas[i][j] = 2;
				}
			}
		}
	}
	else if (Direction == 4)
	{
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				mas[i][j] = 0;
				mas[j][0] = 3;
				mas[j][SizeTank - 1] = 3;
				if ((i > 0 && i < SizeTank - 1) && (j > 0 && j < SizeTank - 1))
				{
					mas[i][j] = 1;
				}
				if ((i > 1 && i < SizeTank) && (j == 2 || j == 3))
				{
					mas[i][j] = 2;
				}
			}
		}
	}
}
//отрисовка танка
void tankImage(int mas[SizeTank][SizeTank], Color color, Color pushka, Color track, int X, int Y)
{
	int x = X + otst, y = Y + otst;

	for (size_t i = 0; i < SizeTank; i++)
	{
		SetCursor(x * 2, y + i);

		for (size_t j = 0; j < SizeTank; j++)
		{

			if (mas[i][j] == 1)//тело
			{
				SetColor(color, color);
			}
			else if (mas[i][j] == 2) //пушка
			{
				SetColor(pushka, pushka);
			}
			else if (mas[i][j] == 3)
			{
				SetColor(Black, track);// гусеницы
			}
			else
			{
				SetColor(Black, Black);
			}
			cout << "**"; SetColor(White, Black);
		}
		cout << endl;
		SetColor(White, Black);
	}
}
//проверка следующего шага движения
bool stepDirection(int X, int Y, int dir, int CountIter) {
	bool key = true;
	if (dir == 1 || dir == 2) {
		for (size_t i = 0; i < CountIter; i++)
		{
			if (masAria[Y + i][X].Type == 1 || masAria[Y + i][X].Type == 2)
			{
				key = false;
			}
		}
	}
	if (dir == 3 || dir == 4) {
		for (size_t i = 0; i < CountIter; i++)
		{
			if (masAria[Y][X + i].Type == 1 || masAria[Y][X + i].Type == 2)
			{
				key = false;
			}
		}
	}
	return key;
}

//описываем класс танк основной
class Tank
{
protected:
	int X;//Х левый верхний угол отрисовки танка 
	int Y;//У левый верхний угол отрисовки танка
	int Size;//Линейный размер танка
	int Direction;//Направление движения танка 1 - лево, 2 право, 3 верх, 4 низ
	int Defens;//Колличество поподаний до уничтожения 1 -минимальное значение
public:
	void SetX(int x)
	{
		X = x;
	}
	void SetY(int y)
	{
		Y = y;
	}
	void SetSize(int Size)
	{
		this->Size = Size;
	}
	void SetDirection(int Dir)
	{
		Direction = Dir;
	}
	void SetDefens(int Def)
	{
		Defens -= Def;
	}
	int getX()const
	{
		return X;
	}
	int getY()const
	{
		return Y;
	}
	int getDirection()const
	{
		return Direction;
	}
	int getDefens()const
	{
		return Defens;
	}
	Tank()
	{
		X = 0;
		Y = 0;
		Size = SizeTank;
		Direction = 3;
		Defens = 1;
	}
	~Tank()
	{
	}
};

//вид танка разных уровней
class ViewTank : public Tank
{
protected:
	Color color;
	int mas[SizeTank][SizeTank] = {};
	Number NumberTank;
	DWORD startTimeFire = 0;
	DWORD startTimeMove = 0;
	int pastDirection = Down; //последние направление
	int fire = 0;//1 - выстрел произведен и нельзя стрелять, 0 - выстрела нет и можно стрелять 
public:
	int getFire()const
	{
		return fire;
	}
	void setFire(int a)
	{
		fire = a;
	}
	void setPastDirection(int dir)
	{
		pastDirection = dir;
	}
	int getPastDirection()const
	{
		return pastDirection;
	}
	void SetStartTimeMove(DWORD time)
	{
		startTimeMove = time;
	}
	DWORD getTimeMove()const
	{
		return startTimeMove;
	}
	void SetStartTimeFire(DWORD time)
	{
		startTimeFire = time;
	}
	DWORD getTimeFire()const
	{
		return startTimeFire;
	}
	void setSvoy(int svoy)
	{
		NumberTank.Svoy = svoy;
	}
	void setNtanka(int N)
	{
		NumberTank.Num = N;
	}
	int getSvoy()const
	{
		return NumberTank.Svoy;
	}
	int getNtanka()const
	{
		return NumberTank.Num;
	}
	Number getNumber()const
	{
		return NumberTank;
	}
	void setColor(Color color)
	{
		this->color = color;
	}
	ViewTank()
	{
		NumberTank.Type = 1;
		color = Green;
		MasTankView(mas, Direction);
	}
	ViewTank(int x, int y, Color color, int Svoy, int Ntanka)
	{
		X = x;
		Y = y;
		this->color = color;
		NumberTank.Svoy = Svoy;
		NumberTank.Num = Ntanka;
		NumberTank.Type = 1;
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				masAria[Y + i][X + j] = NumberTank;
			}
		}
	}
	void ShowMas()
	{
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				cout << mas[i][j];
			}cout << endl;
		}
	}
	//отображение танка и предача информации в поле игры
	void Show()
	{
		MasTankView(mas, Direction);
		Color col;
		if (color == Red) col = LightRed;
		else col = LightGreen;
		tankImage(mas, color, col, Brown, X, Y);
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				masAria[Y + i][X + j] = NumberTank;
			}
		}
	}
	//закрашивание места танка в черный цвет
	void AntiShow()
	{
		tankImage(mas, Black, Black, Black, X, Y);
		for (size_t i = 0; i < SizeTank; i++)
		{
			for (size_t j = 0; j < SizeTank; j++)
			{
				masAria[Y + i][X + j].Type = 0;
			}
		}
	}
	~ViewTank()
	{
	};
};
int SetDirect(ViewTank& obj, int dir)
{
	int temp = dir;
	do
	{
		temp = 1 + rand() % 4;
	} while (temp == dir);
	obj.SetDirection(temp);
	return SwichDirection(temp);
}
//класс построения препятствий 
class relief : public Tank
{
	int Type;//1-кирпич,2 -бетон,3 - вода,4-лес,5 - лед 
	int prozrachnost;//1-пуля пролетает, 0 - пуля не пролетает
	Number numberRelife;
public:
	void SetNumber(int num)
	{
		numberRelife.Num = num;
	}
	void setType(int type)
	{
		Type = type;
		switch (type)
		{
		case 1:
			prozrachnost = 0;
			Defens = 1;
			break;
		case 2:
			prozrachnost = 0;
			Defens = 10;
			break;
		case 3:
			prozrachnost = 1;
			Defens = 10000;
			break;
		case 4:
			prozrachnost = 1;
			Defens = 10000;
		case 5:
			prozrachnost = 1;
			Defens = 10000;
		}
	}
	int getType()const
	{
		return Type;
	}
	int getProzrach()const
	{
		return prozrachnost;
	}
	int getDefens()const
	{
		return Defens;
	}
	int getX()const
	{
		return X;
	}
	int getY()const
	{
		return Y;
	}
	Number getNumber()const
	{
		return numberRelife;
	}
	void Show()
	{
		if (Type == 1 || Type == 2) {

			for (size_t i = 0; i < SizeTank / 2; i++)
			{
				SetCursor((X + otst) * 2, Y + otst + i);
				if (Type == 1)
				{
					SetColor(Black, Brown);
				}
				if (Type == 2)
				{
					SetColor(Black, LightGray);
				}
				if (i % 2 == 0)
				{
					cout << "[_____";
				}
				else if (i % 2 == 1)
				{
					cout << "___[__";
				}
				for (size_t j = 0; j < SizeTank / 2; j++)
				{

					masAria[Y + i][X + j] = numberRelife;
				}
				SetColor(Black, LightGray);
				cout << endl;
			}
		}

	}
	void AntiShow()
	{
		for (size_t i = 0; i < SizeTank / 2; i++)
		{
			SetCursor((X + otst) * 2, Y + otst + i);
			SetColor(Black, Black);
			cout << "[_____";//длинна текста должна быть SizeTank
			for (size_t j = 0; j < SizeTank / 2; j++)
			{

				masAria[Y + i][X + j].Type = 0;
			}
			SetColor(Black, LightGray);
			cout << endl;
		}

	}
	relief()
	{
		numberRelife.Type = 2;
		setType(1);
		X = 0;
		Y = 0;
		Size = 1;
	}
	~relief()
	{

	}
	relief(int x, int y, int type, int num)
	{
		numberRelife.Type = 2;
		numberRelife.Num = num;
		setType(type);
		X = x;
		Y = y;
		Size = 1;
	}
};

void fireImage(int direction, Color pushka, int X, int Y)
{
	int x = X + otst, y = Y + otst;

	SetCursor(x * 2, y);

	if (direction == 1 || direction == 2)
	{
		SetColor(pushka, pushka);
		cout << "**"; SetColor(White, Black);
		cout << endl;
		SetCursor(x * 2, y + 1);
		SetColor(pushka, pushka);
		cout << "**"; SetColor(White, Black);
	}
	else if (direction == 3 || direction == 4)
	{
		SetColor(pushka, pushka);
		cout << "****"; SetColor(White, Black);
	}

}
//класс взрыв
class Vzriv :public ViewTank
{
	int step;
public:
	void SetStep(int a)
	{
		step += a;
	}
	int getStep()const
	{
		return step;
	}
	void showVzriv()
	{
		MasTankView(mas, Direction);
		switch (step)
		{
		case 0:tankImage(mas, White, White, White, X, Y);
			break;
		case 1:tankImage(mas, Yellow, Yellow, Yellow, X, Y);
			break;
		case 2:tankImage(mas, Red, Red, Red, X, Y);
			break;
		case 3:tankImage(mas, Black, Black, Black, X, Y);
			break;
		}
	}

	Vzriv()
	{

	}
	Vzriv(int x, int y, int dir)
	{
		step = 0;
		X = x;
		Y = y;
		Direction = dir;
	}
	~Vzriv()
	{

	}
};
//класс выстрел

class Fire
{
	int X;
	int Y;
	int direction;// Направление движения танка 1 - лево, 2 право, 3 верх, 4 низ
	Number NumberTank;//К какому танку относится
	DWORD StartTime;
	int born;

public:
	void SetStartTime(DWORD time)
	{
		StartTime = time;
	}
	DWORD getTime()const
	{
		return StartTime;
	}
	void setDirection(int dir)
	{
		direction = dir;
	}
	void setNumber(int Num)
	{
		NumberTank.Num = Num;
	}
	void setX(int x)
	{
		X = x;
	}
	void ChekFire(Number& mas1, Number& mas2)
	{
		if (direction == 3 || direction == 4)
		{
			mas1 = masAria[Y][X];
			mas2 = masAria[Y][X + 1];
		}
		if (direction == 1 || direction == 2)
		{
			mas1 = masAria[Y][X];
			mas2 = masAria[Y + 1][X];
		}
	}
	void setY(int y)
	{
		Y = y;
	}
	int getDirect()const
	{
		return direction;
	}
	Number getNumber()const
	{
		return NumberTank;
	}
	int getX()const
	{
		return X;
	}
	int getY()const
	{
		return Y;
	}
	Fire(ViewTank& obj)
	{
		StartTime = GetTickCount();
		NumberTank.Type = 3;
		NumberTank.Num = obj.getNtanka();
		NumberTank.Svoy = obj.getSvoy();
		direction = obj.getDirection();
		switch (obj.getDirection())
		{
		case 1:
			X = obj.getX();
			Y = obj.getY() + SizeTank / 2 - 1;
			break;
		case 2:
			X = obj.getX() + SizeTank - 1;
			Y = obj.getY() + SizeTank / 2 - 1;
			break;
		case 3:
			X = obj.getX() + SizeTank / 2 - 1;
			Y = obj.getY();
			break;
		case 4:
			X = obj.getX() + SizeTank / 2 - 1;
			Y = obj.getY() + SizeTank / 2 - 1;
			break;
		}
	}
	Fire()
	{
		X = 0;
		Y = 0;
		direction = 0;// Направление движения танка 1 - лево, 2 право, 3 верх, 4 низ
		NumberTank.Num = 0;//К какому танку относится
		NumberTank.Svoy = 0;
		NumberTank.Type = 3;
	}
	void Show()
	{
		fireImage(direction, LightGray, X, Y);
		int y = Y, x = X;

		switch (direction)
		{
		case 1:
			masAria[y][x] = NumberTank;
			masAria[y + 1][x] = NumberTank;
			break;
		case 2:
			masAria[y][x] = NumberTank;
			masAria[y + 1][x] = NumberTank;
			break;
		case 3:
			masAria[y][x] = NumberTank;
			masAria[y][x + 1] = NumberTank;
			break;
		case 4:
			masAria[y][x] = NumberTank;
			masAria[y][x + 1] = NumberTank;
			break;
		}
	}
	//закрашивание места танка в черный цвет
	void AntiShow()
	{
		fireImage(direction, Black, X, Y);
		int y = Y, x = X;
		Number temp;
		switch (direction)
		{
		case 1:
			masAria[y][x] = temp;
			masAria[y + 1][x] = temp;
			break;
		case 2:
			masAria[y][x] = temp;
			masAria[y + 1][x] = temp;
			break;
		case 3:
			masAria[y][x] = temp;
			masAria[y][x + 1] = temp;
			break;
		case 4:
			masAria[y][x] = temp;
			masAria[y][x + 1] = temp;
			break;
		}
	}
	~Fire()
	{
		AntiShow();
	}
};
void moveShut(Fire& obj)
{
	obj.AntiShow();
	switch (obj.getDirect())
	{
	case 1:
		obj.setX(obj.getX() - 1);
		break;
	case 2:
		obj.setX(obj.getX() + 1);
		break;
	case 3:
		obj.setY(obj.getY() - 1);
		break;
	case 4:
		obj.setY(obj.getY() + 1);
		break;
	}
}
int Nextstep(ViewTank& obj, int dir)
{
	int nextstep;
	obj.AntiShow();
	nextstep = SetDirect(obj, dir);
	obj.Show();
	obj.setPastDirection(nextstep);
	return nextstep;
}

//функция движения танка
int Move(int dir, ViewTank& obj)
{
	int nextstep = 0;
	obj.AntiShow();
	obj.setPastDirection(dir);
	switch (obj.getPastDirection())
	{
	case Up:
		if (obj.getDirection() == 3)
		{
			if (obj.getY() > 0)
			{
				if (stepDirection(obj.getX(), obj.getY() - 1, 3, SizeTank))
				{
					obj.AntiShow();
					obj.SetY(obj.getY() - 1);
					obj.Show();
					obj.setPastDirection(Up);
					return Up;
				}
				else if (obj.getSvoy() == 2)
				{
					return Nextstep(obj, 3);
				}
			}
			else if (obj.getY() == 0 && obj.getSvoy() == 2)
			{
				return Nextstep(obj, 3);
			}
		}
		else obj.SetDirection(3);
		obj.setPastDirection(Up);
		obj.Show();
		return Up;
		break;
	case Left:
		if (obj.getDirection() == 1)
		{
			if (obj.getX() > 0)
			{
				if (stepDirection(obj.getX() - 1, obj.getY(), 1, SizeTank))
				{
					obj.AntiShow();
					obj.SetX(obj.getX() - 1);
					obj.Show();
					obj.setPastDirection(Left);
					return Left;
				}
				else if (obj.getSvoy() == 2)
				{
					return Nextstep(obj, 1);
				}
			}
			else if (obj.getX() == 0 && obj.getSvoy() == 2)
			{
				return Nextstep(obj, 1);
			}
		}
		else obj.SetDirection(1);
		obj.setPastDirection(Left);
		obj.Show();
		return Left;
		break;
	case Right:
		if (obj.getDirection() == 2)
		{
			if (obj.getX() < GlobalSizeBattle - SizeTank)
			{
				if (stepDirection(obj.getX() + SizeTank, obj.getY(), 2, SizeTank))
				{
					obj.AntiShow();
					obj.SetX(obj.getX() + 1);
					obj.Show();
					obj.setPastDirection(Right);
					return Right;
				}
				else if (obj.getSvoy() == 2)
				{
					return Nextstep(obj, 2);
				}
			}
			else if (obj.getX() == GlobalSizeBattle - SizeTank && obj.getSvoy() == 2)
			{
				return Nextstep(obj, 2);
			}
		}
		else obj.SetDirection(2);
		obj.setPastDirection(Right);
		obj.Show();
		return Right;
		break;
	case Down:
		if (obj.getDirection() == 4)
		{
			if (obj.getY() < GlobalSizeBattle - SizeTank)
			{
				if (stepDirection(obj.getX(), obj.getY() + SizeTank, 4, SizeTank))
				{
					obj.AntiShow();
					obj.SetY(obj.getY() + 1);
					obj.Show();
					obj.setPastDirection(Down);
					return Down;
				}
				else if (obj.getSvoy() == 2)
				{
					return Nextstep(obj, 2);
				}
			}
			else if (obj.getY() == GlobalSizeBattle - SizeTank && obj.getSvoy() == 2)
			{
				return Nextstep(obj, 4);
			}
		}
		else obj.SetDirection(4);
		obj.setPastDirection(Down);
		obj.Show();
		return Down;
		break;
	case 13://Enter
		break;
	}
	//return obj;
}
//задание случайного направления движения
int Rand()
{
	int dir = 0, temp = 0;
	temp = 1 + rand() % 4;
	return SwichDirection(temp);
}
int MoveEnemy(int time, int Count, int dir)
{
	int temp = dir;
	if (Count % 20 == 0)
	{
		dir = Rand();
	}
	return dir;
}
//найти танк из вектора танков
int FindTank(vector<ViewTank>& enemy, Number fire)
{
	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i).getNtanka() == fire.Num)
		{
			return i;
		}
	}return -1;
}
//найти выстрел
int FindFire(vector<Fire>& enemy, Number tank)
{
	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy.at(i).getNumber().Num == tank.Num)
		{
			return i;
		}
	}return -1;
}
int  FindTankAndKill(Number shut, Number number, vector <ViewTank>& Enemy, vector<relief>& karta,
	vector<Fire>& enemyShut, vector<Vzriv>& vzriv)
{
	int a = 0;
	if (number.Svoy == shut.Svoy)
	{
		return 0;
	}
	else if (number.Svoy == 1 && number.Num == 0 && number.Type == 1)
	{
		return 2;
	}
	else
	{
		if (number.Type == 1)
		{
			a = FindTank(Enemy, number);
			Enemy.at(a).SetDefens(1);
			if (Enemy.at(a).getDefens() == 0)
			{
				Enemy.at(a).AntiShow();
				Vzriv dest(Enemy.at(a).getX(), Enemy.at(a).getY(), Enemy.at(a).getDirection());
				vzriv.push_back(dest);
				Enemy.erase(Enemy.begin() + a);
			}
		}
		if (number.Type == 2)
		{
			for (size_t i = 0; i < karta.size(); i++)
			{
				if (karta.at(i).getNumber().Num == number.Num)
				{
					karta.at(i).SetDefens(1);
					if (karta.at(i).getDefens() > 0)
					{
						karta.at(i).Show();
					}
					if (karta.at(i).getDefens() == 0)
					{
						karta.at(i).AntiShow();
						karta.erase(karta.begin() + i);
					}
					break;
				}
			}
		}
		if (number.Type == 3)
		{
			for (size_t i = 0; i < enemyShut.size(); i++)
			{
				if (enemyShut.at(i).getNumber().Num == number.Num)
				{
					a = FindTank(Enemy, enemyShut.at(i).getNumber());
					if (a >= 0)
					{
						Enemy.at(a).setFire(0);
					}
					enemyShut.at(i).AntiShow();
					enemyShut.erase(enemyShut.begin() + i);
					break;
				}
			}
		}
	}
	return 1;
}
//Функция обработки поражения

int MoveOrKill(Fire& fire, vector <ViewTank>& Enemy, vector<relief>& karta, vector<Fire>& enemyShut, vector<Vzriv>& vzriv)
{
	int key = 0;
	Number shut1, shut2;
	moveShut(fire);
	fire.ChekFire(shut1, shut2);
	if (shut1.Type == 0 && shut2.Type == 0)
	{
		fire.Show();
	}
	else if (shut1.Type != 0 || shut2.Type != 0)
	{
		if (shut1.Num == shut2.Num)
		{
			key = FindTankAndKill(fire.getNumber(), shut1, Enemy, karta, enemyShut, vzriv);
		}
		else if (shut1.Num != shut2.Num) {
			if (shut1.Num != 0)
			{
				key = FindTankAndKill(fire.getNumber(), shut1, Enemy, karta, enemyShut, vzriv);
			}
			if (shut2.Num != 0)
			{
				key = FindTankAndKill(fire.getNumber(), shut2, Enemy, karta, enemyShut, vzriv);
			}
		}
	}return key;
}

//Функция движения и отработки поражения
//возвращает 0 если промах и 1 если произошло соударение
int FireShut(Fire& fire, vector <ViewTank>& Enemy, vector<relief>& karta, vector<Fire>& enemyShut, vector<Vzriv>& vzriv)
{

	int key = 0;
	switch (fire.getDirect())
	{
	case 1:
		if (fire.getX() > 0)
		{
			key = MoveOrKill(fire, Enemy, karta, enemyShut, vzriv);
			return key;
		}
		else if (fire.getX() == 0)
		{
			return 1;
		}
		break;
	case 2:
		if (fire.getX() < GlobalSizeBattle - 1)
		{
			key = MoveOrKill(fire, Enemy, karta, enemyShut, vzriv);
			return key;
		}
		else if (fire.getX() == GlobalSizeBattle - 1)
		{
			return 1;
		}
		break;
	case 3:
		if (fire.getY() > 0)
		{
			key = MoveOrKill(fire, Enemy, karta, enemyShut, vzriv);
			return key;
		}
		else if (fire.getY() == 0)
		{
			return 1;
		}
		break;
	case 4:
		if (fire.getY() < GlobalSizeBattle - 1)
		{
			key = MoveOrKill(fire, Enemy, karta, enemyShut, vzriv);
			return key;
		}
		else if (fire.getY() == GlobalSizeBattle - 1)
		{
			return 1;
		}
		break;
	}
}
//отображение массива игрового поля
void Showmas(Number mas[GlobalSizeBattle][GlobalSizeBattle])
{
	for (size_t i = 0; i < GlobalSizeBattle; i++)
	{
		for (size_t j = 0; j < GlobalSizeBattle; j++)
		{
			cout << mas[i][j].Type << "  ";
		}cout << endl;
	}
}
//функция заполнения вектора поля боя из string
void SetButtleAria(vector<relief>& karta, string mas1[GlobalSizeBattle])
{
	int number = 0;
	char* maschar[GlobalSizeBattle];
	for (size_t i = 0; i < GlobalSizeBattle; i++)
	{
		maschar[i] = new char[mas1[i].size() + 1];
		strcpy_s(maschar[i], mas1[i].size() + 1, mas1[i].c_str());
	}
	for (size_t i = 0; i < GlobalSizeBattle; i++)
	{
		for (size_t j = 0; j < mas1[i].size(); j++)
		{
			if (maschar[i][j] != '0')
			{
				relief Wall(j * SizeTank / 2, i * SizeTank / 2, (int)maschar[i][j] - '0', number);
				karta.push_back(Wall);
				number++;
			}
		}
	}
}
//функция отрисовки меню
void showMenu(string mas[], int row, int punct, Color colorText, Color colorBack)
{
	for (size_t i = 0; i < row; i++)
	{
		SetCursor(33, 18 + i * 2);
		if (i == punct)
		{
			SetColor(colorText, colorBack);
		}
		else
			SetColor(Black, colorBack);
		cout << mas[i];
	}
	SetColor(White, Black);
}
//Функция выделения пункта меню
int ActivePunct(string mas[], int row, Color colorText, Color colorBack)
{
	int number = 0, key;
	do
	{
		showMenu(mas, row, number, colorText, colorBack);
		key = _getch();
		switch (key)
		{
		case Up:
			if (number >= 1)
				number--;
			break;
		case Down:
			if (number < row)
				number++;
			break;
		case Enter:
			return number;
			break;
		}
	} while (key != esc);
	return -1;
}
//логотип
void Logo()
{
	SetCursor(0, 0);
	SetColor(Brown, LightGray);
	cout << "XXXXXXXX         XXXXXX     XXXXXXXXXXXX  XXXXXXXXXXXX  XXXX          XXXXXXXXXXXX\n";
	cout << "XXXXXXXXXX      XXXXXXXX    XXXXXXXXXXXX  XXXXXXXXXXXX  XXXX          XXXXXXXXXXXX\n";
	cout << "XXXX  XXXX     XXXX  XXXX       XXXX          XXXX      XXXX          XXXX\n";
	cout << "XXXXXXXXX     XXXXXXXXXXXX      XXXX          XXXX      XXXX          XXXXXXXX\n";
	cout << "XXXXXXXXXX    XXXXXXXXXXXX      XXXX          XXXX      XXXX          XXXXXXXX\n";
	cout << "XXXX    XXXX  XXXX    XXXX      XXXX          XXXX      XXXX          XXXX\n";
	cout << "XXXXXXXXXXXX  XXXX    XXXX      XXXX          XXXX      XXXXXXXXXXXX  XXXXXXXXXXXX\n";
	cout << "XXXXXXXXXX    XXXX    XXXX      XXXX          XXXX      XXXXXXXXXXXX  XXXXXXXXXXXX\n\n";
	cout << "        XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXXXX  XXXX          XXXXXXXXXX  \n";
	cout << "        XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXXXX  XXXX          XXXXXXXXXXXX\n";
	cout << "        XXXX              XXXX      XXXX          XXXX          XXXX    XXXX\n";
	cout << "        XXXXXXXXXX        XXXX      XXXXXXXX      XXXX          XXXX    XXXX\n";
	cout << "        XXXXXXXXXX        XXXX      XXXXXXXX      XXXX          XXXX    XXXX\n";
	cout << "        XXXX              XXXX      XXXX          XXXX          XXXX    XXXX\n";
	cout << "        XXXX          XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXXXX\n";
	cout << "        XXXX          XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXXXX  XXXXXXXXXX\n";
	SetColor(Black, LightGray);
}
void LogoFinish(Color color)
{
	int X = 20;
	int Y = 10;
	for (size_t i = 0; i < 14; i++)
	{
		SetCursor(X, Y + i);
		for (size_t i = 0; i < 38; i++)
		{
			SetColor(color, color);
			cout << "1";
		}
		SetColor(Black, LightGray);
	}
}

//структура для сохранения в файл
struct User
{
	string name;
	string password;
	int Score;
	int CountGame;
	int StatusGame;//0 - не сохранена, 1 - сохранена
	vector <ViewTank> Enemy;//вектор танков противника
	vector <ViewTank> Self;//вектор своих танков
	vector <relief> Karta;//вектор рельефа(препятствий)
	vector <Fire> EnemyFire;//
	vector <Fire> SelfFire;//
	vector <int> enemydir;//вектор предидущих состояний
	vector <Vzriv> vzriv;//вектор взрывов
};

//
class tankEmeg :public Tank
{
	Color color;
	int mas[3][6] = { {1,1,0,0,1,1},{1,1,1,1,1,1},{1,1,0,0,1,1} };
public:
	tankEmeg()
	{
	
	};
	tankEmeg(int x, int y, Color col)
	{
		X = x;
		Y = y;
		color = col;
	};
	void show()
	{
		for (size_t i = 0; i < 3; i++)
		{
			SetCursor(X, Y + i);
			for (size_t j = 0; j < 6; j++)
			{
				if (mas[i][j]==0)
				{
					SetColor(Black, Black);
				}
				if (mas[i][j] == 1)
				{
					SetColor(color, color);
				}
				cout << "*";
			}
			SetColor(Black, LightGray);
			cout << endl;			
		}
	}
	void AntiShow()
	{
		for (size_t i = 0; i < 3; i++)
		{
			SetCursor(X, Y + i);
			for (size_t j = 0; j < 6; j++)
			{
				SetColor(LightGray, LightGray);
				cout << "*";
			}
			SetColor(Black, LightGray);
			cout << endl;
		}
	}
	~tankEmeg()
	{};
};


int main()
{
	srand(time(NULL));
	//system("color 70");//7-цвет background, 0 - цвет шрифта
	HWND hWindowConsole = GetConsoleWindow();
	fontsize(10, 18);//задание размера шрифта
	//доступные размеры с размерами окна
	//4:6
	//6 : 8
	//8 : 8
	//16 : 8
	//5 : 12
	//7 : 12
	//16 : 12		
	//12 : 16
	//10 : 18
	if (hWindowConsole != NULL)//задание размера окна
	{
		MoveWindow(hWindowConsole, 150, 20, 760, 630, TRUE);
	}
	system("color 70");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	// ------------------------------авторизация-----------------------------------------------------

	User user;
	string log, pas, temp, score, countGame;
	ifstream potokR("users.txt");
	ofstream potokW;
	bool key = true;
	bool start = true;
	string menuLogin[3] = { "Зарегистрироваться" ,"Авторизироваться","Выйти" };
	string point;
	int punktLogin = 0;
	hidecursor();

	do {
		punktLogin = 0;
		Logo();
		punktLogin = ActivePunct(menuLogin, 3, Red, LightGray);
		if (punktLogin == 0)
		{
			system("cls");
			system("color 70");
			Logo();
			if (hWindowConsole != NULL)
			{
				MoveWindow(hWindowConsole, 150, 20, 750, 630, TRUE);
			}
			potokR.close();
			potokR.open("users.txt");
			if (potokR.is_open())
			{
				SetCursor(30, 18);
				cout << "Введите логин:  ";
				getline(cin, user.name);
				while (!potokR.eof())
				{
					potokR >> log >> pas >> score >> countGame;
					if (log.compare(user.name) == 0)
					{
						SetCursor(30, 20);
						cout << "Такой пользователь уже есть.";
						key = false;
						Sleep(1000);
						system("cls");
						system("color 70");
						break;
					}
				}potokR.close();
				if (key)
				{
					SetCursor(30, 20);
					cout << "Введите пароль: ";
					getline(cin, user.password);
					potokR.close();
					potokW.open("users.txt", ios::app);
					key = true;
					user.Score = 0;
					user.CountGame = 0;
					potokW << endl << user.name << " " << user.password << " " << user.Score << " " << user.CountGame;
					SetCursor(30, 22);
					cout << "Вы успешно зарегистрировались.";
					start = false;
					Sleep(1000);
					system("cls");					
					potokW.close();
					break;
				}
			}
			potokR.close();
			if(potokW.is_open())
			potokW.close();
		}
		else if (punktLogin == 1)
		{
			system("cls");
			system("color 70");
			Logo();
			potokR.close();
			potokR.open("users.txt");
			if (potokR.is_open())
			{
				bool key1 = true;
				do
				{
					SetCursor(30, 18);
					cout << "Введите логин:  ";
					getline(cin, user.name);
					while (!potokR.eof())
					{
						key = false;
						potokR >> log >> pas >> score >> countGame;
						if (log.compare(user.name) == 0)
						{
							key = true;
							do
							{
								SetCursor(30, 20);
								cout << "Введите пароль: ";
								getline(cin, user.password);
								if (pas.compare(user.password) == 0)
								{
									SetCursor(30, 22);
									cout << "Вы успешно авторизовались.";
									user.Score = atoi(score.c_str());
									user.CountGame = atoi(countGame.c_str());
									start = false;
									Sleep(1000);
									system("cls");
									key1 = false;
									potokR.close();
									break;
								}
								else
								{
									SetCursor(30, 22);
									cout << "Пароль неверный!";
									Sleep(1000);
									SetCursor(46, 20);
									cout << "                              ";
									SetCursor(30, 22);
									cout << "                ";
								}
							} while (start);
							if (!start)break;
						}
					}
					if (!key)
					{
						SetCursor(10, 22);
						cout << "Такого пользователя не существует, зарегистрируйтесь.";
						Sleep(1000);
						system("cls");
						system("color 70");
						break;
					}
				} while (key1); 
				if (!start)break;
			}
			else
			{
				cout << "Пользователей не найдено, зарегистрируйтесь.";
				Sleep(1000);
				system("cls");
				system("color 70");
			}
		}
		else
		{
			system("cls");
			system("color 70");
			return -1;
		}
	} while (start);

	// -----------------------------Игра------------------------------------------------------
	
	bool restart = false;
	do {
		system("cls");
		fontsize(4, 6);
		
			MoveWindow(hWindowConsole, 150, 20, 760, 630, TRUE);
		system("color 70");
		hidecursor();//скрыть мигающий курсор
		user.CountGame++;
		vector <tankEmeg> SelfTank;
		vector <tankEmeg> EnemyTank;
		vector <ViewTank> Enemy;//вектор танков противника
		vector <ViewTank> Self;//вектор своих танков
		vector <relief> Karta;//вектор рельефа(препятствий)
		vector <Fire> EnemyFire;//
		vector <Fire> SelfFire;//
		vector <int> enemydir;//вектор предидущих состояний
		vector <Vzriv> vzriv;//вектор взрывов
		DWORD StartTimeFireEnemy = GetTickCount();
		DWORD StartTimeFire = GetTickCount();
		DWORD StopTimeFire = GetTickCount();
		DWORD StartTime = GetTickCount();
		DWORD StopTime = GetTickCount();
		int TimeDelayShut = 1000;//задержка перед очередным выстрелом
		int TimeMoveShut = 20;// скорость снарядов
		int TimeMoveTank = 100;//скорость танков
		Number shut[2] = {};//массив для проверки выстрелов
		int Count = 0;
		bool result = false;//результат игры
		int CountTotal = 20;//колличество танков противника
		int CountTotalSelf = 3;//колличество собственных жизней
		int CountTotalEnemy = 20;//оставшееся колличество танков противника
		int keyforfind = 0;//статус проверки поля боя
		int keyFire = 0;//статус выстрела
		bool keyfalse = true;//результат игры
		string mas1[GlobalSizeBattle] =
		{
			"00000000000000000000000000",
			"00000000000000000000000000",
			"00001111000111100011110000",
			"00001111000111100011110000",
			"00001111111111111111110000",
			"00001111111111111111110000",
			"00000000000000000000000000",
			"00000000000000000000000000",
			"11111111111000011111111111",
			"11111111111000011111111111",
			"11001111000000000111100011",
			"11001111000000000111100011",
			"11001111000000000111100011",
			"11001111000111100111100011",
			"11000000000111100000000011",
			"11000000000111100000000011",
			"11111111222222221111111111",
			"11111111222222221111111111",
			"00000000000000000000000000",
			"00000000000000000000000000",
			"00011110000000000001111000",
			"00011110000000000001111000",
			"00011110001111110001111000",
			"00000000001111110000000000",
			"00000000001100110000000000",
			"00000000001100110000000000"
		};
		//закрашивание квадрата поля		
		for (size_t i = 0; i < GlobalSizeBattle; i++) {
			SetCursor(otst * 2, otst + i);
			for (size_t i = 0; i < GlobalSizeBattle * 2; i++)
			{
				SetColor(Black, Black);
				cout << "+";
				SetColor(Black, LightGray);
			}cout << endl;
		}
		//установка препятствий
		SetButtleAria(Karta, mas1);
		for (size_t i = 0; i < Karta.size(); i++)
		{
			Karta.at(i).Show();
		}
		//создание своих танков
		ViewTank obj1((GlobalSizeBattle / 2 - SizeTank / 2), GlobalSizeBattle - SizeTank, Green, 1, 0);
		obj1.Show();
		Self.push_back(obj1);
		ViewTank obj(GlobalSizeBattle / 2 - SizeTank*2- SizeTank / 2, GlobalSizeBattle - SizeTank, Green, 1, 1);
		obj.Show();
		Self.push_back(obj);
		//создание танков противника
		for (size_t i = 0; i < 1; i++)
		{
			Sleep(10);
			ViewTank enemy((rand() % 3) * (GlobalSizeBattle - SizeTank) / 2, 0, Red, 2, i);
			enemy.SetDirection(4);
			enemy.setNtanka(i);
			Enemy.push_back(enemy);
			Enemy.at(i).Show();
			enemydir.push_back(Down);
			CountTotal--;
		}
		//заполнение вектора жизней своих
		for (size_t i = 0; i < CountTotalSelf; i++)
		{
			tankEmeg a(195, 3 + i * 4, Green);
			a.show();
			SelfTank.push_back(a);

		}
		//заполнение вектора жизней противника
		for (size_t i = 0; i < CountTotal-2; i++)
		{
			tankEmeg a(180, 3 + i * 4, Red);
			a.show();
			EnemyTank.push_back(a);
		}
				
		//тело игры
		Sleep(1000);
		do
		{
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				_getch();
			}

			if (CountTotal < EnemyTank.size())
			{
				EnemyTank.at(EnemyTank.size() - 1).AntiShow();
				EnemyTank.erase(EnemyTank.end()-1);
			}
			if (CountTotalSelf < SelfTank.size())
			{
				SelfTank.at(SelfTank.size() - 1).AntiShow();
				SelfTank.erase(SelfTank.end() - 1);
			}
			//отображение взрыва
			if (vzriv.size() != 0)
			{
				for (size_t i = 0; i < vzriv.size(); i++)
				{
					StopTime = GetTickCount();
					if ((StopTime - vzriv.at(i).getTimeMove()) > TimeMoveTank)
					{
						vzriv.at(i).showVzriv();
						vzriv.at(i).SetStep(1);
						vzriv.at(i).SetStartTimeMove(StopTime);
						if (vzriv.at(i).getStep() > 3)
						{
							vzriv.erase(vzriv.begin() + i);
						}
					}
				}
			}
			//стирание запасных танков своих
			for (size_t i = 0; i < Self.size(); i++)
			{
				Self.at(i).Show();
			}
			//стирание запасных танков противника
			for (size_t i = 0; i < Enemy.size(); i++)
			{
				Enemy.at(i).Show();
			}
			//движения снарядов противника
			if (EnemyFire.size() != 0)
			{
				for (size_t i = 0; i < EnemyFire.size(); i++)
				{
					StopTime = GetTickCount();
					keyFire = 0;
					if ((StopTime - EnemyFire.at(i).getTime()) > TimeMoveShut)
					{
						keyFire = FireShut(EnemyFire.at(i), Self, Karta, SelfFire, vzriv);
						keyforfind = FindTank(Enemy, EnemyFire.at(i).getNumber());
						if (keyforfind >= 0)
						{
							Enemy.at(keyforfind).Show();
						}
						EnemyFire.at(i).SetStartTime(StopTime);
					}
					if (keyFire == 1)
					{
						if (keyforfind >= 0)
						{
							Enemy.at(keyforfind).setFire(0);
						}
						EnemyFire.at(i).AntiShow();
						EnemyFire.erase(EnemyFire.begin() + i);
					}
					if (keyFire == 2)
					{
						keyfalse = false;
						result = false;
					}
				}
			}
			//Движение своего танка
			if (Self.size() > 1)
			{
				StopTime = GetTickCount();
				Self.at(1).Show();
				if ((StopTime - Self.at(1).getTimeMove()) > TimeMoveTank)
				{
					if (GetAsyncKeyState(VK_LEFT) & 0x8000)
					{
						Move(Left, Self.at(1));
					}
					if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
					{
						Move(Right, Self.at(1));
					}
					if (GetAsyncKeyState(VK_UP) & 0x8000)
					{
						Move(Up, Self.at(1));
					}
					if (GetAsyncKeyState(VK_DOWN) & 0x8000)
					{
						Move(Down, Self.at(1));
					}
					Self.at(1).SetStartTimeMove(StopTime);
				}
			}
			//добовление танка противника если он их колличество меньше назначенного
			if (Enemy.size() < 3 && CountTotal > 0)
			{
				ViewTank enemy((rand() % 3)* (GlobalSizeBattle - SizeTank) / 2, 1, Red, 2, 20 - CountTotal);
				enemy.SetDirection(4);
				Enemy.push_back(enemy);
				enemydir.push_back(Down);
				CountTotal--;
			}
			//добовление танка если он их колличество меньше назначенного
			if (Self.size() < 2 && CountTotalSelf > 0)
			{
				ViewTank self((GlobalSizeBattle / 2 - SizeTank * 2 - SizeTank / 2), GlobalSizeBattle - SizeTank, Green, 1, 20 - CountTotalSelf);
				self.SetDirection(3);
				self.Show();
				Self.push_back(self);
				Self.at(Self.size() - 1).Show();
				CountTotalSelf--;
			}
			//движение танков противника
			for (size_t i = 0; i < Enemy.size(); i++)
			{
				StopTime = GetTickCount();
				if ((StopTime - Enemy.at(i).getTimeMove()) > TimeMoveTank)
				{
					Move(Enemy.at(i).getPastDirection(), Enemy.at(i));
					Enemy.at(i).SetStartTimeMove(StopTime);
				}
			}
			//движение собственных снарядов
			if (SelfFire.size() != 0)
			{
				StopTime = GetTickCount();
				int key = 0;
				if ((StopTime - SelfFire.at(0).getTime()) > TimeMoveShut)
				{
					key = FireShut(SelfFire.at(0), Enemy, Karta, EnemyFire, vzriv);
					Self.at(1).Show();
					SelfFire.at(0).SetStartTime(StopTime);
				}
				if (key == 1)
				{
					SelfFire.erase(begin(SelfFire));
					Self.at(1).Show();
				}
			}
			//стрельба собственными снарядами
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				if (SelfFire.size() == 0)
				{
					StopTimeFire = GetTickCount();
					if ((StopTimeFire - StartTimeFire) > TimeDelayShut) {
						Fire shut(Self.at(1));
						SelfFire.push_back(shut);
						SelfFire.at(0).Show();
						Self.at(1).Show();
						StartTimeFire = StopTimeFire;
					}
				}
			}
			//стрельба противника
			if (Enemy.size() != 0)
			{
				for (size_t i = 0; i < Enemy.size(); i++)
				{
					StopTimeFire = GetTickCount();
					if (Enemy.at(i).getFire() == 0 && (StopTimeFire - Enemy.at(i).getTimeFire()) > TimeDelayShut)
					{
						Fire Shut(Enemy.at(i));
						Enemy.at(i).setFire(1);
						EnemyFire.push_back(Shut);
						Enemy.at(i).Show();
						Enemy.at(i).SetStartTimeFire(StopTimeFire);
					}
				}
			}
			Count++;

			if (Count > 20) Count = 0;
			if (Enemy.size() == 0)
			{
				result = true;
				keyfalse = false;
			}
			if (Self.size() == 1)
			{
				result = false;
				keyfalse = false;
			}
		} while (keyfalse);
		
		//результат игры
		system("cls");
		system("color 70");
		fontsize(10, 18);//задание размера шрифта
		MoveWindow(hWindowConsole, 150, 20, 760, 630, TRUE);
		hidecursor();
		Color color;
			if (result)
			{
				color = LightGreen;
				LogoFinish(color);
				SetColor(Black, color);
				SetCursor(30, 13);
				cout << "Вы выиграли!!!";
				SetCursor(32, 14);
				cout << "+100 очков!";
				user.Score += 100;
				SetCursor(32, 15);
				cout << "Ваш счет: " << user.Score;
			}
			else
			{
				color = LightRed;
				LogoFinish(color);
				SetColor(Black, color);
				SetCursor(30, 13);
				cout << "Вы проиграли!!!";
				SetCursor(30, 15);
				cout << "Ваш счет: " << user.Score;
			}
			string menuRestart[2] = { "Новая игра" ,"Выйти" };
			string point;
			int punktLogin = 0;
			punktLogin = ActivePunct(menuRestart, 2, Magenta, color);
			if (punktLogin == 0)
			{
				restart = true;
			}
			else restart = false;
	} while (restart);

	vector<User> temp1;
	User u;
	int i = 0;
	potokR.close();
	potokR.open("users.txt");
	while (!potokR.eof())
	{
		string log1, pas1, score1, countGame1;
		potokR >> log1 >> pas1 >> score1 >> countGame1;
		u.name = log1;
		u.password = pas1;
		u.Score = atoi(score1.c_str());
		u.CountGame = atoi(countGame1.c_str());
		temp1.push_back(u);
	}
	potokR.close();
	for (size_t i = 0; i < temp1.size(); i++)
	{
		if (temp1.at(i).name == user.name)
		{
			temp1.at(i).Score = user.Score;
			temp1.at(i).CountGame = user.CountGame;
		}
	}
	potokW.open("users.txt");
	for (size_t i = 0; i < temp1.size(); i++)
	{
		potokW << endl << temp1.at(i).name << " " << temp1.at(i).password << " " << temp1.at(i).Score<<" " << temp1.at(i).CountGame;
	}
return -1;
}