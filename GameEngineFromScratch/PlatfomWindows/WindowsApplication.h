#pragma once
#include "Common/BaseApplication.h"
#include <windows.h>

namespace Engine {
	class WindowsApplication : public BaseApplication
	{
	public:
		WindowsApplication(GfxConfiguration& config) : BaseApplication(config) {};
		virtual int Initialize();
		virtual void Finalize();

		// One cycle of the main loop
		virtual void Tick();

		// the WindowProc function prototype
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
