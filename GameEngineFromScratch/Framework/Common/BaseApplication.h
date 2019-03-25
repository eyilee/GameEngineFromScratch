#pragma once

#include "GfxConfiguration.h"
#include "Framework/Interface/IApplication.h"

namespace Engine {

	class BaseApplication : public IApplication
	{
	protected:
		// Flag if need quit the main loop of the application
		static bool m_bQuit;
		GfxConfiguration m_Config;

	public:
		// delete the default construct to enforce a configuration
		BaseApplication() = delete;
		BaseApplication(GfxConfiguration& cfg);
		~BaseApplication() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();

		virtual bool IsQuit();

		inline GfxConfiguration& GetConfiguration() { return m_Config; };

	protected:
		virtual void OnDraw() {};

	private:
	};

}
