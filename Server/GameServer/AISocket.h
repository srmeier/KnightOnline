#pragma once

#include "../shared/KOSocket.h"
#include "MagicProcess.h"

class CAISocket : public KOSocket 
{
public:
	CAISocket(uint16 socketID, SocketMgr * mgr) : KOSocket(socketID, mgr, 0, 262144, 262144), m_bHasConnected(false) {}

	INLINE bool IsReconnecting() { return m_bHasConnected; }

	virtual void OnConnect();
	virtual bool HandlePacket(Packet & pkt);

	void Initialize();

	void InitEventMonster(int index);

	void LoginProcess(Packet & pkt);
	void RecvCheckAlive(Packet & pkt);
	void RecvServerInfo(Packet & pkt);
	void RecvNpcInfoAll(Packet & pkt);
	void RecvNpcMoveResult(Packet & pkt);
	void RecvNpcAttack(Packet & pkt);
	void RecvNpcInfo(Packet & pkt);
	void RecvNpcRegionUpdate(Packet & pkt);
	void RecvUserExp(Packet & pkt);
	void RecvSystemMsg(Packet & pkt);
	void RecvNpcGiveItem(Packet & pkt);
	void RecvUserFail(Packet & pkt);
	void RecvGateDestory(Packet & pkt);
	void RecvNpcDead(Packet & pkt);
	void RecvNpcInOut(Packet & pkt);
	void RecvBattleEvent(Packet & pkt);
	void RecvNpcEventItem(Packet & pkt);
	void RecvGateOpen(Packet & pkt);
	void RecvCompressed(Packet & pkt);
	void RecvNpcHpChange(Packet & pkt);
	void RecvNpcSpawnRequest(Packet & pkt);
	void RegionPacketAdd(Packet & pkt);

	virtual void OnDisconnect();
	virtual ~CAISocket() {}

private:
	bool m_bHasConnected;
};
