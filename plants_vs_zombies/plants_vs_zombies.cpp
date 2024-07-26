#define _CRT_SECURE_NO_WARNINGS 1
#include "plants_vs_zombies.h"

// ��PNGͼ��ȾΪ͸��Ч������ֹ����һ�ַ�����������ʱʹ��
#pragma comment( lib, "MSIMG32.LIB")
void putimageForPNG(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// ʹ�� Windows GDI ����ʵ��͸��λͼ
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}

// ����PNGͼ��ȥ͸������
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}

// ������ y <0 �Լ�x<0���κ����
void putimagePNG(int x, int y, IMAGE* picture) {

	IMAGE imgTmp, imgTmp2, imgTmp3;
	int winWidth = getwidth();
	int winHeight = getheight();
	if (y < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp, 0, -y,
			picture->getwidth(), picture->getheight() + y);
		SetWorkingImage();
		y = 0;
		picture = &imgTmp;
	}
	else if (y >= getheight() || x >= getwidth()) {
		return;
	}
	else if (y + picture->getheight() > winHeight) {
		SetWorkingImage(picture);
		getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
		SetWorkingImage();
		picture = &imgTmp;
	}

	if (x < 0) {
		SetWorkingImage(picture);
		getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
		SetWorkingImage();
		x = 0;
		picture = &imgTmp2;
	}

	if (x > winWidth - picture->getwidth()) {
		SetWorkingImage(picture);
		getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
		SetWorkingImage();
		picture = &imgTmp3;
	}


	_putimagePNG(x, y, picture);
}

// ��ȡ�ӳ�ʱ��
int getDelay() {
	static unsigned long long firstTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (firstTime == 0) {
		// ��������ʱ��Ϊ��ǰʱ��
		firstTime = currentTime;
		return 0;
	}
	else {
		// ��ȡ����ǰʱ������һ��ʱ��Ĳ�ֵ
		int ret = currentTime - firstTime;
		// ��������ʱ��Ϊ��ǰʱ��
		firstTime = currentTime;
		return ret;
	}
}

// �ж��Ƿ��ȡ����Ч�ļ�
bool ifExists(char* plants_name)
{
	FILE* fp = fopen(plants_name, "r");
	if (fp)
	{
		fclose(fp);
	}
	return fp != NULL; // ��Ϊ�շ���true�������ļ�����
}

// �����������汳������
void StartBackgroundMusic()
{
	// ���������ļ�
	Mix_Chunk* background = Mix_LoadWAV("res/music/start.wav");
	if (!background) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, background, -1);
}

// ���ز˵�����
void ClickMenuMusic()
{
	// ���������ļ�
	Mix_Chunk* menu = Mix_LoadWAV("res/music/clickMenu.wav");
	if (!menu) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, menu, 0);
}

// ������Ϸ���̱�������
void GamingBackgroundMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/grass.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, -1);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 80);
}

// ѡ��ֲ�ﱳ������
void ChoosePlantMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/choose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);
}

// ֲ����ֲ����
void PlantsCultivate()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/plant.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);
}

// �����ռ�����
void CollectSunshineMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/sunshine.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);
}

void FailChoosePlantMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/failToChoose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);
}

void ZombiesComingMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/coming.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);
}

void ZombiesGroanMusic()
{
	// ���������ļ�
	Mix_Chunk* sound2 = Mix_LoadWAV("res/music/groan2.wav");
	Mix_Chunk* sound3 = Mix_LoadWAV("res/music/groan3.wav");
	Mix_Chunk* sound4 = Mix_LoadWAV("res/music/groan4.wav");
	Mix_Chunk* sound5 = Mix_LoadWAV("res/music/groan5.wav");
	if (!sound2) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
	if (!sound3) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
	if (!sound4) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}
	if (!sound5) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	static int playCount = 0;
	playCount = rand() % 100;
	if (playCount > 10)
	{
		// ��ʼ��������
		Mix_PlayChannel(-1, sound2, 0);
	}
	else if (playCount > 40)
	{
		// ��ʼ��������
		Mix_PlayChannel(-1, sound3, 0);
	}
	else if (playCount > 70)
	{
		// ��ʼ��������
		Mix_PlayChannel(-1, sound4, 0);
	}
	else
	{
		// ��ʼ��������
		Mix_PlayChannel(-1, sound5, 0);
	}
}

