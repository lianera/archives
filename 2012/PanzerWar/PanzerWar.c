#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")

#define KEY(k) GetAsyncKeyState(k)
#define BUMP(x1,y1,w1,h1,x2,y2,w2,h2) (((x1)+(w1)>(x2))&&((x1)<(x2)+(w2))&&((y1)+(h1)>(y2))&&((y1)<(y2)+(h2)))
#define INFINITE 0xFFFFFFFF
#define DEFRESIST -1
#define RESISTNORMAL 1000
#define ERRORRET -1
#define MAPX g_map[g_curmap].item.x
#define MAPY g_map[g_curmap].item.y
#define MAPWID g_map[g_curmap].item.w
#define MAPHEI g_map[g_curmap].item.h
#define TANKNUM g_map[g_curmap].tanknum
#define BLKNUM g_map[g_curmap].blknum
#define NXTP do{p++;}while(*p!=',');do{p++;}while(*p==' ');//读取配置文件时转到下一个逗号后的参数
#define RMVB do{p++;}while(*p==' ');//消除空格
#define LOOPINC(i,lim) ((i)>=((lim)-1)?(i)=0:++(i))
#define LOOPSUB(i,lim) ((i)<=0?(i)=(lim)-1:--(i))
#define SVPG(buf) fwrite(&(buf),sizeof(buf),1,fp)
#define RDPG(buf) fread(&(buf),sizeof(buf),1,fp)
#define PLAYANI(index) g_ani[(index)].state=ANI_RUN
#define STOPANI(index) g_ani[(index)].state=ANI_INVISIBLE
#define SOUND(name) PlaySound((name),NULL,SND_FILENAME|SND_ASYNC)
#define FULLEXP ((60)*g_curlevel-20)
#define INFO_MAP_X 30
#define INFO_MAP_Y 0
#define INFO_HP_X 30
#define INFO_HP_Y 30
#define INFO_LEVEL_X 30
#define INFO_LEVEL_Y 60
#define INFO_EXP_X 30
#define INFO_EXP_Y 90
#define INFO_TEXTCOLOR(ra) RGB(255,255*(ra),255*(ra))

#define GAP_X 250
#define GAP_Y 150
#define GAP_STEP 30
#define GAP_MAP_X GAP_X
#define GAP_MAP_Y GAP_Y-GAP_STEP
#define GAP_HP_X GAP_X
#define GAP_HP_Y GAP_Y+GAP_STEP*0
#define GAP_LEVEL_X GAP_X
#define GAP_LEVEL_Y GAP_Y+GAP_STEP*1
#define GAP_EXP_X GAP_X
#define GAP_EXP_Y GAP_Y+GAP_STEP*2
#define GAP_POWER_X GAP_X
#define GAP_POWER_Y GAP_Y+GAP_STEP*3
#define GAP_RATE_X GAP_X
#define GAP_RATE_Y GAP_Y+GAP_STEP*4
#define GAP_TEXTCOLOR RGB(50,50,255)
#define EXPADD 20
#define BLOCKEXPRATE 30
#define MAXNAMELEN 50
#define MAXBMPNUM 50
#define MAXANIPAGE 10
#define MAXMAPNUM 30
#define MAXLEVELNUM 30
#define MAXPICITEMNUM 100
#define MAXTANKTYPE 30
#define MAXANINUM 300
#define MAXBLOCKTYPE 30
#define MAXBLOCKNUM 200
#define MAXTANKNUM 50
#define MAXGUNTYPE 30
#define MAXLEVEL 15
#define GAMEOVERDELAY 15
#define WINDELAY 50
#define GAPDELAY 50
#define ADDHPITV 50

#define FILE_PROGRESS "./data/progress.dat"
#define FILE_CONFIG "./data/data.dat"
#define SOUND_GUN "./data/gun0.wav"
#define SOUND_EXPLO "./data/explo0.wav"
#define SOUND_BUMP "./data/bump0.wav"
#define SOUND_MENU "./data/menu0.wav"
//------------------自定数据类型-------------------------
enum E_EORROR{ERR_REMIND,ERR_TERMINAL};
enum E_GAMESTATE{GAME_INIT,GAME_SHOWCOVER,GAME_MENU,GAME_LOADPROGRESS,
					GAME_RETURN,GAME_RUN,GAME_OVER,GAME_WIN,
					GAME_SHOWGAPSCR,GAME_HELP,GAME_QUIT};
enum E_ANI{ANI_LOGO=0,ANI_MENU=1,ANI_EXPLO=2,ANI_GAMEOVER=3,ANI_EXPLO1=4,ANI_UPGRADE=5,
			ANI_WIN=6,ANI_RETMENU=7,ANI_PRESSENTER=8,ANI_BLACKSCR=9,ANI_SAVE=10,ANINUM};
enum E_BLOCK{BLK_GRASS=0};
enum E_DIRECT{DX_LEFT,DX_UP,DX_RIGHT,DX_DOWN};
enum E_ANISTATE{ANI_INVISIBLE,ANI_FREE,ANI_RUN,ANI_SHOWONCE,ANI_RUNTOFREE};

struct tpPicItem
{
	int x,y;
	int w,h;
	int bmp;
}g_picitem[MAXPICITEMNUM];

struct tpScrItem
{
	int x,y;
	int w,h;
	int picid[MAXANIPAGE];
	int timecouter;
	int curpic;
	int picstep;
	int picnum;
};

struct tpANI
{
	struct tpScrItem item;
	int state;
}g_ani[MAXANINUM];

struct tpBlock//地图固定方块
{
	struct tpScrItem item;
	int life;
	int resist;
}g_blk[MAXBLOCKNUM];

struct tpTank
{
	struct tpScrItem item;
	int life;
	int fulllife;
	int rate;
	int currate;
	int gun;
	int guninterval;
	int guncounter;
	enum E_DIRECT dx;
//	int resist;
}g_tank[MAXTANKNUM];

struct tpGun
{
	struct tpScrItem item;
	int rate;
	struct tpTank *father;
	int power;
	enum E_DIRECT dx;
	struct tpGun *next;
}*g_gun;

struct tpMap
{
	struct tpScrItem item;
	int tanknum;
	int blknum;
}g_map[MAXMAPNUM];

struct tpLevel
{
	int mapid;
}g_level[MAXLEVELNUM];

