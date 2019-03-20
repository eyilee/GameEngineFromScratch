#pragma once

#include <cstddef>
#include "MemoryManager.h"

namespace Engine {

	extern MemoryManager* g_pMemoryManager;

	class Buffer {
	public:
		uint8_t* m_pData;
		size_t m_szSize;
		size_t m_szAlignment;

	public:
		Buffer();
		Buffer(size_t size, size_t alignment = 4);
		Buffer(const Buffer& rhs);
		Buffer(Buffer&& rhs);
		~Buffer();

		Buffer& operator = (const Buffer& rhs);
		Buffer& operator = (Buffer&& rhs);
	};

}
