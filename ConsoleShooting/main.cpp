
#include "Turboc.h"

#define OBJECT_BULLET_COUNT 18
#define PLAYER_BULLET_COUNT 10

typedef struct _tagBullet {
	bool m_bUse;
	char m_pGraphic;

	int damage;

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;

	DWORD m_dwTick;
	DWORD m_dwTickLimit;
} BULLET, *PBULLET;

typedef struct _tagObject {
	bool m_bUse;
	bool m_bIsLeft;
	char m_pGraphic[8][32];
	int m_pGraphicRowLen;
	int m_pGraphicColLen;
	int m_iScore;

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;

	int m_iMaxHp;
	int m_iHp;

	DWORD m_dwTick;
	DWORD m_dwTickLimit;
	DWORD m_dwBulletTick;

	BULLET m_Bullet[OBJECT_BULLET_COUNT];
} OBJECT, *POBJECT;

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
		" ��-- -o0o- - ��"
	};

	int m_iPrevX, m_iPrevY;
	int m_iX, m_iY;
	int m_iScore;
	int m_iCount;
	int m_iMaxHp;
	int m_iHp;

	bool m_bLeftPress;
	bool m_bRightPress;

	DWORD m_dwTick;
	DWORD m_dwTickLimit;

	BULLET m_sBullet[PLAYER_BULLET_COUNT];
} SHIP, *PSHIP;

SHIP star;

OBJECT enemy[5];

OBJECT cloud[3];
OBJECT airplane;

void initEnemey()
{
	memset(&enemy[0], 0, sizeof(OBJECT) * 5);

	strcpy(enemy[0].m_pGraphic[0], ". -= -.");	// (3, 0)
	enemy[0].m_pGraphicRowLen = 1;
	enemy[0].m_pGraphicColLen = 8;
	enemy[0].m_iScore = 1;
	enemy[0].m_iMaxHp = 15;
	enemy[0].m_dwTick = 0;
	enemy[0].m_dwTickLimit = 45;

	strcpy(enemy[1].m_pGraphic[0], "    _o_    ");
	strcpy(enemy[1].m_pGraphic[1], "_,-=(_)=-,_"); // (3, 1), (7, 1)
	strcpy(enemy[1].m_pGraphic[2], "'         '");
	enemy[1].m_pGraphicRowLen = 3;
	enemy[1].m_pGraphicColLen = 12;
	enemy[1].m_iScore = 5;
	enemy[1].m_iMaxHp = 25;
	enemy[1].m_dwTick = 0;
	enemy[1].m_dwTickLimit = 40;

	strcpy(enemy[2].m_pGraphic[0], "_��_ /_   ");
	strcpy(enemy[2].m_pGraphic[1], "(_(\'\')_)");	// (1, 1), (6,1)
	strcpy(enemy[2].m_pGraphic[2], "  \"  \"    ");
	enemy[2].m_pGraphicRowLen = 3;
	enemy[2].m_pGraphicColLen = 11;
	enemy[2].m_iScore = 5;
	enemy[2].m_iMaxHp = 25;
	enemy[2].m_dwTick = 0;
	enemy[2].m_dwTickLimit = 49;

	strcpy(enemy[3].m_pGraphic[0], " ,_o_, ");
	strcpy(enemy[3].m_pGraphic[1], "=======");	//     (3, 1)
	strcpy(enemy[3].m_pGraphic[2], " (_o_) ");	// (2, 2), (4, 2)
	enemy[3].m_pGraphicRowLen = 3;
	enemy[3].m_pGraphicColLen = 8;
	enemy[3].m_iScore = 10;
	enemy[3].m_iMaxHp = 40;
	enemy[3].m_dwTick = 0;
	enemy[3].m_dwTickLimit = 55;

	strcpy(enemy[4].m_pGraphic[0], "       / /-A-����      ");
	strcpy(enemy[4].m_pGraphic[1], "  ___---=======---___  ");
	strcpy(enemy[4].m_pGraphic[2], "(=__��  /.. ..��  /__=)");
	strcpy(enemy[4].m_pGraphic[3], "     ---��__O__/---    ");	// (5, 3), (6, 3), (7, 3), (16, 3), (17, 3), (18, 3)
	enemy[4].m_pGraphicRowLen = 4;
	enemy[4].m_pGraphicColLen = 24;
	enemy[4].m_iScore = 20;
	enemy[4].m_iMaxHp = 70;
	enemy[4].m_dwTick = 0;
	enemy[4].m_dwTickLimit = 70;
}

