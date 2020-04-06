#include "walker.h"
#include "MemoryStuff.h"

uint32_t SAMP::GetSampDllAddress()
{
	uint32_t	samp_dll;
	void	*temp = MemoryStuff::dll_baseptr_get(SAMP_DLL);
	__asm mov samp_dll, eax
	return samp_dll;
}
SAMPVER SAMP::CheckVersion() {
	uint32_t	samp_dll = GetSampDllAddress();
	if (samp_dll != NULL)
	{
		DWORD samp_addr = (uint32_t)samp_dll;
		if (samp_addr != NULL)
		{
			auto hex_to_bin = [](const char *str)
			{
				auto hex_to_dec = [](signed char ch)
				{
					if (ch >= '0' && ch <= '9')
						return (signed char)(ch - '0');
					if (ch >= 'A' && ch <= 'F')
						return (signed char)(ch - 'A' + 10);
					if (ch >= 'a' && ch <= 'f')
						return (signed char)(ch - 'A' + 10);

					return (signed char)-1;
				};
				int		len = (int)strlen(str);
				uint8_t *buf, *sbuf;

				if (len == 0 || len % 2 != 0)
					return (uint8_t *)NULL;

				sbuf = buf = (uint8_t *)malloc(len / 2);

				while (*str)
				{
					signed char bh = hex_to_dec(*str++);
					signed char bl = hex_to_dec(*str++);

					if (bl == -1 || bh == -1)
					{
						free(sbuf);
						return (uint8_t *)NULL;
					}

					*buf++ = (uint8_t)(bl | (bh << 4));
				}
				return sbuf;
			};
			if (MemoryStuff::memcmp_safe((uint8_t *)samp_addr + 0xBABE, hex_to_bin(SAMP_R2SIGNATURE), 10))
			{
				return SAMPVER::V037R2;
			}
			else
			{
				if (MemoryStuff::memcmp_safe((uint8_t *)samp_addr + 0xBABE, hex_to_bin(SAMP_R3SIGNATURE), 10))
				{
					return SAMPVER::V037R3;
				}
				else {
					return SAMPVER::UNKNOWN;
				}
			}
		}
	}
	return SAMPVER::UNKNOWN;
}
SAMP::SAMP() {
	m_pRakNet = new RakNetController(122);
}
RakNetController *SAMP::getRakNet() {
	return m_pRakNet;
}
void SAMP::LoadSAMPPoolSystem(SAMPVER ver) {
	switch (ver)
	{
	case V037R2:
		m_stActorPool = *(struct STRUCTSV037R2::pool **)ACTOR_POOL_POINTER;
		if (m_stActorPool == nullptr || m_stActorPool->start == nullptr || m_stActorPool->size <= 0)
			return;

		/* setup & refresh vehicle pool */
		m_stVehiclePool = *(struct STRUCTSV037R2::pool **)VEHICLE_POOL_POINTER;
		if (m_stVehiclePool == nullptr || m_stVehiclePool->start == nullptr || m_stVehiclePool->size <= 0)
			return;
		break;
	case V037R3:
		m_stActorPool = *(struct STRUCTSV037R2::pool **)ACTOR_POOL_POINTER;
		if (m_stActorPool == nullptr || m_stActorPool->start == nullptr || m_stActorPool->size <= 0)
			return;

		/* setup & refresh vehicle pool */
		m_stVehiclePool = *(struct STRUCTSV037R2::pool **)VEHICLE_POOL_POINTER;
		if (m_stVehiclePool == nullptr || m_stVehiclePool->start == nullptr || m_stVehiclePool->size <= 0)
			return;
		break;
	case UNKNOWN:
	default:
		break;
	}
}