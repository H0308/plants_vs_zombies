#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <cmath>
#include <conio.h>

#define WIDTH 900 // ��Ϸ���ڿ��
#define HEIGHT 600 // ��Ϸ���ڸ߶�
#define MAPROW 3 // ��ͼ����
#define MAPCOL 9 // ��ͼ����
#define CARNUM 3 // С�Ƴ��ĸ���
#define SUNSHINENUM 10 // ��������ĸ���
#define PERSUNSHINE 25 // һ���������ֵ
#define ZOMBIENUM 10 // ��ʬ������
#define SUNSHINESPEED 80 // ��������ٶ�
#define PEASHOOTERBULLETNUM 40 //�㶹�����ӵ�����
#define PEASHOOTERSAFETYLINE 850 // �㶹���ֹ��������ߵ�λ��
#define PEASHOOTERBULLETDAMAGE 10 // �㶹�ӵ����˺�
#define PLANTBLOOD 100
#define EATDAMAGE 10

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

// �㶹��������
struct PeaShooter
{
	int type = Pea_Shooter;// �㶹��������
	int sunshine = 100;// ÿһ���㶹������Ҫ������
};
// �㶹���ֶ���
static PeaShooter peaShooter;

// ���տ�����
struct Sunflower
{
	int type = SunFlower; // ���տ�����
	int sunshine = 50;// ÿһ�����տ���Ҫ������
};
// ���տ�����
static Sunflower sunflower;

// �洢ֲ�￨ͼƬ������
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
	int x, y; // ֲ����������
	int blood; // ÿһ��ֲ���Ѫ��
	int eaten; // ֲ���Ƿ񱻳Ե�
};

struct sunshineFromSky
{
	double x, y;// ���������x�����y����
	int frameIndex; // ��¼ͼƬ֡
	int dest; // ��������y����
	int isClick; // �����Ƿ񱻵��
	int isUse; // �����Ƿ�ʹ��
	int timer; // ��ʱ����¼�����ڵ����ͣ��ʱ��
	double xoffset; // x����ƫ����
	double yoffset; // y����ƫ����
};

struct zombie
{
	int x, y; // ��ʬλ��
	int frameIndex; // ����֡��Ƭ
	int isUse; // �Ƿ�ʹ��
	int speed; // ��ʬ�ƶ��ٶ�
	int blood; // ��ʬѪ��
	int row; // ��ʬ���ֵ���
	int isDead; // ��ʬ�Ƿ�����
	int isEat; // ��ʬ�Ƿ��ֲ��
};

struct bullet
{
	int x, y; // �㶹�ӵ�λ��
	int isUse; // �㶹�ӵ��Ƿ�ʹ��
	int speed; // �㶹�ӵ����ٶ�
	int isExplode; // �㶹�ӵ��Ƿ�ը
	int row; // �ӵ����ֵ���
};


// �洢��������
static sunshineFromSky sunshine_sky[SUNSHINENUM];
// ���⶯��֡��Ƭ����
static IMAGE imgSunFrameIndex[29];
static int sunshineScore; // ��ǰ��������
static bullet peaShooterBullets[PEASHOOTERBULLETNUM]; // �㶹�ӵ�������
static IMAGE imgPeaShooterBullets; // �㶹�ӵ�ͼƬ
static IMAGE imgPeaShooterBulletsExploded; // �㶹�ӵ���ըͼƬ
static IMAGE imgZombieDead[11]; // ��ʬ����ͼƬ֡
static IMAGE imgZombieEat[21]; // ��ʬ��ֲ��ͼƬ֡

// �洢��ʬ������
static zombie zombies[ZOMBIENUM];
// ��ʬ����֡����
static IMAGE imgZombieFrameIndex[22];

static plant map[MAPROW][MAPCOL];// ֲ���ͼ

// ����PNGͼƬ�ڱ�����
void putimageForPNG(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor);
int getDelay();

// ��������
void StartBackgroundMusic();
void GamingBackgroundMusic();
void PlantsCultivate();
void ClickMenuMusic();
void ChoosePlantMusic();
void CollectSunshineMusic();
void FailChoosePlantMusic();
void ZombiesComingMusic();
void ZombiesGroanMusic();
void PeaShooterBulletCollideMusic();
void ZombieEatingMusic();
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
void UpdatePlantsMove();
// ��������
void CreateSunshine();
// ��������
void UpdateSunshine();
// �ռ�����
void CollectSunshine(ExMessage* msg);
// ѡ��ֲ��
void ChoosePlant(int index);
// ������ʬ
void CreateZombies();
// ��ʬ�ƶ�
void UpdateZombies();
// �����㶹�����ӵ�
void CreatePeaShooterBullets();
// �����㶹�����ӵ�
void UpdatePeaShooterBullets();
// �㶹�ӵ���ײ���
void CheckPeaShooterBulletsCollision();
// ��ʬ����ֲ����
void CheckZombieCollision();