// �㶹�ӵ���ײ����
void PeaShooterBulletCollideMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/hit.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
}

// ��ʬ��ֲ������
void ZombieEatingMusic()
{
	// ���������ļ�
	Mix_Chunk* sound = Mix_LoadWAV("res/music/eating.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	Mix_VolumeChunk(sound, 50);
}

// ��ʼ���˵�����
void StartInit()
{
	// ���ؿ�ʼ���汳��
	loadimage(&imgstart, "res/MainMenu.png");
	// Ĭ��δ���״̬
	status_leftClick = 0;

	// ����ѡ��
	loadimage(&imgMenu, "res/menu1.png");
	loadimage(&imgMenuClicked, "res/menu2.png");
}

// ��ʼ����ͼƬ��Ⱦ
void ImageRenderStart()
{
	// ��Ⱦ��ʼ�˵�
	putimagePNG(0, 0, &imgstart);
	// ��ȾĬ�ϲ˵���ѡ�в˵�
	putimagePNG(474, 75, status_leftClick ? &imgMenuClicked : &imgMenu);
}

// ��ʼ��Ϸ����
void GamingInit()
{
	// ������Ϸ����ͼƬ������
	loadimage(&imgBackground, "res/bg.jpg");

	// ����ֲ����ͼƬ������
	loadimage(&imgPlantsBar, "res/bar5.png");

	// ��ʼ������������
	status_leftClick = 0;
	// ��ʼ������ֲ��
	curPlant = -1;
	// �����ʼֵ
	sunshineScore = 300;

	// ������ק�����е�ֵΪNULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
	// ����ֲ���ͼ�е�ֵΪ-1
	memset(map, -1, sizeof(map));
	// ��ʼ�����������е�����ֵΪ0
	memset(sunshine_sky, 0, sizeof(sunshine_sky));
	// ��ʼ����ʬ�����е�����ֵΪ0
	memset(zombies, 0, sizeof(zombies));
	// �����㶹�ӵ����������ֵΪ0
	memset(peaShooterBullets, 0, sizeof(peaShooterBullets));

	// ����ֲ�￨Ƭ������
	char plants_name[64] = { 0 }; // ͨ���ֲ�������¼ֲ�￨���ļ�����
	for (int i = 0; i < PlantsCount; i++)
	{
		// ʹ��sprintf������ֲ�����ƶ�ȡ��plants_name��
		sprintf(plants_name, "res/Cards/card_%d.png", i + 1);
		// ����ͼƬ��ֲ��������
		loadimage(&imgPlants[i], plants_name);
	}

	// ����ֲ���˶���קʱ��ͼƬ
	for (int i = 0; i < PlantsCount; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			// sprintf�ڵڶ���forѭ���ڶ�ȡ������NULLΪֹ
			sprintf(plants_name, "res/plants_move/%d/%d.png", i, j + 1);
			if (ifExists(plants_name))
			{
				imgPlantsMove[i][j] = new IMAGE;
				loadimage(imgPlantsMove[i][j], plants_name);
			}
			else
			{
				break;
			}
		}
	}

	// ��ȡС�Ƴ���Ƭ
	for (int i = 0; i < CARNUM; i++)
	{
		loadimage(&imgCar[i], "res/Screen/car.png");
	}

	// ��ȡ���⵽����ͼƬ֡����
	char sun_name[64] = { 0 };
	for (int i = 0; i < 29; i++)
	{
		sprintf(sun_name, "res/sunshine/%d.png", i + 1);
		loadimage(&imgSunFrameIndex[i], sun_name);
	}

	// ��ȡ��ʬ����ʬ����֡������
	char zombie_name[64] = { 0 };
	for (int i = 0; i < 22; i++)
	{
		sprintf(zombie_name, "res/zm/%d.png", i + 1);
		loadimage(&imgZombieFrameIndex[i], zombie_name);
	}

	// �����㶹�ӵ�ͼƬ
	loadimage(&imgPeaShooterBullets, "res/bullets/bullet_normal.png");

	// �����㶹��ըͼƬ

	loadimage(&imgPeaShooterBulletsExploded, "res/bullets/bullet_blast.png",
		imgPeaShooterBulletsExploded.getwidth() * 0.2,
		imgPeaShooterBulletsExploded.getheight() * 0.2,
		true);


	// ���ؽ�ʬ����ͼƬ֡��Ƭ
	char zombie_dead_name[64] = { 0 };
	for (int i = 0; i < 11; i++)
	{
		sprintf(zombie_dead_name, "res/zm_dead/Image%d.png", i + 1);
		loadimage(&imgZombieDead[i], zombie_dead_name);
	}

	// ���ؽ�ʬ��ֲ�ﶯ��֡��Ƭ
	char zombie_eat_name[64] = { 0 };
	for (int i = 0; i < 21; i++)
	{
		sprintf(zombie_eat_name, "res/zm_eat/%d.png", i + 1);
		loadimage(&imgZombieEat[i], zombie_eat_name);
	}

	// ��������
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 30;
	font.lfWeight = 50;
	strcpy(font.lfFaceName, "Segoe UI BLACK");
	font.lfQuality = ANTIALIASED_QUALITY; // �����Ч��
	settextstyle(&font);
	setbkmode(TRANSPARENT); // ����͸��
	settextcolor(BLACK); // ������ɫΪ��ɫ
}

