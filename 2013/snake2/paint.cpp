//*********************paint.cpp**************************

#include "paint.h"
#pragma comment(lib,"Msimg32.lib")
//---------------Picture-----------------------
HINSTANCE Picture::hInst=NULL;
HDC Picture::horgdc=NULL;

//预先初始化函数，由于该类需要在构造时知道某些必要参数，并且对于所有对象，这些参数
//都相同，所以采用静态成员预先初始化的方法
void Picture::PreInit(HINSTANCE hInstance,HDC horiginaldc)
{
	hInst=hInstance;
	horgdc=horiginaldc;
}

Picture::Picture(const char* picfile):hdc(NULL),hbmp(NULL)
{
	//图片的加载
	if(!hInst || !horgdc) throw Init_Error("图片类未预初始化！");
	hbmp=(HBITMAP)LoadImage(hInst,picfile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION |LR_LOADFROMFILE);
	if(!hbmp)throw Outer_Error("加载图片失败！");
	hdc=CreateCompatibleDC(horgdc);
	if(!hdc)throw Outer_Error("创建兼容设备失败！");
	SelectObject(hdc,hbmp);
	BITMAP bmp;
	if(!GetObject(hbmp,sizeof(BITMAP),&bmp))throw Outer_Error("获取图片信息失败！");
	w=bmp.bmWidth;
	h=bmp.bmHeight;
}

Picture::~Picture()
{
	if(hdc)DeleteObject(hdc);
	if(hbmp)DeleteObject(hbmp);
}


//---------------Animation-------------------
void Animation::Refresh()
{
	//动画的控制
	if(intervalpos<interval)
	{
		intervalpos++;
		return;
	}
	else
	{
		intervalpos=0;
	}
	if(pos<(int)tilepv.size()-1 && !stoped)
	{
		pos++;
	}
	else
	{
		pos=0;
		if(cycl!=0)
		{
			cyctimes++;
			if(cyctimes>=cycl)
				stoped=true;
		}
	}
}

//--------------------Painter----------------------------
Painter::Painter(HDC hDC,int cx,int cy,int cw,int ch):hdc(hDC),hcompdc(NULL),hcompbmp(NULL),x(cx),y(cy),w(cw),h(ch)
{
	//采用双缓冲，建立缓冲区
	hcompdc=CreateCompatibleDC(hdc);
	hcompbmp=CreateCompatibleBitmap(hdc,w,h);
	SelectObject(hcompdc,hcompbmp);
}

Painter::~Painter()
{
	if(hcompdc)
	{
		DeleteDC(hcompdc);
		hcompdc=NULL;
	}
	if(hcompbmp)
	{
		DeleteObject(hcompbmp);
		hcompbmp=NULL;
	}
}

void Painter::Draw(const Tile& tile,int x,int y)
{
	//位块传输，用来贴图
	BitBlt(hcompdc,x,y,tile.tilew,tile.tileh,tile.pPic->horgdc,tile.picx,tile.picy,SRCCOPY);
}

void Painter::Draw(const Animation& ani,int x,int y)
{
	if(ani.Stoped())return;
	const Tile& tile=ani.GetTile();
	BitBlt(hcompdc,x,y,tile.tilew,tile.tileh,tile.pPic->hdc,tile.picx,tile.picy,SRCCOPY);
}

void Painter::Draw(const Animation& ani,int x,int y,int w,int h)
{
	if(ani.Stoped())return;
	const Tile& tile=ani.GetTile();
	//过滤透明色的位块传输
	TransparentBlt(hcompdc,x,y,w,h,tile.pPic->hdc,tile.picx,tile.picy,tile.tilew,tile.tileh,RGB(0,0,0));
}

void Painter::Draw(const Text& tex,int x,int y)
{
	//绘制文字
	SetBkMode(hcompdc,TRANSPARENT);
	SetTextColor(hcompdc,tex.color);
	SelectObject(hcompdc,tex.font);
	TextOut(hcompdc,x,y,tex.str.c_str(),tex.str.length());
}

void Painter::Erase()
{
	//对整个绘图区域进行擦除
	HBRUSH brush=CreateSolidBrush(bkcolor);
	SelectObject(hcompdc,brush);
	RECT rect={x,y,x+w,y+h};
	FillRect(hcompdc,&rect,brush);
	DeleteObject(brush);
}

void Painter::Apply()
{
	//把缓冲页面的内容贴到屏幕设备上
	BitBlt(hdc,x,y,w,h,hcompdc,0,0,SRCCOPY);
}