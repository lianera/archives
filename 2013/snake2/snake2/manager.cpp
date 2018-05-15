//*********************manager.cpp*******************

#include "manager.h"
#include "element.h"
#include <time.h>
#include <iterator>
#include "sound.h"
using namespace std;

void FullScrAni::PreInit(int w, int h)
{
	flscrw = w;
	flscrh = h;
}

//-------------------Game-----------------------------

Game::Game(int clw, int clh)
	:clientw(clw), clienth(clh), pPanel(NULL), level(0), pPainter(NULL),
	roundtime(0)
{
	srand((unsigned int)time(0));

	try {
		// 初始化SDL
		if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO) != 0)
			throw SDL_GetError();
		if (TTF_Init() == -1)
			throw SDL_GetError();

		window = SDL_CreateWindow("Snake",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			clw, clh, 
			SDL_WINDOW_SHOWN| SDL_WINDOW_ALLOW_HIGHDPI);
		if (!window)
			throw SDL_GetError();

		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer)
			throw SDL_GetError();
	}
	catch (const string& msg) {
		if (renderer)
			SDL_DestroyRenderer(renderer);
		if (window)
			SDL_DestroyWindow(window);
		throw msg;
	}
	
	pPainter = new Painter(renderer, 0, 0, clientw, clienth);
	//PreInit
	Picture::PreInit(renderer);
	Elebase::PreInit(pPainter, &manager);
	FullScrAni::PreInit(clientw, clienth);
	Text::PreInit(renderer, "res/DroidSansMono.ttf");
	
	//加载资源
	try
	{
		manager.LoadRes();
		ShowLogo();
		pPainter->Apply();
		Sound::Instance()->Load();
	}
	catch (Error err)
	{
		err.Print();
	}
	state = GAME_SHOWMENU;
}

Game::~Game()
{
	manager.ReleaseRes();
	Sound::Instance()->Release();

	if (renderer){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	
	if (window)
		SDL_DestroyWindow(window);
	Text::PostRelease();

	if (pPanel) { delete pPanel; pPanel = NULL; }
	if (pPainter) { delete pPainter; pPainter = NULL; }
	
	TTF_Quit();
	SDL_Quit();
}

void Game::Create()
{
	if (pPanel) delete pPanel;
	//创建游戏面板
	pPanel = new Panel(level);
	Sound::Instance()->PlayMusic("map0.mid");
	state = GAME_RUN;
	roundtime = 0;
}

void Game::ShowWin()//显示胜利信息
{
	static FullScrAni ani;
	static Button butcont(clientw / 2 - 100, clienth * 4 / 5);

	ani.SetAni(ANI_WIN);
	ani.Draw();
	butcont.SetAni(ANI_BUTTON, ANI_BUTTON_CONTINUE);
	butcont.Deal();
	butcont.Draw();
	if (butcont.MousePressed())
	{
		butcont.Recover();
		state = GAME_RESTART;
		level++;
		ani.Ani()->Restart();
	}
}

void Game::ShowFail()//显示失败信息
{
	static FullScrAni ani;
	static Button butcont(clientw * 4 / 5 - 200, clienth * 4 / 5);
	static Button butback(clientw / 5, clienth * 4 / 5);

	ani.SetAni(ANI_FAIL);
	ani.Draw();
	butcont.SetAni(ANI_BUTTON, ANI_BUTTON_CONTINUE);
	butcont.Deal();
	butcont.Draw();
	butback.SetAni(ANI_BUTTON, ANI_BUTTON_BACK);
	butback.Deal();
	butback.Draw();
	if (butcont.MousePressed())
	{
		butcont.Recover();
		state = GAME_RESTART;
		ani.Ani()->Restart();
	}
	else if (butback.MousePressed())
	{
		butback.Recover();
		level = 1;
		Destroy();
		state = GAME_SHOWMENU;
	}
}

void Game::ShowOver()//显示游戏结束
{
	Destroy();
	state = GAME_SHOWMENU;
}

void Game::Destroy()//销毁游戏成员对象
{
	if (pPanel)
	{
		delete pPanel;
		pPanel = NULL;
	}
}

void Game::Loop()//游戏的循环
{
	SDL_Event e;
	bool quit = false;

	while (!quit) {
		pPainter->Erase();
		SDL_PumpEvents();
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT) {
			quit = true;
		}

		try
		{
			//根据游戏状态控制游戏流程
			switch (state)
			{
			case GAME_SHOWMENU:
				ShowMenu();
				break;
			case GAME_SHOWHELP:
				ShowHelp();
				break;
			case GAME_CREATE:
				Create();
				break;
			case GAME_RUN:
				Run();
				break;
			case GAME_DESTROY:
				Destroy();
				break;
			case GAME_RESTART:
				ReStart();
				break;
			case GAME_WIN:
				ShowWin();
				break;
			case GAME_FAIL:
				ShowFail();
				break;
			case GAME_OVER:
				break;
			case GAME_EXIT:
				quit = true;
				break;
			}

		}
		catch (Error err)
		{
			err.Print();
		}
		pPainter->Apply();
		SDL_Delay(1000 / 30);
	}
}

