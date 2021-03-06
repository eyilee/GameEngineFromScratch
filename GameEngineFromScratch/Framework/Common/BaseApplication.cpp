#include <iostream>
#include "BaseApplication.h"

namespace Engine {

	bool BaseApplication::m_bQuit = false;

	BaseApplication::BaseApplication(GfxConfiguration& cfg)
		:m_Config(cfg)
	{
	}

	int BaseApplication::Initialize()
	{
		std::cout << m_Config;

		return 0;
	}

	void BaseApplication::Finalize()
	{
	}

	void BaseApplication::Tick()
	{
	}

	bool BaseApplication::IsQuit()
	{
		return m_bQuit;
	}

}