// ͼƬ��Ⱦ
void ImageRenderGaming()
{
	// ��ʼ����
	BeginBatchDraw();

	// ��Ⱦ����
	putimage(0, 0, &imgBackground);

	// ��Ⱦֲ����
	putimageForPNG(NULL, 250, 0, &imgPlantsBar, BLACK);

	// ��Ⱦֲ��
	int x = 0;// ����x����
	int y = 6; // ����y����
	for (int i = 0; i < PlantsCount; i++)
	{
		x = 340 + i * 65;
		putimageForPNG(NULL, x, y, &imgPlants[i], BLACK);
	}

	// ��Ⱦ��ֲͼ
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			// ��ֲ��ʱ��Ⱦ
			if (map[i][j].type > -1 && map[i][j].eaten == 0)
			{
				// �����x��y������Ϊ��Ⱦλ��
				int x = 257 + j * 81;
				int y = 179 + i * 102;
				putimageForPNG(NULL, x, y, imgPlantsMove[map[i][j].type][map[i][j].frameIndex], BLACK);
			}
		}
	}

	//��Ⱦ��ʬ
	for (int i = 0; i < ZOMBIENUM; i++)
	{
		if (zombies[i].isDead)
		{
			IMAGE img = imgZombieDead[0];
			putimageForPNG(NULL, zombies[i].x, zombies[i].y - img.getheight(),
				&imgZombieDead[zombies[i].frameIndex], BLACK);
		}
		else if (zombies[i].isEat)
		{
			IMAGE img = imgZombieEat[0];
			putimageForPNG(NULL, zombies[i].x, zombies[i].y - img.getheight(),
				&imgZombieEat[zombies[i].frameIndex], BLACK);
		}
		else
		{
			IMAGE img = imgZombieFrameIndex[0];
			// �ӽ�ʬ�ŵ�λ����Ⱦ
			putimageForPNG(NULL, zombies[i].x, zombies[i].y - img.getheight(),
				&imgZombieFrameIndex[zombies[i].frameIndex], BLACK);
		}
	}

	// ��Ⱦ�㶹�ӵ�
	for (int i = 0; i < PEASHOOTERBULLETNUM; i++)
	{
		if (peaShooterBullets[i].isUse)
		{
			putimageForPNG(NULL, peaShooterBullets[i].x, peaShooterBullets[i].y, &imgPeaShooterBullets, BLACK);
		}
		else if (peaShooterBullets[i].isExplode)
		{
			putimageForPNG(NULL, peaShooterBullets[i].x, peaShooterBullets[i].y, &imgPeaShooterBulletsExploded, BLACK);
		}
	}

	// ��ȾС�Ƴ�
	x = 195;
	y = 0;
	for (int i = 0; i < CARNUM; i++)
	{
		y = 210 + i * 100;
		putimageForPNG(NULL, x, y, &imgCar[i], BLACK);
	}

	// ��Ⱦ��קͼ������������ֲ�����ʵ����קͼ�����Ѿ���ֲ��ֲ��ͼ�Ϸ�
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// ȡ������֡��Ƭ�ĵ�һ��
		putimageForPNG(NULL, curX - dragged->getwidth() / 2,
			curY - dragged->getheight() / 2, dragged, BLACK);
	}

	// ����������ʾλ��
	char scoreText[5];
	sprintf(scoreText, "%d", sunshineScore);
	outtextxy(285, 67, scoreText);

	// ��Ⱦ����
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse || sunshine_sky[i].isClick)
		{
			putimageForPNG(NULL, sunshine_sky[i].x, sunshine_sky[i].y,
				&imgSunFrameIndex[sunshine_sky[i].frameIndex], BLACK);
		}
	}

	// ��������
	EndBatchDraw();
}

