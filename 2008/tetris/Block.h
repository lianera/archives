// Block.h: interface for the CBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCK_H__12E43284_EFC1_48FF_9D92_3CCBF20FDA85__INCLUDED_)
#define AFX_BLOCK_H__12E43284_EFC1_48FF_9D92_3CCBF20FDA85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlock  
{
public:
	CBlock();
	virtual ~CBlock();
	void AddBlock();
	void DrawBlock(CDC *pDC);
public:
	bool BlockPanel[30][80];
	COLORREF BlockColor;
};

#endif // !defined(AFX_BLOCK_H__12E43284_EFC1_48FF_9D92_3CCBF20FDA85__INCLUDED_)