void initAirplane()
{
	airplane.m_bIsLeft = true;
	strcpy(airplane.m_pGraphic[0], "  __                ");
	strcpy(airplane.m_pGraphic[1], " ��  ��     _ _ _   ");
	strcpy(airplane.m_pGraphic[2], "  ��**�� ___��/  �� ");
	strcpy(airplane.m_pGraphic[3], "X*######*+^^^��_ _��");
	strcpy(airplane.m_pGraphic[4], "  o/��  ��          ");
	strcpy(airplane.m_pGraphic[5], "     ��__��         ");
	airplane.m_pGraphicRowLen = 6;
	airplane.m_pGraphicColLen = 21;
	airplane.m_dwTick = 0;
}

void initCloud()
{
	strcpy(cloud[0].m_pGraphic[0], "    _  _        ");
	strcpy(cloud[0].m_pGraphic[1], "   ( `   )_     ");
	strcpy(cloud[0].m_pGraphic[2], "  (    )    `)  ");
	strcpy(cloud[0].m_pGraphic[3], "(_   (_ .  _) _)");
	cloud[0].m_pGraphicRowLen = 4;
	cloud[0].m_pGraphicColLen = 17;
	cloud[0].m_dwTick = 0;
	cloud[0].m_dwTickLimit = 25;

	strcpy(cloud[1].m_pGraphic[0], "     _       ");
	strcpy(cloud[1].m_pGraphic[1], "    (  )     ");
	strcpy(cloud[1].m_pGraphic[2], " ( `  ) . )  ");
	strcpy(cloud[1].m_pGraphic[3], "(_, _(  ,_)_)");
	cloud[1].m_pGraphicRowLen = 4;
	cloud[1].m_pGraphicColLen = 14;
	cloud[1].m_dwTick = 0;
	cloud[1].m_dwTickLimit = 20;

	strcpy(cloud[2].m_pGraphic[0], "     _ .   ");
	strcpy(cloud[2].m_pGraphic[1], "   (  _ )_ ");
	strcpy(cloud[2].m_pGraphic[2], " (_  _(_ ,)");
	cloud[2].m_pGraphicRowLen = 3;
	cloud[2].m_pGraphicColLen = 12;
	cloud[2].m_dwTick = 0;
	cloud[2].m_dwTickLimit = 17;
}