// ������Ϸ�����еĲ���
void MouseActionGaming()
{
	// ���������Ϣ
	ExMessage msg;
	// �������ж�������ʼ�������򲻴���
	if (peekmessage(&msg))
	{
		// ����������
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96) // ѡֲ��
			{
				int index = (msg.x - 340) / 66;// ʹ��66����Ƚϱ�Ե��λ��
				//std::cout << index << std::endl;
				status_leftClick = 1;// �����������Ϊ1
				// ��¼�����λ�ã����ֲ����ֲ��Ϻ���ֵ�ֲ��ͼƬ�ͺ�����
				curX = msg.x;
				curY = msg.y;

				// �����ǰӵ�е������������ڵ��ֲ�������������ѡ�У������޷�ѡ��
				ChoosePlant(index);
			}
			else // �ռ�����
			{
				CollectSunshine(&msg);
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status_leftClick) // �����ק
		{
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP && curPlant > -1)// ���������𣬲����ж���ֲ��ʱ�Ž��룬���򲻴�����Ӧ��Ч
		{
			if (msg.x >= 257 && msg.x <= 985 && msg.y >= 179 && msg.y <= 465)
			{
				// ͨ��x��y�����к���
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 257) / 81;
				//std::cout << col << ", " << row << std::endl;

				// ����ָ��λ�õ�ֲ�����ͺͶ���֡
				if (map[row][col].type == -1)
				{
					map[row][col].type = curPlant;
					map[row][col].frameIndex = 0;
					map[row][col].x = col;
					map[row][col].y = row;
					// ����ֲ��Ѫ��
					map[row][col].blood = PLANTBLOOD;
					// ��ʼֲ��δ���Ե�
					map[row][col].eaten = 0;
					PlantsCultivate();
				}

				// �������״̬Ϊ0
				status_leftClick = 0;
				// ����ǰֲ������Ϊ-1
				curPlant = -1;
			}
		}
	}
}

// ����ֲ���˶�
void UpdatePlantsMove()
{
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			if (map[i][j].type > -1 && map[i][j].eaten == 0)
			{
				// ����ֲ���˶�
				map[i][j].frameIndex++;
				if (imgPlantsMove[map[i][j].type][map[i][j].frameIndex] == NULL)
				{
					map[i][j].frameIndex = 0;
				}
			}
		}
	}
}

