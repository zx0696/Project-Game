
//https://github.com/zx0696/c-.git
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include "game.h"
#include "obstacle.h"
#include "user.h"
#include "item.h"
using namespace std;

Game::Game() {
	user = new User();
	count = 0;
	round = 0;
	check = 0; // 1 = ���� ����, 2 = �÷���, 3 = ����
	for (int i = 0; i < WIDTH; i++) {
		obs[i] = new Obstacle();
		obs[i]->setCon(FALSE);
	}
}

void Game::CreateEnemy() {
	for (int i = 0; i < WIDTH; i++)
	{
		//�ش� �ε���[i]�� ���� ������ (FALSE �̸� ����)
		if (obs[i]->getCon() == FALSE)
		{
			//���⿡ Ư�� ������ ���� ���ָ� �ɵ�
			//���� (x��) �������� �� ����, ����(y��)�� ���� ��ġ �׻� ��ġ
			obs[i]->setX(rand() % WIDTH);
			obs[i]->setY(HEIGHT - 1);
			//���� ������ �ε��� [i]�� ���� = TRUE�� ����
			obs[i]->setCon(TRUE);
			return;
		}
	}
}

void Game::FallEnemy() {
	for (int i = 0; i < WIDTH; i++)
	{
		//�ش� �ε��� [i]�� ���� ������ (TRUE���) ������ ����
		if (obs[i]->getCon() == TRUE)
		{
			obs[i]->Ydown();
		}
	}
}

/* ���ϱ� ������ ��(�ٴڿ� ������ ��) ���� */
void Game::DelEnemy() {
	for (int i = 0; i < WIDTH; i++)
	{
		if (obs[i]->getCon() && obs[i]->getY() < 0) {
			obs[i]->setCon(FALSE);
			count++;
		}
	}
}

int Game::DamagedPlayer() {
	for (int i = 0; i < WIDTH; i++)
	{
		//���� ���°� TRUE && ���� ��ġ�� y=0 �� �ٴ� && ���� x�� ��ġ = �÷��̾��� x�� ��ġ
		if ((obs[i]->getCon() && obs[i]->getY() == 0) && (obs[i]->getX() == user->getX())) {
			if (user->getHeart() > 0) {
				user->heartDown();
				init();
				return FALSE;
			}
			else
				return TRUE;

		}
	}
	//���� �ʾ����� FALSE ��ȯ
	return FALSE;
}

int Game::isKeyDown(int key) {
	//***GetAsyncKey�� Ű�� ���ȴٸ� �ֻ��� ��Ʈ�� 1�� ����, ������ Ű�� ���ȴٸ� ������ ��Ʈ�� 1�� ����
	//0x8000�� �ֻ��� ��Ʈ -> 16������ 2��Ʈ�� 4�� ���� ��Ʈ -> 0x8000 = 1000 0000 0000 0000�� �ǹ�
	// �� ������ ��Ʈ�� 0x0001 -> 0000 0000 0000 0001
	// ���� (GetAsyncKeyState(key) & 0x8000) != 0 �� ���� Ű�� �����ִٸ� ��(TRUE)
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void Game::MovePlayer() {
	//���� ����Ű �Է� ��
	int temp = user->getX();
	if (isKeyDown(VK_LEFT))
		user->Xdown();
	if (isKeyDown(VK_RIGHT))
		user->Xup();
	//��ġ ���� ����
	if (user->getX() < 0)
		user->setX(0);
	if (user->getX() > WIDTH - 1)
		user->setX(WIDTH - 1);
}

void Game::PrintGame() {
	//��� ȭ�� clear
	system("cls");

	textcolor(LIGHTGRAY, BLACK);
	gotoxy((WIDTH / 2) - 7, HEIGHT + 2);
	printf("<Project F-Killa>\n");
	//���� ǥ��
	printf("score:  %d", count);

	//�÷��̾��� ���� ��� �� ���� ǥ��
	printf("\t\t        round: %d\t\t\t�������: ", round);
	textcolor(LIGHTRED, BLACK);
	for (int j = 0; j < user->getHeart(); j++) {
		printf("��");
	}

	//���忡 ���� ��ֹ� ��� ����(���� ��¿� ����)
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (obs[i]->getX())
		{
			//�� ��ġ�� ���� ���
			gotoxy(obs[i]->getX(), (HEIGHT - obs[i]->getY()));
			if (round == 0) {
				textcolor(LIGHTGRAY, BLACK);
				printf("F");
			}
			else if (round == 1) {
				textcolor(BROWN, BLACK);
				printf("D");
			}
			else if (round == 2) {
				textcolor(BROWN, BLACK);
				printf("D+");
			}
			else if (round == 3) {
				textcolor(BLUE, BLACK);
				printf("C");
			}
			else if (round == 4) {
				textcolor(BLUE, BLACK);
				printf("C+");
			}
			else if (round == 5) {
				textcolor(CYAN, BLACK);
				printf("B");
			}
			else if (round == 6) {
				textcolor(CYAN, BLACK);
				printf("B+");
			}
			else if (round == 7) {
				textcolor(LIGHTMAGENTA, BLACK);
				printf("A");
			}
			else {
				textcolor(LIGHTMAGENTA, BLACK);
				printf("A+");
			}
		}
	}
	//�÷��̾� ���
	gotoxy(user->getX(), HEIGHT);
	textcolor(RED, BLACK);
	printf("��");

	//������ ���(���� �ÿ���)
	if (item != NULL) {
		gotoxy(item->getX(), HEIGHT);
		textcolor(YELLOW, BLACK);
		printf("��");
	}

	//�ٴ� ���
	gotoxy(0, HEIGHT + 1);
	textcolor(LIGHTGREEN, BLACK);
	for (i = 0; i < WIDTH; i++) {
		printf("��");
	}
}

