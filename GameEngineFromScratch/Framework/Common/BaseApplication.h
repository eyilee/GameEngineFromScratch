#pragma once

#include "AssetLoader.h"
#include "Framework/Interface/IApplication.h"
#include "GraphicsManager.h"
#include "MemoryManager.h"
#include "SceneManager.h"

namespace Engine {

	class BaseApplication : public IApplication
	{
	protected:
		static bool m_bQuit;
		GfxConfiguration m_Config;

	public:
		BaseApplication() = delete;
		BaseApplication(GfxConfiguration& cfg);
		virtual ~BaseApplication() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();

		virtual bool IsQuit();

		inline GfxConfiguration& GetConfiguration() { return m_Config; }

	protected:
		virtual void OnDraw() {}
	};

}
