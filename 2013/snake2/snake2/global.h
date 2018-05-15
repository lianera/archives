//global.h  全局类、枚举、常量的声明
#pragma once
#include <string>
#include <iostream>
using std::string;

//--------------------错误处理类------------------------------
class Error
{
	string info;
public:
	Error(const string& sinf) :info(sinf) {}
	void Print() 
	{ 
		std::cout << info << std::endl; 
		exit(1);
	}
};

class Init_Error :public Error
{
public:
	Init_Error(const string& sinf) :Error(sinf) {}
};

class Outer_Error :public Error
{
public:
	Outer_Error(const string& sinf) :Error(sinf) {}
};

//-------------------Enums--------------------------------
enum ANITYPE {
	ANI_LOGO = 0, ANI_MENUBK, ANI_SNAKE_HEAD, ANI_SNAKE_BODY, ANI_SNAKE_TAIL, ANI_FOOD, ANI_BACKGROUND,
	ANI_WIN, ANI_FAIL, ANI_BUTTON, ANI_OUTLINE, ANI_SELBKGROUND, ANI_HELP
};
enum ANIBACKGROUND { ANI_BACKGROUND_GRASS, ANI_BACKGROUND_WOOD, ANI_BACKGROUND_ROCK, ANI_BACKGROUND_TILE };
enum ANIBUTTON {
	ANI_BUTTON_NEW, ANI_BUTTON_OLD, ANI_BUTTON_SEL, ANI_BUTTON_HELP, ANI_BUTTON_QUIT,
	ANI_BUTTON_CONTINUE, ANI_BUTTON_BACK, ANI_BUTTON_LEFT, ANI_BUTTON_RIGHT, ANI_BUTTON_OK, ANI_BUTTON_CANCEL,
	ANI_BUTTON_PAUSE, ANI_BUTTON_RECOVER, ANI_BUTTON_RETURN
};

enum MID { MID_MENUBKM, MID_MAP0 };

enum DIRECTION { RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3 };

//------------------const------------------------------

const int FPS = 30;