#include<stdio.h>
#include<graphics.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment (lib,"Winmm.lib")

//窗口大小
#define WIDTH 591
#define HEIGHT 864
//bullet num
#define BULLNUM 20

//enemy num
#define ENEMYNUM 10

struct Plane
{
	int x, y;
	bool islive;
	int width, height;
	int hp;
	int type;//enemy plane type:1 big or 0 small
}role,bullet[BULLNUM],enemy[ENEMYNUM];

IMAGE background,imgrole[2],imgbullet[2],imgenemy[4];//背景，角色，子弹,enemy

//加载图片
void loadImg()
{
	//装载背景
	loadimage(&background, "./res/background.jpg");

	//装载role
	loadimage(&imgrole[0], "./res/planeNormal_1.jpg");
	loadimage(&imgrole[1], "./res/planeNormal_2.jpg");

	//load bullet
	loadimage(&imgbullet[0], "./res/bullet1.jpg");
	loadimage(&imgbullet[1], "./res/bullet2.jpg");

	//load enemy
	loadimage(&imgenemy[0], "./res/enemy_1.jpg");
	loadimage(&imgenemy[1], "./res/enemy_2.jpg");
	loadimage(&imgenemy[2], "./res/enemyPlane1.jpg");
	loadimage(&imgenemy[3], "./res/enemyPlane2.jpg");

}
//hp
void enemyHp(int i)
{
	if (rand() % 10 <3)
	{
		enemy[i].type = 1;
		enemy[i].hp = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}
	else
	{
		enemy[i].type = 0;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
}

//初始化
void Init()
{
	//初始化窗口
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);

	//播放背景音乐
	mciSendString("play res/game_music.mp3 repeat",0,0,0);

	loadImg();

	role.x = WIDTH / 2;
	role.y = HEIGHT - 300;
	role.islive = true;

	for (int i = 0; i < BULLNUM; i++)
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].islive = false;
	}

	for (int i = 0; i < ENEMYNUM; i++)
	{
		enemy[i].islive = false;
		enemyHp(i);
	}

}
//Timer
bool Timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}

//create bullet
void createbullet()
{
	for (int i = 0; i < BULLNUM; i++)
	{
		if (!bullet[i].islive)
		{
			bullet[i].x = role.x+60;
			bullet[i].y = role.y;
			bullet[i].islive = true;

			mciSendString("play res/f_gun.mp3 ", 0, 0, 0);
		
			break;
		}
	}
}

//shoot bullet
void movebullet(int speed)
{
	for (int i = 0; i < BULLNUM; i++)
	{
		if (bullet[i].islive)
		{
			bullet[i].y -= speed;	
			if (bullet[i].y < 0)
				bullet[i].islive = false;
		}
	}
}
//control
void rolemove(int speed)
{
	if ((GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) && role.islive)
	{
		if (role.y > 0)
		{
			role.y -= speed;
		}
		
	}
		

	if ((GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) && role.islive)
	{
		if (role.y <= HEIGHT - 120)
		{
			role.y += speed;
		}
		
	}
		

	if ((GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) && role.islive)
	{
		if (role.x+60>= 0)
		{
			role.x -= speed;
		}
		
	}
		

	if ((GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) && role.islive)
	{
		if (role.x <= WIDTH - 60)
		{
			role.x += speed;
		}	
	}

	if (GetAsyncKeyState(VK_SPACE) && Timer(100,1))
	{
		createbullet();
	}
}
//create enemy
void createenmey()
{
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (!enemy[i].islive)
		{
			enemy[i].islive = true;
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHp(i);
			break;
		}		
	}
}
//move enemy
void moveenemy(int speed)
{
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (enemy[i].islive)
		{
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].islive = false;
			}
		}
	}
}
//绘制
void gamedraw()
{
	
	putimage(0, 0, &background);//显示背景

	putimage(role.x, role.y, &imgrole[0], NOTSRCERASE);
	putimage(role.x, role.y, &imgrole[1],SRCINVERT);

	for (int i = 0; i < BULLNUM; i++)
	{
		if (bullet[i].islive)
		{
			putimage(bullet[i].x, bullet[i].y, &imgbullet[0], NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &imgbullet[1], SRCINVERT);
		}
	}
	
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (enemy[i].islive)
		{
			if (enemy[i].type)
			{
				putimage(enemy[i].x,enemy[i].y, &imgenemy[2], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &imgenemy[3], SRCINVERT);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &imgenemy[0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &imgenemy[1], SRCINVERT);
			}
		}
	}
}

//play plane
void playplane()
{
	for (int i = 0; i < ENEMYNUM; i++)
	{
		if (!enemy[i].islive)
			continue;
		for (int j = 0; j < BULLNUM; j++)
		{
			if (!bullet[i].islive)
				continue;
			if (bullet[j].x >= enemy[i].x && bullet[j].x <= enemy[i].x + enemy[i].width
				&& bullet[j].y >= enemy[i].y && bullet[j].y <= enemy[i].y + enemy[i].height)
			{
				bullet[j].islive = false;
				enemy[i].hp--;
			}	
		}
		if (enemy[i].hp<=0)
		{
			enemy[i].islive = false;

			mciSendString("play res/gotEnemy.mp3", 0, 0, 0);
		}
	}
}
int main()
{
	Init();//初始化
	
	BeginBatchDraw();//双缓冲绘图
	while (1)
	{
		gamedraw();
		rolemove(1);
		movebullet(2);

		if(Timer(200,0))
		createenmey();

		moveenemy(1);
		playplane();

		
		FlushBatchDraw();
	}
	EndBatchDraw();
	
	system("pause");
	return 0;
}