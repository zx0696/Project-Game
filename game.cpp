
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
	check = 0; // 1 = 게임 시작, 2 = 플레이, 3 = 종료
	for (int i = 0; i < WIDTH; i++) {
		obs[i] = new Obstacle();
		obs[i]->setCon(FALSE);
	}
}

void Game::CreateEnemy() {
	for (int i = 0; i < WIDTH; i++)
	{
		//해당 인덱스[i]에 적이 없으면 (FALSE 이면 실행)
		if (obs[i]->getCon() == FALSE)
		{
			//여기에 특별 아이템 생성 해주면 될듯
			//가로 (x축) 무작위로 적 출현, 세로(y축)은 출현 위치 항상 일치
			obs[i]->setX(rand() % WIDTH);
			obs[i]->setY(HEIGHT - 1);
			//적이 출현한 인덱스 [i]의 상태 = TRUE로 변경
			obs[i]->setCon(TRUE);
			return;
		}
	}
}

void Game::FallEnemy() {
	for (int i = 0; i < WIDTH; i++)
	{
		//해당 인덱스 [i]에 적이 있으면 (TRUE라면) 움직임 실행
		if (obs[i]->getCon() == TRUE)
		{
			obs[i]->Ydown();
		}
	}
}

/* 피하기 성공한 적(바닥에 떨어진 적) 삭제 */
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
		//적의 상태가 TRUE && 적의 위치가 y=0 즉 바닥 && 적의 x축 위치 = 플레이어의 x축 위치
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
	//닿지 않았으면 FALSE 반환
	return FALSE;
}

