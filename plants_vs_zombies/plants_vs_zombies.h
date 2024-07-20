#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900// ��Ϸ���ڿ��
#define HEIGHT 600// ��Ϸ���ڸ߶�
#define mapRow 3
#define mapCol 9
#define carNum 3

static IMAGE imgstart;// �˵���������
static IMAGE imgMenu, imgMenuClicked;// �˵���ť
static IMAGE imgBackground;// ��Ϸ��������
static IMAGE imgPlantsBar;// ֲ����
static int status_leftClick; // �Ƿ����˵�

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
static IMAGE imgCar[carNum]; // С�Ƴ�����
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
Mix_Chunk* StartBackgroundMusic();
Mix_Chunk* GamingBackgroundMusic();
Mix_Chunk* PlantsCultivate();
Mix_Chunk* ClickMenuMusic();
Mix_Chunk* ChoosePlantMusic();
// ��ʼ���˵�����
void StartInit();
// ��ʼ����Ϸ����
void GamingInit();
// ��Ϸ��ʼ����
void GameStartMenu();
// ��Ϸ��ʼ����
void Gaming();
// ͼƬ��Ⱦ
void ImageRenderStart();
void ImageRenderGaming();
// ��궯��
void MouseActionGaming();
// ����ֲ���˶�
void updatePlantsMove();
// ����������Դ����
//void GameBackgroundMusicDestroy(Mix_Chunk** sound);
