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

	static std::string GetAuthResultName(e_AuthResult eAuthResult);
};

void InitPacketHandlers();
typedef void (LoginSession::* LSPacketHandler)(Packet&);
