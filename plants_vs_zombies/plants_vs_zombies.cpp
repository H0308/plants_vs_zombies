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
Mix_Chunk* StartBackgroundMusic()
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
	Mix_Chunk* background = Mix_LoadWAV("res/music/start.wav");
	if (!background) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, background, -1);
	return background;
}

// ���ز˵�����
Mix_Chunk* ClickMenuMusic()
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
	Mix_Chunk* menu = Mix_LoadWAV("res/music/clickMenu.wav");
	if (!menu) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, menu, 0);

	return menu;
}

// ������Ϸ���̱�������
Mix_Chunk* GamingBackgroundMusic()
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
	Mix_Chunk * sound = Mix_LoadWAV("res/music/grass.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, -1);

	return sound;
}

Mix_Chunk* ChoosePlantMusic()
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
	Mix_Chunk* sound = Mix_LoadWAV("res/music/choose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50);

	return sound;
}

// ֲ����ֲ����
Mix_Chunk* PlantsCultivate()
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
	Mix_Chunk* sound = Mix_LoadWAV("res/music/plant.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// ��ʼ��������
	Mix_PlayChannel(-1, sound, 0);
	// ����������Ϊ50%
	Mix_VolumeChunk(sound, 50); 

	return sound;
}

// ������Դ�ͷš�����ʱ������

//void GameBackgroundMusicDestroy(Mix_Chunk** sound)
//{
//	// ������Դ
//	Mix_FreeChunk(*sound);
//	*sound = nullptr;
//	Mix_CloseAudio();
//	SDL_Quit();
//}

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

void ImageRenderStart()
{
	// ��Ⱦ��ʼ�˵�
	putimageForPNG(0, 0, &imgstart);
	// ��ȾĬ�ϲ˵���ѡ�в˵�
	putimageForPNG(474, 75, status_leftClick ? &imgMenuClicked : &imgMenu);
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

	// ������ק�����е�ֵΪNULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
	// ����ֲ���ͼ�е�ֵΪ-1
	memset(map, -1, sizeof(map));

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

	// ��ȡС�Ƴ���Ƭ
	for (int i = 0; i < carNum; i++)
	{
		loadimage(&imgCar[i], "res/Screen/car.png");
	}
}

// ͼƬ��Ⱦ
void ImageRenderGaming()
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

	// ��Ⱦ��ֲͼ
	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
		{
			// ��ֲ��ʱ��Ⱦ
			if (map[i][j].type > -1)
			{
				// �����x��y������Ϊ��Ⱦλ��
				int x = 257 + j * 81;
				int y = 179 + i * 102;
				putimageForPNG(x, y, imgPlantsMove[map[i][j].type][map[i][j].frameIndex]);
			}
		}
	}

	// ��Ⱦ��קͼ������������ֲ�����ʵ����קͼ�����Ѿ���ֲ��ֲ��ͼ�Ϸ�
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// ȡ������֡��Ƭ�ĵ�һ��
		putimageForPNG(curX - dragged->getwidth() / 2,
			curY - dragged->getheight() / 2, dragged);
	}

	// ��ȾС�Ƴ�
	x = 195;
	y = 0;
	for (int i = 0; i < carNum; i++)
	{
		y = 210 + i * 100;
		putimageForPNG(x, y, &imgCar[i]);
	}

	// ��������
	EndBatchDraw();
}

void MouseActionGaming()
{
	// ���������Ϣ
	ExMessage msg;
	// �������ж�������ʼ�������򲻴���
	if(peekmessage(&msg))
	{
		// ����������
		if (msg.message == WM_LBUTTONDOWN && msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96)
		{
			int index = (msg.x - 340) / 66;// ʹ��66����Ƚϱ�Ե��λ��
			//std::cout << index << std::endl;
			status_leftClick = 1;// �����������Ϊ1
			curPlant = index;// ����ѡ�е�ֲ��

			// ��¼�����λ�ã����ֲ����ֲ��Ϻ���ֵ�ֲ��ͼƬ�ͺ�����
			curX = msg.x;
			curY = msg.y;

			ChoosePlantMusic();
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
					Mix_Chunk* music = PlantsCultivate();
				}
				//GameBackgroundMusicDestroy(&music);

				// �������״̬Ϊ0
				status_leftClick = 0;
				// ����ǰֲ������Ϊ-1
				curPlant = -1;
			}
		}
	}
}

// ����ֲ���˶�
void updatePlantsMove()
{
	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
		{
			if (map[i][j].type > -1)
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

void GameStartMenu()
{
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

void Gaming()
{
	// ������Ϸ��������
	Mix_Chunk* music = GamingBackgroundMusic();
	int time = 0;
	// ������ʾͼƬ����ȡ�û��Ĳ���
	while (1)
	{
		// ����������
		MouseActionGaming();

		// ��ȾͼƬ
		ImageRenderGaming();

		// ���¶���֡��Ƭ
		time += getDelay();
		if (time > 100)
		{
			updatePlantsMove();
			time = 0;
		}
	}

	Mix_CloseAudio();
	Mix_CloseAudio();
}