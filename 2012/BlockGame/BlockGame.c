#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <windows.h>

#define BLKCHAR "■"
#define NUCHAR "□"
#define P_X 8
#define P_Y 5
#define M_X 10
#define M_W 58
#define M_Y 13
#define M_H 11
#define NEXTBLOCK_X 29
#define NEXTBLOCK_Y 10
#define PL_W 12
#define PL_H 20
#define N_S 4
#define N_T 7
#define B_W 4
#define B_H 4
#define INFO_NAME_X 32
#define INFO_NAME_Y 20
#define INFO_LEVEL_X 32
#define INFO_LEVEL_Y 23
#define INFO_MARK_X 32
#define INFO_MARK_Y 26
#define TIME_WAIT 10
#define TIME_FALL 50/curuser.level
#define WIN_CWSIZE "mode con cols=80 lines=30"
#define FILE_WELCOME "welcome"
#define FILE_OUTLINE "outline"
#define FILE_GAMEOVER "gameover"
#define FILE_BLOCKS "blocks"
#define FILE_PROGRESS "progress"
#define FILE_HELP "help"
#define DEF_NAME "NOMANE"


struct tpblk
{
	int x, y;
	int type;
	int state;
};

struct tpuser
{
	long level;
	long mark;
	char name[30];
}curuser;

int panel[PL_H][PL_W]={0};
int blocks[N_T][N_S][B_H][B_W];


void gotoxy(int x,int y)
{
	COORD c;
	c.X = x*2, c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}  
struct tpblk initblock()
{
	struct tpblk rdblk;
	rdblk.x=PL_W/2;
	rdblk.y=-B_H+2;
	rdblk.type=rand()%N_T;
	rdblk.state=rand()%N_S;
	return rdblk;
}

void prtfiletoscr(char *file)
{
	FILE *fp;
	char str[100];
	system("cls");
	gotoxy(0,0);
	if((fp=fopen(file,"r"))==NULL)
	{
		printf("打开文件：\"%s\"失败！\n",file);
		getch();
		exit(0);
	}
	while(fgets(str,100,fp)!=NULL)
		printf("%s",str);
	fclose(fp);
}
//下一个方块
void prtnxtblk(struct tpblk nxtblock)
{
	int i,j;
	for(i=0;i<B_H;i++)
		for(j=0;j<B_W;j++)
		{					
			gotoxy(NEXTBLOCK_X+j,NEXTBLOCK_Y+i);
			printf("%s",blocks[nxtblock.type][nxtblock.state][i][j]?BLKCHAR:NUCHAR);
		}

}
//打印面板
void prtpanel()
{
	int i,j;
	for(i=0;i<PL_H;i++)
	{
		gotoxy(P_X,P_Y+i);
		for(j=0;j<PL_W;j++)
		{
			printf("%s",panel[i][j]?BLKCHAR:NUCHAR);
		}
		//putchar(0);
	}

}
//返回 是否有效
int onkeyboard(int c,struct tpblk *block)
{	
	int i,j;
	switch(c)
	{
	case 75:
		block->x--;break;
	case 77:
		block->x++;break;
	case 72:
	case ' ':
	case 13:
		if(block->state>=N_S-1)block->state=0;
		else block->state++;break;
	case -1://自动下落
	case 80:
		block->y++;break;
	case 'p'://暂停
		gotoxy(5,3);
		printf("暂停中...按任意键恢复。");
		getch();
		gotoxy(5,3);
		printf("                       ");
		return 2;
		break;
	case 27://esc
		return -1;
		break;
	default:
		return 2;
	}
	for(i=0;i<B_H;i++)
		for(j=0;j<B_W;j++)
			if(blocks[block->type][block->state][i][j])//保证有点	
			{
				if(block->x+j<0||block->x+j>=PL_W)return 1;//左右边界
				if(block->y+i>=PL_H)return 1;//是否到底
				if(block->y+i>=0&&panel[block->y+i][block->x+j])return 1;//与其他块碰撞
			}
	return 0;
}

void moveblock(struct tpblk oldblock,struct tpblk newblock)
{
	int i,j;
	//清除原有的方块
	for(i=0;i<B_H;i++)
		for(j=0;j<B_W;j++)
		{
			if(blocks[oldblock.type][oldblock.state][i][j]&&oldblock.y+i>=0)
			{
				gotoxy(P_X+oldblock.x+j,P_Y+oldblock.y+i);
				printf("%s",NUCHAR);		
			}
		}
		
		//画新方块
		for(i=0;i<B_H;i++)
			for(j=0;j<B_W;j++)
			{					
				if(blocks[newblock.type][newblock.state][i][j]&&newblock.y+i>=0)
				{//面板中
						gotoxy(P_X+newblock.x+j,P_Y+newblock.y+i);
						printf("%s",BLKCHAR);
				}
			}
}

