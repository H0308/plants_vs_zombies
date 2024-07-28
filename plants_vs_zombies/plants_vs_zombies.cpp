#define _CRT_SECURE_NO_WARNINGS 1
#include "plants_vs_zombies.h"

// 将PNG图渲染为透明效果，防止下面一种方法出现问题时使用
#pragma comment( lib, "MSIMG32.LIB")
void putimageForPNG(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}

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

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
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

// 获取延迟时间
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
void StartBackgroundMusic()
{
	// 加载音乐文件
	Mix_Chunk* background = Mix_LoadWAV("res/music/start.wav");
	if (!background) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, background, -1);
}

// 加载菜单音乐
void ClickMenuMusic()
{
	// 加载音乐文件
	Mix_Chunk* menu = Mix_LoadWAV("res/music/clickMenu.wav");
	if (!menu) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, menu, 0);
}

// 加载游戏过程背景音乐
void GamingBackgroundMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/grass.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, -1);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 80);
}

// 选择植物背景音乐
void ChoosePlantMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/choose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);
}

// 植物种植音乐
void PlantsCultivate()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/plant.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);
}

// 阳光收集音乐
void CollectSunshineMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/sunshine.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);
}

void FailChoosePlantMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/failToChoose.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);
}

void ZombiesComingMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/coming.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	// 将音量设置为50%
	Mix_VolumeChunk(sound, 50);
}

void ZombiesGroanMusic()
{
	// 加载音乐文件
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
		// 开始播放音乐
		Mix_PlayChannel(-1, sound2, 0);
	}
	else if (playCount > 40)
	{
		// 开始播放音乐
		Mix_PlayChannel(-1, sound3, 0);
	}
	else if (playCount > 70)
	{
		// 开始播放音乐
		Mix_PlayChannel(-1, sound4, 0);
	}
	else
	{
		// 开始播放音乐
		Mix_PlayChannel(-1, sound5, 0);
	}
}

// 豌豆子弹碰撞音乐
void PeaShooterBulletCollideMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/hit.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
}

