#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// ��ʼ��Mixer��ϵͳ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize, SDL_mixer Error: %s\n", Mix_GetError());
		return 1;
	}

	// �������������
	srand((unsigned int)time(NULL));
	// �������ڴ�С����
	initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE);

	GameStartMenu();
	// ��ʼ����Ϸ����
	GamingInit();
	// ����Ѳ�ӽ���
	Overlooking();
	// չʾֲ�￨��
	ShowPlantBoard();
	// ��������
	// ��ʼ��Ϸ
	Gaming();

	// ��ֹ����˲��ر�
	system("pause");

	return 0;
}
