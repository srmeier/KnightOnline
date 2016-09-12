/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EXEC.h"
#include "Global.h"

//-----------------------------------------------------------------------------
void EXEC::Parse(char* pBuf) {
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace(temp, pBuf + index);
	memcpy(m_ExeCmd, temp, strlen(temp) + 1);

	if (!strcmp(temp, "SAY")) {
		m_Exec = EXEC_SAY;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);

	}
	else if (!strcmp(temp, "SELECT_MSG")) {
		m_Exec = EXEC_SELECT_MSG;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 1
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 1

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 2
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 2

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 3
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 3

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 4
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 4

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 5
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 5

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 6
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 6

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 7
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 7

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 8
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 8

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 9
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 9

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 10
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // 10

	}
	else if (!strcmp(temp, "RUN_EVENT")) {
		m_Exec = EXEC_RUN_EVENT;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);

	}
	else if (!strcmp(temp, "GIVE_ITEM")) {
		m_Exec = EXEC_GIVE_ITEM;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // item number
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // item count

	}
	else if (!strcmp(temp, "ROB_ITEM")) {
		m_Exec = EXEC_ROB_ITEM;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // item number
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // item count

	}
	else if (!strcmp(temp, "GIVE_NOAH")) {
		m_Exec = EXEC_GIVE_NOAH;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // amount

	}
	else if (!strcmp(temp, "SAVE_EVENT")) {
		m_Exec = EXEC_SAVE_COM_EVENT;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // event number
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // event state

	}
	else if (!strcmp(temp, "EXP_CHANGE")) {
		m_Exec = EXEC_EXP_CHANGE;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // amount

	}
	else if (!strcmp(temp, "ROB_NOAH")) {
		m_Exec = EXEC_ROB_NOAH;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // amount

	}else if (!strcmp(temp, "PROMOTE_USER_NOVICE")) {
		m_Exec = EXEC_PROMOTE_USER_NOVICE;

	} else if (!strcmp(temp , "PROMOTE_USER")){
		m_Exec = EXEC_PROMOTE_USER;

	} else if (!strcmp(temp, "RETURN")) {
		m_Exec = EXEC_RETURN;

	} else if (!strcmp(temp, "RUN_EXCHANGE")) {
		m_Exec = EXEC_RUN_EXCHANGE;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp); // nIndex in ITEM_EXCHANGE table

	} else if(!strcmp(temp, "ROLL_DICE")) {
		m_Exec = EXEC_ROLL_DICE;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);

	} else if (!strcmp(temp, "ZONE_CHANGE")) {
		m_Exec = EXEC_ZONE_CHANGE;

		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_ExecInt[i++] = atoi(temp);

	} else {
		m_Exec = EXEC_UNKNOWN;
		printf("Unknown execute command: %s\n", temp);
		//system("pause");
		//exit(-1);
	}
}
