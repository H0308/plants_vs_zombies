#pragma once
#include <iostream>
#include <graphics.h>
#include <SDL.h>
#include <SDL_mixer.h>

#define WIDTH 900// 游戏窗口宽度
#define HEIGHT 600// 游戏窗口高度

static IMAGE imgBackground;// 游戏场景背景
static IMAGE imgPlantsBar;// 植物栏

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
static int curPlant; // 记录当前被拖拽的植物，-1默认没有植物

// 处理PNG图片黑边问题
void putimageForPNG(int  picture_x, int picture_y, IMAGE* picture);
int getDelay();

// 加载背景音乐
Mix_Music* GameBackgroundMusic();
// 初始化游戏场景
void GameInit();
// 图片渲染
void ImageRender();
// 鼠标动作
void MouseAction();
// 背景音乐资源清理
void GameBackgroundMusicDestroy(Mix_Music** music);
