#pragma once
#include "../Interface/IRuntimeModule.h"

namespace Engine {
	class GraphicsManager : public IRuntimeModule
	{
	public:
		virtual ~GraphicsManager() {}
		virtual int Initialize();
		virtual void Finalize();
		virtual void Tick();
	};
}

