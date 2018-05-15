//**********************manager.h*******************
//管理模块

#pragma once
#include "global.h"
#include "paint.h"
#include "element.h"

#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::fstream;
using std::vector;

class Picture;
class Tile;
class Animation;
class Painter;
class Panel;
class Mid;

class Manager
{
	typedef vector<Picture*> PICPV;
	typedef vector<Tile*> TILEPV;
	typedef vector<Animation*> ANIPV;
	typedef vector<Mid*> MIDPV;
	//用顺序容器储存所有的图片与背景音乐资源
	PICPV pics;
	TILEPV	tiles;
	ANIPV anis;
	MIDPV mids;
	int curmid;
public:
	Manager():curmid(-1){}
	~Manager();
	void Add(const string& picfile);//添加图片
	void Add(Picture *ppic,int x,int y,int w,int h);//添加贴片
	void Add(int picid,int x,int y,int w,int h);//添加贴片
	void Add(const vector<int>& tilevct,int cycl,int interval);//添加动画
	void AddMid(const string& midfile);
	void Add(int aniIndex);//添加单次循环动画
	Animation& Ani(int id){return *anis.at(id);}
	void LoadRes();
	void LoadMid();
	Animation* PickAni(int p1,int p2=-1,int p3=-1);
	Mid* PicMid(int id);
	void Play(int id);
	void Stop();
};

class Game
{
	//游戏状态
	enum GAMESTATE{GAME_SHOWLOGO,GAME_SHOWMENU,GAME_SHOWHELP,GAME_CREATE,GAME_RESTART,GAME_RUN,GAME_DESTROY,GAME_WIN,GAME_FAIL,GAME_OVER};
	GAMESTATE state;

	HDC hclientdc;
	HWND hWindow;
	Painter *pPainter;
	int clientw,clienth;
	Manager manager;

	int level;
	int roundtime;//每一局流逝的时间
	void ShowLogo();
	void ShowMenu();
	void ShowWin();
	void ShowFail();
	void ShowOver();
	void ShowHelp();
	void Create();
	void ReStart();
	void Run();
	void Destroy();

	void ReadArchive();
	void SaveArchive();
	///////////////////Game Element///////////////////
	Panel *pPanel;
public:
	Game(HINSTANCE hinst,HWND hwnd,int clw,int clh);
	~Game();
	void Loop();
	
};

class Mid//mid背景音乐处理的类
{
	string filename;
	UINT wDeviceID;
public:
	Mid(const string& fname);
	~Mid();
	void Play();
	void Stop();
};