void loadblocks()
{
	FILE *fp;
	int i,j,cx,cy,ch;
	//blocks初始化
	if((fp=fopen(FILE_BLOCKS,"rt"))==NULL)
	{
		gotoxy(0,0);
		printf("打开文件：%s失败\n",FILE_BLOCKS);
		getch();
		exit(0);
	}
	for(i=0;i<N_T;i++)
		for(j=0;j<N_S;j++)
			for(cy=0;cy<B_H;cy++)
				for(cx=0;cx<B_W;cx++)
				{
					while((ch=fgetc(fp)-48)!=0&&ch!=1)
					{	if(ch==EOF)
						{
							gotoxy(0,0);
							printf("读取文件：%s错误!\n",FILE_BLOCKS);
						 	getch();exit(0);
						}
					}
						blocks[i][j][cy][cx]=ch;
				}
	fclose(fp);	
}


void saveprogress(struct tpuser user)
{
	FILE* fp;
	struct tpuser tmpuser[10];
	int iffind=0,i;
	if(!strcmp(user.name,DEF_NAME)) return;
	if((fp=fopen(FILE_PROGRESS,"rb+"))==NULL)
	{
		printf("打开文件：\"%s\"失败！\n",FILE_PROGRESS);
		getch();
		exit(0);
	}	
	for(i=0;iffind==0&&fread(&tmpuser[i],sizeof(struct tpuser),1,fp);i++)
	{
		if(!strcmp(user.name,tmpuser[i].name))
			iffind=1;
	}

	if(iffind)
		fseek(fp,-(long)sizeof(struct tpuser),1);
	else
		fseek(fp,0,2);
	fwrite(&user,sizeof(struct tpuser),1,fp);
	if(ferror(fp))
	{
		printf("存档错误！忽略按Enter\n",FILE_PROGRESS);
		while(getch()!=13);
	}
	fclose(fp);

}

int onbumpbtm(struct tpblk block)
{
	int i,j,lf,nlf=0;
	//合并
	for(i=0;i<B_H;i++)
		for(j=0;j<B_W;j++)
		{
			if(blocks[block.type][block.state][i][j])
			{
				//游戏结束？？
				if(block.y+i<=0)return -1;
				//合并
				panel[block.y+i][block.x+j]=1;
				gotoxy(P_X+block.x+j,P_Y+block.y+i);
				printf("%s",BLKCHAR);
			}
		}
		//是否满行？
		for(i=0;i<B_H;i++)
		{
			lf=1;
			for(j=0;j<PL_W;j++)
				if(!panel[block.y+i][j])
				{lf=0;break;}
				
				if(lf)//满行
				{
					for(i=block.y+i;i>0;i--)
						for(j=0;j<PL_W;j++)
							panel[i][j]=panel[i-1][j];
						nlf++;
				}
				
		}
		curuser.mark+=nlf*nlf;
		saveprogress(curuser);
		return lf;
}


void prtinfo()
{
	gotoxy(INFO_NAME_X,INFO_NAME_Y);
	printf("%s",curuser.name);
	gotoxy(INFO_LEVEL_X,INFO_LEVEL_Y);
	printf("%d",curuser.level);
	gotoxy(INFO_MARK_X,INFO_MARK_Y);
	printf("%d",curuser.mark);

}
void clspanel()
{
	int i,j;
	for(i=0;i<PL_H;i++)
		for(j=0;j<PL_W;j++)
			panel[i][j]=0;

}

struct tpuser loadprogress()
{
	struct tpuser user[10]={0};
	FILE* fp;
	int i,j,c,sel=0,alex=0;
	if((fp=fopen(FILE_PROGRESS,"rb"))==NULL)
	{
		printf("打开文件：\"%s\"失败！\n",FILE_PROGRESS);
		getch();
		exit(0);
	}

	for(i=M_Y;i<=M_Y+M_H;i++)
	{
		gotoxy(M_X/2,i);
		for(j=M_X/2;j<=M_X/2+M_W;j++)
		{
			putchar('　');
		}
	}
	for(i=0;fread(&user[i],sizeof(struct tpuser),1,fp);i++)
	{
		gotoxy(M_X+5,M_Y+2+i);
		printf("NAME:%s LEVEL:%ld MARK:%ld",user[i].name,user[i].level,user[i].mark);
	}
	gotoxy(M_X+5,M_Y+2+i);
	printf("[NEW USER]");

	fclose(fp);
	gotoxy(M_X+3,M_Y+2+sel); printf("◆");
	while(1)
	{
		c=getch();
		switch(c)
		{
		case 72://up
			gotoxy(M_X+3,M_Y+2+sel); printf("　");
			if(sel<=0)sel=i;
			else sel--;
			gotoxy(M_X+3,M_Y+2+sel); printf("◆");

			break;
		case 80://down
			gotoxy(M_X+3,M_Y+2+sel); printf("　");
			if(sel>=i)sel=0;
			else sel++;
			gotoxy(M_X+3,M_Y+2+sel); printf("◆");
			break;
		case 27://esc
			user[0].level=-1;
			return user[0];
			break;
		case 13://enter
			if(sel==i)//New User
			{
				gotoxy(M_X+5,M_Y+2+i+1);
				printf("Input Name:");
				scanf("%s",user[i].name);
				for(j=0;j<i&&!alex;j++)
				{
					if(!strcmp(user[j].name,user[i].name))
					{
						gotoxy(M_X+5,M_Y+2+i+1);
						printf("\"%s\"already exist!",user[i].name);
						getch();
						alex=1;
					}
				}
				if(!alex)
				{
					user[i].level=1L;
					user[i].mark=0;
					saveprogress(user[i]);
				}
			}
			if(user[sel].name<=0||user[sel].level<=0||user[sel].mark<0) user[sel].level=-1;
			return user[sel];
			break;
		default:;
		}
	}
	user[0].level=-1;
	return user[0];
}

