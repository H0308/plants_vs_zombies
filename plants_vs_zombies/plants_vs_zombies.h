#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900 // ��Ϸ���ڿ��
#define HEIGHT 600 // ��Ϸ���ڸ߶�
#define MAPROW 3 // ��ͼ����
#define MAPCOL 9 // ��ͼ����
#define CARNUM 3 // С�Ƴ��ĸ���
#define SUNSHINENUM 10 // ��������ĸ���

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
static IMAGE imgCar[CARNUM]; // С�Ƴ�����
static int curPlant; // ��¼��ǰ����ק��ֲ�-1Ĭ��û��ֲ��

struct plant
{
	int type; // ��¼��ǰ��ֲ��ֲ�-1Ĭ��û��ֲ��
	int frameIndex; // ��¼����֡
};

struct sunshineFromSky
{
	int x, y;// ���������x�����y����
	int frameIndex; // ��¼ͼƬ֡
	int dest; // ��������y����
	int isUse; // �����Ƿ�ʹ��
	int timer; // ��ʱ����¼�����ڵ����ͣ��ʱ��
};

// �洢��������
static sunshineFromSky sunshine_sky[SUNSHINENUM];
// ���⶯��֡��Ƭ����
static IMAGE imgSunFrameIndex[29];

static plant map[MAPROW][MAPCOL];// ֲ���ͼ

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
// ������Ϸ�������
void UpdateGameData();
// ����ֲ���˶�
void updatePlantsMove();
// ��������
void CreateSunshine();
// ��������
void updateSunshine();
// ����������Դ����
//void GameBackgroundMusicDestroy(Mix_Chunk** sound);
