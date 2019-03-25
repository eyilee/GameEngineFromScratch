#include <objbase.h>
#include <cstdio>
#include "GraphicsManager.h"
#include "MemoryManager.h"
#include "Framework/Interface/IApplication.h"

using namespace Engine;

namespace Engine {

	extern IApplication* g_pApp;
	extern MemoryManager* g_pMemoryManager;
	extern GraphicsManager* g_pGraphicsManager;

}

int main(int argc, char** argv) {
	CoInitialize(NULL);

	int ret;

	if ((ret = g_pApp->Initialize()) != 0) {
		printf("App Initialize failed, will exit now.");
		return ret;
	}

	if ((ret = g_pMemoryManager->Initialize()) != 0) {
		printf("Memory Manager Initialize failed, will exit now.");
		return ret;
	}

	if ((ret = g_pGraphicsManager->Initialize()) != 0) {
		printf("Graphics Manager Initialize failed, will exit now.");
		return ret;
	}

	while (!g_pApp->IsQuit()) {
		g_pApp->Tick();
		g_pMemoryManager->Tick();
		g_pGraphicsManager->Tick();
	}

	g_pGraphicsManager->Finalize();
	g_pMemoryManager->Finalize();
	g_pApp->Finalize();

	CoUninitialize();
	return 0;
}
