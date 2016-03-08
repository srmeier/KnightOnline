#pragma once

#include <map>
#include "KOSocketMgr.h"

typedef std::map<uint16, KOSocket *> SessionMap;

template <class T>
class ClientSocketMgr : public KOSocketMgr<T>
{
public:
	ClientSocketMgr<T>() {}

 	virtual Socket *AssignSocket(SOCKET socket) { return nullptr; }

	virtual ~ClientSocketMgr() {}
};