// ��������
void CreateSunshine()
{
	static int count = 0;// ���ƴ���
	static int frequent = 50;// �������������Ƶ��
	count++;// ��¼���ô���
	if (count >= frequent)
	{
		// ͨ��������������������Ƶ��
		frequent = 80; // [100, 199]
		count = 0;// ���¼���
		// ���ҵ����õ�������±�
		int i = 0;
		while (i < SUNSHINENUM && sunshine_sky[i].isUse)
		{
			i++;
		}

		// ��ʱ�������������1. ���ڿ������� 2. �޿���ʹ�õ�����
		// �����һ���������ֹ�ڶ������
		if (i < SUNSHINENUM)
		{
			// ���������������
			sunshine_sky[i].x = 260 + rand() % (900 - 260); // [260, 899]
			sunshine_sky[i].y = 170;
			sunshine_sky[i].dest = 200 + (1 + rand() % 3) * 100; // [200, 500]
			sunshine_sky[i].frameIndex = 0; // �ӵ�һ��֡��Ƭ��ʼ
			sunshine_sky[i].timer = 0;
			// ������״̬����Ϊ0
			sunshine_sky[i].isClick = 0;
			// ������ʹ��״̬��Ϊ1
			sunshine_sky[i].isUse = 1;
			// ��ʼ��ƫ����
			sunshine_sky[i].xoffset = 0;
			sunshine_sky[i].yoffset = 0;
		}
	}
}

// ��������
void UpdateSunshine()
{
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse)
		{
			// ��������ͼƬ֡
			sunshine_sky[i].frameIndex = (sunshine_sky[i].frameIndex + 1) % 29;
			// ����y����
			if (sunshine_sky[i].timer == 0)
			{
				sunshine_sky[i].y += 6;
			}
			// ����destλ�ã�����ͣ��һ��ʱ�䣬����������
			if (sunshine_sky[i].y >= sunshine_sky[i].dest)
			{
				sunshine_sky[i].timer++;
				if (sunshine_sky[i].timer >= 120)
				{
					sunshine_sky[i].timer = 0;
					sunshine_sky[i].isUse = 0;
				}
			}
		}
		else if (sunshine_sky[i].isClick)
		{
			double angle = atan(sunshine_sky[i].y / (sunshine_sky[i].x - 262));
			sunshine_sky[i].xoffset = 60 * cos(angle);
			sunshine_sky[i].yoffset = 60 * sin(angle);

			// ����Ƕ�
			sunshine_sky[i].x -= sunshine_sky[i].xoffset;
			sunshine_sky[i].y -= sunshine_sky[i].yoffset;

			if (sunshine_sky[i].x <= 262 || sunshine_sky[i].y <= 0)
			{
				sunshine_sky[i].isClick = 0;
				sunshineScore += PERSUNSHINE;
			}
		}
	}
}

// �ռ�����
void CollectSunshine(ExMessage* msg)
{
	// �ҵ�����ʹ�õ�����
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse)
		{
			int width = imgSunFrameIndex[0].getwidth();// ��ȡ��̫���Ŀ��
			int height = imgSunFrameIndex[0].getheight(); // ��ȡ̫���ĸ߶�
			// �ж��Ƿ�������Ŀɵ����Χ��
			if (msg->x >= sunshine_sky[i].x && msg->x <= sunshine_sky[i].x + width &&
				msg->y >= sunshine_sky[i].y && msg->y <= sunshine_sky[i].y + height)
			{
				// �ռ�����
				sunshine_sky[i].isClick = 1;
				// ��������״̬Ϊ0
				sunshine_sky[i].isUse = 0;
				// �����ռ���������
				CollectSunshineMusic();
			}
		}
		else if (sunshine_sky[i].isClick)
		{
			// ����б�ʦ�
			double angle = atan(sunshine_sky[i].y / (sunshine_sky[i].x - 262));
			sunshine_sky[i].xoffset = 60 * cos(angle);
			sunshine_sky[i].yoffset = 60 * sin(angle);
		}
	}
}

// ѡ��ֲ��
void ChoosePlant(int index)
{
	if (index == peaShooter.type)// ѡ���㶹����
	{
		if (sunshineScore >= peaShooter.sunshine)// �Ƿ������㶹������Ҫ����������
		{
			// if��ֹͬһ��ֲ����θ�ֵ
			if (curPlant != index)
			{
				// ����curPlant
				curPlant = index;
				// ��������
				sunshineScore -= peaShooter.sunshine;
				// ����ѡ�е�����
				ChoosePlantMusic();
			}
		}
		else
		{
			// ���������޷�ѡ��
			FailChoosePlantMusic();
		}
	}
	else if (index == sunflower.type)// ѡ�����տ�
	{
		if (sunshineScore >= sunflower.sunshine)
		{
			if (curPlant != index)
			{
				sunshineScore -= sunflower.sunshine;
				curPlant = index;
				ChoosePlantMusic();
			}
		}
		else
		{
			FailChoosePlantMusic();
		}
	}
}

