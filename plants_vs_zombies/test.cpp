#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

#undef main
int main()
{
	// ��ʼ����Ϸ����
	GameInit();
	Mix_Music* music = GameBackgroundMusic();
	// ������ʾͼƬ����ȡ�û��Ĳ���
	while (1)
	{
		// ����������
		MouseAction();

		// ��ȾͼƬ
		ImageRender();
	}

	// ��ֹ����˲��ر�
	system("pause");
	
	// �ͷ�������Դ
	GameBackgroundMusicDestroy(&music);
	return 0;
}
