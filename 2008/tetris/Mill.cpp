// Mill.cpp: implementation of the CMill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "俄罗斯方块.h"
#include "Mill.h"
#include "Diamond.h"
#include "Manager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMyApp theApp;
extern CDiamond* pDiamond;

CMill::CMill()
{
	//生成
	PreDiamondType=rand()%7;
	PreDiamondState=rand()%4;
}

CMill::~CMill()
{

}

void CMill::MadeDiamond()
{
	pDiamond->DiamondType=PreDiamondType;
	pDiamond->DiamondState=PreDiamondState;
	pDiamond->DiamondPos.x=5;
	pDiamond->DiamondPos.y=0;
	//生成
	int nMax;
	int Cake=rand()*probabilityCake/100;
					PreDiamondType=DIAMONDCAKE;
					nMax=Cake;
	int Hook=rand()*probabilityHook/100;
	if(Hook>nMax)
	{
		nMax=Hook;
		PreDiamondType=DIAMONDHOOK;
	}
	int Seven=rand()*probabilitySeven/100;
	if(Seven>nMax)
	{
		nMax=Seven;
		PreDiamondType=DIAMONDSEVEN;
	}
	int Heave=rand()*probabilityHeave/100;
	if(Heave>nMax)
	{
		nMax=Heave;
		PreDiamondType=DIAMONDHEAVE;
	}
	int Bar=rand()*probabilityBar/100;
	if(Bar>nMax)
	{
		nMax=Bar;
		PreDiamondType=DIAMONDBAR;
	}
	int Two=rand()*probabilityTwo/100;
	if(Two>nMax) 
	{
		nMax=Two;
		PreDiamondType=DIAMONDTWO;
	}
	int Spade=rand()*probabilitySpade/100;
	if(Spade>nMax) 
	{
		nMax=Spade;
		PreDiamondType=DIAMONDSPADE;
	}

	PreDiamondState=rand()%4;
}

void CMill::DrawPrepare(CDC* pDC)
{
	CBrush brush,GridBrush;
	brush.CreateSolidBrush(MillColor);
	CBrush *pOld=pDC->SelectObject(&brush);
	//
	pDC->Rectangle(&MillRect);
	//
	GridBrush.CreateSolidBrush(PreColor);
	pDC->SelectObject(&GridBrush);
	for(int iy=0;iy<4;iy++)
	{
		for(int ix=0;ix<4;ix++)
		{
			if(pDiamond->DiamondStruct[PreDiamondType][PreDiamondState][ix][iy])
			{	
				pDC->Rectangle(MillRect.left+ix*GridSize.cx,MillRect.top+iy*GridSize.cy,MillRect.left+(ix+1)*GridSize.cx,MillRect.top+(iy+1)*GridSize.cy);
			}
		}
	}
	pDC->SelectObject(pOld);
	GridBrush.DeleteObject();
	brush.DeleteObject();
}