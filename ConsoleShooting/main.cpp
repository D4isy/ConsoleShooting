
#include "Turboc.h"

typedef struct _tagObject {
	bool m_bUse;
	bool m_bIsLeft;
	const char *m_pGraphic[30];
	int m_pGraphicRowLen;
	int m_pGraphicColLen;

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;

	DWORD m_dwTick;
	DWORD m_dwTickLimit;
} OBJECT, *POBJECT;

typedef struct _tagBullet {
	bool m_bUse;
	char m_pGraphic;

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;
} BULLET, *PBULLET;

typedef struct _tagShip {
	char m_pShipLeft[2][17] = {
		"     __ | __    ",
		"  /- - o0o-- - /"
	};

	char m_pShip[2][17] = {
		"     __ | __    ",	// (6, 0), (10, 0)
		"* -- - o0o-- - *"
	};

	char m_pShipRight[2][17] = {
		"     __ | __    ",
		" ＼-- -o0o- - ＼"
	};

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;
	int m_iScore;
	unsigned int hp;

	bool m_bLeftPress;
	bool m_bRightPress;

	DWORD m_dwTick;
	DWORD m_dwTickLimit;

	BULLET m_sBullet[20];
} SHIP, *PSHIP;

SHIP star;
OBJECT cloud[3];
OBJECT airplane;

void initAirplane()
{
	airplane.m_bIsLeft = true;
	airplane.m_pGraphic[0] = "  __                ";
	airplane.m_pGraphic[1] = " ＼  ＼     _ _ _   ";
	airplane.m_pGraphic[2] = "  ＼**＼ ___＼/  ＼ ";
	airplane.m_pGraphic[3] = "X*######*+^^^＼_ _＼";
	airplane.m_pGraphic[4] = "  o/＼  ＼          ";
	airplane.m_pGraphic[5] = "     ＼__＼         ";
	airplane.m_pGraphicRowLen = 6;
	airplane.m_pGraphicColLen = 21;
	airplane.m_dwTick = 0;
}

void initCloud()
{
	cloud[0].m_pGraphic[0] = "    _  _        ";
	cloud[0].m_pGraphic[1] = "   ( `   )_     ";
	cloud[0].m_pGraphic[2] = "  (    )    `)  ";
	cloud[0].m_pGraphic[3] = "(_   (_ .  _) _)";
	cloud[0].m_pGraphicRowLen = 4;
	cloud[0].m_pGraphicColLen = 17;
	cloud[0].m_dwTick = 0;
	cloud[0].m_dwTickLimit = 25;

	cloud[1].m_pGraphic[0] = "     _       ";
	cloud[1].m_pGraphic[1] = "    (  )     ";
	cloud[1].m_pGraphic[2] = " ( `  ) . )  ";
	cloud[1].m_pGraphic[3] = "(_, _(  ,_)_)";
	cloud[1].m_pGraphicRowLen = 4;
	cloud[1].m_pGraphicColLen = 14;
	cloud[1].m_dwTick = 0;
	cloud[1].m_dwTickLimit = 20;

	cloud[2].m_pGraphic[0] = "     _ .   ";
	cloud[2].m_pGraphic[1] = "   (  _ )_ ";
	cloud[2].m_pGraphic[2] = " (_  _(_ ,)";
	cloud[2].m_pGraphicRowLen = 3;
	cloud[2].m_pGraphicColLen = 12;
	cloud[2].m_dwTick = 0;
	cloud[2].m_dwTickLimit = 17;
}

