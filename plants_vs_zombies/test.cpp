#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// �������ڴ�С����
	initgraph(WIDTH, HEIGHT, 1);

	GameStartMenu();
	// ��ʼ����Ϸ����
	GamingInit();
	// ��������
	// ��ʼ��Ϸ
	Gaming();

	// ��ֹ����˲��ر�
	system("pause");

	return 0;
}
