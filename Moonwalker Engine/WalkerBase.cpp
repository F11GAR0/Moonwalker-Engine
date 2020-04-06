#include "walker.h"

DWORD g_dwLastSAMPAddr;

uint8_t _declspec (naked) hook_handle_rpc_packet(void)
{
	static LQ_RPCParameters *pRPCParams = nullptr;
	static LQ_RPCNode *pRPCNode = nullptr;
	static DWORD dwTmp = 0;

	__asm pushad;
	__asm mov pRPCParams, eax;
	__asm mov pRPCNode, edi;

	NJIN->getSAMP()->getRakNet()->OnRPCRecieve(pRPCNode->uniqueIdentifier, pRPCParams, pRPCNode->staticFunctionPointer);
	//HandleRPCPacketFunc(pRPCNode->uniqueIdentifier, pRPCParams, pRPCNode->staticFunctionPointer);
	dwTmp = g_dwLastSAMPAddr + SAMP_HOOKEXIT_HANDLE_RPC;

	__asm popad;
	__asm add esp, 4 // overwritten code
	__asm jmp dwTmp;
}

uint8_t _declspec (naked) hook_handle_rpc_packet2(void)
{
	static LQ_RPCParameters *pRPCParams = nullptr;
	static LQ_RPCNode *pRPCNode = nullptr;
	static DWORD dwTmp = 0;

	__asm pushad;
	__asm mov pRPCParams, ecx;
	__asm mov pRPCNode, edi;

	NJIN->getSAMP()->getRakNet()->OnRPCRecieve(pRPCNode->uniqueIdentifier, pRPCParams, pRPCNode->staticFunctionPointer);
	dwTmp = g_dwLastSAMPAddr + SAMP_HOOKEXIT_HANDLE_RPC2;

	__asm popad;
	__asm jmp dwTmp;
}

MOONWALKER::MOONWALKER() {
	m_fLog.Delete(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log");
	//m_pRakNet = new RakNetController();
	m_SAMP = new SAMP();
	g_dwLastSAMPAddr = m_SAMP->GetSampDllAddress();
	Log("Session started");
}
MOONWALKER::~MOONWALKER() {
	Log("Session end with no errors.");
}
SAMP *MOONWALKER::getSAMP() {
	return m_SAMP;
}
void MOONWALKER::Log(std::string message) {
	SYSTEMTIME time;
	GetLocalTime(&time);
	m_fLog.Open(this->GetCurrentDllDirectory() + "\\WALKER\\walker.log", Mode::M_OPEN_WRITE_APPEND);
	m_fLog.Write(
#ifdef LOG_TIMESTAMP
		"[" + Convert::ToString(time.wDay)
		+ ":" + Convert::ToString(time.wMonth)
		+ ":" + Convert::ToString(time.wYear)
		+ " || " + Convert::ToString(time.wHour)
		+ ":" + Convert::ToString(time.wMinute)
		+ ":" + Convert::ToString(time.wSecond)
		+ ":" + Convert::ToString(time.wMilliseconds)
		+ "]: " +
#endif
		message + '\n');
	m_fLog.Close();
}
std::string MOONWALKER::GetCurrentDllDirectory()
{
	char result[MAX_PATH];
	std::string ret = std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
	bool stop = false;
	while (!stop) {
		if (ret[ret.size() - 1] != '\\')
			ret.erase(ret.end() - 1);
		else stop = true;
	}
	return ret;
}