struct tpConfig
{
	char WIN_CLASS[MAXNAMELEN],WIN_TITLE[MAXNAMELEN];
	int WIN_WIDTH,WIN_HEIGHT;
	int TIME_WAITNUM,TIME_INTERVAL;
	DWORD BMP_TRANSP;
	int MAP_NUM;
//	int LEVEL_NUM;
	int BMPNUM;
	char BMPNAME[MAXBMPNUM][MAXNAMELEN];
	int PICITEMNUM;
}G_CFG;
//----------------全局变量-------------------------
HWND g_hWnd;
HDC g_hDC,g_hSwapDC;
HDC *g_hBmpDC;
HBITMAP g_hSwapBmp;
HBITMAP *g_hBmp;
int g_curlevel;
int g_curexp;
int g_curmap;
int g_offsetX;
int g_offsetY;
char *g_cfgbuf;
struct tpGun *g_pGunHead;
struct tpTank g_tanktype[MAXTANKTYPE];
struct tpBlock g_blktype[MAXBLOCKTYPE];
struct tpGun g_guntype[MAXGUNTYPE];

enum E_GAMESTATE g_gamestate;
//-----------------函数声明---------------------------
void Init(HINSTANCE hInstance,int nCmdShow);
void Destroy();
void GameLoop();
void Draw(struct tpScrItem *scritem);
void ShowMenu();
void ShowRetMenu();
void ShowGapscr();
void ShowHelp();
void GameRun();
void GameInit();
void GameOver();
void GameAIMove(int *x,int *y,struct tpTank *tank);
void GameAIGun(struct tpTank *tank);
void CreateGun(struct tpTank *tank);
void TankEvent();
void GunEvent();
void ClrGun(struct tpGun *gun);
void GameShutdown();
void GameDraw();
void GameWin();
void DrawInfo();
void OnUpgrade();
void SaveProgress();
void LoadProgress();
void CalcTank(int level,struct tpTank* tank);
int ReadInt(char name[]);
int ReadStr(char name[],char *str);
int ReadPicItem(int index,struct tpPicItem* picitem);
int ReadMapInfo(int index,struct tpMap* map);
int ReadLevelInfo(int index,struct tpLevel* level);
int ReadTank(int index,struct tpTank* tank);
int ReadANI(int index,struct tpANI* ani);
int ReadBlock(int index,struct tpBlock* block);
int ReadGun(int index,struct tpGun* gun);
void ReadMapBlock(int index);
void ReadMapTank(int map);
void ReadCfg();
int StartAni(int index,int x,int y,enum E_ANISTATE state);
void OnError(char info[],enum E_EORROR err);
int ProduceRandMap();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//------------------主函数及消息模块START-----------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	//初始化
	int timewaitnum=0;
	Init(hInstance,nCmdShow);
	g_gamestate=GAME_SHOWCOVER;
	while (g_gamestate!=GAME_QUIT)
	{
		//消息处理
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(timewaitnum>=G_CFG.TIME_WAITNUM)
		{//进入游戏循环
			GameLoop();
			timewaitnum=0;
		}
		else timewaitnum++;
		//等待下一帧
		Sleep(G_CFG.TIME_INTERVAL);
	}
	Destroy();//释放变量
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_PAINT:
			hdc = BeginPaint(g_hWnd, &ps);
			EndPaint(g_hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KILLFOCUS:
			break;
		case WM_SETFOCUS:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//--------------主函数及消息模块END----------------------

//-------------初始化模块START-----------------
void Init(HINSTANCE hInstance,int nCmdShow)
{
	int i;
	unsigned flen;
	FILE *fp;
	WNDCLASSEX wc={sizeof(WNDCLASSEX),
		CS_CLASSDC,WndProc,
		0L,
		0L,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(0),
		NULL,
		NULL,
		NULL};

	//全局变量初始化
	g_hWnd=0;
	g_gamestate=GAME_INIT;
	g_pGunHead=NULL;
	g_curlevel=1;
	g_curexp=0;
	g_gun=NULL;

	for(i=0;i<MAXANINUM;i++)
	{
		if(i<ANINUM)
			g_ani[i].state=ANI_INVISIBLE;
		else g_ani[i].state=ANI_FREE;
		g_ani[i].item.timecouter=0;
	}

	//配置文件初始化
	if((fp=fopen(FILE_CONFIG,"rb"))==NULL)
		OnError("打开配置文件失败",ERR_TERMINAL);
	fseek(fp,0L,SEEK_END); 
	flen=ftell(fp);
	g_cfgbuf=(char*)malloc(flen);
	fseek(fp,0L,SEEK_SET);
	if(!fread(g_cfgbuf,flen,1,fp))
		OnError("读取配置文件错误",ERR_TERMINAL);
	fclose(fp);
	
	ReadCfg();

	//窗口初始化
	wc.lpszClassName=G_CFG.WIN_CLASS;
	RegisterClassEx(&wc);

   g_hWnd=CreateWindowEx((unsigned)NULL,G_CFG.WIN_CLASS, G_CFG.WIN_TITLE, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
	   (GetSystemMetrics(SM_CXSCREEN)-G_CFG.WIN_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-G_CFG.WIN_HEIGHT)/2,
	   G_CFG.WIN_WIDTH+GetSystemMetrics(SM_CXFIXEDFRAME)*2,
	   G_CFG.WIN_HEIGHT+GetSystemMetrics(SM_CXFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION),
	   NULL, NULL, hInstance, NULL);
	
   if (!g_hWnd)OnError("创建窗口失败",ERR_TERMINAL);
   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);
	//绘图初始化
	g_hDC=GetDC(g_hWnd);
	g_hSwapDC=CreateCompatibleDC(g_hDC);
	g_hSwapBmp=CreateCompatibleBitmap(g_hDC,G_CFG.WIN_WIDTH,G_CFG.WIN_HEIGHT);
	SelectObject(g_hSwapDC,g_hSwapBmp);
	SetTextColor(g_hSwapDC,INFO_TEXTCOLOR(1));
	SetBkMode(g_hSwapDC,TRANSPARENT);
	for(i=0;i<G_CFG.BMPNUM;i++)
	{
		g_hBmp[i]=(HBITMAP)LoadImage(hInstance,G_CFG.BMPNAME[i],IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION |LR_LOADFROMFILE);
		if(g_hBmp==NULL)OnError("加载位图失败",ERR_TERMINAL);
		g_hBmpDC[i]=CreateCompatibleDC(g_hDC);
		SelectObject(g_hBmpDC[i],g_hBmp[i]);
	}
	
}


