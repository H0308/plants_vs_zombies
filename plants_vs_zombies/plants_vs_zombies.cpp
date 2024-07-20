#define _CRT_SECURE_NO_WARNINGS 1
#include "plants_vs_zombies.h"

// 载入PNG图并去透明部分
void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP，贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
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

// 适用于 y <0 以及x<0的任何情况
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
		// 更新启动时间为当前时间
		firstTime = currentTime;
		return 0;
	}
	else {
		// 获取到当前时间与上一次时间的差值
		int ret = currentTime - firstTime;
		// 更新启动时间为当前时间
		firstTime = currentTime;
		return ret;
	}
}

// 判断是否读取到有效文件
bool ifExists(char* plants_name)
{
	FILE* fp = fopen(plants_name, "r");
	if (fp)
	{
		fclose(fp);
	}
	return fp != NULL; // 不为空返回true，代表文件存在
}

// 加载启动界面背景音乐
Mix_Chunk* StartBackgroundMusic()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 加载音乐文件
	Mix_Chunk* background = Mix_LoadWAV("res/music/start.wav");
	if (!background) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, background, -1);
	return background;
}

// 加载菜单音乐
Mix_Chunk* ClickMenuMusic()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 加载音乐文件
	Mix_Chunk* menu = Mix_LoadWAV("res/music/clickMenu.wav");
	if (!menu) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, menu, 0);

	return menu;
}

// 加载游戏过程背景音乐
Mix_Chunk* GamingBackgroundMusic()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 加载音乐文件
	Mix_Chunk * sound = Mix_LoadWAV("res/music/grass.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, -1);

	return sound;
}

Mix_Chunk* ChoosePlantMusic()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/choose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);

	return sound;
}

// 植物种植音乐
Mix_Chunk* PlantsCultivate()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/plant.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50); 

	return sound;
}

// 音乐资源释放——暂时不处理

//void GameBackgroundMusicDestroy(Mix_Chunk** sound)
//{
//	// 清理资源
//	Mix_FreeChunk(*sound);
//	*sound = nullptr;
//	Mix_CloseAudio();
//	SDL_Quit();
//}

// 初始化菜单场景
void StartInit()
{
	// 加载开始界面背景
	loadimage(&imgstart, "res/MainMenu.png");
	// 默认未点击状态
	status_leftClick = 0;

	// 加载选项
	loadimage(&imgMenu, "res/menu1.png");
	loadimage(&imgMenuClicked, "res/menu2.png");
}

void ImageRenderStart()
{
	// 渲染开始菜单
	putimageForPNG(0, 0, &imgstart);
	// 渲染默认菜单和选中菜单
	putimageForPNG(474, 75, status_leftClick ? &imgMenuClicked : &imgMenu);
}

