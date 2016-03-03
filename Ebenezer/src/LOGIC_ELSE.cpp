// LOGIC_ELSE.cpp: implementation of the LOGIC_ELSE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ebenezer.h"
#include "Define.h"
#include "LOGIC_ELSE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LOGIC_ELSE::LOGIC_ELSE()
{

}

LOGIC_ELSE::~LOGIC_ELSE()
{

}

void LOGIC_ELSE::Init()
{
	for( int i = 0; i < MAX_LOGIC_ELSE_INT; i++)
	{
		m_LogicElseInt[i] = -1;
	}

	m_bAnd = TRUE;
}

void LOGIC_ELSE::Parse_and(char *pBuf)
{
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace( temp, pBuf+index );

	if( !strcmp( temp, "CHECK_UNDER_WEIGHT" ) )
	{
		m_LogicElse = LOGIC_CHECK_UNDER_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
	}
	else if( !strcmp( temp, "CHECK_OVER_WEIGHT" ) )
	{
		m_LogicElse = LOGIC_CHECK_OVER_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
	}
	else if( !strcmp( temp, "CHECK_SKILL_POINT" ) )
	{
		m_LogicElse = LOGIC_CHECK_SKILL_POINT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// SkillPoint
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Below
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Above
	}
	else if( !strcmp( temp, "CHECK_EXIST_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Item no.
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Item count
	}
	else if( !strcmp( temp, "CHECK_CLASS" ) )
	{
		m_LogicElse = LOGIC_CHECK_CLASS;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 1
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 2
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 3
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 4
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 5
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 6
	}
	else if( !strcmp( temp, "CHECK_WEIGHT" ) )
	{
		m_LogicElse = LOGIC_CHECK_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
	}
// 비러머글 복권!!!
	else if( !strcmp( temp, "CHECK_EDITBOX") )
	{
		m_LogicElse = LOGIC_CHECK_EDITBOX;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );
	}
	else if( !strcmp( temp, "RAND") )
	{
		m_LogicElse = LOGIC_RAND;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Chances of you hitting the jackpot		
	}
//
	// 현재 여기까지만 쓰입니다. 아래는 나중에 맞게 수정해서 쓰세여.


/*
	if( !strcmp( temp, "CHECK_NATION" ) )
	{
		m_LogicElse = LOGIC_CHECK_NATION;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Nation
	}
	else if( !strcmp( temp, "CHECK_LV" ) )
	{
		m_LogicElse = LOGIC_CHECK_LEVEL;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Minimum level
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Maximum level
	}
	else if( !strcmp( temp, "NOEXIST_ITEM" ) )
	{
		m_LogicElse = LOGIC_NOEXIST_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Item no.
	}
	else if( !strcmp( temp, "NOQUEST_END" ) )
	{
		m_LogicElse = LOGIC_QUEST_END;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Quest no.
	}
	else if( !strcmp( temp, "CHECK_QUEST_LOG" ) )
	{
		m_LogicElse = LOGIC_QUEST_LOG;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Quest no.
	}

	else if( !strcmp( temp, "CHECK_NOAH" ) )
	{
		m_LogicElse = LOGIC_CHECK_NOAH;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Gold
	}
*/
///////////////////////// 여기서 부턴 내가 한일 ///////////////////////////

/*
	else if( !strcmp( temp, "CHECK_CLASS" ) )
	{
		m_LogicElse = LOGIC_CHECK_CLASS;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 1
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 2
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 3
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 4
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 5
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Class 6
	}
	else if( !strcmp( temp, "CHECK_WEIGHT" ) )
	{
		m_LogicElse = LOGIC_CHECK_WEIGHT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Weight & Empty Slot
	}
	else if( !strcmp( temp, "CHECK_SKILLPOINT" ) )
	{
		m_LogicElse = LOGIC_CHECK_SKILLPOINT;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// SkillPoint
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Below
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Above
	}
	else if( !strcmp( temp, "EXIST_ITEM" ) )
	{
		m_LogicElse = LOGIC_EXIST_ITEM;

		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Item no.
		index += ParseSpace( temp, pBuf+index );	m_LogicElseInt[i++] = atoi( temp );		// Item count
	}
*/

////////////////////////////////////////////////////////////////////////////
	m_bAnd = TRUE;
}

void LOGIC_ELSE::Parse_or(char *pBuf)
{
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace( temp, pBuf+index );
	m_bAnd = FALSE;
}
