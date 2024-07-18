#define _CRT_SECURE_NO_WARNINGS 1
#include "plants_vs_zombies.h"
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

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP����Ҷ˹���������е���ɫ�ĸ��ʼ���
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
void putimageForPNG(int x, int y, IMAGE* picture) {

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

int getDelay() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
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

// ���ر�������
Mix_Music* GameBackgroundMusic()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// ��ʼ��Mixer��ϵͳ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ���������ļ�
	Mix_Music* music = Mix_LoadMUS("res/music/grass.mp3");
	if (!music) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	if (Mix_PlayMusic(music, -1) < 0) {
		printf("Unable to play music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	return music;
}

// ������Դ�ͷ�
void GameBackgroundMusicDestroy(Mix_Music** music)
{
	// ������Դ
	Mix_FreeMusic(*music);
	*music = nullptr;
	Mix_CloseAudio();
	SDL_Quit();
}

// ��ʼ��Ϸ����
void GameInit()
{
	// ������Ϸ����ͼƬ������
	loadimage(&imgBackground, "res/bg.jpg");
	// �������ڴ�С����
	initgraph(WIDTH, HEIGHT, 1);

	// ����ֲ����ͼƬ������
	loadimage(&imgPlantsBar, "res/bar5.png");

	// ������ק�����е�ֵΪNULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
	// ��ʼ������ֲ��
	curPlant = -1;
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
}

// ͼƬ��Ⱦ
void ImageRender()
{
	// ��ʼ����
	BeginBatchDraw();

	// ��Ⱦ����
	putimage(0, 0, &imgBackground);

	// ��Ⱦֲ����
	putimageForPNG(250, 0, &imgPlantsBar);

	// ��Ⱦֲ��
	int x = 0;// ����x����
	int y = 6; // ����y����
	for (int i = 0; i < PlantsCount; i++)
	{
		x = 340 + i * 65;
		putimage(x, y, &imgPlants[i]);
	}

	// ��Ⱦ��קͼ
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// ȡ������֡��Ƭ�ĵ�һ��
		putimageForPNG(curX - dragged->getwidth() / 2, 
			curY - dragged->getheight() / 2, dragged);
	}

	// ��������
	EndBatchDraw();
}

void MouseAction()
{
	// ���������Ϣ
	ExMessage msg;
	// ������¼�Ƿ������
	static int status_leftClick = 0;
	// �������ж�������ʼ�������򲻴���
	if(peekmessage(&msg))
	{
		// ����������
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96)
			{
				int index = (msg.x - 340) / 66;// ʹ��66����Ƚϱ�Ե��λ��
				//std::cout << index << std::endl;
				status_leftClick = 1;// �����������Ϊ1
				curPlant = index;// ����ѡ�е�ֲ��
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status_leftClick) // �����ק
		{
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP)// ����������
		{
			int x = msg.x;
			int y = msg.y;


		}
	}
}
