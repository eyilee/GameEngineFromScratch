#include <iostream>
#include "BaseApplication.h"

bool Engine::BaseApplication::m_bQuit = false;

Engine::BaseApplication::BaseApplication(GfxConfiguration& cfg)
	:m_Config(cfg)
{
}

// Parse command line, read configuration, initialize all sub modules
int Engine::BaseApplication::Initialize()
{
	int result = 0;

	std::cout << m_Config;

	return result;
}

// Finalize all sub modules and clean up all runtime temporary files.
void Engine::BaseApplication::Finalize()
{
}

// One cycle of the main loop
void Engine::BaseApplication::Tick()
{
}

bool Engine::BaseApplication::IsQuit()
{
	return m_bQuit;
}