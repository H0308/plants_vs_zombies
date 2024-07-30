#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// 设置随机数种子
	srand((unsigned int)time(NULL));
	// 启动窗口大小设置
	initgraph(WIDTH, HEIGHT);

	GameStartMenu();
	// 初始化游戏场景
	GamingInit();
	// 加载巡视界面
	Overlooking();
	// 展示植物卡牌
	ShowPlantBoard();
	// 启动界面
	// 开始游戏
	GameStatus = Gaming(); // 注意不同的源文件不共享同一块GameStatus变量空间

	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		//printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		//printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return 0;
	}

	// 检测游戏状态
	if (GameStatus == Success)
	{
		SuccessMusic();
	}
	else if (GameStatus == Fail)
	{
		FailMusic();
	}

	// 防止窗口瞬间关闭
	system("pause");
	return 0;
}