int ProduceRandMap()
{
	int i,j,k,blk,tank;
	char tankbuf[500]={0},buf[50];
	FILE *fp;
	int evryblknum=4;
	blk=0;
	tank=0;
	if((fp=fopen("map.txt","wt"))==NULL)
	{
		printf("Error");
		exit(0);
	}
	srand((int)g_hWnd);
	for(k=0;k<16;k++)
	{
		fprintf(fp,"<MapBlocks%d>\n",k);
		for(i=0;i<550;i+=50)
			for(j=0;j<750;j+=(rand()%50+50))
			{
				if(!(rand()%(20-k))||i==0&&j==0)
				{
					sprintf(buf,"(%d,%d,%d)",j,i+rand()%50,(int)(tank/2)*2);
					strcat(tankbuf,buf);
				}
				else
					fprintf(fp,"(%d,%d,%d)",j,i+rand()%50,blk+rand()%evryblknum);
			}
		if(blk>16)	
		{
			MessageBox(g_hWnd,"错误!","ok",MB_OK);
			exit(0);
		}
		else if((k+1)%evryblknum==0)blk+=evryblknum;
	//	if(blk>=16) blk=0;
		tank++;
		fprintf(fp,"\n</MapBlocks%d>\n\n",k);
		fprintf(fp,"<MapTanks%d>\n",k);
		fputs(tankbuf,fp);
		fprintf(fp,"\n</MapTanks%d>\n\n",k);
		strcpy(tankbuf,"");
	}
	MessageBox(g_hWnd,"Done!","ok",MB_OK);
	fclose(fp);
	return 0;
}


void ReadCfg()
{
	int i,r,g,b;
	char fname[MAXNAMELEN];
	char tmp[MAXNAMELEN];
	//读取配置文件
	ReadStr("WindowClass",G_CFG.WIN_CLASS);
	ReadStr("WindowName",G_CFG.WIN_TITLE);
	G_CFG.WIN_WIDTH		=ReadInt("WindowWidth");
	G_CFG.WIN_HEIGHT	=ReadInt("WindowHeight");
	G_CFG.TIME_WAITNUM	=ReadInt("WaitNumEveryCycle");
	G_CFG.TIME_INTERVAL	=ReadInt("TimeInterval");

	r=ReadInt("TransparentColorRed");
	g=ReadInt("TransparentColorGreen");
	b=ReadInt("TransparentColorBlue");
	G_CFG.BMP_TRANSP=RGB(r,g,b);

/*	//Read Level Info
	for(i=0;ReadLevelInfo(i,&g_level[i]);i++)
		;
	if(i<=0)
		OnError("读取等级信息时发生错误",ERR_TERMINAL);
	G_CFG.LEVEL_NUM=i;
*/
	//Read Map Info
	for(i=0;ReadMapInfo(i,&g_map[i]);i++)
		;
	if(i<=0)
		OnError("读取地图信息时发生错误",ERR_TERMINAL);
	G_CFG.MAP_NUM=i;

	//Read Bitmap name
	for(i=0;;i++)
	{
		sprintf(tmp,"BmpName%d",i);
		if(!ReadStr(tmp,fname)) break;
		strcpy(G_CFG.BMPNAME[i],fname);
	}
	if(i<=0)
		OnError("读取图像时发生错误",ERR_TERMINAL);
	G_CFG.BMPNUM=i;

	g_hBmp=(HBITMAP*)calloc(G_CFG.BMPNUM,sizeof(HBITMAP));
	g_hBmpDC=(HDC*)calloc(G_CFG.BMPNUM,sizeof(HDC));
	
	//Read PicItem
	for(i=0;ReadPicItem(i,&g_picitem[i]);i++)
		;
	if(i<=0)
		OnError("读取图像项目时发生错误",ERR_TERMINAL);
	G_CFG.PICITEMNUM=++i;
	
	//Read Tank Type
	for(i=0;i<MAXTANKTYPE;i++)
		if(!ReadTank(i,&g_tanktype[i]))
			break;
	if(i<=0)
		OnError("读取坦克类型时发生错误",ERR_TERMINAL);
	//read block type
	for(i=0;i<MAXBLOCKTYPE;i++)
		if(!ReadBlock(i,&g_blktype[i]))
			break;
	if(i<=0)
		OnError("读取方块类型时发生错误",ERR_TERMINAL);

	//read ani
	for(i=0;i<MAXANINUM;i++)
		if(!ReadANI(i,&g_ani[i]))
			break;
	if(i<=0)
		OnError("读取用户界面时发生错误",ERR_TERMINAL);
	//read gun
	for(i=0;i<MAXANINUM;i++)
		if(!ReadGun(i,&g_guntype[i]))
			break;
	if(i<=0)
		OnError("读取用户界面时发生错误",ERR_TERMINAL);
}

//保存进度
void SaveProgress()
{
	FILE *fp;
	int i,nblk,ntank,nani;
	nblk=BLKNUM;
	ntank=TANKNUM;
	nani=MAXANINUM;
	if((fp=fopen(FILE_PROGRESS,"wb"))==NULL)
	{
		OnError("打开存档文件错误",ERR_REMIND);
		return;
	}

	SVPG(g_curlevel);
	SVPG(g_curexp);
	SVPG(g_curmap);
	SVPG(g_offsetX);
	SVPG(g_offsetY);
	//方块
	SVPG(nblk);
	for(i=0;i<nblk;i++)
		SVPG(g_blk[i]);
	//坦克
	SVPG(ntank);
	for(i=0;i<ntank;i++)
		SVPG(g_tank[i]);
	
	//动画
	SVPG(nani);
	for(i=0;i<nani;i++)
		SVPG(g_ani[i]);

	fclose(fp);
}

void LoadProgress()
{
	FILE *fp;
	int i,nblk,ntank,nani;
	if((fp=fopen(FILE_PROGRESS,"rb"))==NULL)
	{
		OnError("打开存档文件错误",ERR_REMIND);
		return;
	}
	RDPG(g_curlevel);
	RDPG(g_curexp);
	RDPG(g_curmap);
	RDPG(g_offsetX);
	RDPG(g_offsetY);
	//方块
	RDPG(nblk);
	if(nblk<0||nblk>MAXBLOCKNUM)
		OnError("读取存档错误",ERR_TERMINAL);
	for(i=0;i<nblk;i++)
		RDPG(g_blk[i]);
	//坦克
	RDPG(ntank);
	if(ntank<0||ntank>MAXTANKNUM)
		OnError("读取存档错误",ERR_TERMINAL);
	for(i=0;i<ntank;i++)
		RDPG(g_tank[i]);

	
	//动画
	RDPG(nani);
	if(nani<0||nani>MAXANINUM)
		OnError("读取存档错误",ERR_TERMINAL);
	for(i=0;i<nani;i++)
		RDPG(g_ani[i]);

	BLKNUM=nblk;
	TANKNUM=ntank;

	fclose(fp);
}


