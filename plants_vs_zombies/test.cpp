#define _CRT_SECURE_NO_WARNINGS 1

#include "plants_vs_zombies.h"

int main()
{
	// ��ʼ����Ϸ����
	GameInit();

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
	return 0;
}
