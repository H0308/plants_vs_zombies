#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900 // 游戏窗口宽度
#define HEIGHT 600 // 游戏窗口高度
#define MAPROW 3 // 地图行数
#define MAPCOL 9 // 地图列数
#define CARNUM 3 // 小推车的个数
#define SUNSHINENUM 10 // 产生阳光的个数

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

// 存储植物的数组
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
};

struct sunshineFromSky
{
	int x, y;// 阳光下落的x坐标和y坐标
	int frameIndex; // 记录图片帧
	int dest; // 阳光掉落的y坐标
	int isUse; // 阳光是否被使用
	int timer; // 计时器记录阳光在掉落点停留时间
};

// 存储阳光数组
static sunshineFromSky sunshine_sky[SUNSHINENUM];
// 阳光动作帧照片数组
static IMAGE imgSunFrameIndex[29];

static plant map[MAPROW][MAPCOL];// 植物地图

// 处理PNG图片黑边问题
void putimageForPNG(int  picture_x, int picture_y, IMAGE* picture);
int getDelay();

// 加载音乐音乐
Mix_Chunk* StartBackgroundMusic();
Mix_Chunk* GamingBackgroundMusic();
Mix_Chunk* PlantsCultivate();
Mix_Chunk* ClickMenuMusic();
Mix_Chunk* ChoosePlantMusic();
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
void updatePlantsMove();
// 创建阳光
void CreateSunshine();
// 更新阳光
void updateSunshine();
// 背景音乐资源清理
//void GameBackgroundMusicDestroy(Mix_Chunk** sound);