int Game::isKeyDown(int key) {
	//***GetAsyncKey는 키가 눌렸다면 최상위 비트를 1로 설정, 이전에 키가 눌렸다면 최하위 비트를 1로 설정
	//0x8000은 최상위 비트 -> 16진수는 2비트가 4개 모인 비트 -> 0x8000 = 1000 0000 0000 0000를 의미
	// 즉 최하위 비트는 0x0001 -> 0000 0000 0000 0001
	// 따라서 (GetAsyncKeyState(key) & 0x8000) != 0 은 현재 키가 눌려있다면 참(TRUE)
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void Game::MovePlayer() {
	//왼쪽 방향키 입력 시
	int temp = user->getX();
	if (isKeyDown(VK_LEFT))
		user->Xdown();
	if (isKeyDown(VK_RIGHT))
		user->Xup();
	//위치 범위 제한
	if (user->getX() < 0)
		user->setX(0);
	if (user->getX() > WIDTH - 1)
		user->setX(WIDTH - 1);
}

void Game::PrintGame() {
	//모든 화면 clear
	system("cls");

	textcolor(LIGHTGRAY, BLACK);
	gotoxy((WIDTH / 2) - 7, HEIGHT + 2);
	printf("<Project F-Killa>\n");
	//점수 표시
	printf("score:  %d", count);

	//플레이어의 남은 목숨 및 라운드 표시
	printf("\t\t        round: %d\t\t\t남은목숨: ", round);
	textcolor(LIGHTRED, BLACK);
	for (int j = 0; j < user->getHeart(); j++) {
		printf("♥");
	}

	//라운드에 따른 장애물 모양 변경(학점 상승에 비유)
	int i;
	for (i = 0; i < WIDTH; i++)
	{
		if (obs[i]->getX())
		{
			//적 위치에 적군 출력
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
	//플레이어 출력
	gotoxy(user->getX(), HEIGHT);
	textcolor(RED, BLACK);
	printf("♀");

	//아이템 출력(있을 시에만)
	if (item != NULL) {
		gotoxy(item->getX(), HEIGHT);
		textcolor(YELLOW, BLACK);
		printf("●");
	}

	//바닥 출력
	gotoxy(0, HEIGHT + 1);
	textcolor(LIGHTGREEN, BLACK);
	for (i = 0; i < WIDTH; i++) {
		printf("▩");
	}
}

void Game::gotoxy(int x, int y) {
	//COORD : window.h에 있는 구조체, {SHORT x, SHORT y} 로 구성되어 있다.
	COORD pos = { x * 2, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//점수에 따른 라운드 설정
//편의에 따라 변경 가능
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

//모든 장애물 제거 함수(플래이어가 목숨이 남았는데 장애물에 닿았을 시 필요)
void Game::DelAllEnemy() {
	for (int i = 0; i < WIDTH; i++) {
		obs[i]->setX(0);
		obs[i]->setY(0);
		obs[i]->setCon(FALSE);
	}
}

//유저 위치 및 장애물 초기화 함수
void Game::init() {
	user->setX(1);
	system("cls");
	DelAllEnemy();
}

//콘솔창 텍스트 컬러 변환 함수
void Game::textcolor(int foreground, int background) {
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//아이템 객체 생성 함수
void Game::CreateItem() {
	per = rand() % 10;
	if (item == NULL) {
		if (per == 5) {
			item = new Item();
			item->setX(rand() % WIDTH);
		}
	}

}

//플레이어가 아이템 습득 판별 및 점수 증가
void Game::ItemGetPlayer() {
	if (item != NULL && (item->getX() == user->getX())) {
		count = count + 100;
		item = NULL;
	}
}

//게임 메뉴 화면을 그려주는 함수
void Game::showMenu() {
	PlaySound(TEXT("opening.wav"), NULL, SND_ASYNC | SND_LOOP);
	/*
Good Vibes by MBB https://soundcloud.com/mbbofficial
Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
http://creativecommons.org/licenses/by-sa/3.0/

Distributor: 저작권 걱정없는 유튜브용 음원 & 무료 효과음 서비스 ‘뮤팟’
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
	printf(">   게임 시작");
	gotoxy(x, y + 1);
	printf("게임 종료");
	check = 1;

	//게임 시작 & 게임 종료 메뉴 선택 화살표 표시 구간
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

//게임 오버 시 화면 그려주는 함수
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
	printf(">   메인 메뉴");
	gotoxy(x, y + 1);
	printf("게임 종료");
	check = 1;

	//메인 메뉴 & 게임 종료 표시 선택 판별
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

//게임 클리어 시 화면 출력
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
	printf("\t\t      □□□□□□  □              □□□□□□□            □            □□□□□□□\n");
	printf("\t\t    □              □              □                       □□           □            □\n");
	printf("\t\t   □               □              □                      □  □          □              □\n");
	printf("\t\t  □                □              □                     □    □         □              □\n");
	printf("\t\t □                 □              □                    □      □        □            □\n");
	printf("\t\t □                 □              □□□□□□□       □□□□□□       □□□□□□□\n");
	printf("\t\t □                 □              □                  □          □      □            □\n");
	printf("\t\t □                 □              □                 □            □     □             □\n");
	printf("\t\t  □                □              □                □              □    □              □\n");
	printf("\t\t   □               □              □               □                □   □               □\n");
	printf("\t\t     □□□□□□   □□□□□□□  □□□□□□□  □                  □  □                □\n");


	gotoxy(x - 2, y);
	printf(">   메인 메뉴");
	gotoxy(x, y + 1);
	printf("게임 종료");
	check = 1;

	//게임 오버와 같이 메인 메뉴 및 게임 종료 판별
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

//게임이 클리어 되었는지 판별해주는 함수
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
			//매번 실행할 때마다 다른 값을 주기 위한 시드값 설정
			srand((int)malloc(NULL));

			CreateEnemy(); //기존
			FallEnemy();//기존
			DelEnemy();//기존

			MovePlayer();//기존

			checkRound();
			CreateItem();

			PrintGame();//기존

			ItemGetPlayer();
			//게임의 속도 조절을 위해 Sleep 설정
			Sleep(100);
			if (isClear())
				break;
		} while ((!DamagedPlayer()));    //닿지 않거나 점수가 700점 이하일때 반복 //기존
		system("cls");
		//게임 클리어시 
		if (count >= 500) {
			if (!clearGame())
				break;
		}
		else { //죽었을 시
			if (!drawGameOver())
				break;
		}
	}

}