int main()
{
	star.hp = 10;
	star.m_iScore = 0;
	star.m_iPrevX = star.m_iX = 32;
	star.m_iPrevY = star.m_iY = 26;
	star.m_dwTick = 0;
	star.m_dwTickLimit = 30;
	memset(&star.m_sBullet[0], 0, sizeof(BULLET) * 20);

	initAirplane();
	initCloud();

	setcursortype(NOCURSOR);

	DWORD tick;
	while (true) {

		tick = GetTickCount();

		/// 키 입력
		if (tick > star.m_dwTick + star.m_dwTickLimit) {

			star.m_bLeftPress = false;
			star.m_bRightPress = false;
			star.m_dwTick = tick;

			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
				star.m_iPrevX = star.m_iX;
				star.m_iPrevY = star.m_iY;
				star.m_iX -= 1;
				star.m_bLeftPress = true;
			}

			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
				star.m_iPrevX = star.m_iX;
				star.m_iPrevY = star.m_iY;
				star.m_iX += 1;
				star.m_bRightPress = true;
			}

			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				for (int i = 0; i < 20; i++) {
					if (star.m_sBullet[i].m_bUse == false) {
						star.m_sBullet[i].m_bUse = true;
						star.m_sBullet[i].m_iX = star.m_iX + 6;
						star.m_sBullet[i].m_iY = star.m_iY;
						star.m_sBullet[i].m_pGraphic = '|';

						star.m_sBullet[i + 1].m_bUse = true;
						star.m_sBullet[i + 1].m_iX = star.m_iX + 10;
						star.m_sBullet[i + 1].m_iY = star.m_iY;
						star.m_sBullet[i + 1].m_pGraphic = '|';
						break;
					}
				}
			}
		}


		/// 충돌

		// 캐릭터
		if (star.m_iX < 2) {
			star.m_iX = star.m_iPrevX;
			star.m_iY = star.m_iPrevY;
		}
		if (star.m_iX > 59) {
			star.m_iX = star.m_iPrevX;
			star.m_iY = star.m_iPrevY;
		}

		/// 렌더링

		// UI
		for (int i = 0; i < 30; i++) {
			gotoxy(77, i);
			putc('|', stdout);
		}

		// Object 출력: 구름
		for (int i = 0; i < 3; i++) {
			if (cloud[i].m_bUse == true) {
				for (int j = 0; j < cloud[i].m_pGraphicRowLen; j++) {
					// 구름 지우기
					gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
					for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
						putc(' ', stdout);
					}

					// 구름 그리기
					gotoxy(cloud[i].m_iX, cloud[i].m_iY + j);
					puts(cloud[i].m_pGraphic[j]);
				}
			}
		}

		// Object 출력: 비행기
		if (airplane.m_bUse == true) {
			for (int i = 0; i < airplane.m_pGraphicRowLen; i++) {
				// 비행기 지우기
				gotoxy(airplane.m_iPrevX, airplane.m_iPrevY + i);
				puts("                     ");

				// 비행기 그리기
				gotoxy(airplane.m_iX, airplane.m_iY + i);
				puts(airplane.m_pGraphic[i]);
			}
		}

		// 총알 출력
		for (int i = 0; i < 20; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				// 내 총알 지우기
				gotoxy(star.m_sBullet[i].m_iPrevX, star.m_sBullet[i].m_iPrevY);
				putc(' ', stdout);

				// 내 총알 그리기
				gotoxy(star.m_sBullet[i].m_iX, star.m_sBullet[i].m_iY);
				putc(star.m_sBullet[i].m_pGraphic, stdout);
			}
		}

		// 캐릭터 출력
		for (int i = 0; i < 2; i++) {
			// 내 캐릭터 지우기
			gotoxy(star.m_iPrevX-1, star.m_iPrevY + i);
			puts("                 ");
		}

		for (int i = 0; i < 2; i++) {
			// 내 캐릭터 그리기
			gotoxy(star.m_iX, star.m_iY + i);
			if (star.m_bLeftPress) {		// 왼쪽 움직임
				puts(star.m_pShipLeft[i]);
			}
			else if (star.m_bRightPress) {	// 오른쪽 움직임
				puts(star.m_pShipRight[i]);
			}
			else {
				puts(star.m_pShip[i]);
			}
		}

		/// 렌더 후 처리

		// 총알 충돌
		for (int i = 0; i < 20; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				if (star.m_sBullet[i].m_iY < 1) {
					star.m_sBullet[i].m_bUse = false;

					// 내 총알 지우기
					gotoxy(star.m_sBullet[i].m_iX, star.m_sBullet[i].m_iY);
					putc(' ', stdout);
				}
			}
		}

		// Object 충돌: 비행기
		if (airplane.m_bUse == true) {
			if (airplane.m_iX < 1) {
				airplane.m_bUse = false;

				for (int i = 0; i < airplane.m_pGraphicRowLen; i++) {
					// 비행기 지우기
					gotoxy(airplane.m_iX, airplane.m_iY + i);
					puts("                     ");
				}

				//gotoxy(airplane.m_iX, airplane.m_iY+3);
				//putc(' ', stdout);
			}
		}

		// Object 충돌: 구름
		for (int i = 0; i < 3; i++) {
			if (cloud[i].m_bUse == true) {
				if (cloud[i].m_bIsLeft == true) {
					if (cloud[i].m_iX > (76 - cloud[i].m_pGraphicColLen)) {
						cloud[i].m_bUse = false;

						for (int j = 0; j < cloud[i].m_pGraphicRowLen; j++) {
							// 구름 지우기
							gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// 구름 지우기
							gotoxy(cloud[i].m_iX, cloud[i].m_iY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}
						}
					}
				}
				else {
					if (cloud[i].m_iX < 1) {
						cloud[i].m_bUse = false;

						for (int j = 0; j < cloud[i].m_pGraphicRowLen; j++) {
							// 구름 지우기
							gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// 구름 지우기
							gotoxy(cloud[i].m_iX, cloud[i].m_iY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}
						}
					}
				}
			}
		}

		// 총알 이동
		for (int i = 0; i < 20; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				star.m_sBullet[i].m_iPrevX = star.m_sBullet[i].m_iX;
				star.m_sBullet[i].m_iPrevY = star.m_sBullet[i].m_iY;
				star.m_sBullet[i].m_iY -= 1;
			}
		}

		// Object 이동: 비행기
		if (airplane.m_bUse == true) {
			if (tick > airplane.m_dwTick + airplane.m_dwTickLimit) {
				airplane.m_dwTick = tick;
				airplane.m_iPrevX = airplane.m_iX;
				airplane.m_iPrevY = airplane.m_iY;
				airplane.m_iX -= 1;
			}
		}

		// Object 이동: 구름
		for (int i = 0; i < 3; i++) {
			if (cloud[i].m_bUse == true) {
				if (tick > cloud[i].m_dwTick + cloud[i].m_dwTickLimit) {
					cloud[i].m_dwTick = tick;
					cloud[i].m_iPrevX = cloud[i].m_iX;
					cloud[i].m_iPrevY = cloud[i].m_iY;

					if (cloud[i].m_bIsLeft == true) {
						cloud[i].m_iX += 1;
					}
					else {
						cloud[i].m_iX -= 1;
					}
				}
			}
		}

		// Object 생성: 비행기
		if (tick > airplane.m_dwTick + 500) {
			airplane.m_dwTick = tick;
			if (rand() % 10 == 2) {
				if (airplane.m_bUse == false) {
					airplane.m_bUse = true;
					airplane.m_iX = 77 - airplane.m_pGraphicColLen;
					airplane.m_iY = rand() % (30 - airplane.m_pGraphicRowLen);
					airplane.m_dwTickLimit = rand() % 7 + 10;
				}
			}
		}

		// Object 생성: 구름
		for (int i = 0; i < 3; i++) {
			if (tick > cloud[i].m_dwTick + 500) {
				cloud[i].m_dwTick = tick;
				if (1 || rand() % 10 == 2) {
					if (cloud[i].m_bUse == false) {
						cloud[i].m_bUse = true;
						cloud[i].m_bIsLeft = rand() % 2;

						if (cloud[i].m_bIsLeft == true) {
							cloud[i].m_iX = 0;
							cloud[i].m_iY = rand() % (30 - cloud[i].m_pGraphicRowLen);
						}
						else {
							cloud[i].m_iX = 77 - airplane.m_pGraphicColLen;
							cloud[i].m_iY = rand() % (30 - cloud[i].m_pGraphicRowLen);
						}
					}
				}
			}
		}

		Sleep(10);
	}

	return 0;
}