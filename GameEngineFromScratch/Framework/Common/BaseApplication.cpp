#include <iostream>
#include "BaseApplication.h"

namespace Engine {

	bool BaseApplication::m_bQuit = false;

	BaseApplication::BaseApplication(GfxConfiguration& cfg)
		:m_Config(cfg)
	{
	}

	// Parse command line, read configuration, initialize all sub modules
	int BaseApplication::Initialize()
	{
		int result = 0;

		std::cout << m_Config;

		return result;
	}

	// Finalize all sub modules and clean up all runtime temporary files.
	void BaseApplication::Finalize()
	{
	}

	// One cycle of the main loop
	void BaseApplication::Tick()
	{
	}

	bool BaseApplication::IsQuit()
	{
		return m_bQuit;
	}

}
