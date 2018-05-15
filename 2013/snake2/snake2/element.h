//*********************element.h**************************
//包括了用户交互元素的类、绘图的类以及游戏规则类
#pragma once
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include "global.h"
#include "manager.h"
#include "paint.h"

using namespace std;

class Painter;
class Manager;
class Animation;
class Node;
class Panel;
class Snake;

//运算符重载声明
ofstream& operator<<(ofstream& ofs, const Panel& panel);
ifstream& operator>>(ifstream& ifs, Panel& panel);
ofstream& operator<<(ofstream& ofs, const Snake& snake);
ifstream& operator>>(ifstream& ifs, Snake& snake);
ofstream& operator<<(ofstream& ofs, const Node& node);
ifstream& operator>>(ifstream& ifs, Node& node);

class Elebase//元素虚基类
{
protected:
	static Painter *pPainter;
	static Manager *pManager;
	Animation *pAni;
public:
	void SetAni(Animation *pani) { pAni = pani; }
	virtual void SetAni(int id1, int id2 = -1, int id3 = -1);
	static void PreInit(Painter *pP, Manager *pM) { pPainter = pP; pManager = pM; }
	Elebase() :pAni(NULL) { if (!pPainter || !pManager)throw Init_Error("元素类未初始化！"); }
	virtual void Draw() = 0;
	Animation* Ani() { return pAni; }
};

class Rect :public Elebase//矩形类
{
protected:
	int x, y, w, h;
public:
	virtual void Draw();
	Rect(int rx, int ry, int rw, int rh) :x(rx), y(ry), w(rw), h(rh) {}
};

class FullScrAni :public Rect//全屏动画类
{
	static int flscrw, flscrh;
public:
	static void PreInit(int w, int h);
	FullScrAni() :Rect(0, 0, flscrw, flscrh) {}
};

class Text :public Rect//文字输出类
{
	friend class Painter;
	string str;

	static SDL_Renderer* renderer_;
	static TTF_Font* font;
	SDL_Color color;
	SDL_Texture *texture = nullptr;

	Text(const Text&) = delete;
public:
	static void PreInit(SDL_Renderer* renderer, const char* font_file);
	static void PostRelease();
	Text(int rx, int ry, int rw, int rh);
	~Text();
	void SetText(const string& tex, SDL_Color texcolor = { 0, 0, 0 });
	void SetText(int num, SDL_Color texcolor = { 0, 0, 0 });
	void Draw();
};

class Button :public Rect//按钮类
{
	bool focus;
	bool keydown;
	bool pressed;
	int orgx, orgy, orgw, orgh;
	void EnLarge(int px = 6) { x -= px; w += 2 * px; y -= px; h += 2 * px; }
	void Shift(int px = 2) { x += px; y += px; }
	void EnNormal() { x = orgx; y = orgy; w = orgw; h = orgh; }
public:
	Button(int bx, int by, int bw = 200, int bh = 50);
	void Deal();
	bool MousePressed() { return pressed; }
	void Recover() { focus = false; keydown = false; pressed = false; EnNormal(); }
};

class SelBox :public Rect//选择框类
{
	Button leftbutton;
	Button rightbutton;
	Text tex;
	vector<string> strlist;
	int curstr;
public:
	SelBox(int sx, int sy, int sw, int sh, float scale = 1);//scale为文本和按钮的大小比
	void Deal();
	void SetAni(int psb1, int plb1, int prb1, int psb2 = -1, int plb2 = -1, int prb2 = -1, int psb3 = -1, int plb3 = -1, int prb3 = -1);
	SelBox& Add(const string& str) { strlist.push_back(str); return *this; }
	SelBox& Add(int num) { ostringstream ss; ss << num; strlist.push_back(ss.str()); return *this; }
	string& str() { return strlist[curstr]; }
	void Draw();
};

class Snake//蛇类
{
	friend class Panel;
	int rate;		//移动一次所占帧数
	int curratepos;	//移动之后过去的帧数
	int direction;	//方向
	int newdirect;	//新方向
	bool added;		//遇到食物是否已添加
	bool live;		//活的
	vector<Node*> nodes;
public:
	Snake(int lenth, int v, int startx = 10, int starty = 10, int direc = RIGHT);
	~Snake();
	bool Move();//移动返回真，未移动返回假
	bool IsBumpFood(int fx, int fy);
	bool IsBumpSelf();
	bool IsLive() { return live; }
	void Right() { if (direction != LEFT)newdirect = RIGHT; }
	void Down() { if (direction != UP)newdirect = DOWN; }
	void Left() { if (direction != RIGHT)newdirect = LEFT; }
	void Up() { if (direction != DOWN)newdirect = UP; }
	void AddNode();
	void Draw();
	int lenth() { return nodes.size(); }

	Node* Head() { return nodes[0]; }
	//判断指定坐标是否在蛇上
	inline bool OnHead(int x, int y);
	inline bool OnBelly(int x, int y);
	inline bool OnBody(int x, int y);
	inline bool OnTail(int x, int y);
	bool OnSnake(int x, int y) { return OnBody(x, y) || OnHead(x, y); }

	inline int HeadX();
	inline int HeadY();

	//重载   用于存档
	friend ofstream& operator<<(ofstream& ofs, const Snake& snake);
	friend ifstream& operator>>(ifstream& ifs, Snake& snake);

};

class Grid :public Elebase//格子类
{
	friend class Panel;
	friend class Snake;
protected:
	static int px, py, gw, gh;
	int gx, gy;
public:
	static void PreInit(int panelx, int panely, int gridw, int gridh)
	{
		px = panelx; py = panely; gw = gridw; gh = gridh;
	}
	Grid(int gridx, int gridy) :gx(gridx), gy(gridy) {}
	void Draw();
};

class Food :public Grid//食物类
{
	friend class Panel;
	int maxx, maxy;
public:
	Food(int xnum, int ynum) :Grid(0, 0), maxx(xnum), maxy(ynum) {}
	void Generate()
	{
		gx = rand() % maxx;
		gy = rand() % maxy;
	}
	friend ofstream& operator<<(ofstream& ofs, const Panel& panel);
	friend ifstream& operator>>(ifstream& ifs, Panel& panel);
};

class Node :public Grid//节点类  也即蛇的节点
{
	friend class Snake;
	int direction;
public:
	Node(int gridx, int gridy, int direct = RIGHT) :Grid(gridx, gridy), direction(direct) {}
	int X() { return gx; }
	int Y() { return gy; }
	int Direction() { return direction; }

	friend ofstream& operator<<(ofstream& ofs, const Node& node);
	friend ifstream& operator>>(ifstream& ifs, Node& node);

};

class Panel :public Rect	//游戏面板类
{
	int xnum, ynum;
	Snake *snake;
	Food food;
	bool overed;
	bool won;
	int level;
	int score;

	bool pause;
	void ChkBump();
public:
	explicit Panel(int Level, int rectx = 100, int recty = 100, int rectw = 450, int recth = 450, int gridxnum = 20, int gridynum = 20);
	~Panel() { if (snake) { delete snake; snake = NULL; } }
	void DoFrame();
	bool Overed() { return overed; }
	bool Won() { return won; }
	int Score() { return score; }
	int RestFood();
	void SetPause(bool p) { pause = p; }
	bool IsPause() { return pause; }

	friend ofstream& operator<<(ofstream& ofs, const Panel& panel);
	friend ifstream& operator>>(ifstream& ifs, Panel& panel);
};

