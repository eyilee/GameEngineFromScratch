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

		virtual void Clear();

		virtual void Draw();
	};

	extern GraphicsManager* g_pGraphicsManager;

}