// ������ʬ
void CreateZombies()
{
	static int frequent = 100; // ���ƽ�ʬ����Ƶ��
	static int count = 0; // ����ִ�д���
	static int comingSign = 0; // ��־��һ����ʬ����
	count++;
	if (count >= frequent)
	{
		frequent = 100;
		count = 0;
		// �ҵ�δʹ�õĽ�ʬ
		int i = 0;
		while (i < ZOMBIENUM && zombies[i].isUse)
		{
			i++;
		}

		if (i < ZOMBIENUM)
		{
			// ���½�ʬ����
			zombies[i].x = 900;
			zombies[i].row = rand() % 3;// ��ʼ����ʬ������
			zombies[i].y = 170 + (1 + zombies[i].row) * 100;
			zombies[i].frameIndex = 0;
			zombies[i].isUse = 1;
			zombies[i].speed = 1;
			// ���ý�ʬѪ��
			zombies[i].blood = 100;
			// ���ý�ʬ��ʼ״̬
			zombies[i].isDead = 0;
			zombies[i].isEat = 0;
			// ��־��һ����ʬ����
			comingSign++;
		}

		if (comingSign == 1)
		{
			ZombiesComingMusic();
		}
	}
}

// ��ʬ�ƶ�
void UpdateZombies()
{
	static int count = 0;// count���Ƶ��ô���
	count++;
	if (count >= 1) // ÿ���ε��ý�ʬ����һ��
	{
		count = 0;
		for (int i = 0; i < ZOMBIENUM; i++)
		{
			if (zombies[i].isUse)
			{
				if (zombies[i].blood > 0)
				{
					if (zombies[i].isEat == 0)
					{
						// ���¶���֡��Ƭ
						zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 22;
						// ���½�ʬλ��
						zombies[i].x -= zombies[i].speed;
						static int playcount = 0;
						static int playFrequent = 150;
						playcount++;
						if (playcount > playFrequent)
						{
							playcount = 0;
							playFrequent += rand() % 200 + 50;
							ZombiesGroanMusic();
						}
					}
					CheckZombieCollision();
					if (zombies[i].isEat) 
					{
						zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 21;
					}
					// ��ʬ�����ƺ��߽�
					if (zombies[i].x <= 150)
					{
						// �ж��Ƿ���С�Ƴ��������ƶ�С�Ƴ���û������Ϸ����
						// С�Ƴ���δʵ��
						exit(0);// ��Ϸ����
					}
				}
				else if (zombies[i].isDead)
				{
					zombies[i].frameIndex++;
					if (zombies[i].frameIndex >= 11)
					{
						zombies[i].isUse = 0;
					}
				}
			}

		}
	}
}

