#pragma once 
#include "WalkerBase.h"

//from s0beit
class MemoryStuff {
private:
	static bool isBadPtr_handlerAny(void *pointer, ULONG size, DWORD dwFlags)
	{
		DWORD						dwSize;
		MEMORY_BASIC_INFORMATION	meminfo;

		if (NULL == pointer)
			return true;

		memset(&meminfo, 0x00, sizeof(meminfo));
		dwSize = VirtualQuery(pointer, &meminfo, sizeof(meminfo));

		if (0 == dwSize)
			return true;

		if (MEM_COMMIT != meminfo.State)
			return true;

		if (0 == (meminfo.Protect & dwFlags))
			return true;

		if (size > meminfo.RegionSize)
			return true;

		if ((unsigned)((char *)pointer - (char *)meminfo.BaseAddress) > (unsigned)(meminfo.RegionSize - size))
			return true;

		return false;
	}

	static bool isBadPtr_readAny(void *pointer, ULONG size)
	{
		return isBadPtr_handlerAny(pointer, size, PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
			PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
	}

	static bool isBadPtr_writeAny(void *pointer, ULONG size)
	{
		return isBadPtr_handlerAny(pointer, size,
			PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
	}
public:
	static int __page_size_get(void)
	{
		static int	page_size = -1;
		SYSTEM_INFO si;

		if (page_size == -1)
		{
			GetSystemInfo(&si);
			page_size = (int)si.dwPageSize;
		}

		return page_size;
	}

	static int __page_write(void *_dest, const void *_src, uint32_t len)
	{
		static int		page_size = __page_size_get();
		uint8_t			*dest = (uint8_t *)_dest;
		const uint8_t	*src = (const uint8_t *)_src;
		DWORD			prot_prev = 0;
		int				prot_changed = 0;
		int				ret = 1;

		while (len > 0)
		{
			ret = 1;
			int page_offset = (int)((UINT_PTR)dest % page_size);
			int page_remain = page_size - page_offset;
			int this_len = len;

			if (this_len > page_remain)
				this_len = page_remain;

			if (isBadPtr_writeAny(dest, this_len))
			{
				if (!VirtualProtect((void *)dest, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
					ret = 0;
				else
					prot_changed = 1;
			}

			if (ret)
				memcpy(dest, src, this_len);

			if (prot_changed)
			{
				DWORD	dummy;
				if (!VirtualProtect((void *)dest, this_len, prot_prev, &dummy))
				{
					
				}
			}

			dest += this_len;
			src += this_len;
			len -= this_len;
		}

		return ret;
	}

	static int __page_read(void *_dest, const void *_src, uint32_t len)
	{
		static int	page_size = __page_size_get();
		uint8_t		*dest = (uint8_t *)_dest;
		uint8_t		*src = (uint8_t *)_src;
		DWORD		prot_prev = 0;
		int			prot_changed = 0;
		int			ret = 1;

		while (len > 0)
		{
			ret = 1;
			int page_offset = (int)((UINT_PTR)src % page_size);
			int page_remain = page_size - page_offset;
			int this_len = len;

			if (this_len > page_remain)
				this_len = page_remain;

			if (isBadPtr_readAny(src, this_len))
			{
				if (!VirtualProtect((void *)src, this_len, PAGE_EXECUTE_READWRITE, &prot_prev))
					ret = 0;
				else
					prot_changed = 1;
			}

			if (ret)
				memcpy(dest, src, this_len);
			else
				memset(dest, 0, this_len);

			if (prot_changed)
			{
				DWORD	dummy;
				if (!VirtualProtect((void *)src, this_len, prot_prev, &dummy)) {
					//���� ��
				}
					//NJIN->Log("__page_read() could not restore original permissions for ptr " + Convert::ToString(src));
			}

			dest += this_len;
			src += this_len;
			len -= this_len;
		}

		return ret;
	}

	static int memcpy_safe(void *_dest, const void *_src, uint32_t len, int check = NULL, const void *checkdata = NULL)
	{
		static int		page_size = __page_size_get();
		static int		recurse_ok = 1;
		uint8_t			buf[4096];
		uint8_t			*dest = (uint8_t *)_dest;
		const uint8_t	*src = (const uint8_t *)_src;
		int				ret = 1;

		if (check && checkdata)
		{
			if (!memcmp_safe(checkdata, _dest, len))
				return 0;
		}

		while (len > 0)
		{
			uint32_t	this_len = sizeof(buf);

			if (this_len > len)
				this_len = len;

			if (!__page_read(buf, src, this_len))
				ret = 0;

			if (!__page_write(dest, buf, this_len))
				ret = 0;

			len -= this_len;
			src += this_len;
			dest += this_len;
		}

		return ret;
	}

	static int memset_safe(void *_dest, int c, uint32_t len)
	{
		uint8_t *dest = (uint8_t *)_dest;
		uint8_t buf[4096];

		memset(buf, c, (len > 4096) ? 4096 : len);

		for (;; )
		{
			if (len > 4096)
			{
				if (!memcpy_safe(dest, buf, 4096))
					return 0;
				dest += 4096;
				len -= 4096;
			}
			else
			{
				if (!memcpy_safe(dest, buf, len))
					return 0;
				break;
			}
		}

		return 1;
	}

	static int memcmp_safe(const void *_s1, const void *_s2, uint32_t len)
	{
		const uint8_t	*s1 = (const uint8_t *)_s1;
		const uint8_t	*s2 = (const uint8_t *)_s2;
		uint8_t			buf[4096];

		for (;; )
		{
			if (len > 4096)
			{
				if (!memcpy_safe(buf, s1, 4096))
					return 0;
				if (memcmp(buf, s2, 4096))
					return 0;
				s1 += 4096;
				s2 += 4096;
				len -= 4096;
			}
			else
			{
				if (!memcpy_safe(buf, s1, len))
					return 0;
				if (memcmp(buf, s2, len))
					return 0;
				break;
			}
		}

		return 1;
	}
	
	static void *dll_baseptr_get(const char *dll_name)
	{
		return GetModuleHandle(dll_name);
	}
};