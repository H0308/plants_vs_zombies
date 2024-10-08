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
#define ZOMBIENUM 20 // 生产僵尸的数量
#define SUNSHINESPEED 100 // 阳光飞行速度
#define PEASHOOTERBULLETNUM 40 //豌豆射手子弹个数
#define PEASHOOTERSAFETYLINE 930 // 豌豆射手攻击警戒线的位置
#define PEASHOOTERBULLETDAMAGE 10 // 豌豆子弹的伤害
#define PLANTBLOOD 100 // 植物的血量
#define EATDAMAGE 10 // 僵尸吃植物的伤害
#define GRAVITY 7 // 重力加速度
#define PI 3.14 // 圆周率
#define ZOMBIECOUNT 25 // 一局僵尸的数量

static IMAGE imgstart;// 菜单场景背景
static IMAGE imgMenu, imgMenuClicked;// 菜单按钮
static IMAGE imgBackground;// 游戏场景背景
static IMAGE imgPlantsBar;// 植物栏
static int status_leftClick; // 是否点击菜单
static int status_leftHover; // 是否在指定位置悬停

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

struct sunshineFromSunflower
{
	double start_x, start_y; // 起始位置
	double end_x, end_y; // 终点位置
	double angle; // 起始角度
	double speed; // 起始速度
	double t_change; // 变化时间
	double max_y; // 最高点位置
	int frameIndex; // 记录图片帧
	int isUse; // 阳光是否被使用
	int timer; // 计时器记录阳光在掉落点停留时间
	int isClick; // 阳光是否被点击
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
	int restBlood; // 被击打后剩余的血量
};

struct bullet
{
	int x, y; // 豌豆子弹位置
	int isUse; // 豌豆子弹是否被使用
	int speed; // 豌豆子弹的速度
	int isExplode; // 豌豆子弹是否爆炸
	int row; // 子弹出现的行
};

struct point
{
	int _x;
	int _y;
};

struct car
{
	int x, y; // 小推车位置
	int isUsed; // 小推车状态
	int isRun; // 小推车移动状态
	int row; // 小推车所在行
};

// 存储阳光数组
static sunshineFromSky sunshine_sky[SUNSHINENUM];
// 阳光动作帧照片数组
static IMAGE imgSun_skyFrameIndex[29];
// 存储向日葵阳光
static sunshineFromSunflower sunshine_sunflower[SUNSHINENUM];
static IMAGE imgSun_flowerFrameIndex[29];

static int sunshineScore; // 当前阳光分数
static bullet peaShooterBullets[PEASHOOTERBULLETNUM]; // 豌豆子弹的数量
static IMAGE imgPeaShooterBullets; // 豌豆子弹图片
static IMAGE imgPeaShooterBulletsExploded; // 豌豆子弹爆炸图片
static IMAGE imgZombieDead[11]; // 僵尸死亡图片帧
static IMAGE imgZombieEat[21]; // 僵尸吃植物图片帧
static car cars[CARNUM]; // 存储小推车
static IMAGE imgShovelSlot; // 存储铁铲框图片
static IMAGE imgShovelSmall; // 存储铁铲图片
static IMAGE imgShovelNormal; // 存储铁铲图片
static int holdingShovel; // 当前是否已经拿到铁铲
static int holdingPlants; // 当前是否持有植物

// 存储僵尸的数组
static zombie zombies[ZOMBIENUM];
// 僵尸动作帧数组
static IMAGE imgZombieFrameIndex[22];
// 僵尸站立动作帧数组
static IMAGE imgZombieStandFrameIndex[11];
static plant map[MAPROW][MAPCOL];// 植物地图

enum
{
	Success, // 游戏胜利
	Ongoing, // 游戏进行
	Fail // 游戏结束
};

static int ZombieAppeared; // 游戏对局出现的僵尸个数
static int GameStatus; // 游戏状态
static int ZombieKilled; // 杀死的僵尸个数

// 处理PNG图片黑边问题
void putimageForPNG(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor);
int getDelay();

// 加载音乐
void StartBackgroundMusic();
void GamingBackgroundMusic();
void OverlookingMusic();
void PlantsCultivate();
void ClickMenuMusic();
void ChoosePlantMusic();
void ChooseShovelMusic();
void MovePlantMusic();
void CollectSunshineMusic();
void FailChoosePlantMusic();
void ZombiesComingMusic();
void ZombiesGroanMusic();
void PeaShooterBulletCollideMusic();
void ZombieEatingMusic();
void LawnmowerMusic();
void SuccessMusic();
void FailMusic();
// 初始化菜单场景
void StartInit();
// 初始化游戏场景
void GamingInit();
// 游戏开始界面
void GameStartMenu();
// 俯视全局
void Overlooking();
// 展示植物卡牌
void ShowPlantBoard();
// 游戏开始运行
int Gaming();
// 图片渲染
void ImageRenderStart();
void ImageRenderGaming();
// 鼠标动作
void MouseActionGaming();
// 移除植物
void MovePlants(ExMessage* msg);
// 更新游戏相关数据
void UpdateGameData();
// 更新植物运动
void UpdatePlantsMove();
// 创建阳光
void CreateSunshineFromSky();
// 更新阳光
void UpdateSunshineFromSky();
// 创建向日葵阳光
void CreateSunshineFromSunflower();
// 更新向日葵阳光
void UpdateSunshineFromSunflower();
// 收集阳光
void CollectSunshine(ExMessage* msg);
// 收集来自天空的阳光
void CollectSunshineFromSky(ExMessage* msg);
// 收集向日葵阳光
void CollectSunshineFromSunFlower(ExMessage* msg);
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
// 创建小推车
void CreateCar();
// 更新小推车
void UpdateCar();
// 小推车碰撞检测
void CheckCarCollision();
// 检查游戏状态
int CheckGameStatus();