void Game::gotoxy(int x, int y) {
	//COORD : window.h�� �ִ� ����ü, {SHORT x, SHORT y} �� �����Ǿ� �ִ�.
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//������ ���� ���� ����
//���ǿ� ���� ���� ����
void Game::checkRound() {
	if (count >= 0 && count < 50)
		round = 0;
	else if (count >= 50 && count < 100)
		round = 1;
	else if (count >= 100 && count < 150)
		round = 2;
	else if (count >= 150 && count < 200)
		round = 3;
	else if (count >= 200 && count < 250)
		round = 4;
	else if (count >= 250 && count < 300)
		round = 5;
	else if (count >= 300 && count < 350)
		round = 6;
	else if (count >= 350 && count < 400)
		round = 7;
	else if (count >= 400 && count < 450)
		round = 8;
	else if (count >= 500)
		round = 9;

}

//��� ��ֹ� ���� �Լ�(�÷��̾ ����� ���Ҵµ� ��ֹ��� ����� �� �ʿ�)
void Game::DelAllEnemy() {
	for (int i = 0; i < WIDTH; i++) {
		obs[i]->setX(0);
		obs[i]->setY(0);
		obs[i]->setCon(FALSE);
	}
}

//���� ��ġ �� ��ֹ� �ʱ�ȭ �Լ�
void Game::init() {
	user->setX(1);
	system("cls");
	DelAllEnemy();
}

//�ܼ�â �ؽ�Ʈ �÷� ��ȯ �Լ�
void Game::textcolor(int foreground, int background) {
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//������ ��ü ���� �Լ�
void Game::CreateItem() {
	per = rand() % 10;
	if (item == NULL) {
		if (per == 5) {
			item = new Item();
			item->setX(rand() % WIDTH);
		}
	}

}

//�÷��̾ ������ ���� �Ǻ� �� ���� ����
void Game::ItemGetPlayer() {
	if (item != NULL && (item->getX() == user->getX())) {
		count = count + 100;
		item = NULL;
	}
}

//���� �޴� ȭ���� �׷��ִ� �Լ�
void Game::showMenu() {
	PlaySound(TEXT("opening.wav"), NULL, SND_ASYNC | SND_LOOP);
	/*
Good Vibes by MBB https://soundcloud.com/mbbofficial
Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
http://creativecommons.org/licenses/by-sa/3.0/

Distributor: ���۱� �������� ��Ʃ��� ���� & ���� ȿ���� ���� �����̡�
https://www.mewpot.com
	*/

	int i = 0;
	int key = 0;
	int x = 26;
	int y = 20;
	system("cls");
	for (int k = 0; k < 4; k++) {
		printf("\n");
	}
	textcolor(LIGHTGRAY, BLACK);
	printf("\t\t    FFFFFFFFFFFFFFFF                      #      #   #   #  #            #\n");
	printf("\t\t    F                                     #     #    #   #  #           # #\n");
	printf("\t\t    F                                     #    #         #  #          #   #\n");
	printf("\t\t    F                                     #   #      #   #  #         #     #\n");
	printf("\t\t    F                                     #  #       #   #  #        #       #\n");
	printf("\t\t    FFFFFFFFF              ======         ##         #   #  #       ###########\n");
	printf("\t\t    F                                     #  #       #   #  #      #           #\n");
	printf("\t\t    F                                     #   #      #   #  #     #             #\n");
	printf("\t\t    F                                     #    #     #   #  #    #               #\n");
	printf("\t\t    F                                     #     #    #   #  #   #                 #\n");
	printf("\t\t    F                                     #      #   #   #  #  #                   #\n");


	for (int k = 0; k < 4; k++) {
		printf("\n");
	}

	gotoxy(x - 2, y);
	printf(">   ���� ����");
	gotoxy(x, y + 1);
	printf("���� ����");
	check = 1;

	//���� ���� & ���� ���� �޴� ���� ȭ��ǥ ǥ�� ����
	while (1) {
		if (_kbhit()) {
			key = _getch();
			if (key == 224 || key == 0) {
				key = _getch();
				switch (key) {
				case 72:
					if (y > 20) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, --y);
						printf(">");
						check = 1;
					}
					break;
				case 80:
					if (y < 21) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, ++y);
						printf(">");
						check = 3;
					}
					break;
				}
			}
			else {
				if (key == 13) {
					if (check == 1) {
						PlaySound(NULL, 0, 0);
						PlaySound(TEXT("playing.wav"), NULL, SND_ASYNC | SND_LOOP);

						break;
					}
					else if (check == 3) {
						exit(1);
					}
				}
			}
		}
	}

}