void Game::ReStart()
{
	Destroy();
	Create();
}

void Game::Run()//游戏运行
{
	//界面元素的创建
	static FullScrAni outline;
	static Text texlevel(675, 217, 20, 20);
	static Text texfood(675, 300, 20, 20);
	static Text texscore(675, 387, 20, 20);

	static Button butpause(628, 500, 100, 50);
	static Button butret(740, 40, 50, 50);
	//界面元素动画的设置
	butpause.SetAni(ANI_BUTTON, pPanel->IsPause() ? ANI_BUTTON_RECOVER : ANI_BUTTON_PAUSE);
	butret.SetAni(ANI_BUTTON, ANI_BUTTON_RETURN);

	outline.SetAni(ANI_OUTLINE);
	//响应
	butpause.Deal();
	butret.Deal();
	if (butpause.MousePressed())
	{//是否暂停
		butpause.Recover();
		pPanel->SetPause(!pPanel->IsPause());
	}

	outline.Draw();
	pPanel->DoFrame();
	//绘制
	butpause.Draw();
	butret.Draw();

	texlevel.SetText(level, SDL_Color{ 255, 255, 255 });
	texscore.SetText(pPanel->Score(), SDL_Color{ 255, 255, 255 });
	texfood.SetText(pPanel->RestFood(), SDL_Color{ 255, 255, 255 });
	texlevel.Draw();
	texscore.Draw();
	texfood.Draw();
	if (pPanel->Overed())
	{
		if (pPanel->Won())state = GAME_WIN;
		else state = GAME_FAIL;
		Sound::Instance()->StopMusic();
	}
	if (butret.MousePressed())
	{
		butret.Recover();
		Destroy();
		state = GAME_SHOWMENU;
	}

	roundtime += 1000 / FPS;
	if (roundtime % 1000<1000 / FPS)//每秒储存一次
		SaveArchive();
}

void Game::ShowHelp()//显示帮助信息
{
	static FullScrAni helppage;
	static Button helpback(clientw * 3 / 4 - 30, clienth * 7 / 8, 200, 50);
	helppage.SetAni(ANI_HELP);
	helpback.SetAni(ANI_BUTTON, ANI_BUTTON_BACK);

	helpback.Deal();
	helppage.Draw();
	helpback.Draw();
	if (helpback.MousePressed())
	{
		helpback.Recover();
		state = GAME_SHOWMENU;
	}
}

