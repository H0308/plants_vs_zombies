#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// 初始化Mixer子系统
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return 1;
	}

	// 设置随机数种子
	srand((unsigned int)time(NULL));
	// 启动窗口大小设置
	initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE);

	GameStartMenu();
	// 初始化游戏场景
	GamingInit();
	// 加载巡视界面
	Overlooking();
	// 展示植物卡牌
	ShowPlantBoard();
	// 启动界面
	// 开始游戏
	Gaming();

	// 防止窗口瞬间关闭
	system("pause");

	return 0;
}