int ReadInt(char name[])
{
	char *p;
	if((p=strstr(g_cfgbuf,name))==NULL)
		OnError("读取整数时发生错误",ERR_TERMINAL);
	p=strchr(p,'=');
	RMVB
	return atoi(p);
}

int ReadStr(char name[],char *str)
{
	char *p,*q;
	int len;
	if((p=strstr(g_cfgbuf,name))==NULL) 
		return 0;
	while(*p!='\"'){p++;}
	q=++p;
	while(*q!='\"'){q++;}
	len=q-p;
	memcpy(str,p,len);
	str[len]='\0';
	return 1;
}

int ReadMapInfo(int index,struct tpMap* map)
{
	char *p;
	char tmp[MAXNAMELEN];
	sprintf(tmp,"Map%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	map->item.w=atoi(p);
	NXTP map->item.h=atoi(p);
	NXTP map->item.picid[0]=atoi(p);
	map->item.picstep=1;
	return 1;	
}

int ReadANI(int index,struct tpANI* ani)
{
	char *p;
	char tmp[MAXNAMELEN];
	int pit;
	sprintf(tmp,"Animation%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	ani->item.x=atoi(p);
	NXTP ani->item.y=atoi(p);
	NXTP ani->item.w=atoi(p);
	NXTP ani->item.h=atoi(p);
	NXTP ani->item.picstep=atoi(p);
	NXTP for(pit=0;(ani->item.picid[pit]=atoi(p))!=0&&*p!='0';pit++)
	{NXTP}
	ani->item.picnum=pit;
	ani->item.timecouter=0;
	ani->state=ANI_INVISIBLE;
	return 1;	
}

int ReadBlock(int index,struct tpBlock* block)
{
	char *p;
	char tmp[MAXNAMELEN];
	int pit;
	sprintf(tmp,"Block%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	block->item.x=atoi(p);
	NXTP block->item.y=atoi(p);
	NXTP block->item.w=atoi(p);
	NXTP block->item.h=atoi(p);
	NXTP block->resist=atoi(p);
	NXTP block->life=atoi(p);
	NXTP block->item.picstep=atoi(p);
	NXTP for(pit=0;(block->item.picid[pit]=atoi(p))!=0;pit++)
	{NXTP}
	block->item.timecouter=0;
	block->item.picnum=pit;
	return 1;	
}

int ReadGun(int index,struct tpGun* gun)
{
	char *p;
	char tmp[MAXNAMELEN];
	int pit;
	sprintf(tmp,"Gun%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	gun->item.x=atoi(p);
	NXTP gun->item.y=atoi(p);
	NXTP gun->item.w=atoi(p);
	NXTP gun->item.h=atoi(p);
	NXTP gun->rate=atoi(p);
	NXTP gun->power=atoi(p);
	NXTP gun->item.picstep=atoi(p);
	NXTP for(pit=0;(gun->item.picid[pit]=atoi(p))!=0;pit++)
	{NXTP}
	gun->item.picnum=pit;
	gun->item.timecouter=0;
	return 1;
}

int ReadTank(int index,struct tpTank* tank)
{
	char *p;
	char tmp[MAXNAMELEN];
	int pit;
	sprintf(tmp,"Tank%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	tank->item.x=atoi(p);
	NXTP tank->item.y=atoi(p);
	NXTP tank->item.w=atoi(p);
	NXTP tank->item.h=atoi(p);
	NXTP tank->rate=atoi(p);
	NXTP tank->life=atoi(p);
	NXTP tank->gun=atoi(p);
	NXTP tank->guninterval=atoi(p);
	NXTP tank->item.picstep=atoi(p);
	NXTP for(pit=0;(tank->item.picid[pit]=atoi(p))!=0;pit++)
	{NXTP}
	tank->item.picnum=pit;
	tank->item.timecouter=0;
	tank->guncounter=0;
	return 1;	
	
}

int ReadLevelInfo(int index,struct tpLevel* level)
{
	char *p;
	char tmp[MAXNAMELEN];
	sprintf(tmp,"Level%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	level->mapid=atoi(p);
	return 1;	
}

int ReadPicItem(int index,struct tpPicItem* picitem)
{
	char *p;
	char tmp[MAXNAMELEN];
	sprintf(tmp,"PictureItem%d",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL) 
		return 0;
	p=strchr(p,'=');
	RMVB
	picitem->x=atoi(p);
	NXTP picitem->y=atoi(p);
	NXTP picitem->w=atoi(p);
	NXTP picitem->h=atoi(p);
	NXTP picitem->bmp=atoi(p);
	
	return 1;
}

void ReadMapTank(int map)
{
	char* p,*q;
	int i,x,y,type;
	char tmp[MAXNAMELEN],endflag[MAXNAMELEN];
	sprintf(tmp,"<MapTanks%d>",map);
	sprintf(endflag,"</MapTanks%d>",map);
	if((p=strstr(g_cfgbuf,tmp))==NULL||
		(q=strstr(g_cfgbuf,endflag))==NULL) 
		OnError("加载地图失败",ERR_TERMINAL);
	i=0;
	while(1)
	{
		p=strchr(p,'(');
		if(p>q)break;
		sscanf(p,"(%d,%d,%d)",&x,&y,&type);
		memcpy(&g_tank[i],&g_tanktype[type],sizeof(struct tpTank));
		g_tank[i].item.x=x;
		g_tank[i].item.y=y;
		g_tank[i].currate=g_tank[i].rate;
		g_tank[i].fulllife=g_tank[i].life;
		p++;i++;
		p=strchr(p,'(');
	}
	TANKNUM=i;


}

void ReadMapBlock(int index)
{
	char* p,*q;
	int i=0,x,y,type;
	char tmp[MAXNAMELEN],endflag[MAXNAMELEN];
	sprintf(tmp,"<MapBlocks%d>",index);
	sprintf(endflag,"</MapBlocks%d>",index);
	if((p=strstr(g_cfgbuf,tmp))==NULL||
		(q=strstr(g_cfgbuf,endflag))==NULL) 
		OnError("加载地图失败",ERR_TERMINAL);
	while(1)
	{
		p=strchr(p,'(');
		if(p>q)break;
		sscanf(p,"(%d,%d,%d)",&x,&y,&type);
		memcpy(&g_blk[i],&g_blktype[type],sizeof(struct tpBlock));
		g_blk[i].item.x=x;
		g_blk[i].item.y=y;
		p++;i++;
	}
	BLKNUM=i;
}
//-------------初始化模块END-----------------

//-------------游戏流程控制START---------------
void GameLoop()
{
	switch(g_gamestate)
	{
	case GAME_SHOWCOVER:
		g_ani[ANI_LOGO].state=ANI_RUN;
		g_ani[ANI_PRESSENTER].state=ANI_RUN;
		Draw(&g_ani[ANI_LOGO].item);
		Draw(&g_ani[ANI_PRESSENTER].item);
		if(KEY(VK_RETURN)) 
		{
			g_ani[ANI_LOGO].state=ANI_INVISIBLE;
			g_ani[ANI_PRESSENTER].state=ANI_INVISIBLE;
			g_gamestate=GAME_MENU;
			Sleep(100);
		}
		
		break;
	case GAME_RUN:
		if(KEY(VK_ESCAPE)) 
		{
			g_gamestate=GAME_RETURN;
		}
		else GameRun();
		break;
	case GAME_OVER:
		g_offsetX=0;
		g_offsetY=0;
		GameOver();
		break;
	case GAME_HELP:
		ShowHelp();
		break;
	case GAME_LOADPROGRESS:
		GameShutdown();
		LoadProgress();
		g_ani[ANI_RETMENU].state=ANI_INVISIBLE;
		g_gamestate=GAME_RUN;
		break;
	case GAME_MENU:
		g_offsetX=0;
		g_offsetY=0;
		ShowMenu();
		break;
	case GAME_RETURN:
		g_offsetX=0;
		g_offsetY=0;
		ShowRetMenu();
		break;
	case GAME_SHOWGAPSCR:
		g_offsetX=0;
		g_offsetY=0;
		ShowGapscr();
		break;
	case GAME_WIN:
		g_offsetX=0;
		g_offsetY=0;
		GameWin();
		Draw(&g_ani[ANI_WIN].item);
		break;
	case GAME_QUIT:
		return;
	//default:
		
	}
	//swap to screen
	if(!BitBlt(g_hDC,0,0,G_CFG.WIN_WIDTH,G_CFG.WIN_HEIGHT,g_hSwapDC,0,0,SRCCOPY)) OnError("绘图错误",ERR_TERMINAL);
}

void GameOver()
{
	static int timer=0;
	g_ani[ANI_WIN].state=ANI_RUN;
	if(timer>=WINDELAY)
	{
		GameShutdown();
		g_gamestate=GAME_MENU;
		g_ani[GAME_MENU].item.curpic=0;
		g_curlevel=1;
		g_curexp=0;
	}
	else
		timer++;

	Draw(&g_ani[ANI_GAMEOVER].item);
}

void GameWin()
{
	static int timer=0;
	g_ani[ANI_WIN].state=ANI_RUN;
		if(timer>=WINDELAY)
	{
		g_ani[ANI_WIN].state=ANI_INVISIBLE;
		g_curmap++;
		GameShutdown();
		g_gamestate=GAME_SHOWGAPSCR;
		timer=0;
	}
	else
		timer++;
}

void GameInit()
{
	
	//初始化地图方块
	ReadMapBlock(g_curmap);
	
	//初始化坦克
	ReadMapTank(g_curmap);
	CalcTank(g_curlevel,&g_tank[0]);
}

void GameShutdown()
{
	ClrGun(g_pGunHead);
	g_pGunHead=NULL;
}

void ClrGun(struct tpGun *gun)
{
	if(gun==0) return;
	if(gun->next==0)
		free(gun);
	else 
	{
		ClrGun(gun->next);
		free(gun);
	}
}

void GameRun()
{
	static int delay=0;
	static unsigned long addhp=0;
	int i,ifwin=1;
	TankEvent();
	GunEvent();
	addhp++;
	if(!(addhp%ADDHPITV)&&g_tank[0].life<g_tank[0].fulllife) 
		g_tank[0].life++;
	if(g_tank[0].life<=0)
	{//游戏结束
		if(delay>=GAMEOVERDELAY)
		{
			g_gamestate=GAME_OVER;
			GameShutdown();
		}
		else delay++;
	}
	else delay=0;
	for(i=1;i<TANKNUM;i++)//检查是否胜利
		if(g_tank[i].life>0)
			ifwin=0;
	if(ifwin) g_gamestate=GAME_WIN;
	GameDraw();
}
//-------------游戏流程控制END---------------

//-------------游戏逻辑控制START-------------

void GameAIMove(int *x,int *y,struct tpTank *tank)
{//敌方移动
	enum E_DIRECT dx;
	int rate;
//	dx=(enum E_DIRECT)(rand()%4);
	if(rand()%2)
		if(g_tank[0].item.x>*x) dx=DX_RIGHT;
		else dx=DX_LEFT;
	else
		if(g_tank[0].item.y>*y) dx=DX_DOWN;
		else dx=DX_UP;

	tank->dx=dx;
	rate=tank->rate;
	switch(dx)
	{
	case DX_LEFT:
		*x-=rate;
		break;
	case DX_RIGHT:
		*x+=rate;
		break;
	case DX_UP:
		*y-=rate;
		break;
	case DX_DOWN:
		*y+=rate;
		break;
	}
	
}

void GameAIGun(struct tpTank *tank)
{//敌方shot
	if(!(rand()%5))
		CreateGun(tank);
}

void TankEvent()
{
	int x,y,w,h;
	int i,j,bumped=0;
	//移动坦克
	for(i=0;i<TANKNUM;i++)
	{
		if(g_tank[i].life<=0) continue;
		x=g_tank[i].item.x;
		y=g_tank[i].item.y;
		w=g_tank[i].item.w;
		h=g_tank[i].item.h;
		
		if(g_tank[i].guninterval>g_tank[i].guncounter)
			g_tank[i].guncounter++;

		//移动
		if(i)
		{//不是己方
			GameAIMove(&x,&y,&g_tank[i]);
			GameAIGun(&g_tank[i]);
		}
		else
		{//是己方
			if(KEY(VK_LEFT)){x-=g_tank[i].currate;g_tank[0].dx=DX_LEFT;}
			else if(KEY(VK_RIGHT)){x+=g_tank[i].currate;g_tank[0].dx=DX_RIGHT;}
			else if(KEY(VK_UP)){y-=g_tank[i].currate;g_tank[0].dx=DX_UP;}
			else if(KEY(VK_DOWN)){y+=g_tank[i].currate;g_tank[0].dx=DX_DOWN;}
			if(KEY(VK_SPACE))
			{//制造子弹 bug here
				CreateGun(&g_tank[0]);
			}
		}
		g_tank[i].item.curpic=g_tank[i].dx;
		//地图碰撞检测
		if(x<0||x+w>=MAPWID||y<0||y+h>=MAPHEI)
			bumped=1;

		if(!bumped)
		{//与方块碰撞检测
			g_tank[i].currate=g_tank[i].rate;
			for(j=0;j<BLKNUM;j++)
				if(BUMP(x,y,w,h,
					g_blk[j].item.x,g_blk[j].item.y,
					g_blk[j].item.w,g_blk[j].item.h)
					&&g_blk[j].life>0)
				{
					//阻碍与加速?
					if(g_blk[j].resist==DEFRESIST)
					{
						bumped=1;
						break;
					}
					else g_tank[i].currate=(int)(g_tank[i].rate*g_blk[j].resist/RESISTNORMAL);
				}
		}
		
		//坦克间碰撞检测
		if(!bumped)
			for(j=0;j<TANKNUM;j++)
				if(g_tank[j].life>0&&
					i!=j&&BUMP(x,y,w,h,
					g_tank[j].item.x,g_tank[j].item.y,
					g_tank[j].item.w,g_tank[j].item.h))
				{
					bumped=1;
					break;
				}

	  if(!bumped) 
		{
			g_tank[i].item.x=x;
			g_tank[i].item.y=y;
		}else bumped=0;
	}
}

void CreateGun(struct tpTank *tank)
{//制造子弹
	struct tpGun *pgun;
	int x,y;

	if(tank->guninterval==tank->guncounter)
	{
		tank->guncounter=0;
	}
	else return;

	x=tank->item.x;
	y=tank->item.y;
	pgun=(struct tpGun*)malloc(sizeof(struct tpGun));
	memcpy(pgun,&g_guntype[tank->gun],sizeof(struct tpGun));

	if(g_pGunHead==NULL)
	{
		g_pGunHead=pgun;
		pgun->next=NULL;
	}
	else 
	{
		g_gun=g_pGunHead->next;
		g_pGunHead->next=pgun;
		pgun->next=g_gun;
	}
	//调整子弹方位与坐标
	switch(tank->dx)
	{
	case DX_LEFT:
		pgun->item.x=x;
		pgun->item.y=y+(int)((tank->item.h-pgun->item.h)/2);
		break;
	case DX_RIGHT:
		pgun->item.x=x+tank->item.w;
		pgun->item.y=y+(int)((tank->item.h-pgun->item.h)/2);
		break;
	case DX_UP:
		pgun->item.x=x+(int)((tank->item.w-pgun->item.w)/2);
		pgun->item.y=y;
		break;
	case DX_DOWN:
		pgun->item.x=x+(int)((tank->item.w-pgun->item.w)/2);
		pgun->item.y=y+tank->item.h;
		break;
	}
	pgun->dx=tank->dx;
	pgun->father=tank;
	if(pgun->father==&g_tank[0]) SOUND(SOUND_GUN);
}

void OnUpgrade()
{//升级
	g_curlevel++;
	g_curexp=0;
	CalcTank(g_curlevel,&g_tank[0]);
	g_ani[ANI_UPGRADE].item.x=g_tank[0].item.x-(int)((g_ani[ANI_UPGRADE].item.w-g_tank[0].item.w)/2);
	g_ani[ANI_UPGRADE].item.y=g_tank[0].item.y-(int)((g_ani[ANI_UPGRADE].item.h-g_tank[0].item.h)/2);
	g_ani[ANI_UPGRADE].state=ANI_SHOWONCE;
	if(g_curlevel>MAXLEVEL)
	{
		g_gamestate=GAME_MENU;
		MessageBox(g_hWnd,"恭喜通关","通关",MB_OK);
	}
}

void CalcTank(int level,struct tpTank* tank)
{//由等级计算坦克数值
	char str[30];
	int x,y,r;
	enum E_DIRECT drt;
	x=tank->item.x;
	y=tank->item.y;
	drt=tank->dx;
	sprintf(str,"Level%d_Tank",level);
	r=ReadInt(str);
	memcpy(tank,&g_tanktype[r],sizeof(struct tpTank));
	tank->item.x=x;
	tank->item.y=y;
	tank->fulllife=tank->life;
	tank->dx=drt;
}

void GunEvent()
{
	int i,j,bumped,ifsndply;
	struct tpGun *ptemp;
	//移动子弹
	ptemp=g_pGunHead;
	g_gun=g_pGunHead;
	ifsndply=0;
	while(g_gun!=NULL)
	{
		bumped=0;
		switch(g_gun->dx)
		{
		case DX_LEFT:
			g_gun->item.x-=g_gun->rate;
			break;
		case DX_RIGHT:
			g_gun->item.x+=g_gun->rate;
			break;
		case DX_UP:
			g_gun->item.y-=g_gun->rate;
			break;
		case DX_DOWN:
			g_gun->item.y+=g_gun->rate;
			break;
		}

		//子弹碰撞边界
		if(!BUMP(MAPX,MAPY,MAPWID,MAPHEI,
			g_gun->item.x,g_gun->item.y,g_gun->item.w,g_gun->item.h))
			bumped=1;
		//子弹碰撞检测:坦克
		if(!bumped)
			for(j=0;j<TANKNUM;j++)
				if(g_tank[j].life>0&&
					g_gun->father!=&g_tank[j]&&
					BUMP(g_gun->item.x,g_gun->item.y,
					g_gun->item.w,g_gun->item.h,
					g_tank[j].item.x,g_tank[j].item.y,
					g_tank[j].item.w,g_tank[j].item.h))
				{
					g_tank[j].life-=g_gun->power;
					SOUND(SOUND_BUMP);
					if(g_tank[j].life<=0)
					{
						//播放爆炸动画
						g_ani[ANI_EXPLO1].item.x=g_tank[j].item.x;
						g_ani[ANI_EXPLO1].item.y=g_tank[j].item.y;
						g_ani[ANI_EXPLO1].item.curpic=0;
						g_ani[ANI_EXPLO1].state=ANI_SHOWONCE;
						//增加经验
 						if(g_gun->father==&g_tank[0])
							g_curexp+=(g_tank[j].life/10+EXPADD);
						if(g_curexp>=FULLEXP)
							OnUpgrade();
						//SOUND(SOUND_EXPLO);
						ifsndply=1;
					}
					bumped=1;
					break;
				}
			
			
		//子弹碰撞检测:地图
		if(!bumped)
			for(i=0;i<BLKNUM;i++)
				if(BUMP(g_gun->item.x,g_gun->item.y,
					g_gun->item.w,g_gun->item.h,
					g_blk[i].item.x,g_blk[i].item.y,
					g_blk[i].item.w,g_blk[i].item.h)
					&&g_blk[i].life>0)
				{
					if(g_blk[i].resist==DEFRESIST)
					{
						bumped=1;
						g_blk[i].life-=g_gun->power;
						if(g_blk[i].life<=0)
						{
							//增加经验
 							if(g_gun->father==&g_tank[0])
								g_curexp+=(int)((g_blk[i].life/10+EXPADD)*BLOCKEXPRATE/100);
							if(g_curexp>=FULLEXP)
								OnUpgrade();
						}
						break;
					}
				}



		if(bumped)
		{//子弹与其它相撞
			StartAni(ANI_EXPLO,g_gun->item.x-(int)(g_ani[ANI_EXPLO].item.w/2),
						g_gun->item.y-(int)(g_ani[ANI_EXPLO].item.h/2),ANI_RUNTOFREE);
			if(!ifsndply&&g_gun->father==&g_tank[0])
				SOUND(SOUND_BUMP);
			//删除子弹
			if(g_pGunHead->next==NULL)
			{
				free(g_pGunHead);
				g_pGunHead=NULL;
				g_gun=NULL;
			}
			else
			{
				if(g_gun==g_pGunHead)
				{
					g_pGunHead=g_pGunHead->next;
					free(g_gun);
					g_gun=g_pGunHead;
				}
				else
				{
					ptemp->next=g_gun->next;
					free(g_gun);
					g_gun=ptemp->next;
				}
			}
		}	
		else
		{
			ptemp=g_gun;
			g_gun=g_gun->next;
		}		
	}
}
//-------------游戏逻辑控制END-------------

//-------------绘图及界面START----------------
void GameDraw()
{
	int i;
	int offx,offy;
	//视窗控制
	offx=(int)(G_CFG.WIN_WIDTH/2)-g_tank[0].item.x;
	offy=(int)(G_CFG.WIN_HEIGHT/2)-g_tank[0].item.y;
	if(offx<=0&&offx>=G_CFG.WIN_WIDTH-MAPWID) 
		g_offsetX=offx;
	if(offy<=0&&offy>=G_CFG.WIN_HEIGHT-MAPHEI)
		g_offsetY=offy;
	//画背景
	Draw(&g_map[g_curmap].item);
	//画地图
	for(i=0;i<BLKNUM;i++)
			if(g_blk[i].life>0)
				Draw(&g_blk[i].item);
	//画坦克
	for(i=0;i<TANKNUM;i++)
		if(g_tank[i].life>0)
			Draw(&g_tank[i].item);
	//画子弹
	for(g_gun=g_pGunHead;g_gun!=NULL;g_gun=g_gun->next)
	{
		Draw(&g_gun->item);
	}
	//画动画
	for(i=0;i<MAXANINUM;i++)
		switch(g_ani[i].state)
		{
		case ANI_RUN:
			Draw(&g_ani[i].item);			
			break;
		case ANI_SHOWONCE:
			if(g_ani[i].item.curpic==g_ani[i].item.picnum-1)
				g_ani[i].state=ANI_INVISIBLE;
			else Draw(&g_ani[i].item);	
			break;
		case ANI_RUNTOFREE:
			if(g_ani[i].item.curpic==g_ani[i].item.picnum-1)
				g_ani[i].state=ANI_FREE;
			else Draw(&g_ani[i].item);	
			break;
			
		}
	DrawInfo();
}

void DrawInfo()
{//显示HP、LEVEL、EXP
	static char str[30];
	if(g_tank[0].life<=g_tank[0].fulllife)SetTextColor(g_hSwapDC,INFO_TEXTCOLOR((double)g_tank[0].life/(double)g_tank[0].fulllife));
	else SetTextColor(g_hSwapDC,INFO_TEXTCOLOR(1));
	sprintf(str,"Map:%7d",g_curmap+1);
	TextOut(g_hSwapDC,INFO_MAP_X,INFO_MAP_Y,str,strlen(str));
	sprintf(str,"HP:%7d",g_tank[0].life>=0?g_tank[0].life:0);
	TextOut(g_hSwapDC,INFO_HP_X,INFO_HP_Y,str,strlen(str));
	sprintf(str,"EXP:%5d/%d",g_curexp,FULLEXP);
	TextOut(g_hSwapDC,INFO_EXP_X,INFO_EXP_Y,str,strlen(str));
	sprintf(str,"LEVEL:%7d",g_curlevel);
	TextOut(g_hSwapDC,INFO_LEVEL_X,INFO_LEVEL_Y,str,strlen(str));
}

int StartAni(int index,int x,int y,enum E_ANISTATE state)
{//为动画寻找空间并播放
	int i;
	if(g_ani[index].state==ANI_FREE)
	{
		g_ani[index].state=state;
		return index;
	}
	for(i=0;i<MAXANINUM;i++)
	{
		if(index==1) continue;
		if(g_ani[i].state==ANI_FREE)
		{
			memcpy(&g_ani[i],&g_ani[index],sizeof(struct tpANI));
			g_ani[i].item.x=x;
			g_ani[i].item.y=y;
			g_ani[i].state=state;
			return i;
		}
	}
	return ERRORRET;
}

void Draw(struct tpScrItem *scritem)
{//绘图
	int scrX,scrY,orgW,orgH;
	if(scritem->picstep>0)
	{
		scritem->timecouter++;
		if(scritem->timecouter>=scritem->picstep)
		{
			scritem->curpic++;
			scritem->timecouter=0;
		}
		if(scritem->curpic>=scritem->picnum)
			scritem->curpic=0;
	}
	
	scrX=scritem->x+g_offsetX;
	scrY=scritem->y+g_offsetY;
	orgW=g_picitem[scritem->picid[scritem->curpic]].w;
	orgH=g_picitem[scritem->picid[scritem->curpic]].h;
	
	if(BUMP(scrX,scrY,scritem->w,scritem->h,
		0,0,G_CFG.WIN_WIDTH,G_CFG.WIN_HEIGHT))
	TransparentBlt(g_hSwapDC,scrX,scrY,
		scritem->w,scritem->h,
		g_hBmpDC[g_picitem[scritem->picid[scritem->curpic]].bmp],
		g_picitem[scritem->picid[scritem->curpic]].x,g_picitem[scritem->picid[scritem->curpic]].y,
		orgW,orgH,G_CFG.BMP_TRANSP);
	
}

void ShowGapscr()
{//显示菜单与游戏之间的画面
	static int timer=0;
	unsigned long color;
	char str[30];
	g_ani[ANI_BLACKSCR].state=ANI_RUN;
	if(timer>=GAPDELAY)
	{
		g_ani[ANI_BLACKSCR].state=ANI_INVISIBLE;
		GameInit();
		g_gamestate=GAME_RUN;
		timer=0;
	}
	else
		timer++;
	color=GetTextColor(g_hSwapDC);
	SetTextColor(g_hSwapDC,GAP_TEXTCOLOR);

	Draw(&g_ani[ANI_BLACKSCR].item);	
	//输出文字信息
	sprintf(str,"MAP:%7d",g_curmap+1);
	TextOut(g_hSwapDC,GAP_MAP_X,GAP_MAP_Y,str,strlen(str));
	sprintf(str,"HP:%7d",g_tank[0].life);
	TextOut(g_hSwapDC,GAP_HP_X,GAP_HP_Y,str,strlen(str));
	sprintf(str,"EXP:%5d/%d",g_curexp,FULLEXP);
	TextOut(g_hSwapDC,GAP_EXP_X,GAP_EXP_Y,str,strlen(str));
	sprintf(str,"LEVEL:%7d",g_curlevel);
	TextOut(g_hSwapDC,GAP_LEVEL_X,GAP_LEVEL_Y,str,strlen(str));
	sprintf(str,"Power:%7d",g_guntype[g_tank[0].gun].power);
	TextOut(g_hSwapDC,GAP_POWER_X,GAP_POWER_Y,str,strlen(str));
	sprintf(str,"Rate:%7d",g_tank[0].rate);
	TextOut(g_hSwapDC,GAP_RATE_X,GAP_RATE_Y,str,strlen(str));

	SetTextColor(g_hSwapDC,color);
}

void ShowHelp()
{
	char str[100]={0};
	if(KEY(VK_RETURN)||KEY(VK_ESCAPE))
	{
		g_gamestate=GAME_MENU;
	}
	Draw(&g_ani[ANI_BLACKSCR].item);
	ReadStr("HelpStr0",str);
	TextOut(g_hSwapDC,100,100,str,strlen(str));
	ReadStr("HelpStr1",str);
	TextOut(g_hSwapDC,100,130,str,strlen(str));
	ReadStr("HelpStr2",str);
	TextOut(g_hSwapDC,100,160,str,strlen(str));
	ReadStr("HelpStr3",str);
	TextOut(g_hSwapDC,100,190,str,strlen(str));
	ReadStr("HelpStr4",str);
	TextOut(g_hSwapDC,100,220,str,strlen(str));
	if(KEY(VK_LSHIFT)) ProduceRandMap();

}

void ShowMenu()
{
	static int pressed=0;
	if((KEY(VK_LEFT)||KEY(VK_UP))&&!pressed)
	{
		LOOPSUB(g_ani[ANI_MENU].item.curpic,g_ani[ANI_MENU].item.picnum);
		pressed=1;
	}
	else if((KEY(VK_RIGHT)||KEY(VK_DOWN))&&!pressed)
	{
		LOOPINC(g_ani[ANI_MENU].item.curpic,g_ani[ANI_MENU].item.picnum);
		pressed=1;
	} 
	if(!(KEY(VK_LEFT)||KEY(VK_UP)||KEY(VK_RIGHT)||KEY(VK_DOWN)))
		pressed=0;
	else SOUND(SOUND_MENU);
	KEY(VK_RETURN);
	if(KEY(VK_RETURN))
	{
		switch(g_ani[ANI_MENU].item.curpic)
		{
		case 0:
			g_curlevel=1;
			g_curexp=0;
			g_curmap=0;
			GameInit();
			g_ani[ANI_MENU].state=ANI_INVISIBLE;
			g_gamestate=GAME_SHOWGAPSCR;
			break;
		case 1:
			g_gamestate=GAME_LOADPROGRESS;
			break;
		case 2:
			g_gamestate=GAME_HELP;
			break;
		case 3:
			g_gamestate=GAME_QUIT;
			break;
		}
	}
	else g_ani[ANI_MENU].state=ANI_RUN;
	Draw(&g_ani[ANI_LOGO].item);
	Draw(&g_ani[ANI_MENU].item);
}

void ShowRetMenu()
{
	static int pressed=0;
	if((KEY(VK_LEFT)||KEY(VK_UP))&&!pressed)
	{
		LOOPSUB(g_ani[ANI_RETMENU].item.curpic,g_ani[ANI_RETMENU].item.picnum);
		pressed=1;
	}
	else if((KEY(VK_RIGHT)||KEY(VK_DOWN))&&!pressed)
	{
		LOOPINC(g_ani[ANI_RETMENU].item.curpic,g_ani[ANI_RETMENU].item.picnum);
		pressed=1;
	} 
	if(!(KEY(VK_LEFT)||KEY(VK_UP)||KEY(VK_RIGHT)||KEY(VK_DOWN)))pressed=0;

	if(KEY(VK_RETURN))
	{
		switch(g_ani[ANI_RETMENU].item.curpic)
		{
		case 0:
			g_gamestate=GAME_RUN;
			g_ani[ANI_RETMENU].state=ANI_INVISIBLE;
			break;
		case 1:
			SaveProgress();
			g_ani[ANI_SAVE].state=ANI_SHOWONCE;
			break;
		case 2:
			GameShutdown();
			g_curlevel=1;
			g_curexp=0;
			g_curmap=0;
			GameInit();
			g_ani[ANI_RETMENU].state=ANI_INVISIBLE;
			g_gamestate=GAME_RUN;
			break;
		case 3:
			g_gamestate=GAME_LOADPROGRESS;
			break;
		case 4:
			g_gamestate=GAME_HELP;
			break;
		case 5:
			g_gamestate=GAME_QUIT;
			break;
		}
	}
	else g_ani[ANI_RETMENU].state=ANI_RUN;
	Draw(&g_ani[ANI_LOGO].item);
	Draw(&g_ani[ANI_RETMENU].item);
	if(g_ani[ANI_SAVE].state!=ANI_INVISIBLE)Draw(&g_ani[ANI_SAVE].item);
}
//-------------绘图及界面END----------------

//-------------错误处理及内存释放START-------------
void Destroy()
{
	
	DeleteObject(g_hSwapBmp);
	DeleteDC(g_hDC);
	free(g_hBmp);
	free(g_hBmpDC);
	free(g_cfgbuf);
}
void OnError(char info[],enum E_EORROR err)
{
	switch(err)
	{
	case ERR_REMIND:
		MessageBox(g_hWnd,info,"错误",MB_OK);
		break;
	case ERR_TERMINAL:
		MessageBox(g_hWnd,info,"严重错误",MB_OK);
		exit(0);
		break;
	}
}

//-------------错误处理及内存释放END-------------