// �����㶹�ӵ�
void CreatePeaShooterBullets()
{
	// ���ҵ��н�ʬ����λ��
	int zombieRow[3] = { 0 };
	for (int i = 0; i < ZOMBIENUM; i++)
	{
		// ���ڽ�ʬ���ҽ�ʬԽ��������ʱ��¼
		if (zombies[i].isUse)
		{
			zombieRow[zombies[i].row] = 1;// ���ڽ�ʬʱ�����б��Ϊ1
		}
	}

	// �ҵ����㶹���ֵ�λ�ã������㶹�ӵ�
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			// �ҵ��㶹���ֵ�λ�ò���ȷ���㶹���ֵ�λ���뽩ʬ��λ����ͬһ��
			if (map[i][j].type == peaShooter.type && zombieRow[i] && map[i][j].eaten == 0)
			{
				// ����ִ�д���
				static int counts[MAPROW] = { 0 }; // ÿ�еļ�����
				counts[i]++;
				if (counts[i] > 20)
				{
					counts[i] = 0;
					int k = 0;
					// �ҵ�����
					while (k < PEASHOOTERBULLETNUM && peaShooterBullets[k].isUse)
					{
						k++;
					}

					// ������ǰ�е��㶹�ӵ�
					// ����ֲ����ֲλ���ж��ӵ�����λ��
					int x = 257 + j * 81;
					int y = 179 + i * 102;
					peaShooterBullets[k].x = x + imgPlantsMove[Pea_Shooter][0]->getwidth() - 15;
					peaShooterBullets[k].y = y + 5;

					// ��ʼ���ӵ��������
					peaShooterBullets[k].row = i; // �㶹�����м��㶹�ӵ�������
					peaShooterBullets[k].isUse = 1;
					// ��ʼ�������ӵ�ʱ����ȫ�������ӵ��ı�ը״̬ȫ����Ϊ0����ֹ���ֲ����ӵ���ײ����ڻ�����
					for (int k = 0; k < PEASHOOTERBULLETNUM; k++)
					{
						peaShooterBullets[k].isExplode = 0;
					}
					peaShooterBullets[k].speed = 30;
				}
			}
		}
	}
}

// �����ӵ�
void UpdatePeaShooterBullets()
{
	// �ҵ�����ʹ�õ��ӵ�
	for (int i = 0; i < PEASHOOTERBULLETNUM; i++)
	{
		if (peaShooterBullets[i].isUse && peaShooterBullets[i].isExplode == 0)
		{
			peaShooterBullets[i].x += peaShooterBullets[i].speed;
			// �ӵ���ײ���
			CheckPeaShooterBulletsCollision();
			// ����ӵ�Խ�������߻���������ʬ����
			if (peaShooterBullets[i].x > PEASHOOTERSAFETYLINE || peaShooterBullets[i].isExplode)
			{
				peaShooterBullets[i].isUse = 0;
			}
		}
	}
}

// �㶹�ӵ���ײ���
void CheckPeaShooterBulletsCollision()
{
	// �ҵ����õ��㶹�ӵ�
	for (int i = 0; i < PEASHOOTERBULLETNUM; i++)
	{
		if (peaShooterBullets[i].isUse && peaShooterBullets[i].isExplode == 0)
		{
			// �ҵ����õĽ�ʬ
			for (int j = 0; j < ZOMBIENUM; j++)
			{
				if (zombies[j].isUse)
				{
					// �����ʬ���ӵ���ͬһ�У����ж��Ƿ��Ѿ��ִ���ײ��Χ
					if (peaShooterBullets[i].row == zombies[j].row)
					{
						if (peaShooterBullets[i].x >= zombies[j].x + 60 && peaShooterBullets[i].x <= zombies[j].x + 110)
						{
							// �ӵ���ײ
							// ��ʬѪ������
							zombies[j].blood -= PEASHOOTERBULLETDAMAGE;
							// printf("%d %d %d\n", peaShooterBullets[i].row, zombies[j].row, zombies[j].blood);
							peaShooterBullets[i].isExplode = 1;
							peaShooterBullets[i].speed = 0;
							// �����ӵ���ʹ��״̬
							peaShooterBullets[i].isUse = 0;
							// ��ʬû��֮ǰ������ײ��Ч
							if (zombies[j].blood >= 0)
							{
								PeaShooterBulletCollideMusic();
							}
							if (zombies[j].blood == 0)
							{
								// ��ʬ������
								zombies[j].speed = 0;
								zombies[j].isDead = 1;
								// ��ʬ����������ͼƬ֡
								zombies[j].frameIndex = 0;
							}
							// ��ǰ���ӵ����е�ǰ��ʬ����Ҫ�ȽϺ�����ӵ��Ƿ����
							break;
						}
					}
				}
			}
		}
	}
}

