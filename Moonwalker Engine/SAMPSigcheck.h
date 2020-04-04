#pragma once
#include "walker.h"
#include "MemoryStuff.h"

#define SAMP_DLL "samp.dll"

#define SAMP_R2SIGNATURE "F8036A004050518D4C24"
#define SAMP_R3SIGNATURE "E86D9A0A0083C41C85C0"

enum SAMPVER {
	UNKNOWN = -13,
	V037R2 = 1, 
	V037R3
};

class SAMP {
public:
	static uint32_t GetSampDllAddress()
	{
		uint32_t	samp_dll;
		void	*temp = MemoryStuff::dll_baseptr_get(SAMP_DLL);
		__asm mov samp_dll, eax
		char add[256];
		sprintf(add, "samp address getted: 0x%X", samp_dll);
		NJIN->Log(add);
		return samp_dll;
	}
	static SAMPVER CheckVersion() {
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
	}
};