#include "Framework/Common/BaseApplication.h"
#include "Framework/Common/GraphicsManager.h"
#include "Framework/Common/MemoryManager.h"

namespace Engine {

	GfxConfiguration config;
	BaseApplication g_App(config);
	IApplication* g_pApp = &g_App;
	GraphicsManager* g_pGraphicsManager = new GraphicsManager();
	MemoryManager* g_pMemoryManager = new MemoryManager();

}