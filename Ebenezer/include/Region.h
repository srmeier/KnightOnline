// Region.h: interface for the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_H__46F2BE85_327F_40DE_9FE3_4D28B034CDC2__INCLUDED_)
#define AFX_REGION_H__46F2BE85_327F_40DE_9FE3_4D28B034CDC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

class CN3VMesh;

//typedef std::vector<void*> CPtrArray22;

//CTypedPtrArray
typedef std::list<CN3VMesh*> VMeshArray;

class CRegion  
{
public:
	CRegion();
	virtual ~CRegion();

	VMeshArray	m_VMeshArray;
protected:
	int		m_nIndex;
};

#endif // !defined(AFX_REGION_H__46F2BE85_327F_40DE_9FE3_4D28B034CDC2__INCLUDED_)
