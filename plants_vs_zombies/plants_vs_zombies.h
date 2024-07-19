#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900// ��Ϸ���ڿ��
#define HEIGHT 600// ��Ϸ���ڸ߶�
#define mapRow 3
#define mapCol 9

static IMAGE imgBackground;// ��Ϸ��������
static IMAGE imgPlantsBar;// ֲ����

// ֲ�￨��
enum 
{
	Pea_Shooter, // �㶹����
	SunFlower, // ���տ�
	PlantsCount // ֲ�����
};

// �洢ֲ�������
static IMAGE imgPlants[PlantsCount];
static int curX, curY;// �洢��ǰ����x�����y����
// �洢ֲ���˶�ͼ������
static IMAGE* imgPlantsMove[PlantsCount][20];
static int curPlant; // ��¼��ǰ����ק��ֲ�-1Ĭ��û��ֲ��

typedef struct plant
{
	int type; // ��¼��ǰ��ֲ��ֲ�-1Ĭ��û��ֲ��
	int frameIndex; // ��¼����֡
}plant;

static plant map[mapRow][mapCol];// ֲ���ͼ

// ����PNGͼƬ�ڱ�����
void putimageForPNG(int  picture_x, int picture_y, IMAGE* picture);
int getDelay();

// ������������
Mix_Chunk* GameBackgroundMusic();
Mix_Chunk* PlantsCultivate();
// ��ʼ����Ϸ����
void GameInit();
// ͼƬ��Ⱦ
void ImageRender();
// ��궯��
void MouseAction();
// ����������Դ����
//void GameBackgroundMusicDestroy(Mix_Chunk** sound);
