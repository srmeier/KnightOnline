#ifndef __LOGIC_ELSE_H_
#define __LOGIC_ELSE_H_

//-----------------------------------------------------------------------------
#define MAX_LOGIC_ELSE_INT 10

#define LOGIC_UNKNOWN                  0x00
#define LOGIC_CHECK_UNDER_WEIGHT       0x01
#define LOGIC_CHECK_OVER_WEIGHT        0x02
#define LOGIC_CHECK_SKILL_POINT        0x03
#define LOGIC_EXIST_ITEM               0x04
#define LOGIC_CHECK_CLASS              0x05
#define LOGIC_CHECK_WEIGHT             0x06
#define LOGIC_CHECK_EDITBOX            0x07
#define LOGIC_RAND                     0x08
#define LOGIC_HOWMUCH_ITEM             0x09
#define	LOGIC_CHECK_LEVEL              0x0A
#define LOGIC_NOEXIST_COM_EVENT        0x0B
#define LOGIC_EXIST_COM_EVENT          0x0C
#define LOGIC_CHECK_NOAH               0x0D
#define LOGIC_CHECK_PROMOTION_ELIGIBLE 0x0E
#define LOGIC_CHECK_EXIST_ITEM         0x0F
#define LOGIC_CHECK_DICE               0x10
#define LOGIC_CHECK_EMPTY_SLOT         0x11
#define LOGIC_CHECK_NOCLASS            0x12

//-----------------------------------------------------------------------------
class LOGIC_ELSE {
public:
	bool m_bAnd;
	char m_LogicCmd[1024];
	unsigned char m_LogicElse;
	int m_LogicElseInt[MAX_LOGIC_ELSE_INT];

public:
	void Init(void);
	void Parse_or(char* pBuf);
	void Parse_and(char* pBuf);

public:
	LOGIC_ELSE(void) {}
	virtual ~LOGIC_ELSE(void) {}
};

#endif
