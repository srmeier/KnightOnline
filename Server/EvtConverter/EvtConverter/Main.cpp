/*
*/

#include <stdio.h>
#include <stdlib.h>

#include "EVENT.h"

//-----------------------------------------------------------------------------
EVENT testEvent;
FILE* outputFile;

void AddExecCode(EXEC* pExec);
void FillEventID(EVENT_DATA* pEvent);

//-----------------------------------------------------------------------------
void AddExecCode(EXEC* pExec) {
	switch (pExec->m_Exec) {
		case EXEC_UNKNOWN: {
			fprintf(outputFile, "\tpUser:SendDebugString(\"Unknown EXEC command '%s'.\"); -- unknown execute command (%s)\n",
				pExec->m_ExeCmd, pExec->m_ExeCmd
			);
			fprintf(outputFile, "\tdo return; end\n"); // NOTE: weird Lua thing
		} break;

		case EXEC_RUN_EVENT: {
			// NOTE: then execute the commands
			EventDataArray testArray = testEvent.m_arEvent;
			std::map<int, EVENT_DATA*>::iterator iter1 = testArray.m_UserTypeMap.begin();
			std::map<int, EVENT_DATA*>::iterator iter2 = testArray.m_UserTypeMap.end();

			for (int i = 0; iter1 != iter2; ++iter1, ++i) {
				EVENT_DATA* eventData = (*iter1).second;

				// TODO: if it gets pulled here then I need to prevent it form being read later
				if (eventData->m_EventNum == pExec->m_ExecInt[0]) {
					FillEventID(eventData);

					eventData->m_bDup = true;
					eventData->m_bUsedByAnother = true;
				}
			}
		} break;

		case EXEC_SELECT_MSG: {
			fprintf(outputFile, "\tpUser:SelectMsg(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d);\n",
				pExec->m_ExecInt[1],
				pExec->m_ExecInt[2],  pExec->m_ExecInt[3],
				pExec->m_ExecInt[4],  pExec->m_ExecInt[5],
				pExec->m_ExecInt[6],  pExec->m_ExecInt[7],
				pExec->m_ExecInt[8],  pExec->m_ExecInt[9],
				pExec->m_ExecInt[10], pExec->m_ExecInt[11],
				pExec->m_ExecInt[12], pExec->m_ExecInt[13],
				pExec->m_ExecInt[14], pExec->m_ExecInt[15],
				pExec->m_ExecInt[16], pExec->m_ExecInt[17],
				pExec->m_ExecInt[18], pExec->m_ExecInt[19],
				pExec->m_ExecInt[20], pExec->m_ExecInt[21]
			);

			EventDataArray testArray = testEvent.m_arEvent;
			std::map<int, EVENT_DATA*>::iterator iter1 = testArray.m_UserTypeMap.begin();
			std::map<int, EVENT_DATA*>::iterator iter2 = testArray.m_UserTypeMap.end();

			for (int i = 0; iter1 != iter2; ++iter1, ++i) {
				EVENT_DATA* eventData = (*iter1).second;

				if ((eventData->m_EventNum == pExec->m_ExecInt[3])  ||
					(eventData->m_EventNum == pExec->m_ExecInt[5])  ||
					(eventData->m_EventNum == pExec->m_ExecInt[7])  ||
					(eventData->m_EventNum == pExec->m_ExecInt[9])  ||
					(eventData->m_EventNum == pExec->m_ExecInt[11]) ||
					(eventData->m_EventNum == pExec->m_ExecInt[13]) ||
					(eventData->m_EventNum == pExec->m_ExecInt[15]) ||
					(eventData->m_EventNum == pExec->m_ExecInt[17]) ||
					(eventData->m_EventNum == pExec->m_ExecInt[19]) ||
					(eventData->m_EventNum == pExec->m_ExecInt[21])
				) eventData->m_bUsedByAnother = true;
			}
		} break;

		case EXEC_SAY: {
			fprintf(outputFile, "\tpUser:NpcSay(%d, %d, %d, %d, %d, %d, %d, %d);\n",
				pExec->m_ExecInt[2],  pExec->m_ExecInt[3],
				pExec->m_ExecInt[4],  pExec->m_ExecInt[5],
				pExec->m_ExecInt[6],  pExec->m_ExecInt[7],
				pExec->m_ExecInt[8],  pExec->m_ExecInt[9]
			);
		} break;

		case EXEC_RETURN: {
			fprintf(outputFile, "\tdo return; end\n"); // NOTE: weird Lua thing
		} break;

		case EXEC_SAVE_COM_EVENT: {
			fprintf(outputFile, "\tpUser:SaveEvent(%d, %d);\n", pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		} break;

		case EXEC_ROB_ITEM: {
			fprintf(outputFile, "\tpUser:RobItem(%d, %d);\n", pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		} break;

		case EXEC_GIVE_ITEM: {
			fprintf(outputFile, "\tpUser:GiveItem(%d, %d);\n", pExec->m_ExecInt[0], pExec->m_ExecInt[1]);
		} break;

		case EXEC_EXP_CHANGE: {
			fprintf(outputFile, "\tpUser:ExpChange(%d);\n", pExec->m_ExecInt[0]);
		} break;

		case EXEC_ROB_NOAH: {
			fprintf(outputFile, "\tpUser:GoldLose(%d);\n", pExec->m_ExecInt[0]);
		} break;

		case EXEC_GIVE_NOAH: {
			fprintf(outputFile, "\tpUser:GoldGain(%d);\n", pExec->m_ExecInt[0]);
		} break;

		case EXEC_PROMOTE_USER_NOVICE: {
			fprintf(outputFile, "\tpUser:PromoteUserNovice();\n");
		} break;

		case EXEC_PROMOTE_USER: {
			fprintf(outputFile, "\tpUser:PromoteUser();\n");
		} break;

		case EXEC_RUN_EXCHANGE: {
			fprintf(outputFile, "\tRunCountExchange(sUID, %d, 0);\n", pExec->m_ExecInt[0]);
		} break;

		case EXEC_ROLL_DICE: {
			fprintf(outputFile, "\tlocal rand_num = RollDice(sUID, %d);\n", pExec->m_ExecInt[0]);
		} break;

		case EXEC_ZONE_CHANGE: {
			fprintf(outputFile, "\tpUser:ZoneChange(%d, %d, %d);\n", pExec->m_ExecInt[0], pExec->m_ExecInt[1], pExec->m_ExecInt[2]);
		} break;

		default: {
			printf("Missing EXEC code for command %d.\n", pExec->m_Exec);
			//system("pause");
			//exit(-1);

			fprintf(outputFile, "\tpUser:SendDebugString(\"Unhandled EXEC command '%s'.\"); -- unhandled execute command (%s)\n",
				pExec->m_ExeCmd, pExec->m_ExeCmd
			);
			fprintf(outputFile, "\tdo return; end\n"); // NOTE: weird Lua thing
		} break;
	}
}

//-----------------------------------------------------------------------------
void FillEventID(EVENT_DATA* pEvent) {
	// NOTE: if there is logic then check for it first
	int num_logic_to_close = 0;

	LogicElseArray::iterator iter1 = pEvent->m_arLogicElse.begin();
	for(; iter1!=pEvent->m_arLogicElse.end(); ++iter1) {
		LOGIC_ELSE* pLogicElse = *iter1;

		switch(pLogicElse->m_LogicElse) {
			case LOGIC_UNKNOWN: {
				fprintf(outputFile, "\tpUser:SendDebugString(\"Unknown LOGIC command '%s'.\");\n"\
					"\tif false then -- unknown logic command (%s)\n",
					pLogicElse->m_LogicCmd, pLogicElse->m_LogicCmd
				); num_logic_to_close++;
			} break;

			case LOGIC_CHECK_LEVEL: {
				fprintf(outputFile, "\tlocal lvl = pUser:GetLevel();\n"\
					"\tif lvl >= %d and lvl <= %d then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				); num_logic_to_close++;
			} break;

			case LOGIC_HOWMUCH_ITEM: {
				fprintf(outputFile, "\tlocal count = pUser:HowMuchItem(%d);\n"\
					"\tif count >= %d and count <= %d then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1],
					pLogicElse->m_LogicElseInt[2]
				); num_logic_to_close++;
			} break;

			case LOGIC_EXIST_COM_EVENT: {
				fprintf(outputFile, "\tlocal state = pUser:SearchQuest(%d);\n", pLogicElse->m_LogicElseInt[0]);
				fprintf(outputFile, "\tif state == %d then\n", pLogicElse->m_LogicElseInt[1]);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_PROMOTION_ELIGIBLE: {

				fprintf(outputFile, "\tif pUser:CheckPromoteEligible() then\n");

				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_CLASS: {
				fprintf(outputFile, "\tif pUser:CheckClass(%d, %d, %d, %d, %d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1],
					pLogicElse->m_LogicElseInt[2], pLogicElse->m_LogicElseInt[3],
					pLogicElse->m_LogicElseInt[4], pLogicElse->m_LogicElseInt[5]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_NOCLASS: {
				fprintf(outputFile, "\tif not pUser:CheckClass(%d, %d, %d, %d, %d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1],
					pLogicElse->m_LogicElseInt[2], pLogicElse->m_LogicElseInt[3],
					pLogicElse->m_LogicElseInt[4], pLogicElse->m_LogicElseInt[5]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_NOAH: {
				fprintf(outputFile, "\tlocal coins = pUser:GetCoins();\n");
				fprintf(outputFile, "\tif coins >= %d and coins <= %d then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_WEIGHT: {

				fprintf(outputFile, "\tif not pUser:CheckWeight(%d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_EXIST_ITEM: {
				fprintf(outputFile, "\tif pUser:CheckExistItem(%d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_NOEXIST_ITEM: {
				fprintf(outputFile, "\tif pUser:CheckNoExistItem(%d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_DICE: {
				fprintf(outputFile, "\tif rand_num == %d then\n", pLogicElse->m_LogicElseInt[0]);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_EMPTY_SLOT: {
				// NOTE: ambiguous case, is m_LogicElseInt the number of empty slots to check for??
				if (pLogicElse->m_LogicElseInt[0] == 0) {
					fprintf(outputFile, "\tif pUser:EmptySlotCount() == 0 then\n");
				} else {
					fprintf(outputFile, "\tif pUser:EmptySlotCount() > 0 then\n");
				}

				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_SKILL_POINT: {
				fprintf(outputFile, "\tif pUser:CheckSkillPoint(%d, %d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1], pLogicElse->m_LogicElseInt[2]
				);
				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_KNIGHT: {

				fprintf(outputFile, "\tif not pUser:CheckKnight() then\n");

				num_logic_to_close++;
			} break;

			case LOGIC_CHECK_LOYALTY: {

				fprintf(outputFile, "\tif pUser:CheckLoyalty(%d, %d) then\n",
					pLogicElse->m_LogicElseInt[0], pLogicElse->m_LogicElseInt[1]
				);

				num_logic_to_close++;
			} break;

			default: {
				printf("Missing LOGIC code for command %d.\n", pLogicElse->m_LogicElse);
				//system("pause");
				//exit(-1);

				fprintf(outputFile, "\tpUser:SendDebugString(\"Unhandled LOGIC command '%s'.\"); -- unhandled logic command (%s)\n",
					pLogicElse->m_LogicCmd, pLogicElse->m_LogicCmd
				);
				fprintf(outputFile, "\tdo return; end\n"); // NOTE: weird Lua thing
			} break;
		}
	}

	// NOTE: then execute the commands
	ExecArray::iterator iter2 = pEvent->m_arExec.begin();
	for(; iter2!=pEvent->m_arExec.end(); ++iter2) {
		AddExecCode(*iter2);
	}

	// NOTE: close the if-statements
	for(int i=0; i<num_logic_to_close; ++i) {
		fprintf(outputFile, "\tend\n");
	}
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {
	int iZone = atoi(argv[1]);

	char buf[0xFF] = "";
	sprintf(buf, "%d.lua", iZone);

	outputFile = fopen(buf, "w"); //freopen("test.lua", "w", stdout);

	if(testEvent.LoadEvent(iZone) == false) {
		printf("ERROR!\n");
	}

	EventDataArray testArray = testEvent.m_arEvent;
	std::map<int, EVENT_DATA*>::iterator iter1 = testArray.m_UserTypeMap.begin();
	std::map<int, EVENT_DATA*>::iterator iter2 = testArray.m_UserTypeMap.end();

	for(int i=0; iter1!=iter2; ++iter1, ++i) {
		EVENT_DATA* eventData = (*iter1).second;
		if (eventData->m_bDup) continue;

		if(i == 0) {
			fprintf(outputFile, "if nEventID == %d then\n", eventData->m_EventNum);
			FillEventID(eventData);
		} else {
			fprintf(outputFile, "elseif nEventID == %d then\n", eventData->m_EventNum);
			FillEventID(eventData);
		}
	}

	fprintf(outputFile, "else\n");
	fprintf(outputFile, "\tpUser:SendDebugString(\"Unprocessed event id (\"..nEventID..\")\");\n");
	fprintf(outputFile, "end\n");

	fclose(outputFile);

	/*
	printf("\n\n----- NODES -----\n");

	iter1 = testArray.m_UserTypeMap.begin();
	iter2 = testArray.m_UserTypeMap.end();

	for (int i = 0; iter1 != iter2; ++iter1, ++i) {
		EVENT_DATA* eventData = (*iter1).second;
		if (eventData->m_bUsedByAnother) continue;

		int sSid = 0;
		ExecArray::iterator iter3 = eventData->m_arExec.begin();
		for (; iter3 != eventData->m_arExec.end(); ++iter3) {
			switch ((*iter3)->m_Exec) {
				case EXEC_SELECT_MSG:
					sSid = (*iter3)->m_ExecInt[0];
					printf("nEventID = %d; sSid = %d\n", eventData->m_EventNum, sSid);
				break;
			}
		}
	}
	*/

	system("pause");

	return 0;
}
