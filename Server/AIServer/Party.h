#pragma once

class CParty  
{
public:
	CParty();
	virtual ~CParty();

	void Initialize();

	void PartyDelete(Packet & pkt);
	void PartyRemove(Packet & pkt);
	void PartyInsert(Packet & pkt);
	void PartyCreate(Packet & pkt);
	void PartyProcess(Packet & pkt);
};
