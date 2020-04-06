#include "walker.h"

RakNetController::RakNetController(DWORD samp_raknet_address) {
	m_pSeqPacketCallbacks = new Sequence<RakNetCallback>();
	m_pSeqRPCCallbacks = new Sequence<RakNetCallback>();
}
RakNetController::~RakNetController() {
	delete m_pSeqPacketCallbacks;
	delete m_pSeqRPCCallbacks;
}
void RakNetController::RegisterPacketCallback(RakNetCallback _callback) {
	this->m_pSeqPacketCallbacks->Push(_callback);
}
void RakNetController::RegisterRPCCallback(RakNetCallback _callback) {
	this->m_pSeqRPCCallbacks->Push(_callback);
}
void RakNetController::OnRPCRecieve(uint8_t unique_id, LQ_RPCParameters *rpc_parameters, void(*callback) (LQ_RPCParameters *)) {
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