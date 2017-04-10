// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__8EB88CBE_7260_416F_8DB7_34B782532B75__INCLUDED_)
#define AFX_NODE_H__8EB88CBE_7260_416F_8DB7_34B782532B75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSnake;

class CNode  
{
	friend CSnake;
public:
	CNode();
	virtual ~CNode();

private:
	CNode* m_former;
	CNode* m_next;
	int x;
	int y;
};

#endif // !defined(AFX_NODE_H__8EB88CBE_7260_416F_8DB7_34B782532B75__INCLUDED_)