//���� ���� �� ȭ�� �׷��ִ� �Լ�
int Game::drawGameOver() {
	PlaySound(NULL, 0, 0);
	PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC | SND_LOOP);
	/*
	Piano Sad 2 (Piano Version) by PeriTune | http://peritune.com
  Music promoted by https://www.mewpot.com
  CC BY 4.0 | https://creativecommons.org/licenses/by/4.0/
	*/
	int i = 0;
	int key = 0;
	int x = 28;
	int y = 27;
	system("cls");
	for (int k = 0; k < 2; k++) {
		printf("\n");
	}
	textcolor(LIGHTGRAY, BLACK);
	printf("\t\t     ##################             #            #             #  ###############\n");
	printf("\t\t    #                  #           # #           # #         # #  #\n");
	printf("\t\t   #                    #         #   #          #  #       #  #  #\n");
	printf("\t\t  #                      #       #     #         #   #     #   #  #\n");
	printf("\t\t #                              #       #        #    #   #    #  #\n");
	printf("\t\t#                              ###########       #     # #     #  ###############\n");
	printf("\t\t #            ##############  #           #      #      #      #  #\n");
	printf("\t\t  #                       #  #             #     #             #  #\n");
	printf("\t\t   #                     #  #               #    #             #  #\n");
	printf("\t\t    #                   #  #                 #   #             #  #\n");
	printf("\t\t     ###################  #                   #  #             #  ###############\n\n");

	printf("\t\t        ###########    #                     #  ###############    ##############\n");
	printf("\t\t       #           #    #                   #   #                  #             #\n");
	printf("\t\t      #             #    #                 #    #                  #              #\n");
	printf("\t\t     #               #    #               #     #                  #               #\n");
	printf("\t\t    #                 #    #             #      #                  #              #\n");
	printf("\t\t    #                 #     #           #       ###############    ##############\n");
	printf("\t\t    #                 #      #         #        #                  #            #\n");
	printf("\t\t     #               #        #       #         #                  #             #\n");
	printf("\t\t      #             #          #     #          #                  #              #\n");
	printf("\t\t       #           #            #   #           #                  #               #\n");
	printf("\t\t        ###########              ###            ###############    #                #\n");


	gotoxy(x - 2, y);
	printf(">   ���� �޴�");
	gotoxy(x, y + 1);
	printf("���� ����");
	check = 1;

	//���� �޴� & ���� ���� ǥ�� ���� �Ǻ�
	while (1) {
		if (_kbhit()) {
			key = _getch();
			if (key == 224 || key == 0) {
				key = _getch();
				switch (key) {
				case 72:
					if (y > 27) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, --y);
						printf(">");
						check = 1;
					}
					break;
				case 80:
					if (y < 28) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, ++y);
						printf(">");
						check = 3;
					}
					break;
				}
			}
			else {
				if (key == 13) {
					if (check == 1) {
						init();
						item = NULL;
						user->setX(0);
						user->initHeart();
						for (int i = 0; i < WIDTH; i++) {
							obs[i]->setX(0);
							obs[i]->setY(0);
							obs[i]->setCon(FALSE);
						}
						count = 0;
						round = 0;
						PlaySound(NULL, 0, 0);
						return true;
					}
					else if (check == 3) {
						return false;
					}
				}
			}
		}
	}
}

