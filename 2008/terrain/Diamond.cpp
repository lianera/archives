// Diamond.cpp: implementation of the CDiamond class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "¶íÂÞË¹·½¿é.h"
#include "Diamond.h"
#include "Panel.h"
#include "Block.h"
#include "Manager.h"
#include <mmsystem.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMyApp theApp;
extern CPanel* pPanel;
extern CBlock* pBlock;
extern CManager* pManager;
CDiamond::CDiamond()
{
	DimDiamond();
}

CDiamond::~CDiamond()
{

}

bool CDiamond::FullLine()
{
	bool IsFull,Full=false;
	pManager->SeriesLine=0;
	for(int iy=0;iy<=pPanel->nVGridNum;iy++)
	{
		IsFull=true;
		for(int ix=0;ix<=pPanel->nHGridNum;ix++)
		{
			if(!pBlock->BlockPanel[ix][iy]) IsFull=false;
		}
		if(IsFull)
		{	
			Full=true;
			pManager->SeriesLine++;
			for(int jy=iy;jy>0;jy--)
			{	Sleep(10);
				for(int jx=0;jx<=pPanel->nHGridNum;jx++)
				{
					pBlock->BlockPanel[jx][jy]=pBlock->BlockPanel[jx][jy-1];
				}
			}
			if(pManager->Sound) PlaySound("sound\\fullline.wav",NULL,SND_FILENAME | SND_ASYNC);	
		}
	}
	pManager->LineNumber+=pManager->SeriesLine;
	pManager->Result+=pManager->SeriesLine*pManager->SeriesLine;
	if(Full)
		return true;
	else return false;
}

bool CDiamond::MoveDiamondLeft()
{
	DiamondPos.x--;
	if(overlap())
	{
		DiamondPos.x++;
		return false;
	}
	if(pManager->Sound) PlaySound("sound\\move.wav",NULL,SND_FILENAME | SND_ASYNC);
	return true;
}
bool CDiamond::MoveDiamondRight()
{
	DiamondPos.x++;
	if(overlap())
	{
		DiamondPos.x--;
		return false;
	}
	if(pManager->Sound) PlaySound("sound\\move.wav",NULL,SND_FILENAME | SND_ASYNC);
	return true;
}
bool CDiamond::MoveDiamondDown()
{
	DiamondPos.y++;
	if(overlap())
	{
		DiamondPos.y--;
	if(pManager->Sound) PlaySound("sound\\fall.wav",NULL,SND_FILENAME | SND_ASYNC);
		return false;
	}
	return true;
}

bool CDiamond::RollDiamond()
{
	if(DiamondState<3) DiamondState++;
	else DiamondState=0;
	if(overlap())
	{
		if(DiamondState>0) DiamondState--;
		else DiamondState =3;
		return false;
	}
	if(pManager->Sound) PlaySound("sound\\roll.wav",NULL,SND_FILENAME | SND_ASYNC);
	return true;
}

void CDiamond::DrawDiamond(CDC* pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(DiamondColor);
	CBrush *pOldBrush=pDC->SelectObject(&brush);
	
	for(int iy=0;iy<4;iy++)
	{
		for(int ix=0;ix<4;ix++)
			if(DiamondStruct[DiamondType][DiamondState][ix][iy]) 
			{
				POINT aimPos=DiamondPos;
				aimPos.x+=ix;
				aimPos.y+=iy;
				pPanel->PanelPosToPos(aimPos);
				pDC->Rectangle(aimPos.x,aimPos.y,aimPos.x+pPanel->GridSize.cx,aimPos.y+pPanel->GridSize.cy);
			}
	}
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

bool CDiamond::overlap()
{
	bool bTuFa=false;
	POINT TexPos;
	for(int iy=3;iy>=0;iy--)
	{
		for(int ix=0;ix<4;ix++)
		{
			if(DiamondStruct[DiamondType][DiamondState][ix][iy])
			{	//ÅÐ¶Ï±ß¿ò
				TexPos.x=ix+DiamondPos.x;
				TexPos.y=iy+DiamondPos.y;
				pPanel->PanelPosToPos(TexPos);
				//TexPos.x+=pPanel->GridSize.cy;
				TexPos.y+=pPanel->GridSize.cy;
				if(TexPos.x<pPanel->PanelRect.left ||	//ÊÇ·ñÔ½½ç
				   TexPos.x>pPanel->PanelRect.right||
				   TexPos.y>pPanel->PanelRect.bottom)
				   bTuFa=true;
				//ÅÐ¶ÏÕÏ°­Îï
				if(pBlock->BlockPanel[DiamondPos.x+ix][DiamondPos.y+iy]) bTuFa=true;
			}
		}
	}
	if(bTuFa) return true;
	return 0;
}

void CDiamond::DimDiamond()
{
	bool DiaStruct[7][4][4][4]={
		{					
			{	{1,1,0,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{1,1,0,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{1,1,0,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{1,1,0,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							}	
									},//·½¿é
		{	
			{	{1,1,0,0},
				{1,0,0,0},
				{1,0,0,0},
				{0,0,0,0}
							},
			{	{1,1,1,0},
				{0,0,1,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{0,1,0,0},
				{0,1,0,0},
				{1,1,0,0},
				{0,0,0,0}
							},
			{	{1,0,0,0},
				{1,1,1,0},
				{0,0,0,0},
				{0,0,0,0}
							}
								},	//¹³×Ó	
		{	
			{	{1,1,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,0,0,0}
							},
			{	{0,0,1,0},
				{1,1,1,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{1,0,0,0},
				{1,0,0,0},
				{1,1,0,0},
				{0,0,0,0}
							},
			{	{1,1,1,0},
				{1,0,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
								},//Æß×ÖÐÎ
		{
			{	{0,1,0,0},
				{1,1,1,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{1,0,0,0},
				{1,1,0,0},
				{1,0,0,0},
				{0,0,0,0}
							},
			{	{1,1,1,0},
				{0,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{0,1,0,0},
				{1,1,0,0},
				{0,1,0,0},
				{0,0,0,0}
							},	
								},//Í¹
		{	
			{	{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0}
							},
			{	{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0},
				{0,0,0,0}
							},
			{	{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0}
							},
			{	{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0},
				{0,0,0,0}
							}
								},//¸Ü
		{	
			{
				{1,1,0,0},
				{0,1,1,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{
				{0,1,0,0},
				{1,1,0,0},
				{1,0,0,0},
				{0,0,0,0}		
							},
			{	
				{1,1,0,0},
				{0,1,1,0},
				{0,0,0,0},
				{0,0,0,0}		
							},
			{	
				{0,1,0,0},
				{1,1,0,0},
				{1,0,0,0},
				{0,0,0,0}
							}
								},//¶þ
		{	
			{
				{0,1,1,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{
				{1,0,0,0},
				{1,1,0,0},
				{0,1,0,0},
				{0,0,0,0}
							},
			{
				{0,1,1,0},
				{1,1,0,0},
				{0,0,0,0},
				{0,0,0,0}
							},
			{
				{1,0,0,0},
				{1,1,0,0},
				{0,1,0,0},
				{0,0,0,0}
							}
								}//²ù×Ó
	};
	for(int i1=0;i1<7;i1++)
		for(int i2=0;i2<4;i2++)
			for(int i3=0;i3<4;i3++)
				for(int i4=0;i4<4;i4++)
				{
					DiamondStruct[i1][i2][i3][i4]=DiaStruct[i1][i2][i3][i4];
				}

}