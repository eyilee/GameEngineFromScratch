#include <tchar.h>
#include "Framework/Platform/WindowsApplication.h"
#include "Framework/RHI/D3D11GraphicsManager.h"

namespace Engine {

	GfxConfiguration config(8, 8, 8, 8, 32, 0, 0, 960, 540, _T("Game Engine From Scratch (Windows)"));
	IApplication* g_pApp = static_cast<IApplication*>(new WindowsApplication(config));
	MemoryManager* g_pMemoryManager = static_cast<MemoryManager*>(new MemoryManager);
	AssetLoader* g_pAssetLoader = static_cast<AssetLoader*>(new AssetLoader);
	SceneManager* g_pSceneManager = static_cast<SceneManager*>(new SceneManager);
	GraphicsManager* g_pGraphicsManager = static_cast<GraphicsManager*>(new D3D11GraphicsManager);

}