void Game::ShowMenu()//显示菜单
{
	static bool firstrun = true;
	//创建界面元素
	static FullScrAni menubk;

	static Button butnew(clientw / 2, clienth / 3 + 20);
	static Button butold(clientw / 2, clienth / 3 + 80);
	static Button butsel(clientw / 2, clienth / 3 + 140);
	static Button buthelp(clientw / 2, clienth / 3 + 200);
	static Button butquit(clientw / 2, clienth / 3 + 260);

	static SelBox sel(clientw / 8 + 110, clienth * 2 / 5 + 45, 150, 50);
	static Rect selbkground(clientw / 8, clienth * 2 / 5, clientw / 3, clienth / 3);
	static Button butok(clientw / 8 + 20, clienth * 2 / 5 + 120, 100, 50);
	static Button butcl(clientw / 8 + 145, clienth * 2 / 5 + 120, 100, 50);

	Sound::Instance()->PlayMusic("menubkm.mid");

	if (firstrun)
	{
		//设置动画
		menubk.SetAni(ANI_MENUBK);
		butnew.SetAni(ANI_BUTTON, ANI_BUTTON_NEW);
		butold.SetAni(ANI_BUTTON, ANI_BUTTON_OLD);
		butsel.SetAni(ANI_BUTTON, ANI_BUTTON_SEL);
		buthelp.SetAni(ANI_BUTTON, ANI_BUTTON_HELP);
		butquit.SetAni(ANI_BUTTON, ANI_BUTTON_QUIT);

		butok.SetAni(ANI_BUTTON, ANI_BUTTON_OK);
		butcl.SetAni(ANI_BUTTON, ANI_BUTTON_CANCEL);
		sel.Add(1).Add(2).Add(3).Add(4).Add(5).Add(6).Add(7).Add(8).Add(9);
		sel.SetAni(-1, ANI_BUTTON, ANI_BUTTON, -1, ANI_BUTTON_LEFT, ANI_BUTTON_RIGHT);
		selbkground.SetAni(ANI_SELBKGROUND);
	}

	//界面元素的响应
	butnew.Deal();
	butold.Deal();
	butsel.Deal();
	buthelp.Deal();
	butquit.Deal();
	//界面元素的绘制
	menubk.Draw();
	butnew.Draw();
	butold.Draw();
	butsel.Draw();
	buthelp.Draw();
	butquit.Draw();

	//检测界面元素的状态
	if (butnew.MousePressed())//新的开始
	{
		level = 1;
		butnew.Recover();
		state = GAME_CREATE;
	}
	if (butold.MousePressed())//旧的回忆
	{
		ReadArchive();
		butold.Recover();
		state = GAME_RUN;
	}
	if (butsel.MousePressed())//选择关卡
	{
		selbkground.Draw();
		
		sel.Deal();
		
		sel.Draw();
		
		butok.Deal();
		butcl.Deal();
		
		butok.Draw();
		butcl.Draw();
		if (butok.MousePressed())
		{
			butsel.Recover();
			butok.Recover();
			level = atoi(sel.str().c_str());
			state = GAME_CREATE;
		}
		if (butcl.MousePressed())
		{
			butcl.Recover();
			butsel.Recover();
		}
	}
	if (buthelp.MousePressed())//帮助
	{
		buthelp.Recover();
		state = GAME_SHOWHELP;
	}
	if (butquit.MousePressed())
	{
		state = GAME_EXIT;
	}
	firstrun = false;

}

void Game::ShowLogo()
{
	Rect anilog(400, 450, 350, 100);
	anilog.SetAni(ANI_LOGO);
	anilog.Draw();

}

void Game::ReadArchive()//读取存档
{
	ifstream ifs("res/archive.txt");
	ifs >> level;
	if (pPanel) delete pPanel;
	if (level <= 0)
	{
		level = 1;
		pPanel = new Panel(level);
	}
	else
	{
		pPanel = new Panel(level);
		ifs >> *pPanel;
	}
	ifs.close();
	Sound::Instance()->PlayMusic("map0.mid");
	state = GAME_RUN;
	roundtime = 0;
}

void Game::SaveArchive()//保存存档
{
	ofstream ofs("res/archive.txt");
	if (!ofs)throw Outer_Error("打开存档文件失败!");
	ofs << level << endl;
	ofs << *pPanel;
	ofs.close();
}


//----------------------Manager----------------------------
Manager::~Manager()
{
}

void Manager::ReleaseRes()
{
	for (auto e : pics)
		delete e;
	pics.clear();
	for (auto e : tiles)
		delete e;
	tiles.clear();
	for (auto e : anis)
		delete e;
	anis.clear();
}

void Manager::LoadRes()//加载资源
{
	Add("res/logo.png");
	Add("res/background.png");
	Add("res/fail.png");
	Add("res/win.png");//3
	Add("res/game.png");//4
	Add("res/ui.png");//5
	Add("res/outline.png");//6
	Add("res/help.png");//7

	Add(0, 0, 0, 800, 600);//0 logo
	Add(1, 0, 0, 450, 450);//1 ground 
	Add(1, 450, 0, 450, 450);//2 ground 
	Add(1, 0, 450, 450, 450);//3 ground 
	Add(1, 450, 450, 450, 450);//4 ground 

	Add(2, 0, 0, 800, 600);//5 fail pic

	Add(3, 0, 0, 800, 600);//6 win pic

	Add(4, 0, 0, 30, 30);//7 snake head
	Add(4, 30, 0, 30, 30);//8 snake body
	Add(4, 60, 0, 30, 30);//9 snake tail
	Add(4, 90, 0, 30, 30);//10 food

						  //ui
	Add(5, 0, 0, 200, 50);//11new 
	Add(5, 0, 50, 200, 50);//12 old
	Add(5, 0, 100, 200, 50);//13 sel
	Add(5, 0, 150, 200, 50);//14 help
	Add(5, 0, 200, 200, 50);//15 quit
	Add(5, 0, 250, 200, 50);//16 继续
	Add(5, 0, 300, 200, 50);//17 返回
	Add(6, 0, 0, 800, 600);//18 outline

	Add(5, 200, 0, 50, 50);//19selbox <
	Add(5, 250, 0, 50, 50);//20selbox >
	Add(5, 300, 0, 350, 250);//21select level background

	Add(5, 0, 350, 100, 50);//22 确定
	Add(5, 100, 350, 100, 50);//23 取消

	Add(7, 0, 0, 800, 600);//24 帮助

	Add(5, 0, 400, 100, 50);//25 确定
	Add(5, 100, 400, 100, 50);//26 取消
	Add(5, 200, 50, 50, 50);//27 x按钮

	Add(5, 300, 250, 350, 100);//logo 28

	vector<int> iv;

	Add(0);//menubackground
	Add(7);//snake 1
	Add(8);
	Add(9);
	Add(10);//food 4
			//5 ground
	Add(1); Add(2); Add(3); Add(4);

	Add(5);//fail 9

	Add(6);//win 10

		   //ui botton 11-15
	Add(11); Add(12); Add(13); Add(14); Add(15); Add(16); Add(17);
	//outline
	Add(18);
	Add(19); Add(20);//selbox<>
	Add(21);//select level background
	Add(22); Add(23);//确定 取消22 23

	Add(24);//帮助页面24
	Add(25); Add(26); Add(27);//暂停 返回 X 25 26 27
	Add(28);//logo 28
}

