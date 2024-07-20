#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// 启动窗口大小设置
	initgraph(WIDTH, HEIGHT, 1);

	GameStartMenu();
	// 初始化游戏场景
	GamingInit();
	// 启动界面
	// 开始游戏
	Gaming();

	// 防止窗口瞬间关闭
	system("pause");

	return 0;
}
