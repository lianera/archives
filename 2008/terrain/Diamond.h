// Diamond.h: interface for the CDiamond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIAMOND_H__EEE5D09C_2685_434A_9349_3EA0EC22951E__INCLUDED_)
#define AFX_DIAMOND_H__EEE5D09C_2685_434A_9349_3EA0EC22951E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDiamond  
{
public:
	CDiamond();
	virtual ~CDiamond();
	bool MoveDiamondLeft();
	bool MoveDiamondRight();
	bool MoveDiamondDown();
	bool RollDiamond();
	void DrawDiamond(CDC* pDC);
private:
	void DimDiamond();
public:
	bool DiamondStruct[7][4][4][4];	//第一维：方块类型，第二维：状态，第三、四维：方块结构;
	POINT DiamondPos;
	COLORREF DiamondColor;
	int DiamondType;
	int DiamondState;
	bool FullLine();//满了一行
private:
	bool overlap();
};

#endif // !defined(AFX_DIAMOND_H__EEE5D09C_2685_434A_9349_3EA0EC22951E__INCLUDED_)
