#include "Common/BaseApplication.h"

namespace Engine {
	GfxConfiguration config;
	BaseApplication g_App(config);
	IApplication* g_pApp = &g_App;
}