//���� Ŭ���� �� ȭ�� ���
int Game::clearGame() {
	PlaySound(TEXT("ending.wav"), NULL, SND_ASYNC | SND_LOOP);

	/*
	Happy Farmlife by Alexander Nakarada | https://www.serpentsoundstudios.com
  Music promoted by https://www.mewpot.com
  CC BY 4.0 | https://creativecommons.org/licenses/by/4.0/
	*/
	int i = 0;
	int key = 0;
	int x = 28;
	int y = 23;

	system("cls");
	for (int k = 0; k < 5; k++) {
		printf("\n");
	}
	textcolor(LIGHTGRAY, BLACK);
	printf("\t\t      �������  ��              ��������            ��            ��������\n");
	printf("\t\t    ��              ��              ��                       ���           ��            ��\n");
	printf("\t\t   ��               ��              ��                      ��  ��          ��              ��\n");
	printf("\t\t  ��                ��              ��                     ��    ��         ��              ��\n");
	printf("\t\t ��                 ��              ��                    ��      ��        ��            ��\n");
	printf("\t\t ��                 ��              ��������       �������       ��������\n");
	printf("\t\t ��                 ��              ��                  ��          ��      ��            ��\n");
	printf("\t\t ��                 ��              ��                 ��            ��     ��             ��\n");
	printf("\t\t  ��                ��              ��                ��              ��    ��              ��\n");
	printf("\t\t   ��               ��              ��               ��                ��   ��               ��\n");
	printf("\t\t     �������   ��������  ��������  ��                  ��  ��                ��\n");


	gotoxy(x - 2, y);
	printf(">   ���� �޴�");
	gotoxy(x, y + 1);
	printf("���� ����");
	check = 1;

	//���� ������ ���� ���� �޴� �� ���� ���� �Ǻ�
	while (1) {
		if (_kbhit()) {
			key = _getch();
			if (key == 224 || key == 0) {
				key = _getch();
				switch (key) {
				case 72:
					if (y > 23) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, --y);
						printf(">");
						check = 1;
					}
					break;
				case 80:
					if (y < 24) {
						gotoxy(x - 2, y);
						printf(" ");
						gotoxy(x - 2, ++y);
						printf(">");
						check = 3;
					}
					break;
				}
			}
			else {
				if (key == 13) {
					if (check == 1) {
						init();
						item = NULL;
						user->setX(0);
						user->initHeart();
						for (int i = 0; i < WIDTH; i++) {
							obs[i]->setX(0);
							obs[i]->setY(0);
							obs[i]->setCon(FALSE);
						}
						count = 0;
						round = 0;
						PlaySound(NULL, 0, 0);
						return true;
					}
					else if (check == 3) {
						return false;
					}
				}
			}
		}
	}
}

//������ Ŭ���� �Ǿ����� �Ǻ����ִ� �Լ�
int Game::isClear() {
	if (count >= 500) {
		return TRUE;
	}
	else
		return FALSE;
}

void Game::run() {
	system("tiTle Project F-Killa");
	while (1) {
		showMenu();
		do {
			//�Ź� ������ ������ �ٸ� ���� �ֱ� ���� �õ尪 ����
			srand((int)malloc(NULL));

			CreateEnemy(); //����
			FallEnemy();//����
			DelEnemy();//����

			MovePlayer();//����

			checkRound();
			CreateItem();

			PrintGame();//����

			ItemGetPlayer();
			//������ �ӵ� ������ ���� Sleep ����
			Sleep(100);
			if (isClear())
				break;
		} while ((!DamagedPlayer()));    //���� �ʰų� ������ 700�� �����϶� �ݺ� //����
		system("cls");
		//���� Ŭ����� 
		if (count >= 500) {
			if (!clearGame())
				break;
		}
		else { //�׾��� ��
			if (!drawGameOver())
				break;
		}
	}

}