// 僵尸吃植物音乐
void ZombieEatingMusic()
{
	// 加载音乐文件
	Mix_Chunk* sound = Mix_LoadWAV("res/music/eating.wav");
	if (!sound) {
		printf("Failed to load music, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 开始播放音乐
	Mix_PlayChannel(-1, sound, 0);
	Mix_VolumeChunk(sound, 50);
}

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

// 开始界面图片渲染
void ImageRenderStart()
{
	// 渲染开始菜单
	putimagePNG(0, 0, &imgstart);
	// 渲染默认菜单和选中菜单
	putimagePNG(474, 75, status_leftClick ? &imgMenuClicked : &imgMenu);
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
	// 起始不存在植物
	curPlant = -1;
	// 阳光初始值
	sunshineScore = 50;

	// 设置拖拽数组中的值为NULL
	memset(imgPlantsMove, NULL, sizeof(imgPlantsMove));
	// 设置植物地图中的值为-1
	memset(map, -1, sizeof(map));
	// 初始化阳光数组中的所有值为0
	memset(sunshine_sky, 0, sizeof(sunshine_sky));
	memset(sunshine_sunflower, 0, sizeof(sunshine_sunflower));
	// 初始化僵尸数组中的所有值为0
	memset(zombies, 0, sizeof(zombies));
	// 设置豌豆子弹数组的所有值为0
	memset(peaShooterBullets, 0, sizeof(peaShooterBullets));

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
	for (int i = 0; i < CARNUM; i++)
	{
		loadimage(&imgCar[i], "res/Screen/car.png");
	}

	// 读取阳光到阳光图片帧数组
	char sun_name[64] = { 0 };
	for (int i = 0; i < 29; i++)
	{
		sprintf(sun_name, "res/sunshine/%d.png", i + 1);
		loadimage(&imgSun_skyFrameIndex[i], sun_name);
	}

	char sun[64] = { 0 };
	for (int i = 0; i < 29; i++)
	{
		sprintf(sun, "res/sunshine/%d.png", i + 1);
		loadimage(&imgSun_flowerFrameIndex[i], sun);
	}

	// 读取僵尸到僵尸动作帧数组中
	char zombie_name[64] = { 0 };
	for (int i = 0; i < 22; i++)
	{
		sprintf(zombie_name, "res/zm/%d.png", i + 1);
		loadimage(&imgZombieFrameIndex[i], zombie_name);
	}

	// 加载豌豆子弹图片
	loadimage(&imgPeaShooterBullets, "res/bullets/bullet_normal.png");

	// 加载豌豆爆炸图片

	loadimage(&imgPeaShooterBulletsExploded, "res/bullets/bullet_blast.png",
		imgPeaShooterBulletsExploded.getwidth() * 0.2,
		imgPeaShooterBulletsExploded.getheight() * 0.2,
		true);


	// 加载僵尸死亡图片帧照片
	char zombie_dead_name[64] = { 0 };
	for (int i = 0; i < 11; i++)
	{
		sprintf(zombie_dead_name, "res/zm_dead/Image%d.png", i + 1);
		loadimage(&imgZombieDead[i], zombie_dead_name);
	}

	// 加载僵尸吃植物动画帧照片
	char zombie_eat_name[64] = { 0 };
	for (int i = 0; i < 21; i++)
	{
		sprintf(zombie_eat_name, "res/zm_eat/%d.png", i + 1);
		loadimage(&imgZombieEat[i], zombie_eat_name);
	}

	// 设置字体
	LOGFONT font;
	gettextstyle(&font);
	font.lfHeight = 30;
	font.lfWeight = 50;
	strcpy(font.lfFaceName, "Segoe UI BLACK");
	font.lfQuality = ANTIALIASED_QUALITY; // 抗锯齿效果
	settextstyle(&font);
	setbkmode(TRANSPARENT); // 背景透明
	settextcolor(BLACK); // 字体颜色为黑色
}

// 图片渲染
void ImageRenderGaming()
{
	// 开始缓冲
	BeginBatchDraw();

	// 渲染背景
	putimage(0, 0, &imgBackground);

	// 渲染植物栏
	putimageForPNG(NULL, 250, 0, &imgPlantsBar, BLACK);

	// 渲染植物
	int x = 0;// 定义x坐标
	int y = 6; // 定义y坐标
	for (int i = 0; i < PlantsCount; i++)
	{
		x = 340 + i * 65;
		putimageForPNG(NULL, x, y, &imgPlants[i], BLACK);
	}

	// 渲染种植图
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			// 有植物时渲染
			if (map[i][j].type > -1 && map[i][j].eaten == 0)
			{
				// 计算出x和y坐标作为渲染位置
				int x = 257 + j * 81;
				int y = 179 + i * 102;
				putimageForPNG(NULL, x, y, imgPlantsMove[map[i][j].type][map[i][j].frameIndex], BLACK);
			}
		}
	}

	//渲染僵尸
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
			// 从僵尸脚的位置渲染
			putimageForPNG(NULL, zombies[i].x, zombies[i].y - img.getheight(),
				&imgZombieFrameIndex[zombies[i].frameIndex], BLACK);
		}
	}

	// 渲染豌豆子弹
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

	// 渲染小推车
	x = 195;
	y = 0;
	for (int i = 0; i < CARNUM; i++)
	{
		y = 210 + i * 100;
		putimageForPNG(NULL, x, y, &imgCar[i], BLACK);
	}

	// 渲染拖拽图，本步骤在种植步骤后实现拖拽图置于已经种植的植物图上方
	if (curPlant >= 0)
	{
		IMAGE* dragged = imgPlantsMove[curPlant][0];// 取到动作帧照片的第一张
		putimageForPNG(NULL, curX - dragged->getwidth() / 2,
			curY - dragged->getheight() / 2, dragged, BLACK);
	}

	// 设置字体显示位置
	char scoreText[5];
	sprintf(scoreText, "%d", sunshineScore);
	outtextxy(285, 67, scoreText);

	// 渲染阳光
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse || sunshine_sky[i].isClick)
		{
			putimageForPNG(NULL, sunshine_sky[i].x, sunshine_sky[i].y,
				&imgSun_skyFrameIndex[sunshine_sky[i].frameIndex], BLACK);
		}
	}

	// 渲染阳光
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sunflower[i].isUse || sunshine_sunflower[i].isClick)
		{
			putimageForPNG(NULL, sunshine_sunflower[i].start_x, sunshine_sunflower[i].start_y,
				&imgSun_flowerFrameIndex[sunshine_sunflower[i].frameIndex], BLACK);
		}
	}

	// 结束缓冲
	EndBatchDraw();
}

