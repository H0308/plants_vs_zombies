#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// 初始化游戏场景
	GameInit();
	Mix_Music* music = GameBackgroundMusic();
	// 持续显示图片并读取用户的操作
	while (1)
	{
		// 处理鼠标操作
		MouseAction();

		// 渲染图片
		ImageRender();
	}

	// 防止窗口瞬间关闭
	system("pause");
	
	// 释放音乐资源
	GameBackgroundMusicDestroy(&music);
	return 0;
}
