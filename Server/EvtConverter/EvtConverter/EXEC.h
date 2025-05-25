#ifndef __EXEC_H_
#define __EXEC_H_

//-----------------------------------------------------------------------------
#define MAX_EXEC_INT 30

#define EXEC_UNKNOWN                0x00
#define EXEC_SAY                    0x01
#define EXEC_SELECT_MSG             0x02
#define EXEC_RUN_EVENT              0x03
#define EXEC_GIVE_ITEM              0x04
#define EXEC_ROB_ITEM               0x05
#define EXEC_RETURN                 0x06
#define EXEC_OPEN_EDITBOX           0x07
#define EXEC_GIVE_NOAH              0x08
#define EXEC_LOG_COUPON_ITEM        0x09
#define EXEC_SAVE_COM_EVENT         0x0A
#define EXEC_ROB_NOAH               0x0B
#define EXEC_EXP_CHANGE             0x0C
#define EXEC_PROMOTE_USER_NOVICE    0x0D
#define EXEC_RUN_EXCHANGE           0x0E
#define EXEC_ROLL_DICE              0x0F
#define EXEC_STATE_CHANGE           0x10
#define EXEC_ZONE_CHANGE            0x11
#define EXEC_STAT_POINT_DISTRIBUTE  0x12
#define EXEC_SKILL_POINT_DISTRIBUTE 0x13

//-----------------------------------------------------------------------------
class EXEC {
public:
	char m_ExeCmd[1024];
	unsigned char m_Exec;
	int m_ExecInt[MAX_EXEC_INT];

public:
	void Init(void);
	void Parse(char* pBuf);

public:
	EXEC(void) {}
	virtual ~EXEC(void) {}
};

#endif