void CollectSunshine(ExMessage* msg)
{
	CollectSunshineFromSky(msg);
	CollectSunshineFromSunFlower(msg);
}

// 处理游戏过程中的操作
void MouseActionGaming()
{
	// 接收鼠标消息
	ExMessage msg;
	// 如果鼠标有动作，则开始处理，否则不处理
	if (peekmessage(&msg))
	{
		// 鼠标左键单击
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 340 && msg.x <= 340 + PlantsCount * 65 && msg.y <= 96) // 选植物
			{
				int index = (msg.x - 340) / 66;// 使用66处理比较边缘的位置
				//std::cout << index << std::endl;
				status_leftClick = 1;// 左键单击后置为1
				// 记录点击的位置，解决植物种植完毕后出现的植物图片滞后现象
				curX = msg.x;
				curY = msg.y;

				// 如果当前拥有的阳光数量大于点击植物的需求量，则选中，否则无法选择
				ChoosePlant(index);
			}
			else // 收集阳光
			{
				CollectSunshine(&msg);
			}
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
					map[row][col].x = col;
					map[row][col].y = row;
					// 设置植物血量
					map[row][col].blood = PLANTBLOOD;
					// 起始植物未被吃掉
					map[row][col].eaten = 0;
					PlantsCultivate();
				}

				// 设置鼠标状态为0
				status_leftClick = 0;
				// 将当前植物设置为-1
				curPlant = -1;
			}
		}
	}
}

