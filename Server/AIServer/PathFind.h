#pragma once

// temporary
struct CPoint
{
	CPoint() {}
	CPoint(int x, int y) : x(x), y(y) {}
	int x, y;
};

class CRect
{
public:
	CRect(int x1, int y1, int x3, int y3) 
		: A(x1, y1), C(x3, y3) {}
	bool PtInRect(int x, int y) { return (x >= A.x && x <= C.x && y >= A.y && y <= C.y); }
private:
	CPoint A, C;
};

class _PathNode {
public:
	int f;
	int h;
	int g;
	int x;
	int y;
	_PathNode *Parent;
	_PathNode *Child[8];
	_PathNode *NextNode;
};

class STACK {
public:
	_PathNode *NodePtr;
	STACK *NextStackPtr;
};

class CPathFind  
{
public:
	bool IsBlankMap(int x, int y);
	void SetMap(int x, int y, MAP * pMap, int16 min_x, int16 min_y);
	void PropagateDown(_PathNode *old);
	void Insert(_PathNode *node);
	_PathNode *CheckOpen(int x, int y);
	_PathNode *CheckClosed(int x, int y);
	void FindChildPathSub(_PathNode *node, int x, int y, int dx, int dy, int arg);
	void FindChildPath(_PathNode *node, int dx, int dy);
	void ClearData();
	_PathNode *ReturnBestNode();
	_PathNode *FindPath(int start_x, int start_y, int dest_x, int dest_y);
	CPathFind();
	virtual ~CPathFind();

	void Push(_PathNode *node);
	_PathNode *Pop();

protected:
	long	m_lMapUse;	
	_PathNode *m_pOpen, *m_pClosed;
	STACK *m_pStack;
	MAP *m_pMap;
	int16 m_min_x, m_min_y;
	uint32 m_nMapSize;
	CSize m_vMapSize;
};