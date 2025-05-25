/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Global.h"
#include "LOGIC_ELSE.h"

//-----------------------------------------------------------------------------
void LOGIC_ELSE::Parse_and(char* pBuf) {
	int index = 0, i = 0;
	char temp[1024];

	index += ParseSpace(temp, pBuf+index);
	memcpy(m_LogicCmd, temp, strlen(temp) + 1);

	if(!strcmp(temp, "CHECK_LV")) {
		m_LogicElse = LOGIC_CHECK_LEVEL;

		index += ParseSpace(temp, pBuf+index); m_LogicElseInt[i++] = atoi(temp); // minimum level
		index += ParseSpace(temp, pBuf+index); m_LogicElseInt[i++] = atoi(temp); // maximum level

	} else if(!strcmp(temp, "CHECK_EXIST_EVENT")) {
		m_LogicElse = LOGIC_EXIST_COM_EVENT;

		index += ParseSpace(temp, pBuf+index); m_LogicElseInt[i++] = atoi(temp); // event number
		index += ParseSpace(temp, pBuf+index); m_LogicElseInt[i++] = atoi(temp); // state (is the event at this state?)

	} else if (!strcmp(temp, "HOWMUCH_ITEM")) {
		m_LogicElse = LOGIC_HOWMUCH_ITEM;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // item number
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // minimum amount
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // maximum amount

	} else if (!strcmp(temp, "CHECK_PROMOTION_ELIGIBLE")) {
		m_LogicElse = LOGIC_CHECK_PROMOTION_ELIGIBLE;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // typically -1

	} else if (!strcmp(temp, "CHECK_CLASS")) {
		m_LogicElse = LOGIC_CHECK_CLASS;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 1

		int old_index = index;
		index += ParseSpace(temp, pBuf + index);

		if (index != old_index) {
			m_LogicElseInt[i++] = atoi(temp); // class 2
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 3
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 4
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 5
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 6
		}
		else {
			m_LogicElseInt[i++] = -1;
			m_LogicElseInt[i++] = -1; m_LogicElseInt[i++] = -1;
			m_LogicElseInt[i++] = -1; m_LogicElseInt[i++] = -1;
		}

	} else if(!strcmp(temp, "CHECK_NOCLASS")) {
		m_LogicElse = LOGIC_CHECK_NOCLASS;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 1

		int old_index = index;
		index += ParseSpace(temp, pBuf + index);

		if (index != old_index) {
			m_LogicElseInt[i++] = atoi(temp); // class 2
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 3
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 4
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 5
			index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); // class 6
		}
		else {
			m_LogicElseInt[i++] = -1;
			m_LogicElseInt[i++] = -1; m_LogicElseInt[i++] = -1;
			m_LogicElseInt[i++] = -1; m_LogicElseInt[i++] = -1;
		}

	} else if (!strcmp(temp, "CHECK_NOAH")) {
		m_LogicElse = LOGIC_CHECK_NOAH;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);

	} else if (!strcmp(temp, "CHECK_EXIST_ITEM")) {
		m_LogicElse = LOGIC_CHECK_EXIST_ITEM;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);

	} else if (!strcmp(temp, "CHECK_DICE")) {
		m_LogicElse = LOGIC_CHECK_DICE;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);

	} else if (!strcmp(temp, "CHECK_EMPTY_SLOT")) {
		m_LogicElse = LOGIC_CHECK_EMPTY_SLOT;

		// NOTE: number of empty slots to check for
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp);

	} else if (!strcmp(temp, "CHECK_EXIST_EVENT")){
		m_LogicElse = LOGIC_CHECK_EXIST_EVENT;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); //event id
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); //state
	} else if (!strcmp(temp, "CHECK_NOEXIST_EVENT")){
		m_LogicElse = LOGIC_CHECK_NOEXIST_EVENT;

		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); //event id
		index += ParseSpace(temp, pBuf + index); m_LogicElseInt[i++] = atoi(temp); //state
	} else {
		m_LogicElse = LOGIC_UNKNOWN;
		printf("Unknown logic command: %s\n", temp);
		//system("pause");
		//exit(-1);
	}

	m_bAnd = true;
}