// 更新植物运动
void UpdatePlantsMove()
{
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			if (map[i][j].type > -1 && map[i][j].eaten == 0)
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

// 创建天上的阳光
void CreateSunshineFromSky()
{
	static int count = 0;// 控制次数
	static int frequent = 50;// 控制阳光产生的频率
	count++;// 记录调用次数
	if (count >= frequent)
	{
		// 通过随机数控制阳光产生的频率
		frequent = 150; // [100, 199]
		count = 0;// 重新计数
		// 先找到可用的阳光的下标
		int i = 0;
		while (i < SUNSHINENUM && sunshine_sky[i].isUse)
		{
			i++;
		}

		// 此时存在两种情况：1. 存在可用阳光 2. 无可以使用的阳光
		// 处理第一种情况，防止第二种情况
		if (i < SUNSHINENUM)
		{
			// 更改新阳光的数据
			sunshine_sky[i].x = 260 + rand() % (900 - 260); // [260, 899]
			sunshine_sky[i].y = 170;
			sunshine_sky[i].dest = 200 + (1 + rand() % 3) * 100; // [200, 500]
			sunshine_sky[i].frameIndex = 0; // 从第一张帧照片开始
			sunshine_sky[i].timer = 0;
			// 阳光点击状态设置为0
			sunshine_sky[i].isClick = 0;
			// 将阳光使用状态改为1
			sunshine_sky[i].isUse = 1;
			// 初始化偏移量
			sunshine_sky[i].xoffset = 0;
			sunshine_sky[i].yoffset = 0;
		}
	}
}

// 更新天上的阳光
void UpdateSunshineFromSky()
{
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse)
		{
			// 更新阳光图片帧
			sunshine_sky[i].frameIndex = (sunshine_sky[i].frameIndex + 1) % 29;
			// 更新y坐标
			if (sunshine_sky[i].timer == 0)
			{
				sunshine_sky[i].y += 6;
			}
			// 到达dest位置，阳光停留一段时间，再销毁阳光
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

			// 计算角度
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

// 创建向日葵阳光
void CreateSunshineFromSunflower()
{
	static int count = 0;// 控制次数
	static int frequent = 80;// 控制阳光产生的频率
	count++;// 记录调用次数
	if (count >= frequent)
	{
		// 通过随机数控制阳光产生的频率
		frequent = 100; // [100, 199]
		count = 0;// 重新计数
		// 找到种向日葵的位置
		int row = -1;
		int col = -1;
		static int lastRow = 0;
		static int lastCol = 0;
		static int sunflowerCount = 0;
		static int data[3] = { 0 };
		for (int i = 0; i < MAPROW; i++)
		{
			if (data[i] == 0)
			{
				data[i] = 1;
			}
			else if (data[0] && data[1])
			{
				i = 2;
				data[0] = data[1] = 0;
			}
			for (int j = 0; j < MAPCOL; j++)
			{
				// 记录位置
				if (map[i][j].type == sunflower.type)
				{
					row = i;
					col = j;
					break;
				}
			}

			if(row != lastRow || col != lastCol)
			{
				lastRow = row;
				lastCol = col;
				sunflowerCount++;
				break;
			}
		}

		// 找到可用的阳光的下标
		int i = 0;
		while (i < SUNSHINENUM && sunshine_sunflower[i].isUse)
		{
			i++;
		}

		int x = 257 + col * 81;
		int y = 179 + row * 102;

		// 确定位置创建阳光
		if (row < MAPROW && col < MAPCOL && map[row][col].type == sunflower.type && i < SUNSHINENUM)
		{
			// 初始化起始位置和终点位置
			double startX = x + 10;
			double startY = y + 10;
			double endX = startX + 50;
			double endY = startY + 70;
			sunshine_sunflower[i].speed = 5;
			sunshine_sunflower[i].t_change = 1;
			sunshine_sunflower[i].start_x = startX;
			sunshine_sunflower[i].start_y = startY;
			printf("运动x方向：%f\n", sunshine_sunflower[i].start_x);
			printf("运动y方向：%f\n", sunshine_sunflower[i].start_y);
			sunshine_sunflower[i].end_x = endX;
			sunshine_sunflower[i].end_y = endY;
			// 初始化斜抛角度
			sunshine_sunflower[i].angle = 85;
			// 初始化其余变量
			sunshine_sunflower[i].isUse = 1;
			sunshine_sunflower[i].isClick = 0;
			sunshine_sunflower[i].timer = 0;
			sunshine_sunflower[i].frameIndex = 0;
			sunshine_sunflower[i].max_y = (sunshine_sunflower[i].speed * sin((sunshine_sunflower[i].angle * PI / 180)) * sunshine_sunflower[i].speed * sin((sunshine_sunflower[i].angle * PI / 180))) / (2 * GRAVITY) + sunshine_sunflower[i].start_y - 50;
			printf("最高点高度：%f\n", sunshine_sunflower[i].max_y);
		}
	}
}

// 更新向日葵阳光
void UpdateSunshineFromSunflower()
{
	// 找到向日葵阳光
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		// 计算最高点高度
		if (sunshine_sunflower[i].isUse)
		{
			// 更新阳光图片帧
			sunshine_sunflower[i].frameIndex = (sunshine_sunflower[i].frameIndex + 1) % 29;
			if (sunshine_sunflower[i].timer == 0)
			{

				if (sunshine_sunflower[i].start_y >= sunshine_sunflower[i].max_y)
				{
					// 水平方向位移，x轴正方向向右，持续增加
					sunshine_sunflower[i].start_x += sunshine_sunflower[i].speed * cos((sunshine_sunflower[i].angle * PI / 180)) * sunshine_sunflower[i].t_change;
					// 竖直方向位移，y轴正方向向下，先减后增
					sunshine_sunflower[i].start_y -= (sunshine_sunflower[i].speed * sin((sunshine_sunflower[i].angle * PI / 180)) * sunshine_sunflower[i].t_change - 0.5 * GRAVITY * pow(sunshine_sunflower[i].t_change, 2));
					// angle为0时为平抛运动
					if (sunshine_sunflower[i].angle > 0)
					{
						sunshine_sunflower[i].angle -= 15;
					}
					printf("斜抛运动x方向：%f\n", sunshine_sunflower[i].start_x);
					printf("斜抛运动y方向：%f\n", sunshine_sunflower[i].start_y);
					printf("斜抛运动y方向角度：%f\n", sunshine_sunflower[i].angle);
				}
			}
			if (sunshine_sunflower[i].start_x >= sunshine_sunflower[i].end_x || sunshine_sunflower[i].start_y >= sunshine_sunflower[i].end_y)
			{
				sunshine_sunflower[i].timer++;
				if (sunshine_sunflower[i].timer > 120)
				{
					sunshine_sunflower[i].isUse = 0;
					sunshine_sunflower[i].timer = 0;
				}
			}
		}
		else if (sunshine_sunflower[i].isClick)
		{
			double angle = atan(sunshine_sunflower[i].start_y / (sunshine_sunflower[i].start_x - 262));
			sunshine_sunflower[i].xoffset = 60 * cos(angle);
			sunshine_sunflower[i].yoffset = 60 * sin(angle);

			// 计算角度
			sunshine_sunflower[i].start_x -= sunshine_sunflower[i].xoffset;
			sunshine_sunflower[i].start_y -= sunshine_sunflower[i].yoffset;

			if (sunshine_sunflower[i].start_x <= 262 || sunshine_sunflower[i].start_y <= 0)
			{
				sunshine_sunflower[i].isClick = 0;
				sunshineScore += PERSUNSHINE;
			}
		}
	}
}

// 收集阳光
void CollectSunshineFromSky(ExMessage* msg)
{
	// 找到正在使用的阳光
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sky[i].isUse)
		{
			int width = imgSun_skyFrameIndex[0].getwidth();// 获取到太阳的宽度
			int height = imgSun_skyFrameIndex[0].getheight(); // 获取太阳的高度
			// 判断是否在阳光的可点击范围内
			if (msg->x >= sunshine_sky[i].x && msg->x <= sunshine_sky[i].x + width &&
				msg->y >= sunshine_sky[i].y && msg->y <= sunshine_sky[i].y + height)
			{
				// 收集阳光
				sunshine_sky[i].isClick = 1;
				// 更新阳光状态为0
				sunshine_sky[i].isUse = 0;
				// 播放收集阳光音乐
				CollectSunshineMusic();
			}
		}
		else if (sunshine_sky[i].isClick)
		{
			// 计算斜率θ
			double angle = atan(sunshine_sky[i].y / (sunshine_sky[i].x - 262));
			sunshine_sky[i].xoffset = 60 * cos(angle);
			sunshine_sky[i].yoffset = 60 * sin(angle);
		}
	}
}

