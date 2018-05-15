//*********************paint.cpp**************************
#include <SDL2/SDL_image.h>
#include "paint.h"
//---------------Picture-----------------------

//预先初始化函数，由于该类需要在构造时知道某些必要参数，并且对于所有对象，这些参数
//都相同，所以采用静态成员预先初始化的方法
void Picture::PreInit(SDL_Renderer* renderer)
{
	renderer_ = renderer;
}

Picture::Picture(const char* picfile) 
{
	//图片的加载
	if (!renderer_) throw Init_Error("图片类未预初始化！");
	texture = IMG_LoadTexture(renderer_, picfile);
	if (!texture)throw Outer_Error("加载图片失败！");
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Picture::~Picture()
{
	if (texture)
		SDL_DestroyTexture(texture);
}


//---------------Animation-------------------
void Animation::Refresh()
{
	//动画的控制
	if (intervalpos<interval)
	{
		intervalpos++;
		return;
	}
	else
	{
		intervalpos = 0;
	}
	if (pos<(int)tilepv.size() - 1 && !stoped)
	{
		pos++;
	}
	else
	{
		pos = 0;
		if (cycl != 0)
		{
			cyctimes++;
			if (cyctimes >= cycl)
				stoped = true;
		}
	}
}

//--------------------Painter----------------------------
Painter::Painter(SDL_Renderer* renderer, int cx, int cy, int cw, int ch) 
	:renderer_(renderer), x(cx), y(cy), w(cw), h(ch)
{
}

Painter::~Painter()
{
}

void Painter::Draw(const Tile& tile, int x, int y)
{
	//位块传输，用来贴图
	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = tile.tilew;
	dstrect.h = tile.tileh;
	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Animation& ani, int x, int y)
{
	if (ani.Stoped())return;
	const Tile& tile = ani.GetTile();
	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = tile.tilew;
	dstrect.h = tile.tileh;

	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Animation& ani, int x, int y, int w, int h)
{
	if (ani.Stoped())return;
	const Tile& tile = ani.GetTile();

	SDL_Rect srcrect, dstrect;
	srcrect.x = tile.picx;
	srcrect.y = tile.picy;
	srcrect.w = tile.tilew;
	srcrect.h = tile.tileh;

	dstrect.x = x;
	dstrect.y = y;
	dstrect.w = w;
	dstrect.h = h;

	SDL_RenderCopy(renderer_, tile.pPic->texture, &srcrect, &dstrect);
}

void Painter::Draw(const Text& tex, int x, int y)
{
	//绘制文字
	SDL_Rect dstrect;
	dstrect.x = tex.x;
	dstrect.y = tex.y;
	dstrect.w = tex.w;
	dstrect.h = tex.h;

	SDL_RenderCopy(renderer_, tex.texture, NULL, &dstrect);
}

void Painter::Erase()
{
	//对整个绘图区域进行擦除
	SDL_RenderClear(renderer_);
}

void Painter::Apply()
{
	//把缓冲页面的内容贴到屏幕设备上
	SDL_RenderPresent(renderer_);
}