int main()
{
	bool isCollapse = false;
	bool runFlag = true;
	srand(time(NULL));

	star.m_iMaxHp = 200;
	star.m_iHp = 200;
	star.m_iCount = 0;
	star.m_iScore = 0;
	star.m_iPrevX = star.m_iX = 32;
	star.m_iPrevY = star.m_iY = 26;
	star.m_dwTick = 0;
	star.m_dwTickLimit = 30;
	memset(&star.m_sBullet[0], 0, sizeof(BULLET) * PLAYER_BULLET_COUNT);

	initEnemey();
	initAirplane();
	initCloud();

	setcursortype(NOCURSOR);

	DWORD tick;
	while (runFlag) {

		tick = GetTickCount();

		/// Ű �Է�
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

			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				runFlag = false;
			}

			int damage = 0;
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				for (int i = 0; i < PLAYER_BULLET_COUNT; i++) {
					if (star.m_sBullet[i].m_bUse == false) {
						damage = rand() % 3 + 1;
						star.m_sBullet[i].m_bUse = true;
						star.m_sBullet[i].damage = damage;
						star.m_sBullet[i].m_iX = star.m_iX + 6;
						star.m_sBullet[i].m_iY = star.m_iY;
						star.m_sBullet[i].m_pGraphic = '|';

						star.m_sBullet[i + 1].m_bUse = true;
						star.m_sBullet[i + 1].damage = damage;
						star.m_sBullet[i + 1].m_iX = star.m_iX + 10;
						star.m_sBullet[i + 1].m_iY = star.m_iY;
						star.m_sBullet[i + 1].m_pGraphic = '|';
						break;
					}
				}
			}
		}


		/// �浹

		// ĳ����
		if (star.m_iX < 2) {
			star.m_iX = star.m_iPrevX;
			star.m_iY = star.m_iPrevY;
		}
		if (star.m_iX > 59) {
			star.m_iX = star.m_iPrevX;
			star.m_iY = star.m_iPrevY;
		}

		/// ������

		// ���� UI
		for (int i = 0; i < 30; i++) {
			gotoxy(77, i);
			putc('|', stdout);
		}

		// ���� ���
		gotoxy(82, 1);
		puts("[ ��  ��  �� : wpwpals@naver.com ]");

		// ���� ���
		gotoxy(79, 4);
		puts("�¿� ����Ű: ����⸦ ������ �� �ִ�.");

		// ���� ���
		gotoxy(79, 6);
		puts("�����̽��� : �̻����� �߻��� �� �ִ�.");

		// ���� ���
		gotoxy(79, 8);
		puts("E   S   C  : ������ ������ �� �ִ�.");

		// ���� UI
		for (int i = 0; i < 40; i += 2) {
			gotoxy(79 + i, 10);
			putc('=', stdout);
		}

		// ���� ���
		gotoxy(79, 12);
		puts("�̻��� ������");

		// ���� ���
		gotoxy(78, 14);
		puts("   *   : ������ 1");

		// ���� ���
		gotoxy(97, 14);
		puts("|   @    : ������ 1");

		// ���� ���
		gotoxy(78, 15);
		puts("   #   : ������ 2");

		// ���� ���
		gotoxy(97, 15);
		puts("| DAIAY! : ������ 2");

		// ���� UI
		for (int i = 0; i < 40; i += 2) {
			gotoxy(79 + i, 17);
			putc('=', stdout);
		}

		// ���� ���
		gotoxy(79, 19);
		puts("ĳ���� �ǰ� ���� ���� : o0o");

		// ���� ���
		gotoxy(79, 20);
		puts("���� �ǰ� ���� ���� : ��ü ����");

		// ���� UI
		for (int i = 0; i < 40; i+=2) {
			gotoxy(79 + i, 22);
			putc('=', stdout);
		}

		// ü�� ���
		gotoxy(79, 24);
		printf("���� ���� ü��: [%3d/%3d]", star.m_iHp, star.m_iMaxHp);

		// ���� ���
		gotoxy(79, 26);
		printf("     ���� ����: [%d]", star.m_iScore);

		// ���� Ƚ�� ���
		gotoxy(79, 28);
		printf("���� ����  �� : [%d]", star.m_iCount);

		// Object ���: ����
		for (int i = 0; i < 3; i++) {
			if (cloud[i].m_bUse == true) {
				for (int j = 0; j < cloud[i].m_pGraphicRowLen; j++) {
					// ���� �����
					gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
					for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
						putc(' ', stdout);
					}

					// ���� �׸���
					gotoxy(cloud[i].m_iX, cloud[i].m_iY + j);
					puts(cloud[i].m_pGraphic[j]);
				}
			}
		}

		// Object ���: �����
		if (airplane.m_bUse == true) {
			for (int i = 0; i < airplane.m_pGraphicRowLen; i++) {
				// ����� �����
				gotoxy(airplane.m_iPrevX, airplane.m_iPrevY + i);
				puts("                     ");

				// ����� �׸���
				gotoxy(airplane.m_iX, airplane.m_iY + i);
				puts(airplane.m_pGraphic[i]);
			}
		}

		// Object ���: ��
		for (int i = 0; i < 5; i++) {
			if (enemy[i].m_bUse == true) {
				for (int j = 0; j < enemy[i].m_pGraphicRowLen; j++) {
					// �� �����
					gotoxy(enemy[i].m_iPrevX, enemy[i].m_iPrevY + j);
					for (int h = 0; h < enemy[i].m_pGraphicColLen; h++) {
						putc(' ', stdout);
					}

					// �� �׸���
					gotoxy(enemy[i].m_iX, enemy[i].m_iY + j);
					puts(enemy[i].m_pGraphic[j]);
				}
			}
		}

		// �Ѿ� ���
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < OBJECT_BULLET_COUNT; j++) {
				if (enemy[i].m_Bullet[j].m_bUse == true) {
					// �� �Ѿ� �����
					gotoxy(enemy[i].m_Bullet[j].m_iPrevX, enemy[i].m_Bullet[j].m_iPrevY);
					putc(' ', stdout);

					// �� �Ѿ� �׸���
					gotoxy(enemy[i].m_Bullet[j].m_iX, enemy[i].m_Bullet[j].m_iY);
					putc(enemy[i].m_Bullet[j].m_pGraphic, stdout);
				}
			}
		}

		for (int i = 0; i < PLAYER_BULLET_COUNT; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				// �� �Ѿ� �����
				gotoxy(star.m_sBullet[i].m_iPrevX, star.m_sBullet[i].m_iPrevY);
				putc(' ', stdout);

				// �� �Ѿ� �׸���
				gotoxy(star.m_sBullet[i].m_iX, star.m_sBullet[i].m_iY);
				putc(star.m_sBullet[i].m_pGraphic, stdout);
			}
		}

		// ĳ���� ���
		for (int i = 0; i < 3; i++) {
			// �� ĳ���� �����
			gotoxy(star.m_iPrevX-1, star.m_iPrevY + i);
			puts("                 ");
		}

		for (int i = 0; i < 2; i++) {
			// �� ĳ���� �׸���
			gotoxy(star.m_iX, star.m_iY + i);
			if (star.m_bLeftPress) {		// ���� ������
				puts(star.m_pShipLeft[i]);
			}
			else if (star.m_bRightPress) {	// ������ ������
				puts(star.m_pShipRight[i]);
			}
			else {
				puts(star.m_pShip[i]);
			}
		}

		/// ���� �� ó��

		// �� �Ѿ� �浹
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < OBJECT_BULLET_COUNT; j++) {
				if (enemy[i].m_Bullet[j].m_bUse == true) {
					isCollapse = false;
					if ((star.m_iX + 7 <= enemy[i].m_Bullet[j].m_iX && enemy[i].m_Bullet[j].m_iX < star.m_iX + 10) &&
						(star.m_iY + 1 == enemy[i].m_Bullet[j].m_iY)) {
						star.m_iHp -= enemy[i].m_Bullet[j].damage;
						isCollapse = true;
					}
					if (isCollapse || enemy[i].m_Bullet[j].m_iY > 28) {
						enemy[i].m_Bullet[j].m_bUse = false;

						// �� �Ѿ� �����
						gotoxy(enemy[i].m_Bullet[j].m_iX, enemy[i].m_Bullet[j].m_iY);
						putc(' ', stdout);
					}
				}
			}
		}

		// �� �Ѿ� �浹
		for (int i = 0; i < PLAYER_BULLET_COUNT; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				isCollapse = false;

				for (int j = 0; j < 5; j++) {
					if (enemy[j].m_bUse == true) {
						if ((enemy[j].m_iX <= star.m_sBullet[i].m_iX && star.m_sBullet[i].m_iX < enemy[j].m_iX + enemy[j].m_pGraphicColLen) &&
							(enemy[j].m_iY <= star.m_sBullet[i].m_iY && star.m_sBullet[i].m_iY < enemy[j].m_iY + enemy[j].m_pGraphicRowLen)) {
							// �浹
							enemy[j].m_iHp -= star.m_sBullet[i].damage;
							isCollapse = true;
							break;
						}
					}
				}

				if (isCollapse == true || star.m_sBullet[i].m_iY < 1) {
					star.m_sBullet[i].m_bUse = false;

					// �� �Ѿ� �����
					gotoxy(star.m_sBullet[i].m_iX, star.m_sBullet[i].m_iY);
					putc(' ', stdout);
				}
			}
		}

		// Object �浹: �����
		if (airplane.m_bUse == true) {
			if (airplane.m_iX < 1) {
				airplane.m_bUse = false;

				for (int i = 0; i < airplane.m_pGraphicRowLen; i++) {
					// ����� �����
					gotoxy(airplane.m_iX, airplane.m_iY + i);
					puts("                     ");
				}

				//gotoxy(airplane.m_iX, airplane.m_iY+3);
				//putc(' ', stdout);
			}
		}

		// Object �浹: ����
		for (int i = 0; i < 3; i++) {
			if (cloud[i].m_bUse == true) {
				if (cloud[i].m_bIsLeft == true) {
					if (cloud[i].m_iX > (76 - cloud[i].m_pGraphicColLen)) {
						cloud[i].m_bUse = false;

						for (int j = 0; j < cloud[i].m_pGraphicRowLen; j++) {
							// ���� �����
							gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// ���� �����
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
							// ���� �����
							gotoxy(cloud[i].m_iPrevX, cloud[i].m_iPrevY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// ���� �����
							gotoxy(cloud[i].m_iX, cloud[i].m_iY + j);
							for (int h = 0; h < cloud[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}
						}
					}
				}
			}
		}

		// Object �浹: ��
		for (int i = 0; i < 5; i++) {
			if (enemy[i].m_bUse == true) {
				if (enemy[i].m_iHp < 1) {
					star.m_iScore += enemy[i].m_iScore;
					star.m_iCount += 1;
				}

				if (enemy[i].m_bIsLeft == true) {
					if (enemy[i].m_iX > (76 - enemy[i].m_pGraphicColLen) || enemy[i].m_iHp < 1) {
						enemy[i].m_bUse = false;

						for (int j = 0; j < enemy[i].m_pGraphicRowLen; j++) {
							// ���� �����
							gotoxy(enemy[i].m_iPrevX, enemy[i].m_iPrevY + j);
							for (int h = 0; h < enemy[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// ���� �����
							gotoxy(enemy[i].m_iX, enemy[i].m_iY + j);
							for (int h = 0; h < enemy[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}
						}
					}
				}
				else {
					if (enemy[i].m_iX < 1 || enemy[i].m_iHp < 1) {
						enemy[i].m_bUse = false;

						for (int j = 0; j < enemy[i].m_pGraphicRowLen; j++) {
							// ���� �����
							gotoxy(enemy[i].m_iPrevX, enemy[i].m_iPrevY + j);
							for (int h = 0; h < enemy[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}

							// ���� �����
							gotoxy(enemy[i].m_iX, enemy[i].m_iY + j);
							for (int h = 0; h < enemy[i].m_pGraphicColLen; h++) {
								putc(' ', stdout);
							}
						}
					}
				}
			}
		}

		// �Ѿ� �̵�
		for (int i = 0; i < PLAYER_BULLET_COUNT; i++) {
			if (star.m_sBullet[i].m_bUse == true) {
				star.m_sBullet[i].m_iPrevX = star.m_sBullet[i].m_iX;
				star.m_sBullet[i].m_iPrevY = star.m_sBullet[i].m_iY;
				star.m_sBullet[i].m_iY -= 1;
			}
		}

		// Object �̵�: �����
		if (airplane.m_bUse == true) {
			if (tick > airplane.m_dwTick + airplane.m_dwTickLimit) {
				airplane.m_dwTick = tick;
				airplane.m_iPrevX = airplane.m_iX;
				airplane.m_iPrevY = airplane.m_iY;
				airplane.m_iX -= 1;
			}
		}

		// Object �̵�: ����
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

		// Object �̵�: ��
		for (int i = 0; i < 5; i++) {
			if (enemy[i].m_bUse == true) {
				if (tick > enemy[i].m_dwTick + enemy[i].m_dwTickLimit) {
					enemy[i].m_dwTick = tick;
					enemy[i].m_iPrevX = enemy[i].m_iX;
					enemy[i].m_iPrevY = enemy[i].m_iY;

					if (enemy[i].m_bIsLeft == true) {
						enemy[i].m_iX += 1;
					}
					else {
						enemy[i].m_iX -= 1;
					}
				}
			}

			// �� �Ѿ� �̵�
			for (int j = 0; j < OBJECT_BULLET_COUNT; j++) {
				if (tick > enemy[i].m_Bullet[j].m_dwTick + enemy[i].m_Bullet[j].m_dwTickLimit) {
					enemy[i].m_Bullet[j].m_dwTick = tick;

					if (enemy[i].m_Bullet[j].m_bUse == true) {
						enemy[i].m_Bullet[j].m_iPrevX = enemy[i].m_Bullet[j].m_iX;
						enemy[i].m_Bullet[j].m_iPrevY = enemy[i].m_Bullet[j].m_iY;
						enemy[i].m_Bullet[j].m_iY += 1;
					}
				}
			}
		}

		// Object ����: �����
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

		// Object ����: ����
		for (int i = 0; i < 3; i++) {
			if (tick > cloud[i].m_dwTick + 500) {
				cloud[i].m_dwTick = tick;
				if (rand() % 10 == 2) {
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

		// Object ����: ��
		for (int i = 0; i < 5; i++) {
			if (tick > enemy[i].m_dwTick + 500) {
				enemy[i].m_dwTick = tick;
				if (rand() % (8 * (i + 1)) == 2) {
					if (enemy[i].m_bUse == false) {
						enemy[i].m_bUse = true;
						enemy[i].m_iHp = enemy[i].m_iMaxHp;
						enemy[i].m_bIsLeft = rand() % 2;

						if (enemy[i].m_bIsLeft == true) {
							enemy[i].m_iX = 0;
							enemy[i].m_iY = rand() % 10 + 1;
						}
						else {
							enemy[i].m_iX = 77 - airplane.m_pGraphicColLen - 2;
							enemy[i].m_iY = rand() % 10 + 1;
						}
					}
				}
			}
		}

		// Object ����: ��-�Ѿ�
		for (int i = 0; i < 5; i++) {
			if (tick > enemy[i].m_dwBulletTick + 200) {
				enemy[i].m_dwBulletTick = tick;
				if (rand() % (25 / (i + 1)) <= 5) {
					if (enemy[i].m_bUse == true) {
						int count = 0;
						switch (i) {
						case 0: count = 1; break;
						case 1: count = 2; break;
						case 2: count = 2; break;
						case 3: count = 3; break;
						case 4: count = 6; break;
						}

						for (int j = 0; j < OBJECT_BULLET_COUNT && count; j++) {
							if (enemy[i].m_Bullet[j].m_bUse == false) {
								switch (i) {
								case 0:
									enemy[i].m_Bullet[j].m_bUse = true;
									enemy[i].m_Bullet[j].damage = 1;
									enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 3;
									enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY;
									enemy[i].m_Bullet[j].m_dwTick = 0;
									enemy[i].m_Bullet[j].m_dwTickLimit = 70;
									enemy[i].m_Bullet[j].m_pGraphic = '*';
									count--;
									break;
								case 1:
									enemy[i].m_Bullet[j].m_bUse = true;
									enemy[i].m_Bullet[j].damage = 1;
									enemy[i].m_Bullet[j].m_dwTick = 0;
									enemy[i].m_Bullet[j].m_dwTickLimit = 70;
									if (count == 2) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 3;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 1;
									}
									else if (count == 1) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 7;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 1;
									}
									enemy[i].m_Bullet[j].m_pGraphic = '@';
									count--;
									break;
								case 2:
									enemy[i].m_Bullet[j].m_bUse = true;
									enemy[i].m_Bullet[j].damage = 1;
									enemy[i].m_Bullet[j].m_dwTick = 0;
									enemy[i].m_Bullet[j].m_dwTickLimit = 70;
									if (count == 2) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 1;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 1;
									}
									else if (count == 1) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 6;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 1;
									}
									enemy[i].m_Bullet[j].m_pGraphic = '@';
									count--;
									break;
								case 3:
									enemy[i].m_Bullet[j].m_bUse = true;
									enemy[i].m_Bullet[j].damage = 1;
									enemy[i].m_Bullet[j].m_pGraphic = '@';
									enemy[i].m_Bullet[j].m_dwTick = 0;
									enemy[i].m_Bullet[j].m_dwTickLimit = 70;
									if (count == 3) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 2;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 2;
									}
									else if (count == 2) {
										enemy[i].m_Bullet[j].damage = 2;
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 3;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 1;
										enemy[i].m_Bullet[j].m_pGraphic = '#';
									}
									else if (count == 1) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 4;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 2;
									}
									count--;
									break;
								case 4:
									enemy[i].m_Bullet[j].m_bUse = true;
									enemy[i].m_Bullet[j].damage = 2;
									enemy[i].m_Bullet[j].m_dwTick = 0;
									enemy[i].m_Bullet[j].m_dwTickLimit = 70;
									if (count == 6) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 5;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = 'D';
									}
									else if (count == 5) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 6;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = 'A';
									}
									else if (count == 4) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 7;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = 'I';
									}
									else if (count == 3) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 16;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = 'S';
									}
									else if (count == 2) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 17;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = 'Y';
									}
									else if (count == 1) {
										enemy[i].m_Bullet[j].m_iX = enemy[i].m_iX + 18;
										enemy[i].m_Bullet[j].m_iY = enemy[i].m_iY + 3;
										enemy[i].m_Bullet[j].m_pGraphic = '!';
									}
									count--;
									break;
								}
							}
						}
					}
				}
			}
		}

		if (star.m_iHp < 1) {
			runFlag = false;
		}
		Sleep(10);
	}

	system("cls");
	puts("���� ����� ����� �����Ͽ����ϴ�.");
	printf("���� ����: %d\n", star.m_iScore);
	printf("���� ���� Ƚ��: %d\n", star.m_iCount);
	Sleep(5000);
	return 0;
}