#pragma once

#include "Framework/Common/GfxConfiguration.h"
#include "IRuntimeModule.h"

namespace Engine {

	class IApplication : public IRuntimeModule
	{
	public:
		IApplication() = default;
		virtual ~IApplication() = default;

		virtual int Initialize() = 0;

		virtual void Finalize() = 0;

		virtual void Tick() = 0;

		virtual bool IsQuit() = 0;

		virtual GfxConfiguration& GetConfiguration() = 0;
	};

	extern IApplication* g_pApp;

}