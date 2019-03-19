#pragma once
#include "GfxConfiguration.h"
#include "../Interface/IApplication.h"

namespace Engine {
	class BaseApplication : public IApplication
	{
	protected:
		// Flag if need quit the main loop of the application
		static bool m_bQuit;
		GfxConfiguration m_Config;

	public:
		BaseApplication(GfxConfiguration& cfg);
		virtual int Initialize();
		virtual void Finalize();

		// One cycle of the main loop
		virtual void Tick();
		virtual bool IsQuit();

	private:
		// hide the default construct to enforce a configuration
		BaseApplication() {};
	};
}