// 收集向日葵阳光
void CollectSunshineFromSunFlower(ExMessage* msg)
{
	// 找到正在使用的阳光
	for (int i = 0; i < SUNSHINENUM; i++)
	{
		if (sunshine_sunflower[i].isUse)
		{
			int width = imgSun_flowerFrameIndex[0].getwidth();// 获取到太阳的宽度
			int height = imgSun_flowerFrameIndex[0].getheight(); // 获取太阳的高度
			// 判断是否在阳光的可点击范围内
			if (msg->x >= sunshine_sunflower[i].start_x && msg->x <= sunshine_sunflower[i].start_x + width &&
				msg->y >= sunshine_sunflower[i].start_y && msg->y <= sunshine_sunflower[i].start_y + height)
			{
				// 收集阳光
				sunshine_sunflower[i].isClick = 1;
				// 更新阳光状态为0
				sunshine_sunflower[i].isUse = 0;
				// 播放收集阳光音乐
				CollectSunshineMusic();
			}
		}
		else if (sunshine_sunflower[i].isClick)
		{
			// 计算斜率θ
			double angle = atan(sunshine_sunflower[i].start_x / (sunshine_sunflower[i].start_x - 262));
			sunshine_sunflower[i].xoffset = 60 * cos(angle);
			sunshine_sunflower[i].yoffset = 60 * sin(angle);
		}
	}
}