int gameloop()
{
	int iskh=0,c,val,timec=0,bumpst=0,timewait=TIME_WAIT;
	struct tpblk curblock,nxtblock,tmpblock;
	
	curblock=initblock();
	nxtblock=initblock();
	
	prtfiletoscr(FILE_OUTLINE);
	prtpanel();
	prtinfo();
	prtnxtblk(nxtblock);
	while(1)
	{
		
		if(kbhit()||iskh)
		{
			tmpblock=curblock;
			if(!iskh)c=getch();
				val=onkeyboard(c,&tmpblock);
				if(val==2) continue;
				else if(val==-1) return 2;
				else if(val==0)
				{
					moveblock(curblock,tmpblock);
					curblock=tmpblock;
				}
				else if(val==1 && (c==-1||c==80)) //向下撞到
				{
					bumpst=onbumpbtm(curblock);
					if(bumpst==-1) return 1;
					else
					{
						//是否过关
						if(curuser.mark>=100*curuser.level)
						{
							clspanel();
							curuser.mark=0;
							curuser.level++;
						}

						prtinfo();
						prtpanel();
					}
					
					//产生新的BLOCK
					curblock=nxtblock;
					nxtblock=initblock();
					//画下一个方块
					prtnxtblk(nxtblock);
					
				}
				if(c==80)timewait=TIME_WAIT/2;
			

			gotoxy(0,0);
			iskh=0;
		}
		Sleep(timewait);
		timewait=TIME_WAIT;
		if(timec>=TIME_FALL)
		{
			c=-1;
			iskh=1;
			timec=0;
		}
		else timec++;
	}
	return 0;
}


int showmenu()
{
	int i,j,c,cursel=0;
	gotoxy(M_X,M_Y);
	for(i=M_Y;i<=M_Y+M_H;i++)
	{
		gotoxy(M_X/2,i);
		for(j=M_X/2;j<=M_X/2+M_W;j++)
		{
		//	putchar('\b');
		//	gotoxy(j,i);
			putchar('　');
		}
	}
	gotoxy(M_X+5,M_Y+2);
	printf("快 速 游 戏");
	gotoxy(M_X+5,M_Y+4);
	printf("读 取 存 档");
	gotoxy(M_X+5,M_Y+6);
	printf("帮 助");
	gotoxy(M_X+5,M_Y+8);
	printf("退 出");
	gotoxy(M_X+3,M_Y+2+cursel*2); printf("◆");

	while(1)
	{
		c=getch();
		switch(c)
		{
		case 72://up
			gotoxy(M_X+3,M_Y+2+cursel*2); printf("　");
			if(cursel<=0)cursel=3;
			else cursel--;
			gotoxy(M_X+3,M_Y+2+cursel*2); printf("◆");

			break;
		case 80://down
			gotoxy(M_X+3,M_Y+2+cursel*2); printf("　");
			if(cursel>=3)cursel=0;
			else cursel++;
			gotoxy(M_X+3,M_Y+2+cursel*2); printf("◆");
			break;
		case 27://esc
			return 3;
			break;
		case 13://enter
			return cursel;
			break;
		default:;
		}
	}

	return 0;
}

int main()
{
	int looprst=0,ch;
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	struct tpuser tmpuser;
	system("color F8");
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	system(WIN_CWSIZE);

	loadblocks();

	while(1)
	{
		prtfiletoscr(FILE_WELCOME);	
		if(getch()==27) break;
		switch(showmenu())
		{
		case 0://fast mode
			strcpy(curuser.name,DEF_NAME);
			curuser.level=1;
			curuser.mark=0;
			clspanel();
			looprst=gameloop();
			break;
		case 1://read
			tmpuser=loadprogress();
			if(tmpuser.level!=-1)
			{
				curuser=tmpuser;
				clspanel();
				looprst=gameloop();	
			}
			break;
		case 2://help
			prtfiletoscr(FILE_HELP);
			getch();
			break;
		case 3://esc
			break;
		}
		if(looprst==1)
		{
			prtfiletoscr(FILE_GAMEOVER);
			gotoxy(10,3);
			printf("[%s] [LEVEL %ld] [MARK %ld]",curuser.name,curuser.level,curuser.mark);
			while((ch=getch())!=27&&ch!=13);
			looprst=0;
		}
	}
	return 0;
}
