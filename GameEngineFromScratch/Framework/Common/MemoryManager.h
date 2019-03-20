#pragma once

#include <new>
#include "Allocator.h"
#include "Framework/Interface/IRuntimeModule.h"

namespace Engine {

	class MemoryManager : public IRuntimeModule
	{
	private:
		static size_t* m_pBlockSizeLookup;
		static Allocator* m_pAllocators;

	public:
		virtual ~MemoryManager() {}
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();
		void* Allocate(size_t size);
		void* Allocate(size_t size, size_t alignment);
		void Free(void* p, size_t size);

	public:
		template<typename T, typename... Arguments>
		T* New(Arguments... parameters)
		{
			return new (Allocate(sizeof(T))) T(parameters...);
		}

		template<typename T>
		void Delete(T *p)
		{
			reinterpret_cast<T*>(p)->~T();
			Free(p, sizeof(T));
		}

	private:
		static Allocator* LookUpAllocator(size_t size);
	};

}
