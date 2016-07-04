#pragma once

class ChatPacket
{
public:
	// Construct a chat packet from the data provided
	// NOTE: Using pointers here for more flexible input (references would be nice, but implementation can be fiddly)
	static void Construct(Packet * result, uint8 bType, std::string * strMessage = nullptr, std::string * strSender = nullptr, 
		uint8 bNation = 1, int16 senderID = -1)
	{
		result->Initialize(WIZ_CHAT);
		*result << bType << bNation << senderID;

		result->SByte();
		if (strSender == nullptr) *result << uint8(0);
		else *result << *strSender;

		result->DByte();
		if (strMessage == nullptr) *result << uint16(0);
		else *result << *strMessage;
	}

	static void Construct(Packet * result, uint8 bType, const char * szMessage, const char * szSender = "",
		uint8 bNation = 1, int16 senderID = -1)
	{
		std::string strSender = szSender, strMessage = szMessage;
		Construct(result, bType, &strMessage, &strSender, bNation, senderID);
	}
};

#define COMMAND_HANDLER(name) bool name (CommandArgs & vargs, const char *args, const char *description)

typedef std::list<std::string> CommandArgs;

template <class T>
class Command
{
public:
	const char * Name;
	COMMAND_HANDLER((T::*Handler));
	const char * Help;
};

INLINE void* allocate_and_copy(uint32 len, void * pointer)
{
	void * data = (void*)malloc(len);
	if (data == nullptr)
		return data;

	memcpy(data, pointer, len);
	return data;
}

#define init_command_table(t, command_table, command_map) \
	for (int i = 0; i < sizeof(command_table) / sizeof(*command_table); i++) \
	command_map.insert(std::make_pair(command_table[i].Name, (Command<t> *)(allocate_and_copy(sizeof(*command_table), (void *)&command_table[i]))));

#define free_command_table(command_map) \
	for (auto itr = command_map.begin(); itr != command_map.end(); ++itr) \
	delete itr->second; \
	command_map.clear();

static std::list<std::string> StrSplit(const std::string &src, const std::string &sep)
{
	std::list<std::string> r;
	std::string s;
	for (std::string::const_iterator i = src.begin(); i != src.end(); ++i)
	{
		if (sep.find(*i) != std::string::npos)
		{
			if (!s.empty())
				r.push_back(s);
			s = "";
		}
		else
		{
			s += *i;
		}
	}
	if (!s.empty()) 
		r.push_back(s);
	return r;
}

#include <map>
class CGameServerDlg;
typedef std::map<std::string, Command<CUser> *> ChatCommandTable;
typedef std::map<std::string, Command<CGameServerDlg> *> ServerCommandTable;

#define CHAT_COMMAND_PREFIX		'+'
#define SERVER_COMMAND_PREFIX	'/'
