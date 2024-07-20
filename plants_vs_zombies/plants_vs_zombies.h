#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900// 游戏窗口宽度
#define HEIGHT 600// 游戏窗口高度
#define mapRow 3
#define mapCol 9
#define carNum 3

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
static IMAGE imgCar[carNum]; // 小推车个数
static int curPlant; // 记录当前被拖拽的植物，-1默认没有植物


typedef struct plant
{
	int type; // 记录当前种植的植物，-1默认没有植物
	int frameIndex; // 记录动作帧
}plant;

static plant map[mapRow][mapCol];// 植物地图

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
// 更新植物运动
void updatePlantsMove();
// 背景音乐资源清理
//void GameBackgroundMusicDestroy(Mix_Chunk** sound);
