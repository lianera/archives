// Block.cpp: implementation of the CBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "¶íÂÞË¹·½¿é.h"
#include "Block.h"
#include "Diamond.h"
#include "Panel.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CDiamond* pDiamond;
extern CMyApp theApp;
extern CPanel* pPanel;

CBlock::CBlock()
{
	for(int iy=0;iy<80;iy++)
		for(int ix=0;ix<30;ix++)
			BlockPanel[ix][iy]=false;
}

CBlock::~CBlock()
{

}

void CBlock::AddBlock()
{
	for(int iy=0;iy<4;iy++)
		for(int ix=0;ix<4;ix++)
		{
			if(pDiamond->DiamondStruct[pDiamond->DiamondType][pDiamond->DiamondState][ix][iy])
				BlockPanel[pDiamond->DiamondPos.x+ix][pDiamond->DiamondPos.y+iy]=true;
		}
}

void CBlock::DrawBlock(CDC *pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(BlockColor);
	CBrush *pOld=pDC->SelectObject(&brush);
	for(int iy=0;iy<=pPanel->nVGridNum;iy++)
	{
		for(int ix=0;ix<=pPanel->nHGridNum;ix++)
		{
			if(BlockPanel[ix][iy]==true)
			{
				POINT point;
				point.x=ix;
				point.y=iy;
				pPanel->PanelPosToPos(point);
				pDC->Rectangle(point.x,point.y,point.x+pPanel->GridSize.cx,point.y+pPanel->GridSize.cy);
			}
		}
	}
	pDC->SelectObject(pOld);
	brush.DeleteObject();
}