Animation* Manager::PickAni(int p1, int p2, int p3)
{      //这个函数提供从标示符到动画指针的转换
	switch (p1)
	{
	case ANI_LOGO:		return anis[28]; break;
	case ANI_MENUBK:	return anis[0];	break;
	case ANI_SNAKE_HEAD:return anis[1];	break;
	case ANI_SNAKE_BODY:return anis[2];	break;
	case ANI_SNAKE_TAIL:return anis[3];	break;
	case ANI_FOOD:		return anis[4];	break;
	case ANI_BACKGROUND:
		switch (p2)
		{
		case ANI_BACKGROUND_GRASS:	return anis[5];	break;
		case ANI_BACKGROUND_WOOD:	return anis[6];	break;
		case ANI_BACKGROUND_ROCK:	return anis[7];	break;
		case ANI_BACKGROUND_TILE:	return anis[8];	break;
		}
		return anis[5];
		break;
	case ANI_FAIL:	return anis[9];	break;
	case ANI_WIN:	return anis[10]; break;
	case ANI_BUTTON:
		switch (p2)
		{
		case ANI_BUTTON_NEW:return anis[11]; break;
		case ANI_BUTTON_OLD:return anis[12]; break;
		case ANI_BUTTON_SEL:return anis[13]; break;
		case ANI_BUTTON_HELP:return anis[14]; break;
		case ANI_BUTTON_QUIT:return anis[15]; break;
		case ANI_BUTTON_CONTINUE:return anis[16]; break;
		case ANI_BUTTON_BACK:return anis[17]; break;
		case ANI_BUTTON_LEFT:return anis[19]; break;
		case ANI_BUTTON_RIGHT:return anis[20]; break;
		case ANI_BUTTON_OK:return anis[22]; break;
		case ANI_BUTTON_CANCEL:return anis[23]; break;
		case ANI_BUTTON_PAUSE:return anis[25]; break;
		case ANI_BUTTON_RECOVER:return anis[26]; break;
		case ANI_BUTTON_RETURN:return anis[27]; break;
		}
		break;
	case ANI_OUTLINE:return anis[18]; break;
	case ANI_SELBKGROUND:return anis[21]; break;
	case ANI_HELP:return anis[24]; break;
	}
	return NULL;
}

//以下是添加图元的函数
void Manager::Add(const string& picfile)
{
	pics.push_back(new Picture(picfile.c_str()));
}

void Manager::Add(Picture *ppic, int x, int y, int w, int h)
{
	tiles.push_back(new Tile(ppic, x, y, w, h));
}

void Manager::Add(int picid, int x, int y, int w, int h)
{
	tiles.push_back(new Tile(pics.at(picid), x, y, w, h));
}

void Manager::Add(const vector<int>& tilevct, int cycl, int interval)
{
	Animation* pani = new Animation(cycl, interval);
	anis.push_back(pani);
	for (int i = 0; i<(int)tilevct.size(); i++)
		pani->AddTile(tiles.at(tilevct[i]));
}

void Manager::Add(int aniIndex)
{
	Animation* pani = new Animation(0, 10);
	anis.push_back(pani);
	pani->AddTile(tiles.at(aniIndex));
}

