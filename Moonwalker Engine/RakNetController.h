#pragma once
#include "walker.h"

class RakNetController {
public:
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
	typedef void (_stdcall *RakNetCallback)(RakNetCallbackData);

	RakNetController(DWORD samp_raknet_address) {
		m_pSeqPacketCallbacks = new Sequence<RakNetCallback>();
		m_pSeqRPCCallbacks = new Sequence<RakNetCallback>();
	}
	~RakNetController() {
		delete m_pSeqPacketCallbacks;
		delete m_pSeqRPCCallbacks;
	}
	void RegisterPacketCallback(RakNetCallback _callback) {
		this->m_pSeqPacketCallbacks->Push(_callback);
	}
	void RegisterRPCCallback(RakNetCallback _callback) {
		this->m_pSeqRPCCallbacks->Push(_callback);
	}
	void OnRPCRecieve(uint8_t unique_id, LQ_RPCParameters *rpc_parameters, void(*callback) (LQ_RPCParameters *)) {
		for (int i = 0; i < m_pSeqRPCCallbacks->Count(); i++) {
			BitStream bsData(rpc_parameters->input, rpc_parameters->numberOfBitsOfData / 8, false);
			Packet packet;
			packet.bitSize = rpc_parameters->numberOfBitsOfData;
			packet.data = rpc_parameters->input;
			packet.deleteData = false;
			packet.playerId = rpc_parameters->sender;
			(*m_pSeqRPCCallbacks)[i](RakNetCallbackData(unique_id, packet, bsData));
		}
	}
private:
	Sequence<RakNetCallback> *m_pSeqPacketCallbacks;
	Sequence<RakNetCallback> *m_pSeqRPCCallbacks;
	LQ_RakClientInterface *m_pRakClient;
};