// 初始游戏场景
void GamingInit()
{
	// 加载游戏背景图片到变量
	loadimage(&imgBackground, "res/bg.jpg");

	// 加载植物栏图片到变量
	loadimage(&imgPlantsBar, "res/bar5.png");

	// 初始化鼠标左键动作
	status_leftClick = 0;

	// 设置拖拽数组中的值为NULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
	// 设置植物地图中的值为-1
	memset(map, -1, sizeof(map));

	// 起始不存在植物
	curPlant = -1;
	// 加载植物卡片到变量
	char plants_name[64] = { 0 }; // 通过局部变量记录植物卡牌文件名称
	for (int i = 0; i < PlantsCount; i++)
	{
		// 使用sprintf函数将植物名称读取到plants_name中
		sprintf(plants_name, "res/Cards/card_%d.png", i + 1);
		// 加载图片到植物数组中
		loadimage(&imgPlants[i], plants_name);
	}

	// 加载植物运动拖拽时的图片
	for (int i = 0; i < PlantsCount; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			// sprintf在第二层for循环内读取，读到NULL为止
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

	// 读取小推车照片
	for (int i = 0; i < carNum; i++)
	{
		loadimage(&imgCar[i], "res/Screen/car.png");
	}
}

// 图片渲染
void ImageRenderGaming()
{
	// 开始缓冲
	BeginBatchDraw();

	// 渲染背景
	putimage(0, 0, &imgBackground);

	// 渲染植物栏
	putimageForPNG(250, 0, &imgPlantsBar);

	// 渲染植物
	int x = 0;// 定义x坐标
	int y = 6; // 定义y坐标
	for (int i = 0; i < PlantsCount; i++)
	{
		x = 340 + i * 65;
		putimage(x, y, &imgPlants[i]);
	}

	// 渲染种植图
	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
		{
			// 有植物时渲染
			if (map[i][j].type > -1)
			{
				// 计算出x和y坐标作为渲染位置
				int x = 257 + j * 81;
				int y = 179 + i * 102;
				putimageForPNG(x, y, imgPlantsMove[map[i][j].type][map[i][j].frameIndex]);
			}
		}
	}

	// 渲染拖拽图，本步骤在种植步骤后实现拖拽图置于已经种植的植物图上方
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// 取到动作帧照片的第一张
		putimageForPNG(curX - dragged->getwidth() / 2,
			curY - dragged->getheight() / 2, dragged);
	}

	// 渲染小推车
	x = 195;
	y = 0;
	for (int i = 0; i < carNum; i++)
	{
		y = 210 + i * 100;
		putimageForPNG(x, y, &imgCar[i]);
	}

	// 结束缓冲
	EndBatchDraw();
}

void MouseActionGaming()
{
	// 接收鼠标消息
	ExMessage msg;
	// 如果鼠标有动作，则开始处理，否则不处理
	if(peekmessage(&msg))
	{
		// 鼠标左键单击
		if (msg.message == WM_LBUTTONDOWN && msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96)
		{
			int index = (msg.x - 340) / 66;// 使用66处理比较边缘的位置
			//std::cout << index << std::endl;
			status_leftClick = 1;// 左键单击后置为1
			curPlant = index;// 更新选中的植物

			// 记录点击的位置，解决植物种植完毕后出现的植物图片滞后现象
			curX = msg.x;
			curY = msg.y;

			ChoosePlantMusic();
		}
		else if (msg.message == WM_MOUSEMOVE && status_leftClick) // 鼠标拖拽
		{
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP && curPlant > -1)// 鼠标左键弹起，并且判定有植物时才进入，否则不触发对应音效
		{
			if (msg.x >= 257 && msg.x <= 985 && msg.y >= 179 && msg.y <= 465)
			{
				// 通过x和y计算行和列
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 257) / 81;
				//std::cout << col << ", " << row << std::endl;

				// 设置指定位置的植物类型和动作帧
				if (map[row][col].type == -1)
				{
					map[row][col].type = curPlant;
					map[row][col].frameIndex = 0;
					Mix_Chunk* music = PlantsCultivate();
				}
				//GameBackgroundMusicDestroy(&music);

				// 设置鼠标状态为0
				status_leftClick = 0;
				// 将当前植物设置为-1
				curPlant = -1;
			}
		}
	}
}

// 更新植物运动
void updatePlantsMove()
{
	for (int i = 0; i < mapRow; i++)
	{
		for (int j = 0; j < mapCol; j++)
		{
			if (map[i][j].type > -1)
			{
				// 更新植物运动
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
	// 加载开始背景
	StartInit();

	// 加载开始背景音乐
	StartBackgroundMusic();

	while (1)
	{
		// 处理鼠标操作
		ExMessage msg;
		BeginBatchDraw();
		// 渲染图片
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
	// 关闭两个音乐
	Mix_CloseAudio();
	Mix_CloseAudio();

	return;
}

void Gaming()
{
	// 加载游戏背景音乐
	Mix_Chunk* music = GamingBackgroundMusic();
	int time = 0;
	// 持续显示图片并读取用户的操作
	while (1)
	{
		// 处理鼠标操作
		MouseActionGaming();

		// 渲染图片
		ImageRenderGaming();

		// 更新动作帧照片
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