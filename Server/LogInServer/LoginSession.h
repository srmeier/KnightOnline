#pragma once

class LoginSession : public KOSocket
{
public:
	LoginSession(uint16_t socketID, SocketMgr* mgr);

	bool HandlePacket(Packet& pkt) override;
	void HandleVersion(Packet& pkt);
	void HandlePatches(Packet& pkt);
	void HandleLogin(Packet& pkt);
	void HandleServerlist(Packet& pkt);
	void HandleNews(Packet& pkt);
};

void InitPacketHandlers();
typedef void (LoginSession::* LSPacketHandler)(Packet&);
