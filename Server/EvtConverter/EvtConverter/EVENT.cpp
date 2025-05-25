/*
*/

#include <sstream>

#include "Global.h"
#include "EVENT.h"

//-----------------------------------------------------------------------------
void EVENT::DeleteAll(void) {
	if(!m_arEvent.IsEmpty()) m_arEvent.DeleteAllData();
}

//-----------------------------------------------------------------------------
bool EVENT::LoadEvent(int zone) {
	char byte;
	FILE* pFile;
	int event_num;
	int index = 0;
	int t_index = 0;
	char buf[4096];
	char temp[1024];
	char first[1024];
	std::string filename;
	unsigned int length, count;

	EVENT_DATA* newData = NULL;
	EVENT_DATA* eventData = NULL;

	std::ostringstream stringStream;
	stringStream << ".\\QUESTS\\" << zone << ".evt";
	filename = stringStream.str();

	pFile = fopen(filename.c_str(), "rb");
	if(pFile == NULL) {
		printf("Failed to open the file! (%s)\n", filename.c_str());
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	length = ftell(pFile);
	fseek(pFile, 0L, SEEK_SET);

	count = 0;

	while(count < length) {
		fread(&byte, sizeof(char), 1, pFile); count++;

		if(byte != '\r' && byte != '\n') buf[index++] = byte;

		if((byte=='\n' || count==length) && index > 1) {
			buf[index] = '\0';

			t_index = 0;

			// NOTE: ignore the comments
			if(buf[t_index]==';' || buf[t_index]=='/') {
				index = 0;
				continue;
			}

			t_index += ParseSpace(first, buf+t_index);

			if(!strcmp(first, "EVENT")) {

				t_index += ParseSpace(temp, buf+t_index);
				event_num = atoi(temp);

				if(newData) {
					delete newData;
					goto cancel_event_load;
				}

				if(m_arEvent.GetData(event_num)) {
					printf("Event occurs twice! (%d)\n", event_num);
					goto cancel_event_load;
				}

				eventData = new EVENT_DATA;
				eventData->m_EventNum = event_num;

				if(!m_arEvent.PutData(eventData->m_EventNum, eventData)) {
					delete eventData;
					eventData = NULL;
				}

				newData = m_arEvent.GetData(event_num);

			} else if(!strcmp(first, "E")) {
				if(!newData) goto cancel_event_load;

				EXEC* newExec = new EXEC;
				newExec->Parse(buf+t_index);

				newData->m_arExec.push_back(newExec);

			} else if(!strcmp(first, "A")) {
				if(!newData) goto cancel_event_load;

				LOGIC_ELSE* newLogicElse = new LOGIC_ELSE;
				newLogicElse->Parse_and(buf+t_index);

				newData->m_arLogicElse.push_back(newLogicElse);

			} else if(!strcmp(first, "END")) {
				if(!newData) goto cancel_event_load;

				newData = NULL;
			}

			index = 0;
		}
	}

	fclose(pFile);

	return true;

cancel_event_load:
	printf("Event failed: (zone = %d, event = %d)", zone, event_num);
	fclose(pFile);
	DeleteAll();

	return false;
}
