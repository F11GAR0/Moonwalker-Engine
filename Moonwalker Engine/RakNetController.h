#pragma once
#ifndef WALKER_MAIN
#error "блять залупа мейн не заинклудили!"
#endif
#include "walker.h"

struct RakNetCallbackData {
	BYTE id;
	Packet pack;
	BitStream bs;
	RakNetCallbackData(BYTE _id, const Packet &_pack, const BitStream &_bs) {
		this->id = _id;
		this->pack = _pack;
		this->bs = _bs;
	}
};
typedef void(_stdcall *RakNetCallback)(RakNetCallbackData);

class RakNetController {
public:
	RakNetController(DWORD samp_raknet_address);
	~RakNetController();
	void RegisterPacketCallback(RakNetCallback _callback);
	void RegisterRPCCallback(RakNetCallback _callback);
	void OnRPCRecieve(uint8_t unique_id, LQ_RPCParameters *rpc_parameters, void(*callback) (LQ_RPCParameters *));
private:
	Sequence<RakNetCallback> *m_pSeqPacketCallbacks;
	Sequence<RakNetCallback> *m_pSeqRPCCallbacks;
	LQ_RakClientInterface *m_pRakClient;
};