// ��ʬ����ֲ����
void CheckZombieCollision()
{
	// �ҵ���ʬ������
	for (int i = 0; i < ZOMBIENUM; i++)
	{
		int row = zombies[i].row;
		// �������ҵ�ǰ���Ƿ���ֲ��
		for (int j = 0; j < MAPCOL; j++)
		{
			// ��ʬ���ֵ��д���ֲ��
			if (map[row][j].type > -1 && map[row][j].eaten == 0)
			{
				// ���ֲ�������
				int left = 257 + j * 81 + 10;
				int right = 257 + j * 81 + 60;
				// �����ʬ�����žͳ�ֲ��
				if (zombies[i].isDead == 0 && zombies[i].x + 80 >= left && zombies[i].x + 80 <= right)
				{
					// ��ʬ����֡����ʱ������
					static int frameMove = 0;
					frameMove++;
					if (frameMove == 1)
					{
						// ���Ľ�ʬ�Ķ���֡
						zombies[i].frameIndex = 0;
					}
					// ��ʬ״̬����
					zombies[i].isEat = 1;
					// ��ֲ��ʱ��ʬ���ƶ�
					zombies[i].speed = 0;
					// ���ƽ�ʬ��ֲ����ٶ�
					static int count = 0;
					count++;
					if (count > 25)
					{
						count = 0;
						// ֲ��Ѫ������
						map[row][j].blood -= EATDAMAGE;
						printf("%d %d %d\n", row, j, map[row][j].blood);
					}
					if (map[row][j].blood > 0)
					{
						ZombieEatingMusic();
					}
					if (map[row][j].blood == 0)
					{
						// ֲ�ﱻ�Ե�
						map[row][j].eaten = 1;
						// ��ʬ�ָ���ʽ
						for (int k = 0; k < ZOMBIENUM; k++)
						{
							// �ҵ����ڳԵ���ֲ���Ѿ�û�˵Ľ�ʬ�ָ���״̬
							if (zombies[k].isEat && map[row][j].eaten)
							{
								zombies[k].frameIndex = 0;
								zombies[k].speed = 1;
								zombies[k].isEat = 0;
							}
						}
						// ��֡���¹���
						frameMove = 0;
						// ��������ѷ�����ӵ���ֹ�㶹�����ӵ�ͣ��
						for (int k = 0; k < PEASHOOTERBULLETNUM; k++)
						{
							peaShooterBullets[k].isExplode = 0;
						}
					}
				}
			}
		}
	}
}

// ��Ϸ��ʼ����˵�
void GameStartMenu()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// ��ʼ��Mixer��ϵͳ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ���ؿ�ʼ����
	StartInit();

	// ���ؿ�ʼ��������
	StartBackgroundMusic();

	while (1)
	{
		// ����������
		ExMessage msg;
		BeginBatchDraw();
		// ��ȾͼƬ
		ImageRenderStart();
		EndBatchDraw();
		if (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				if (msg.x >= 474 && msg.x <= 774 && msg.y >= 75 && msg.y <= 215)
				{
					status_leftClick = 1;
					ClickMenuMusic();
				}
			}
			else if (msg.message == WM_LBUTTONUP && status_leftClick)
			{
				status_leftClick = 0;
				break;
			}
		}
	}

	// �ر���������
	Mix_CloseAudio();
	Mix_CloseAudio();

	return;
}

// ������Ϸ����
void UpdateGameData()
{
	// ����ֲ�ﶯ��
	UpdatePlantsMove();
	// ��������
	CreateSunshine();
	// ��������
	UpdateSunshine();
	// ������ʬ
	CreateZombies();
	// ��ʬ�ƶ�
	UpdateZombies();
	// �����㶹�ӵ�
	CreatePeaShooterBullets();
	// �����㶹�ӵ�
	UpdatePeaShooterBullets();
}

// ��Ϸ����
void Gaming()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// ��ʼ��Mixer��ϵͳ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// ������Ϸ��������
	GamingBackgroundMusic();
	int time = 0;
	// ������ʾͼƬ����ȡ�û��Ĳ���
	while (1)
	{
		// ����������
		MouseActionGaming();

		// ��ȾͼƬ
		ImageRenderGaming();

		// ������Ϸ����
		time += getDelay();
		if (time > 100)
		{
			UpdateGameData();
			time = 0;
		}
	}

	Mix_CloseAudio();
	Mix_CloseAudio();
}