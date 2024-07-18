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

// 加载背景音乐
Mix_Music* GameBackgroundMusic()
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
	Mix_Music* music = Mix_LoadMUS("res/music/grass.mp3");
	if (!music) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	// 开始播放音乐
	if (Mix_PlayMusic(music, -1) < 0) {
		printf("Unable to play music, SDL_mixer Error: %s\n", Mix_GetError());
		return nullptr;
	}

	return music;
}

// 音乐资源释放
void GameBackgroundMusicDestroy(Mix_Music** music)
{
	// 清理资源
	Mix_FreeMusic(*music);
	*music = nullptr;
	Mix_CloseAudio();
	SDL_Quit();
}

// 初始游戏场景
void GameInit()
{
	// 加载游戏背景图片到变量
	loadimage(&imgBackground, "res/bg.jpg");
	// 启动窗口大小设置
	initgraph(WIDTH, HEIGHT, 1);

	// 加载植物栏图片到变量
	loadimage(&imgPlantsBar, "res/bar5.png");

	// 设置拖拽数组中的值为NULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
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
}

// 图片渲染
void ImageRender()
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

	// 渲染拖拽图
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// 取到动作帧照片的第一张
		putimageForPNG(curX - dragged->getwidth() / 2, 
			curY - dragged->getheight() / 2, dragged);
	}

	// 结束缓冲
	EndBatchDraw();
}

void MouseAction()
{
	// 接收鼠标消息
	ExMessage msg;
	// 变量记录是否按下左键
	static int status_leftClick = 0;
	// 如果鼠标有动作，则开始处理，否则不处理
	if(peekmessage(&msg))
	{
		// 鼠标左键单击
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96)
			{
				int index = (msg.x - 340) / 66;// 使用66处理比较边缘的位置
				//std::cout << index << std::endl;
				status_leftClick = 1;// 左键单击后置为1
				curPlant = index;// 更新选中的植物
			}
		}
		else if (msg.message == WM_MOUSEMOVE && status_leftClick) // 鼠标拖拽
		{
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP)// 鼠标左键弹起
		{
			int x = msg.x;
			int y = msg.y;


		}
	}
}
