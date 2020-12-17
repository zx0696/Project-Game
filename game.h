#pragma once
#include "user.h"
#include "obstacle.h"
#include "item.h"
#include <time.h>

#define WIDTH 40        //����
#define HEIGHT 25        //����
#define TRUE 1
#define FALSE 0

#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 


class Game {
private:
	//�ʵ� ���� �κ�
	Obstacle* obs[WIDTH];
	User* user;
	Item* item;
	int count; //���� ����
	int round; //���� ����
	int per; //������ Ȯ�� ����
	int check; //�������� �� �÷��� ���� 
public:
	//���� ���࿡ �ʿ��� �Լ� �κ�
	Game();
	void run();
	void CreateEnemy();
	void FallEnemy();
	void DelEnemy();
	void MovePlayer();
	void PrintGame();
	void gotoxy(int x, int y);
	int isKeyDown(int key);
	int DamagedPlayer();

	void checkRound();
	void init();
	void DelAllEnemy();
	void textcolor(int foreground, int background);
	void CreateItem();
	void ItemGetPlayer();
	void showMenu();
	int drawGameOver();
	int clearGame();
	int isClear();
};