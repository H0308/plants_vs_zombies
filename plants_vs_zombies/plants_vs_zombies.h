#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <cmath>
#include <conio.h>

#define WIDTH 900 // 游戏窗口宽度
#define HEIGHT 600 // 游戏窗口高度
#define MAPROW 3 // 地图行数
#define MAPCOL 9 // 地图列数
#define CARNUM 3 // 小推车的个数
#define SUNSHINENUM 10 // 产生阳光的个数
#define PERSUNSHINE 25 // 一个阳光的增值
#define ZOMBIENUM 10 // 僵尸的数量
#define SUNSHINESPEED 80 // 阳光飞行速度
#define PEASHOOTERBULLETNUM 40 //豌豆射手子弹个数
#define PEASHOOTERSAFETYLINE 850 // 豌豆射手攻击警戒线的位置
#define PEASHOOTERBULLETDAMAGE 10 // 豌豆子弹的伤害
#define PLANTBLOOD 100
#define EATDAMAGE 10

static IMAGE imgstart;// 菜单场景背景
static IMAGE imgMenu, imgMenuClicked;// 菜单按钮
static IMAGE imgBackground;// 游戏场景背景
static IMAGE imgPlantsBar;// 植物栏
static int status_leftClick; // 是否点击菜单

// 植物卡牌
enum
{
	Pea_Shooter, // 豌豆射手
	SunFlower, // 向日葵
	PlantsCount // 植物计数
};

// 豌豆射手属性
struct PeaShooter
{
	int type = Pea_Shooter;// 豌豆射手类型
	int sunshine = 100;// 每一个豌豆射手需要的阳光
};
// 豌豆射手对象
static PeaShooter peaShooter;

// 向日葵属性
struct Sunflower
{
	int type = SunFlower; // 向日葵类型
	int sunshine = 50;// 每一个向日葵需要的阳光
};
// 向日葵对象
static Sunflower sunflower;

// 存储植物卡图片的数组
static IMAGE imgPlants[PlantsCount];
static int curX, curY;// 存储当前鼠标的x坐标和y坐标
// 存储植物运动图的数组
static IMAGE* imgPlantsMove[PlantsCount][20];
static IMAGE imgCar[CARNUM]; // 小推车个数
static int curPlant; // 记录当前被拖拽的植物，-1默认没有植物

struct plant
{
	int type; // 记录当前种植的植物，-1默认没有植物
	int frameIndex; // 记录动作帧
	int x, y; // 植物所在坐标
	int blood; // 每一个植物的血量
	int eaten; // 植物是否被吃掉
};

struct sunshineFromSky
{
	double x, y;// 阳光下落的x坐标和y坐标
	int frameIndex; // 记录图片帧
	int dest; // 阳光掉落的y坐标
	int isClick; // 阳光是否被点击
	int isUse; // 阳光是否被使用
	int timer; // 计时器记录阳光在掉落点停留时间
	double xoffset; // x坐标偏移量
	double yoffset; // y坐标偏移量
};

struct zombie
{
	int x, y; // 僵尸位置
	int frameIndex; // 动作帧照片
	int isUse; // 是否被使用
	int speed; // 僵尸移动速度
	int blood; // 僵尸血量
	int row; // 僵尸出现的行
	int isDead; // 僵尸是否死亡
	int isEat; // 僵尸是否吃植物
};

struct bullet
{
	int x, y; // 豌豆子弹位置
	int isUse; // 豌豆子弹是否被使用
	int speed; // 豌豆子弹的速度
	int isExplode; // 豌豆子弹是否爆炸
	int row; // 子弹出现的行
};


// 存储阳光数组
static sunshineFromSky sunshine_sky[SUNSHINENUM];
// 阳光动作帧照片数组
static IMAGE imgSunFrameIndex[29];
static int sunshineScore; // 当前阳光数量
static bullet peaShooterBullets[PEASHOOTERBULLETNUM]; // 豌豆子弹的数量
static IMAGE imgPeaShooterBullets; // 豌豆子弹图片
static IMAGE imgPeaShooterBulletsExploded; // 豌豆子弹爆炸图片
static IMAGE imgZombieDead[11]; // 僵尸死亡图片帧
static IMAGE imgZombieEat[21]; // 僵尸吃植物图片帧

// 存储僵尸的数组
static zombie zombies[ZOMBIENUM];
// 僵尸动作帧数组
static IMAGE imgZombieFrameIndex[22];

static plant map[MAPROW][MAPCOL];// 植物地图

// 处理PNG图片黑边问题
void putimageForPNG(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor);
int getDelay();

// 加载音乐
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
// 初始化菜单场景
void StartInit();
// 初始化游戏场景
void GamingInit();
// 游戏开始界面
void GameStartMenu();
// 游戏开始运行
void Gaming();
// 图片渲染
void ImageRenderStart();
void ImageRenderGaming();
// 鼠标动作
void MouseActionGaming();
// 更新游戏相关数据
void UpdateGameData();
// 更新植物运动
void UpdatePlantsMove();
// 创建阳光
void CreateSunshine();
// 更新阳光
void UpdateSunshine();
// 收集阳光
void CollectSunshine(ExMessage* msg);
// 选择植物
void ChoosePlant(int index);
// 创建僵尸
void CreateZombies();
// 僵尸移动
void UpdateZombies();
// 创建豌豆射手子弹
void CreatePeaShooterBullets();
// 更新豌豆射手子弹
void UpdatePeaShooterBullets();
// 豌豆子弹碰撞检测
void CheckPeaShooterBulletsCollision();
// 僵尸碰到植物检测
void CheckZombieCollision();