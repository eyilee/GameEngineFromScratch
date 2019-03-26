#pragma once

#include "Framework/Interface/IRuntimeModule.h"

namespace Engine {

	class GraphicsManager : public IRuntimeModule
	{
	public:
		GraphicsManager() = default;
		virtual ~GraphicsManager() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();
	};

}