// 选择植物
void ChoosePlant(int index)
{
	if (index == peaShooter.type)// 选择豌豆射手
	{
		if (sunshineScore >= peaShooter.sunshine)// 是否满足豌豆射手需要的阳光数量
		{
			// if防止同一个植物二次赋值
			if (curPlant != index)
			{
				// 更新curPlant
				curPlant = index;
				// 更新阳光
				sunshineScore -= peaShooter.sunshine;
				// 播放选中的音乐
				ChoosePlantMusic();
			}
		}
		else
		{
			// 播放音乐无法选择
			FailChoosePlantMusic();
		}
	}
	else if (index == sunflower.type)// 选择向日葵
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

// 创建僵尸
void CreateZombies()
{
	static int frequent = 150; // 控制僵尸创建频率
	static int count = 0; // 控制执行次数
	static int comingSign = 0; // 标志第一个僵尸产生
	count++;
	if (count >= frequent)
	{
		frequent = 200;
		count = 0;
		// 找到未使用的僵尸
		int i = 0;
		while (i < ZOMBIENUM && zombies[i].isUse)
		{
			i++;
		}

		if (i < ZOMBIENUM)
		{
			// 更新僵尸数据
			zombies[i].x = 900;
			zombies[i].row = rand() % 3;// 初始化僵尸所在行
			zombies[i].y = 170 + (1 + zombies[i].row) * 100;
			zombies[i].frameIndex = 0;
			zombies[i].isUse = 1;
			zombies[i].speed = 1;
			// 设置僵尸血量
			zombies[i].blood = 100;
			// 设置僵尸起始状态
			zombies[i].isDead = 0;
			zombies[i].isEat = 0;
			// 标志第一个僵尸诞生
			comingSign++;
		}

		if (comingSign == 1)
		{
			ZombiesComingMusic();
		}
	}
}

// 僵尸移动
void UpdateZombies()
{
	static int count = 0;// count控制调用次数
	count++;
	if (count >= 1) // 每三次调用僵尸更新一次
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
						// 更新动作帧照片
						zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 22;
						// 更新僵尸位置
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
					// 僵尸到达草坪左边界
					if (zombies[i].x <= 150)
					{
						// 判断是否有小推车，有则移动小推车，没有则游戏结束
						// 小推车暂未实现
						exit(0);// 游戏结束
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

// 创建豌豆子弹
void CreatePeaShooterBullets()
{
	// 先找到有僵尸的行位置
	int zombieRow[3] = { 0 };
	for (int i = 0; i < ZOMBIENUM; i++)
	{
		// 存在僵尸并且僵尸越过警戒线时记录
		if (zombies[i].isUse)
		{
			zombieRow[zombies[i].row] = 1;// 存在僵尸时将该行标记为1
		}
	}

	// 找到有豌豆射手的位置，创建豌豆子弹
	for (int i = 0; i < MAPROW; i++)
	{
		for (int j = 0; j < MAPCOL; j++)
		{
			// 找到豌豆射手的位置并且确定豌豆射手的位置与僵尸的位置在同一行
			if (map[i][j].type == peaShooter.type && zombieRow[i] && map[i][j].eaten == 0)
			{
				// 控制执行次数
				static int counts[MAPROW] = { 0 }; // 每行的计数器
				counts[i]++;
				if (counts[i] > 20)
				{
					counts[i] = 0;
					int k = 0;
					// 找到可用
					while (k < PEASHOOTERBULLETNUM && peaShooterBullets[k].isUse)
					{
						k++;
					}

					// 创建当前行的豌豆子弹
					// 根据植物种植位置判断子弹出现位置
					int x = 257 + j * 81;
					int y = 179 + i * 102;
					peaShooterBullets[k].x = x + imgPlantsMove[Pea_Shooter][0]->getwidth() - 15;
					peaShooterBullets[k].y = y + 5;

					// 初始化子弹其余变量
					peaShooterBullets[k].row = i; // 豌豆所在行即豌豆子弹所在行
					peaShooterBullets[k].isUse = 1;
					// 初始化所有子弹时，将全局所有子弹的爆炸状态全部置为0，防止出现部分子弹碰撞完后卡在画面中
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

// 更新子弹
void UpdatePeaShooterBullets()
{
	// 找到正在使用的子弹
	for (int i = 0; i < PEASHOOTERBULLETNUM; i++)
	{
		if (peaShooterBullets[i].isUse && peaShooterBullets[i].isExplode == 0)
		{
			peaShooterBullets[i].x += peaShooterBullets[i].speed;
			// 子弹碰撞检测
			CheckPeaShooterBulletsCollision();
			// 如果子弹越过警戒线或者碰到僵尸销毁
			if (peaShooterBullets[i].x > PEASHOOTERSAFETYLINE + 20 || peaShooterBullets[i].isExplode)
			{
				peaShooterBullets[i].isUse = 0;
			}
		}
	}
}

// 豌豆子弹碰撞检测
void CheckPeaShooterBulletsCollision()
{
	// 找到可用的豌豆子弹
	for (int i = 0; i < PEASHOOTERBULLETNUM; i++)
	{
		if (peaShooterBullets[i].isUse && peaShooterBullets[i].isExplode == 0)
		{
			// 找到可用的僵尸
			for (int j = 0; j < ZOMBIENUM; j++)
			{
				if (zombies[j].isUse)
				{
					// 如果僵尸和子弹是同一行，则判断是否已经抵达碰撞范围
					if (peaShooterBullets[i].row == zombies[j].row)
					{
						if (peaShooterBullets[i].x >= zombies[j].x + 60 && peaShooterBullets[i].x <= zombies[j].x + 110)
						{
							// 子弹碰撞
							// 僵尸血量减少
							zombies[j].blood -= PEASHOOTERBULLETDAMAGE;
							// printf("%d %d %d\n", peaShooterBullets[i].row, zombies[j].row, zombies[j].blood);
							peaShooterBullets[i].isExplode = 1;
							peaShooterBullets[i].speed = 0;
							// 更新子弹的使用状态
							peaShooterBullets[i].isUse = 0;
							// 僵尸没死之前播放碰撞音效
							if (zombies[j].blood >= 0)
							{
								PeaShooterBulletCollideMusic();
							}
							if (zombies[j].blood == 0)
							{
								// 僵尸死亡后
								zombies[j].speed = 0;
								zombies[j].isDead = 1;
								// 僵尸的死亡动画图片帧
								zombies[j].frameIndex = 0;
								// 清空所有已发射的子弹防止豌豆死亡子弹停留
								for (int k = 0; k < PEASHOOTERBULLETNUM; k++)
								{
									peaShooterBullets[k].isExplode = 0;
								}
							}
							// 当前的子弹击中当前僵尸后不需要比较后面的子弹是否击中
							break;
						}
					}
				}
			}
		}
	}
}

// 僵尸碰到植物检测
void CheckZombieCollision()
{
	// 找到僵尸所在行
	for (int i = 0; i < ZOMBIENUM; i++)
	{
		int row = zombies[i].row;
		// 根据行找当前行是否有植物
		for (int j = 0; j < MAPCOL; j++)
		{
			// 僵尸出现的行存在植物
			if (map[row][j].type > -1 && map[row][j].eaten == 0)
			{
				// 左侧植物的坐标
				int left = 257 + j * 81 + 10;
				int right = 257 + j * 81 + 60;
				// 如果僵尸还活着就吃植物
				if (zombies[i].isDead == 0 && zombies[i].x + 80 >= left && zombies[i].x + 80 <= right)
				{
					// 僵尸动画帧更改时机控制
					static int frameMove = 0;
					frameMove++;
					if (frameMove == 1)
					{
						// 更改僵尸的动画帧
						zombies[i].frameIndex = 0;
					}
					// 僵尸状态更新
					zombies[i].isEat = 1;
					// 吃植物时僵尸不移动
					zombies[i].speed = 0;
					// 控制僵尸吃植物的速度
					static int count = 0;
					count++;
					if (count > 25)
					{
						count = 0;
						// 植物血量减少
						map[row][j].blood -= EATDAMAGE;
						printf("%d %d %d\n", row, j, map[row][j].blood);
					}
					if (map[row][j].blood > 0)
					{
						ZombieEatingMusic();
					}
					if (map[row][j].blood == 0)
					{
						// 植物被吃掉
						map[row][j].eaten = 1;
						// 僵尸恢复样式
						for (int k = 0; k < ZOMBIENUM; k++)
						{
							// 找到还在吃但是植物已经没了的僵尸恢复其状态
							if (zombies[k].isEat && map[row][j].eaten)
							{
								zombies[k].frameIndex = 0;
								zombies[k].speed = 1;
								zombies[k].isEat = 0;
							}
						}
						// 将帧更新归零
						frameMove = 0;
						// 清空所有已发射的子弹防止豌豆死亡子弹停留
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

// 游戏开始界面菜单
void GameStartMenu()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

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
				EndBatchDraw();
				break;
			}
		}
	}

	// 关闭两个音乐
	Mix_CloseAudio();
	Mix_CloseAudio();

	return;
}

// 更新游戏数据
void UpdateGameData()
{
	// 更新植物动作
	UpdatePlantsMove();
	// 创建阳光
	CreateSunshineFromSky();
	// 更新阳光
	UpdateSunshineFromSky();
	// 创建阳光
	CreateSunshineFromSunflower();
	// 更新阳光
	UpdateSunshineFromSunflower();
	// 创建僵尸
	CreateZombies();
	// 僵尸移动
	UpdateZombies();
	// 创建豌豆子弹
	CreatePeaShooterBullets();
	// 更新豌豆子弹
	UpdatePeaShooterBullets();
}

// 游戏进行
void Gaming()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	// 加载游戏背景音乐
	GamingBackgroundMusic();
	int time = 0;
	// 持续显示图片并读取用户的操作
	while (1)
	{
		// 处理鼠标操作
		MouseActionGaming();

		// 渲染图片
		ImageRenderGaming();

		// 更新游戏数据
		time += getDelay();
		if (time >= 80)
		{
			UpdateGameData();
			time = 0;
		}
	}

	Mix_CloseAudio();
	Mix_CloseAudio();
}